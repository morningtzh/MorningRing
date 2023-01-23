//
// Created by MorningTZH on 2023/1/20.
//

#include "light.hpp"

#include <memory>

static const char *MODULE = "Light";

namespace light {

#define EXAMPLE_CHASE_SPEED_MS 10

    Manager::Manager() :
            inside2812(GPIO_NUM_13, LIGHT_INSIDE_POINTS, lightBuffer.inside),
            outside2812(GPIO_NUM_12, LIGHT_OUTSIDE_POINTS, lightBuffer.outside) {}

    std::list<std::string> Manager::GetModeList() {
        std::list<std::string> list;

        for (auto &mode: modeList) {
            list.emplace_back(mode.first);
        }

        return list;
    }

    std::string Manager::GetMode() { return mode.name; }

    bool Manager::SetMode(std::string name) {
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

        ESP_LOGI(MODULE, "Set Mode to %s", name.c_str());

        return true;
    }

    std::unique_ptr<mode::Config> Manager::GetModeConfig(const std::string &name) {
        return std::make_unique<mode::Config>();
    }

    bool Manager::SetModeConfig(std::string name, mode::Config &config) { return false; }

    [[noreturn]] void RenderNext(void *m) {
        auto mode = (mode::Mode *) m;

        while (true) {
            mode->CalculateNext();

            manager()->TxWs2812();

            vTaskDelay(pdMS_TO_TICKS(EXAMPLE_CHASE_SPEED_MS));
        }
    }

    void Manager::Start() {
        xTaskCreatePinnedToCore(RenderNext, "RenderLight", 100 * 1024, mode.mode, 0, &renderTaskHandle, 0);
    }

    void Manager::Stop() {
        if (renderTaskHandle == nullptr) {
            return;
        }

        vTaskDelete(renderTaskHandle);
        renderTaskHandle = nullptr;
    }

    void Manager::RegisterMode(const std::string &name, mode::Creator c, mode::Destroyer d) {
        modeList[name] = ModeInfo{name, c, d};

        ESP_LOGI(MODULE, "Light Mode[%s] Registered", name.c_str());
    }

    void Manager::TxWs2812() {
        inside2812.led_strip_update();
        outside2812.led_strip_update();
    }

    Manager *manager() {
        static class Manager *m = nullptr;
        if (!m) {
            m = new class Manager;
        }

        return m;
    }
}  // namespace light
