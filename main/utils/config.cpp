//
// Created by MorningTZH on 2023/2/4.
//

#include "config.hpp"

#include <nvs_flash.h>
#include <memory>
#include "esp_log.h"

static const char *MODULE = "Config";

std::string ShortKey(const std::string& key) {
    std::string shortKey;
    for (auto &c : key) {
        if (isupper(c) || isdigit(c)) {
            shortKey.push_back(c);
        }
    }
    return shortKey;
}

Config::Config(const std::string &configPrefix, const ConfigData &defaultConfig) {

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

        _config[key] = c.second;

        std::string hwKey = std::string{}.append(configPrefix).append("_").append(shortKey);

        err = nvs_get_i32(my_handle, hwKey.c_str(), &storage_value);
        if (err != ESP_OK) {
            ESP_LOGW(MODULE, "Nvs can't find key [%s] (%s)!\n", hwKey.c_str(), esp_err_to_name(err));
        } else {
            _config[key].value = storage_value;
        }
    }

    nvs_close(my_handle);
}


bool Config::set(ConfigData &newConfig) {

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
        if (_config.find(key) == _config.end()) {
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

        _config[key].value = value;
    }

    err = nvs_commit(my_handle);
    if (err != ESP_OK) {
        ESP_LOGE(MODULE, "Set Config commit nvs failed: %s.", esp_err_to_name(err));
        return false;
    }
    nvs_close(my_handle);

    newConfig = _config;

    return true;
}

std::unique_ptr<ConfigData> Config::all() {

    auto returnConfig = std::make_unique<ConfigData>(_config);

    return returnConfig;
}

int Config::get(const std::string& key) {

    auto conf = _config.find(key);

    if (conf == _config.end()) {
        ESP_LOGE(MODULE, "Get Config key[%s] not exist", key.c_str());
        return INT32_MAX;
    }

    return conf->second.value;
}