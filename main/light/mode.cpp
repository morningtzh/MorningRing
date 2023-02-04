//
// Created by MorningTZH on 2023/1/21.
//

#include "mode.hpp"

[[maybe_unused]] static const char *MODULE = "LightMode";

namespace light::mode {

    Mode::Mode(LightBuffer &lightBuffer) :    lightBuffer(lightBuffer),
    config("", ConfigData{}){}

    Mode::Mode(LightBuffer &lightBuffer, const std::string& configPrefix, const ConfigData& defaultConfig) :
    lightBuffer(lightBuffer),
    config(configPrefix, defaultConfig){}

    Mode::~Mode() = default;;

}


