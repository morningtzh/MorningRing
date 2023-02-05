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
#include "utils/config.hpp"
#include "fast_hsv2rgb.h"

class string;
namespace light::mode {

    class Mode {
    private:

        bool dirty = true;

    protected:
        LightBuffer &lightBuffer;
    public:

        Config config;

        explicit Mode(LightBuffer &lightBuffer);

        Mode(LightBuffer &lightBuffer, const std::string& configPrefix, const ConfigData& defaultConfig);

        virtual ~Mode();

        virtual void CalculateNext() = 0;
    };

    using Creator = std::function<Mode *(light::LightBuffer &, bool start)>;

    typedef void (*Destroyer)(Mode *);

    template<typename T>
    Mode *CreateMode(light::LightBuffer &lightBuffer, bool start = true) {
        return new T(lightBuffer, start);
    };
}

#endif //MORNINGRING_MODE_HPP