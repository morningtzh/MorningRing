//
// Created by MorningTZH on 2023/2/26.
//

#ifndef MORNINGRING_INFO_HPP
#define MORNINGRING_INFO_HPP

#include <string>
#include <list>
#include "utils/config.hpp"

// 一分钟更新一次设备
const uint DEVICE_RENEW_TIME = 60;

namespace System {
    struct DeviceInfo {
        uint64_t id;
        std::string name;
        std::string version;

        bool self;
        bool needResponse;
        time_t renewTime;
    };

    class System {
        DeviceInfo selfInfo;
        std::list<DeviceInfo> devices;

        static ConfigData defaultConfig;

    public:
        Config config;

        System();

        const DeviceInfo &GetInfo();
        const std::list<DeviceInfo> &GetDevices();
        bool SetDevice(const DeviceInfo &device);

    };

} // System

#endif //MORNINGRING_INFO_HPP
