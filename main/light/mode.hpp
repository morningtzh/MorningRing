//
// Created by MorningTZH on 2023/1/21.
//

#ifndef MORNINGRING_MODE_HPP
#define MORNINGRING_MODE_HPP

#include <string>
#include <map>
#include <list>
#include <memory>
#include <functional>
#include <utility>
#include <nvs_flash.h>
#include "common.hpp"
#include "fast_hsv2rgb.h"

class string;
namespace light::mode {

    struct ConfigItem {
        int32_t value;
    };

    using Config = std::map<std::string, ConfigItem>;

    class Mode {
    private:

        bool isSupportConfig = false;
        std::string configPrefix;
        Config config;

        bool dirty = true;

    protected:
        LightBuffer &lightBuffer;
    public:
        Mode(LightBuffer &lightBuffer);

        Mode(LightBuffer &lightBuffer, std::string configPrefix, const Config& defaultConfig);

        virtual ~Mode();

        void InitConfig(const Config &defaultConfig);

        virtual bool SetConfig(Config &newConfig);

        virtual std::unique_ptr<Config> GetConfig();

        int GetConfig(std::string key);

        virtual void CalculateNext() = 0;

    };

    using Creator = std::function<Mode *(light::LightBuffer &, bool start)>;
    //typedef Mode *(*Creator)();

    typedef void (*Destroyer)(Mode *);

    template<typename T>
    Mode *CreateMode(light::LightBuffer &lightBuffer, bool start = true) {
        return new T(lightBuffer, start);
    };
}

#endif //MORNINGRING_MODE_HPP