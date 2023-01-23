//
// Created by MorningTZH on 2023/1/21.
//

#include <esp_http_server.h>

#include "utils/ArduinoJson-v6.20.0.hpp"
#include "light.hpp"
#include "restful.hpp"

esp_err_t GetModeList(httpd_req_t *r) {
    auto modeList = light::manager()->GetModeList();
    auto currentMode = light::manager()->GetMode();

    ArduinoJson::StaticJsonDocument<200> doc;
    ArduinoJson::JsonArray modes = doc.createNestedArray("modes");
    for (auto &mode: modeList) {
        modes.add(mode);
    }

    doc["currentMode"] = currentMode;

    std::string response;
    ArduinoJson::serializeJson(doc, response);

    httpd_resp_set_type(r, "application/json");
    httpd_resp_sendstr(r, response.c_str());

    return ESP_OK;
}

esp_err_t SetMode(httpd_req_t *r) {
    char modeName[32];
    httpd_query_key_value(r->uri, "mode", modeName, 32);

    light::manager()->SetMode(modeName);

    return ESP_OK;
}

esp_err_t GetModeConfig(httpd_req_t *r) { return ESP_OK; }

esp_err_t SetModeConfig(httpd_req_t *r) { return ESP_OK; }


void InitLightApi() {
    Restful::api()->RegisterUri("GetModes", "/api/light/modes", HTTP_GET, GetModeList);
    Restful::api()->RegisterUri("SetMode", "/api/light/mode", HTTP_POST, SetMode);
    Restful::api()->RegisterUri("GetModeConfig", "/api/light/mode/config", HTTP_GET, GetModeConfig);
    Restful::api()->RegisterUri("SetModeConfig", "/api/light/mode/config", HTTP_POST, SetModeConfig);
}
