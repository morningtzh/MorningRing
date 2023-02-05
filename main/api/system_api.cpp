#include <esp_http_server.h>

#include "utils/ArduinoJson-v6.20.0.hpp"
#include "restful.hpp"
#include "utils/system_config.hpp"

esp_err_t GetSystemStatus(httpd_req_t *r) {
    std::string modeName;

    ArduinoJson::StaticJsonDocument<200> doc;
    doc["version"] = VERSION;
    doc["timestamp"] = time(nullptr);

    std::string response;
    ArduinoJson::serializeJson(doc, response);

    httpd_resp_set_type(r, "application/json");
    httpd_resp_sendstr(r, response.c_str());

    return ESP_OK;
}

esp_err_t GetSystemConfig(httpd_req_t *r) {
    std::string modeName;

    auto config = SystemConfig.all();

    if (config == nullptr) {
        httpd_resp_send_err(r, HTTPD_404_NOT_FOUND, std::string("invalid mode" + modeName).c_str());
        return ESP_FAIL;
    }

    ArduinoJson::StaticJsonDocument<200> doc;
    doc["version"] = VERSION;

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

esp_err_t SetSystemConfig(httpd_req_t *r) {
    std::string modeName;
    std::string data;
    ConfigData config;

    if (GetData(r,data)) {
        httpd_resp_send_err(r, HTTPD_404_NOT_FOUND, "No data");
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
//    bool ret = light::manager()->SetModeConfig(modeName, config);
//    if (!ret) {
//        httpd_resp_send_err(r, HTTPD_404_NOT_FOUND, std::string("invalid mode" + modeName).c_str());
//    }

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

void InitLightApi() {

    Restful::api()->RegisterUri("GetSystemConfig", "/api/system/config", HTTP_GET, GetSystemConfig);
    Restful::api()->RegisterUri("SetSystemConfig", "/api/system/config", HTTP_POST, SetSystemConfig);
}
