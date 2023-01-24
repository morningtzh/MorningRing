//
// Created by MorningTZH on 2023/1/21.
//

#include "mode.hpp"

static const char *MODULE = "LightMode";

namespace light::mode {

    std::string ShortKey(std::string key) {
        std::string shortKey;
        for (auto &c : key) {
            if (isupper(c)) {
                shortKey.push_back(c);
            }
        }
        return shortKey;
    }

    Mode::Mode(LightBuffer &lightBuffer) : lightBuffer(lightBuffer) {}

    Mode::Mode(LightBuffer &lightBuffer, std::string configPrefix, const Config& defaultConfig) : isSupportConfig(true), configPrefix(std::move(configPrefix)),
    lightBuffer(lightBuffer) {
        InitConfig(defaultConfig);
    }

    Mode::~Mode() {

    };

    bool Mode::SetConfig(Config &newConfig) {

        if (!isSupportConfig) {
            return true;
        }

        nvs_handle_t my_handle;
        esp_err_t err = nvs_open("nvs", NVS_READWRITE, &my_handle);
        if (err != ESP_OK) {
            ESP_LOGE(MODULE, "Error (%s) opening NVS handle!\n", esp_err_to_name(err));
            return false;
        }

        for (auto &c:newConfig) {
            const std::string &key = c.first;
            const std::string &shortKey = ShortKey(key);
            const int &value = c.second.value;

            // 不存在的key就跳过
            if (config.find(key) == config.end()) {
                ESP_LOGW(MODULE, "Set Config has invalid key[%s], skip.", key.c_str());
                continue;
            }

            // 写入NVS
            err = nvs_set_i32(my_handle, (configPrefix + "_" + shortKey).c_str(), value);
            if (err != ESP_OK) {
                ESP_LOGE(MODULE, "Set Config write nvs failed: %s.", esp_err_to_name(err));
                nvs_close(my_handle);
                return false;
            }

            config[key].value = value;
        }

        err = nvs_commit(my_handle);
        if (err != ESP_OK) {
            ESP_LOGE(MODULE, "Set Config commit nvs failed: %s.", esp_err_to_name(err));
            return false;
        }
        nvs_close(my_handle);

        newConfig = config;

        return true;
    }

    std::unique_ptr<Config> Mode::GetConfig() {

        auto returnConfig = std::make_unique<Config>(config);

        return returnConfig;
    }

    int Mode::GetConfig(std::string key) {

        auto conf = config.find(key);

        if (conf == config.end()) {
            ESP_LOGE(MODULE, "Get Config key[%s] not exist", key.c_str());
            return INT32_MAX;
        }

        return conf->second.value;
    }

    void Mode::InitConfig(const Config &defaultConfig) {

        nvs_handle_t my_handle;
        esp_err_t err = nvs_open("nvs", NVS_READWRITE, &my_handle);
        if (err != ESP_OK) {
            ESP_LOGE(MODULE, "Error (%s) opening NVS handle!\n", esp_err_to_name(err));
            return;
        }

        for (auto &c : defaultConfig) {
            const std::string &key = c.first;
            const std::string &shortKey = ShortKey(key);
            int32_t storage_value;

            config[key] = c.second;

            err = nvs_get_i32(my_handle, (configPrefix+"_"+shortKey).c_str(), &storage_value);
            switch (err) {
                case ESP_OK:
                    config[key].value = storage_value;
                    break;
                default :
                    ESP_LOGE(MODULE, "Error nvs (%s) reading!\n", esp_err_to_name(err));
            }
        }

        nvs_close(my_handle);
    }



}


