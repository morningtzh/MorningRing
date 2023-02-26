//
// Created by MorningTZH on 2023/2/26.
//

#include <esp_mac.h>
#include <cstring>
#include <esp_app_desc.h>
#include "info.hpp"

namespace System {

    ConfigData System::defaultConfig{
            {"Name",  {std::string("MorningRing")}},
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

    System::System():selfInfo(), config("Sys", defaultConfig) {
        selfInfo.name = config.get<std::string>("Name");
        selfInfo.version = esp_app_get_description()->version;
        esp_base_mac_addr_get((uint8_t*)&selfInfo.id);

        selfInfo.self = true;
        selfInfo.renewTime = UINT64_MAX;
    }

    const DeviceInfo & System::GetInfo() {
        return selfInfo;
    }

    const std::list<DeviceInfo> &System::GetDevices() {

        time_t outtime = time(nullptr) - 2 * DEVICE_RENEW_TIME;

        // 借此清理一波设备
        for (auto deviceI = devices.begin(); deviceI != devices.end();) {
            if (!deviceI->self && deviceI->renewTime < outtime) {

                auto tmp = deviceI;
                deviceI = deviceI++;
                devices.erase(tmp);
                continue;
            }

            deviceI++;
        }

        return devices;
    }

    bool System::SetDevice(const DeviceInfo &device) {

        // 借此清理一波设备
        for (auto deviceI = devices.begin(); deviceI != devices.end();) {
            if (deviceI->id == device.id) {
                deviceI->name = device.name;
                deviceI->version = device.version;
                deviceI->renewTime = time(nullptr);
            }

            deviceI++;
        }

        return false;
    }


} // System