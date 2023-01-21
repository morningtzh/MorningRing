//
// Created by MorningTZH on 2023/1/20.
//

#ifndef MORNINGRING_LIGHT_HPP
#define MORNINGRING_LIGHT_HPP

#include <string>
#include <map>
#include <list>
#include "restful.hpp"
#include "mode.hpp"
#include "common.hpp"

namespace light {

    struct ModeInfo {
        std::string name;
        mode::Creator creator;
        mode::Destroyer destroier;
        mode::Mode *mode;
    };

    class manager {
    private:
        std::map<std::string, ModeInfo> modeList;
        ModeInfo mode;
        LightBuffer lightBuffer;
        TaskHandle_t renderTaskHandle = nullptr;

    public:
        manager() {
            memset(&lightBuffer, 0, sizeof(lightBuffer));
        };

        std::list<std::string> GetModeList();
        bool SetMode(std::string name);
        std::unique_ptr<mode::Config> GetModeConfig(const std::string& name);
        bool SetModeConfig(std::string name, mode::Config &config);

        void RegisterMode(const std::string& name, mode::Creator c, mode::Destroyer d) {
            modeList[name] = ModeInfo{
                name,
                c,
                d,
                nullptr
            };
        }

        void Start();
        void Stop();
    };
}





#endif //MORNINGRING_LIGHT_HPP
