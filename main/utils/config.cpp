//
// Created by MorningTZH on 2023/2/4.
//

#include "config.hpp"

#include <nvs_flash.h>
#include <memory>
#include "esp_log.h"

static const char *MODULE = "Config";

template<typename T>
esp_err_t SetNvs(nvs_handle_t handle, const char* key, T value) {
    esp_err_t err;

    if (std::is_same<T, int16_t>::value){
        err = nvs_set_i16 ( handle, key,  value);
    }
    if (std::is_same<T, uint16_t>::value){
        err = nvs_set_u16 ( handle, key,  value);
    }
    if (std::is_same<T, int32_t>::value){
        err = nvs_set_i32 ( handle, key,  value);
    }
    if (std::is_same<T, uint32_t>::value){
        err = nvs_set_u32 ( handle, key,  value);
    }
    if (std::is_same<T, int64_t>::value){
        err = nvs_set_i64 ( handle, key,  value);
    }
    if (std::is_same<T, uint64_t>::value){
        err = nvs_set_u64 ( handle, key,  value);
    }
    if (std::is_same<T, const char *>::value){
        err = nvs_set_str ( handle, key, value);
    }

    return err;
}
esp_err_t SetNvs(nvs_handle_t handle, const char* key, std::any value) {
    esp_err_t err;

    if (value.type() == typeid(int16_t)){
        err = nvs_set_i16 (handle, key, std::any_cast<int16_t> (value));
    }
    if (value.type() == typeid(uint16_t)){
        err = nvs_set_u16 (handle, key, std::any_cast<uint16_t> (value));
    }
    if (value.type() == typeid(int32_t)){
        err = nvs_set_i32 (handle, key, std::any_cast<int32_t> (value));
    }
    if (value.type() == typeid(uint32_t)){
        err = nvs_set_u32 (handle, key, std::any_cast<uint32_t> (value));
    }
    if (value.type() == typeid(int64_t)){
        err = nvs_set_i64 (handle, key, std::any_cast<int64_t> (value));
    }
    if (value.type() == typeid(uint64_t)){
        err = nvs_set_u64 (handle, key, std::any_cast<uint64_t> (value));
    }
    if (value.type() == typeid(char *)){
        err = nvs_set_str (handle, key, std::any_cast<char*> (value));
    }
    if (value.type() == typeid(std::string)){
        err = nvs_set_str (handle, key, std::any_cast<std::string> (value).c_str());
    }

    return err;
}

esp_err_t GetNvs(nvs_handle_t handle, const char* key, std::any &value) {
    esp_err_t err;

    if (value.type() == typeid(int16_t)){
        int16_t tmp;
        err = nvs_get_i16 (handle, key, &tmp);
        value = tmp;
    }
    if (value.type() == typeid(uint16_t)){
        uint16_t tmp;
        err = nvs_get_u16 (handle, key, &tmp);
        value = tmp;
    }
    if (value.type() == typeid(int32_t)){
        int32_t tmp;
        err = nvs_get_i32 (handle, key, &tmp);
        value = tmp;
    }
    if (value.type() == typeid(uint32_t)){
        uint32_t tmp;
        err = nvs_get_u32 (handle, key, &tmp);
        value = tmp;
    }
    if (value.type() == typeid(int64_t)){
        int64_t tmp;
        err = nvs_get_i64 (handle, key, &tmp);
        value = tmp;
    }
    if (value.type() == typeid(uint64_t)){
        uint64_t tmp;
        err = nvs_get_u64 (handle, key, &tmp);
        value = tmp;
    }
    if (value.type() == typeid(char *)){
        size_t length;
        char tmp[64];
        err = nvs_get_str (handle, key, tmp, &length);
        value = tmp;
    }

    if (value.type() == typeid(std::string)){
        size_t length;
        char tmp[64];
        err = nvs_get_str (handle, key, tmp, &length);
        value = std::string(tmp);
    }

    return err;
}

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
        std::any storage_value = c.second.value;

        _config[key] = c.second;

        std::string hwKey = std::string{}.append(configPrefix).append("_").append(shortKey);

        err = GetNvs(my_handle, hwKey.c_str(), storage_value);
        if (err != ESP_OK) {
            ESP_LOGE(MODULE, "Error nvs (%s) reading!\n", esp_err_to_name(err));
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
        const std::any &value = c.second.value;

        // 不存在的key就跳过
        if (_config.find(key) == _config.end()) {
            ESP_LOGW(MODULE, "Set Config has invalid key[%s], skip.", key.c_str());
            continue;
        }

        // 写入NVS
        err = SetNvs(my_handle, (configPrefix + "_" + shortKey).c_str(), value);
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

std::any Config::get(const std::string& key) {

    auto conf = _config.find(key);

    if (conf == _config.end()) {
        ESP_LOGE(MODULE, "Get Config key[%s] not exist", key.c_str());
        return INT32_MAX;
    }

    return conf->second.value;
}

template<typename T>
T Config::get(const std::string& key) {

    auto conf = _config.find(key);

    if (conf == _config.end()) {
        ESP_LOGE(MODULE, "Get Config key[%s] not exist", key.c_str());
        return INT32_MAX;
    }

    return any_cast<T>(conf->second.value);
}

bool Config::set(const std::string& key, std::any value) {

    ConfigData tmp {
            {key, {value}}
    };

    return set(tmp);
}

template<typename T>
bool Config::set(const std::string& key, T value) {
    std::any v = value;
    return set(key, value);
}