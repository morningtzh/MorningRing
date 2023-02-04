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
        [[maybe_unused]] mode::Destroyer destroier = nullptr;
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

        [[nodiscard]] std::string GetMode() const;

        bool SetMode(const std::string& name);

        std::unique_ptr<ConfigData> GetModeConfig(const std::string &name);

        bool SetModeConfig(const std::string& name, ConfigData &config);

        void Start();

        void Stop();

        void RegisterMode(const std::string &name, mode::Creator c, mode::Destroyer d);

        void TxWs2812();
    };

    Manager *manager();
}

#define REGISTER_LIGHT_MODE(name) { \
    light::manager()->RegisterMode(#name, light::mode::CreateMode<light::mode::name>, nullptr); \
}

#endif //MORNINGRING_LIGHT_HPP
