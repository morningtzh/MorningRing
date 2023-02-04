//
// Created by MorningTZH on 2023/1/20.
//

#include "light.hpp"

#include <memory>
#include <utility>

static const char *MODULE = "Light";

namespace light {

#define EXAMPLE_CHASE_SPEED_MS 10

    Manager::Manager() :
            inside2812(GPIO_NUM_13, LIGHT_INSIDE_POINTS, lightBuffer.inside),
            outside2812(GPIO_NUM_12, LIGHT_OUTSIDE_POINTS, lightBuffer.outside) {}

    std::list<std::string> Manager::GetModeList() {
        std::list<std::string> list;

        for (auto &tmpMode: modeList) {
            list.emplace_back(tmpMode.first);
        }

        return list;
    }

    std::string Manager::GetMode() const { return mode.name; }

    bool Manager::SetMode(const std::string& name) {
        auto m = modeList.find(name);
        if (m == modeList.end()) {
            ESP_LOGE(MODULE, "Set ModeInfo[%s] Failed, it's not exist", name.c_str());
            return false;
        }

        Stop();

        delete mode.mode;

        mode = m->second;
        mode.mode = mode.creator(lightBuffer, true);

        Start();

        ESP_LOGI(MODULE, "Set Mode to %s", name.c_str());

        return true;
    }

    std::unique_ptr<ConfigData> Manager::GetModeConfig(const std::string &name) {

        if (modeList.find(name) == modeList.end()) {
            ESP_LOGE(MODULE, "Get Mode[%s] Config not exist", name.c_str());
            return nullptr;
        }

        auto tmpMode = mode.name == name ? mode.mode : modeList[name].creator(lightBuffer, false);

        return tmpMode->config.all();
    }

    bool Manager::SetModeConfig(const std::string& name, ConfigData &config) {
        if (modeList.find(name) == modeList.end()) {
            ESP_LOGE(MODULE, "Set Mode[%s] Config not exist", name.c_str());
            return false;
        }

        auto tmpMode = mode.name == name ? mode.mode : modeList[name].creator(lightBuffer, false);

        return tmpMode->config.set(config);
    }

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
        modeList[name] = ModeInfo{name, std::move(c), d};

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
