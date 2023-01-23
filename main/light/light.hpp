//
// Created by MorningTZH on 2023/1/20.
//

#ifndef MORNINGRING_LIGHT_HPP
#define MORNINGRING_LIGHT_HPP

#include <string>
#include <map>
#include <list>
#include <utility>

#include "restful.hpp"
#include "mode.hpp"
#include "common.hpp"
#include "SPI_ws2812.hpp"


namespace light {

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
        TaskHandle_t renderTaskHandle = nullptr;

        LightBuffer lightBuffer{};
        Ws2812Spi inside2812;
        Ws2812Spi outside2812;

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

        void InitWs2812();

        void TxWs2812();
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
