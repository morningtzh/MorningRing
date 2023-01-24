//
// Created by MorningTZH on 2023/1/21.
//

#include <esp_http_server.h>

#include "utils/ArduinoJson-v6.20.0.hpp"
#include "light.hpp"
#include "restful.hpp"

static const char *MODULE = "LightApi";

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
    std::string modeName;

    GetQuery(r, "mode", modeName);

    if(!light::manager()->SetMode(modeName)) {
        httpd_resp_send_err(r, HTTPD_404_NOT_FOUND, std::string("invalid mode" + modeName).c_str());
    }
    //httpd_resp_set_status()

    httpd_resp_sendstr(r, "Set Mode success.");


    return ESP_OK;
}

esp_err_t GetModeConfig(httpd_req_t *r) {
    std::string modeName;

    GetQuery(r, "mode", modeName);

    auto config = light::manager()->GetModeConfig(modeName);

    if (config == nullptr) {
        httpd_resp_send_err(r, HTTPD_404_NOT_FOUND, std::string("invalid mode" + modeName).c_str());
        return ESP_FAIL;
    }

    ArduinoJson::StaticJsonDocument<200> doc;
    doc["mode"] = modeName;

    ArduinoJson::JsonObject jsonObject = doc.createNestedObject("config");
    for (auto &c: *config) {
        jsonObject[c.first] = c.second.value;
    }

    std::string response;
    ArduinoJson::serializeJson(doc, response);

    httpd_resp_set_type(r, "application/json");
    httpd_resp_sendstr(r, response.c_str());

    return ESP_OK;
}

esp_err_t SetModeConfig(httpd_req_t *r) {
    std::string modeName;
    std::string data;
    light::mode::Config config;

    if (!(GetQuery(r, "mode", modeName) && GetData(r,data))) {
        httpd_resp_send_err(r, HTTPD_404_NOT_FOUND, "No query or data");
    }

    // 读取配置
    {
        ArduinoJson::DynamicJsonDocument doc(512);
        ArduinoJson6200_71::deserializeJson(doc, data);

        auto configJson = doc["config"].as<ArduinoJson::JsonObject>();
        for (auto c: configJson) {
            config[c.key().c_str()].value = c.value();
        }
    }

    // 设置配置
    bool ret = light::manager()->SetModeConfig(modeName, config);
    if (!ret) {
        httpd_resp_send_err(r, HTTPD_404_NOT_FOUND, std::string("invalid mode" + modeName).c_str());
    }

    // 回复配置
    {
        ArduinoJson::StaticJsonDocument<200> doc;
        doc["mode"] = modeName;

        ArduinoJson::JsonObject jsonObject = doc.createNestedObject("config");
        for (auto &c: config) {
            jsonObject[c.first] = c.second.value;
        }

        std::string response;
        ArduinoJson::serializeJson(doc, response);

        httpd_resp_set_type(r, "application/json");
        httpd_resp_sendstr(r, response.c_str());
    }
    return ESP_OK;
}

typedef esp_err_t (*RestfulHandler)(httpd_req_t *r);

//RestfulHandler D(RestfulHandler h) {
//
//    return ([h](httpd_req_t *r) -> esp_err_t {
//        ESP_LOGI(MODULE, "Request %d %s", r->method, r->uri);
//        esp_err_t ret = h(r);
//    });
//}


void InitLightApi() {
    Restful::api()->RegisterUri("GetModes", "/api/light/modes", HTTP_GET, GetModeList);
    Restful::api()->RegisterUri("SetMode", "/api/light/mode", HTTP_POST, SetMode);
    Restful::api()->RegisterUri("GetModeConfig", "/api/light/mode/config", HTTP_GET, GetModeConfig);
    Restful::api()->RegisterUri("SetModeConfig", "/api/light/mode/config", HTTP_POST, SetModeConfig);
}
