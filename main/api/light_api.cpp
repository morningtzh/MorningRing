//
// Created by MorningTZH on 2023/1/21.
//

#include <esp_http_server.h>

#include "restful.hpp"
#include "light.hpp"

esp_err_t GetModeList(httpd_req_t *r) {

    light::manager().GetModeList();
    return ESP_OK;
}

RESTFUL_REGISTER(GetModes, "/api/light/modes", HTTP_GET, GetModeList);

esp_err_t SetMode(httpd_req_t *r) {

    light::manager().SetMode("aa");

    return ESP_OK;
}

RESTFUL_REGISTER(SetMode, "/api/light/mode", HTTP_POST, GetModeList);

esp_err_t GetModeConfig(httpd_req_t *r) {



    return ESP_OK;
}

RESTFUL_REGISTER(GetModeConfig, "/api/light/mode/config", HTTP_GET, GetModeConfig);

esp_err_t SetModeConfig(httpd_req_t *r) {
    return ESP_OK;
}

RESTFUL_REGISTER(SetModeConfig, "/api/light/mode/config", HTTP_POST, SetModeConfig);


