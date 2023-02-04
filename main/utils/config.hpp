//
// Created by MorningTZH on 2023/2/4.
//

#ifndef MORNINGRING_CONFIG_HPP
#define MORNINGRING_CONFIG_HPP

#include <map>
#include <string>
#include <memory>

struct ConfigItem {
    int32_t value;
};

using ConfigData = std::map<std::string, ConfigItem>;


class Config {
    std::string configPrefix;
    ConfigData _config;

public:
    Config(const std::string &configPrefix, const ConfigData &defaultConfig);

    bool set(ConfigData &newConfig);

    std::unique_ptr<ConfigData> all();

    int get(const std::string& key);
};


#endif //MORNINGRING_CONFIG_HPP
