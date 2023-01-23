idf_component_register(
        SRCS
        "MorningRing.cpp"
        "network/wifi.cpp"
        "network/mdns.cpp"
        "api/restful.cpp"
        "light/light.cpp"
        "light/mode.cpp"
        "light/common_modes.cpp"
        "api/light_api.cpp"
        "utils/fast_hsv2rgb_8bit.c"
        "utils/SPI_ws2812.c"

        INCLUDE_DIRS
        "."
        "./network"
        "./api"
        "./light"
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


