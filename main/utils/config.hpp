//
// Created by MorningTZH on 2023/2/4.
//

#ifndef MORNINGRING_CONFIG_HPP
#define MORNINGRING_CONFIG_HPP

#include <map>
#include <string>
#include <memory>
#include <any>

struct ConfigItem {
    std::any value;


};

struct ConfigItem2 {
    std::any value;
};

using ConfigData = std::map<std::string, ConfigItem>;

class Config {
    std::string configPrefix;
    ConfigData _config;

public:
    Config(const std::string &configPrefix, const ConfigData &defaultConfig);

    bool set(ConfigData &newConfig);

    std::unique_ptr<ConfigData> all();

    std::any get(const std::string &key);

    template<typename T>
    T get(const std::string &key);

    bool set(const std::string &key, std::any value);

    template<typename T>
    bool set(const std::string &key, T value);

};

#endif //MORNINGRING_CONFIG_HPP
