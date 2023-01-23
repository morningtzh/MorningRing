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
        "utils/led_strip_encoder.c"

        INCLUDE_DIRS
        "."
        "./network"
        "./api"
        "./light"
        "./utils"
)


