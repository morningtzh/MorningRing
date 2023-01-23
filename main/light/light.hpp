//
// Created by MorningTZH on 2023/1/20.
//

#ifndef MORNINGRING_LIGHT_HPP
#define MORNINGRING_LIGHT_HPP

#include <string>
#include <map>
#include <list>
#include <utility>
#include "driver/rmt_tx.h"
#include "utils/led_strip_encoder.h"

#include "restful.hpp"
#include "mode.hpp"
#include "common.hpp"

namespace light {

    struct RmtInfo {
        rmt_channel_handle_t led_chan = nullptr;
        rmt_encoder_handle_t led_encoder = nullptr;
        rmt_transmit_config_t tx_config{
                .loop_count = 0, // no transfer loop
        };
    };
    struct ModeInfo {
        std::string name;
        mode::Creator creator = nullptr;
        mode::Destroyer destroier = nullptr;
        mode::Mode *mode = nullptr;
    };

    class Manager {
    private:
        std::map<std::string, ModeInfo> modeList;
        ModeInfo mode;
        LightBuffer lightBuffer{};
        TaskHandle_t renderTaskHandle = nullptr;
        RmtInfo rmt;

    public:
        Manager();

        std::list<std::string> GetModeList();

        std::string GetMode();

        bool SetMode(std::string name);

        std::unique_ptr<mode::Config> GetModeConfig(const std::string &name);

        bool SetModeConfig(std::string name, mode::Config &config);

        void Start();

        void Stop();

        void RegisterMode(const std::string &name, mode::Creator c, mode::Destroyer d);

        void InitRMT();

        void TxRMT();
    };

    Manager *manager();

    class LightModeRegister {
    public:
        LightModeRegister(const std::string &name, mode::Creator c, mode::Destroyer d) {
            manager()->RegisterMode(name, std::move(c), d);
        }
    };
}

#define REGISTER_LIGHT_MODE(name) { \
    light::manager()->RegisterMode(#name, light::mode::CreateMode<light::mode::name>, nullptr); \
}

#define LIGHT_MODE_REGISTER(name)  \
    LightModeRegister lightModeRegister##name(#name, CreateMode<name>, nullptr);

#endif //MORNINGRING_LIGHT_HPP
