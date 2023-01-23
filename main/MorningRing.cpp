#include <cstring>

#include <unistd.h>
#include "esp_event.h"
#include "esp_log.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "gpio_cxx.hpp"
#include "freertos/task.h"

#include "network/network.h"
#include "light/light.hpp"
#include "light/common_modes.hpp"
#include "api/api_register.hpp"

using namespace idf;
using namespace std;

static const char *MODULE = "MAIN";

void blink([[maybe_unused]] void *nothing) {
    /* The functions of GPIO_Output throws exceptions in case of parameter errors or if there are underlying driver
       errors. */
    try {
        /* This line may throw an exception if the pin number is invalid.
         * Alternatively to 4, choose another output-capable pin. */
        GPIO_Output gpio(GPIONum(2));

        while (true) {
            ESP_LOGV(MODULE, "LED ON");
            gpio.set_high();
            sleep(1);
            ESP_LOGV(MODULE, "LED OFF");
            gpio.set_low();
            sleep(1);
        }
    } catch (GPIOException &e) {
        ESP_LOGE(MODULE, "GPIO exception occurred: %s", esp_err_to_name(e.error));
        ESP_LOGE(MODULE, "stopping.");
        vTaskDelete(nullptr);
    }
}

extern "C" [[maybe_unused]] void app_main(void) {
    // Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    xTaskCreate(blink, "Blink", 10 * 1024, nullptr, 20, nullptr);

    ESP_LOGI(MODULE, "Will init WIFI");
    wifi_init_sta();

    ESP_LOGI(MODULE, "Will init mDNS");
    mdns_init_services();

    InitApi();

    REGISTER_LIGHT_MODE(HSVRing);
    REGISTER_LIGHT_MODE(Test);
    REGISTER_LIGHT_MODE(Single);


    ESP_LOGI(MODULE, "Will start light");
    light::manager()->SetMode("HSVRing");
}