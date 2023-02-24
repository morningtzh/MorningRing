idf_component_register(
        SRC_DIRS
        "."
        "network"
        "api"
        "light"
        "utils"
        "system"


        INCLUDE_DIRS
        "."
        "./network"
        "./system"
        "./api"
        "./light"
        "./light/modes"
        "./utils"
        #
        #        PRIV_REQUIRES nvs_flash esp_wifi
        #        driver
        #        efuse
        #        esp_timer
        #        lwip
        #        vfs
        #        esp_wifi
        #        esp_event
        #        esp_netif
        #        esp_eth
        #        esp_phy
        #        esp_http_server
        #        ESP32-NeoPixel-WS2812-RMT
)

target_compile_options(${COMPONENT_LIB} PUBLIC -std=gnu++2b)


