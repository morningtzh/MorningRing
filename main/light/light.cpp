//
// Created by MorningTZH on 2023/1/20.
//

#include <memory>
#include "light.hpp"

namespace light {
    manager *manager() {
        static class manager *m = nullptr;
        if (!m) {
            m = new class manager;
        }

        return m;
    }

    std::list<std::string> manager::GetModeList() {

        std::list<std::string> list;

        for (auto &mode : modeList) {
            list.emplace_back(mode.first);
        }

        return list;
    }

    bool manager::SetMode(std::string name) {
        auto m = modeList.find(name);
        if (m == modeList.end()) {
            ESP_LOGE(MODULE, "Set ModeInfo[%s] Failed, it's not exist", name.c_str());
            return false;
        }

        Stop();

        delete mode.mode;

        mode = m->second;
        mode.mode = mode.creator(lightBuffer);

        Start();

        return true;
    }

    std::unique_ptr<mode::Config> manager::GetModeConfig(const std::string& name) {

        return std::make_unique<mode::Config>();
    }

    bool manager::SetModeConfig(std::string name, mode::Config &config) {
        return false;
    }

    void RenderNext(void *m) {
        auto mode = (mode::Mode *)m;

        while (true) {
            mode->CalculateNext();

            // TODO: 设置引脚
            // https://github.com/espressif/esp-idf/blob/master/examples/peripherals/rmt/led_strip/main/led_strip_example_main.c
        }
    }

    void manager::Start() {
        xTaskCreate(RenderNext, "RenderLight", 100 * 1024, mode.mode, 5, &renderTaskHandle);
    }

    void manager::Stop() {
        if (renderTaskHandle == nullptr) {
            return;
        }

        vTaskDelete(renderTaskHandle);
        renderTaskHandle = nullptr;
    }



}
