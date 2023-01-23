#include "esp_log.h"
#include "mdns.h"

static const char *MODULE = "mDNS";

void mdns_init_services() {
    ESP_LOGI(MODULE, "starting mDNS...");

    // 初始化 mDNS 服务
    esp_err_t err = mdns_init();
    if (err) {
        ESP_LOGI(MODULE, "MDNS Init failed: %d\n", err);
        return;
    }

    // 设置 hostname
    mdns_hostname_set("MorningRing");
    // 设置默认实例
    mdns_instance_name_set("MorningRing bright your life");

    // 添加服务
    mdns_service_add(nullptr, "_http", "_tcp", 80, nullptr, 0);

    // 注意：必须先添加服务，然后才能设置其属性
    // web 服务器使用自定义的实例名
    mdns_service_instance_name_set("_http", "_tcp", "MorningRing bright your life");

    mdns_txt_item_t serviceTxtData[3] = {
            {"board", "esp32"},
            {"u",     "user"},
            {"p",     "password"}
    };
    // 设置服务的文本数据（会释放并替换当前数据）
    mdns_service_txt_set("_http", "_tcp", serviceTxtData, 3);

    ESP_LOGI(MODULE, "mDNS running");
}