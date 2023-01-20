//
// Created by MorningTZH on 2023/1/20.
//

#include "light.hpp"

namespace light {
    manager *manager() {
        static class manager *m = nullptr;
        if (!m) {
            m = new class manager;
        }

        return m;
    }

    std::list<std::string> manager::GetModeList() {

        std::list<std::string> list;

        for (auto &mode : modeList) {
            list.emplace_back(mode.first);
        }

        return list;
    }
}

esp_err_t GetModeList(httpd_req_t *r) {
    return ESP_OK;
}

RESTFUL_REGISTER(GetModes, "/api/light/modes", HTTP_GET, GetModeList);

esp_err_t SetMode(httpd_req_t *r) {
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


