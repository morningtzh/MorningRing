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
#include "common.hpp"
#include "fast_hsv2rgb.h"

class string;
namespace light::mode {
    using Config = std::map<std::string, int>;

    class Mode {
    private:
        Config config;
        bool dirty = true;

    protected:
        LightBuffer &lightBuffer;
    public:
        Mode(LightBuffer &lightBuffer) : lightBuffer(lightBuffer) {};

        virtual bool SetConfig(Config &newConfig) {
            return true;
        };

        virtual std::unique_ptr<Config> GetConfig() {
            return std::make_unique<Config>();
        };

        virtual void CalculateNext() = 0;

        virtual ~Mode() = default;

    };

    using Creator = std::function<Mode *(light::LightBuffer &)>;
    //typedef Mode *(*Creator)();

    typedef void (*Destroyer)(Mode *);

    template<typename T>
    Mode *CreateMode(light::LightBuffer &lightBuffer) {
        return new T(lightBuffer);
    };
}


#endif //MORNINGRING_MODE_HPP