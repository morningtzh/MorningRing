//
// Created by MorningTZH on 2023/2/5.
//

#ifndef MORNINGRING_SYSTEM_CONFIG_HPP
#define MORNINGRING_SYSTEM_CONFIG_HPP

#include "config.hpp"
#include "global.hpp"

const ConfigData defaultSystemConfig {
        {"HumanDetect", {1}},
        {"AutoStart1", {INT32_MAX}},
        {"AutoStart2", {INT32_MAX}},
        {"AutoStart3", {INT32_MAX}},
        {"AutoStart4", {INT32_MAX}},
        {"AutoStart5", {INT32_MAX}},
        {"AutoEnd1", {INT32_MAX}},
        {"AutoEnd2", {INT32_MAX}},
        {"AutoEnd3", {INT32_MAX}},
        {"AutoEnd4", {INT32_MAX}},
        {"AutoEnd5", {INT32_MAX}},
};

extern Config SystemConfig;

#endif //MORNINGRING_SYSTEM_CONFIG_HPP
