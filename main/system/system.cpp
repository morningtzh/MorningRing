//
// Created by MorningTZH on 2023/2/17.
//

#include "system.hpp"

#include <string.h>
#include <sys/param.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_netif.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include <lwip/netdb.h>

static const char *MODULE = "System";

int DEVICE_SOCKET = -1;

static void SendDeviceInfoTask(void *pvParameters)
{
    char rx_buffer[128];
    const char *host_ip = pvParameters == nullptr ? "255.255.255.255" : (char *) pvParameters;
    int addr_family = AF_INET;
    int ip_protocol = IPPROTO_IP;

    sockaddr_in dest_addr;
    dest_addr.sin_addr.s_addr = inet_addr(host_ip);
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(DEVICE_CONNECT_PORT);

    int err = sendto(DEVICE_SOCKET, payload, strlen(payload), 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
    if (err < 0) {
        ESP_LOGE(MODULE, "Error occurred during sending: errno %d, %s", errno, strerror(err));
        vTaskDelete(nullptr);
    }
    ESP_LOGI(MODULE, "Message sent");

    vTaskDelete(nullptr);
}

static void RecvDeviceInfoTask(void *pvParameters) {

    char rx_buffer[128];
    char addr_str[16];
    sockaddr_in source_addr;
    socklen_t socklen = sizeof(source_addr);

    // 该任务启动前应该先广播一次，因此此时暂时不广播
    time_t lastBoardcast = time(nullptr);


    while (true) {
        int len = recvfrom(DEVICE_SOCKET, rx_buffer, sizeof(rx_buffer) - 1, 0, (sockaddr *)&source_addr, &socklen);
        if (len < 0) {
            ESP_LOGE(MODULE, "recvfrom failed: errno %d, %s", errno, strerror(errno));
            continue;
        }

        rx_buffer[len] = 0;

        inet_ntoa_r(((struct sockaddr_in *)&source_addr)->sin_addr, addr_str, sizeof(addr_str) - 1);

        // TODO: 数据处理

        time_t now = time(nullptr);


    }
}


bool InitSocket() {
    DEVICE_SOCKET = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
    if (DEVICE_SOCKET < 0) {
        ESP_LOGE(MODULE, "Unable to create socket: errno %d", errno);
        return false;
    }

    sockaddr_in src_addr;
    src_addr.sin_addr.s_addr = inet_addr("0.0.0.0");
    src_addr.sin_family = AF_INET;
    src_addr.sin_port = htons(DEVICE_CONNECT_PORT);

    int err = bind(DEVICE_SOCKET, (sockaddr *)&src_addr, sizeof(src_addr));
    if (err < 0) {
        ESP_LOGE(MODULE, "Socket bind err: errno %d, %s", errno, strerror(err));
        return false;
    }

    // Set timeout
//    struct timeval timeout;
//    timeout.tv_sec = 10;
//    timeout.tv_usec = 0;
//    setsockopt (DEVICE_SOCKET, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof timeout);

    ESP_LOGI(MODULE, "Socket created");
}


int StartDetectDevice() {



}
