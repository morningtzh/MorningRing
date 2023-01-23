#include <unistd.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "freertos/task.h"
#include "esp_wifi.h"

static const char *MODULE = "WIFI";

#define EXAMPLE_ESP_WIFI_SSID "" // TODO: 上库前记得删除SSID和密码
#define EXAMPLE_ESP_WIFI_PASS "" // TODO: 上库前记得删除SSID和密码
#define EXAMPLE_ESP_MAXIMUM_RETRY 5

/* FreeRTOS event group to signal when we are connected*/
static EventGroupHandle_t s_wifi_event_group;

/* The event group allows multiple bits for each event, but we only care about two events:
 * - we are connected to the AP with an IP
 * - we failed to connect after the maximum amount of retries */
#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT BIT1

static int s_retry_num = 0;

void wifi_connect_sta(void *afterDisconnect = nullptr) {

    if (afterDisconnect != nullptr && (bool) afterDisconnect) {
        s_retry_num++;

        int8_t sleepTime = s_retry_num < EXAMPLE_ESP_MAXIMUM_RETRY ? 5 : 60;

        ESP_LOGI(MODULE, "fail to connect for %d times, will retry after %d seconds", s_retry_num, sleepTime);

        sleep(sleepTime);
    }

    esp_wifi_connect();

    ESP_LOGI(MODULE, "try to connect to the AP");

    vTaskDelete(nullptr);
}


static void event_handler([[maybe_unused]] void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data) {
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        xTaskCreate(wifi_connect_sta, "WiFi Connect", 10 * 1024, nullptr, 5, nullptr);
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        bool isDisconnect = true;
        xTaskCreate(wifi_connect_sta, "WiFi Reconnect", 10 * 1024, &isDisconnect, 5, nullptr);
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        auto *event = (ip_event_got_ip_t *) event_data;
        ESP_LOGI(MODULE, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
        s_retry_num = 0;
        xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
    }
}

void wifi_init_sta() {
    s_wifi_event_group = xEventGroupCreate();

    ESP_ERROR_CHECK(esp_netif_init());

    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    esp_event_handler_instance_t instance_any_id;
    esp_event_handler_instance_t instance_got_ip;
    ESP_ERROR_CHECK(
            esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, nullptr,
                                                &instance_any_id));
    ESP_ERROR_CHECK(
            esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, nullptr,
                                                &instance_got_ip));

    wifi_config_t wifi_config = {
            .sta =
                    {
                            .ssid = EXAMPLE_ESP_WIFI_SSID,
                            .password = EXAMPLE_ESP_WIFI_PASS,
                            /* Authmode threshold resets to WPA2 as default if password matches WPA2 standards (password len => 8).
                             * If you want to connect the device to deprecated WEP/WPA networks, Please set the threshold value
                             * to WIFI_AUTH_WEP/WIFI_AUTH_WPA_PSK and set the password with length and format matching to
                             * WIFI_AUTH_WEP/WIFI_AUTH_WPA_PSK standards.
                             */
                            // .threshold = {
                            //     .authmode = ESP_WIFI_SCAN_AUTH_MODE_THRESHOLD,
                            // }
                            // .sae_pwe_h2e = WPA3_SAE_PWE_BOTH,
                    },
    };
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(MODULE, "wifi_init_sta finished.");

    /* Waiting until either the connection is established (WIFI_CONNECTED_BIT) or connection failed for the maximum
     * number of re-tries (WIFI_FAIL_BIT). The bits are set by event_handler() (see above) */
    EventBits_t bits =
            xEventGroupWaitBits(s_wifi_event_group, WIFI_CONNECTED_BIT | WIFI_FAIL_BIT, pdFALSE, pdFALSE,
                                portMAX_DELAY);

    /* xEventGroupWaitBits() returns the bits before the call returned, hence we can test which event actually
     * happened. */
    if (bits & WIFI_CONNECTED_BIT) {
        ESP_LOGI(MODULE, "connected to ap SSID:%s password:%s", EXAMPLE_ESP_WIFI_SSID, EXAMPLE_ESP_WIFI_PASS);
    } else if (bits & WIFI_FAIL_BIT) {
        ESP_LOGI(MODULE, "Failed to connect to SSID:%s, password:%s", EXAMPLE_ESP_WIFI_SSID, EXAMPLE_ESP_WIFI_PASS);
    } else {
        ESP_LOGE(MODULE, "UNEXPECTED EVENT");
    }
}
