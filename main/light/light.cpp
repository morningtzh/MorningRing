//
// Created by MorningTZH on 2023/1/20.
//

#include <memory>
#include "light.hpp"

static const char *MODULE = "Light";

namespace light {

#define RMT_LED_STRIP_RESOLUTION_HZ 10000000 // 10MHz resolution, 1 tick = 0.1us (led strip needs a high resolution)
#define RMT_LED_STRIP_GPIO_NUM      13

#define EXAMPLE_LED_NUMBERS         24
#define EXAMPLE_CHASE_SPEED_MS      100

    Manager::Manager() {
        InitRMT();
    }

    std::list<std::string> Manager::GetModeList() {
        std::list<std::string> list;

        for (auto &mode: modeList) {
            list.emplace_back(mode.first);
        }

        return list;
    }

    std::string Manager::GetMode() {
        return mode.name;
    }

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

    bool Manager::SetModeConfig(std::string name, mode::Config &config) {
        return false;
    }

    void RenderNext(void *m) {
        auto mode = (mode::Mode *) m;

        while (true) {
            mode->CalculateNext();

            manager()->TxRMT();

            vTaskDelay(pdMS_TO_TICKS(EXAMPLE_CHASE_SPEED_MS));

            // TODO: 设置引脚
            // https://github.com/espressif/esp-idf/blob/master/examples/peripherals/rmt/led_strip/main/led_strip_example_main.c
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
        modeList[name] = ModeInfo{
                name,
                c,
                d
        };

        ESP_LOGI(MODULE, "Light Mode[%s] Registered", name.c_str());
    }

    void Manager::InitRMT() {
        ESP_LOGI(MODULE, "Create RMT TX channel");
        rmt_tx_channel_config_t tx_chan_config = {
                .gpio_num = RMT_LED_STRIP_GPIO_NUM,
                .clk_src = RMT_CLK_SRC_DEFAULT, // select source clock
                .resolution_hz = RMT_LED_STRIP_RESOLUTION_HZ,
                .mem_block_symbols = 512, // increase the block size can make the LED less flickering
                .trans_queue_depth = 4, // set the number of transactions that can be pending in the background
        };
        ESP_ERROR_CHECK(rmt_new_tx_channel(&tx_chan_config, &rmt.led_chan));

        ESP_LOGI(MODULE, "Install led strip encoder");
        led_strip_encoder_config_t encoder_config = {
                .resolution = RMT_LED_STRIP_RESOLUTION_HZ,
        };
        ESP_ERROR_CHECK(rmt_new_led_strip_encoder(&encoder_config, &rmt.led_encoder));

        ESP_LOGI(MODULE, "Enable RMT TX channel");
        ESP_ERROR_CHECK(rmt_enable(rmt.led_chan));

        ESP_LOGI(MODULE, "Start LED rainbow chase");
//        rmt_transmit_config_t tx_config = {
//                .loop_count = 0, // no transfer loop
//        };
    }

    const float POWER_LIMIT = 1;

    void Manager::TxRMT() {

        // int i = 0;
        // ESP_LOGI(MODULE, "buffer %d %2x%2x%2x", i / 3, lightBuffer.inside[i], lightBuffer.inside[i+1], lightBuffer.inside[i+2]);

        uint8_t buf[LIGHT_INSIDE_POINTS * 3];

        for (int i = 0; i < LIGHT_INSIDE_POINTS * 3; i++) {
            buf[i] = lightBuffer.inside[i] * POWER_LIMIT;
        }

        // Flush RGB values to LEDs
        ESP_ERROR_CHECK(rmt_transmit(rmt.led_chan, rmt.led_encoder, buf, sizeof(lightBuffer.inside), &rmt.tx_config));

    }


    Manager *manager() {
        static class Manager *m = nullptr;
        if (!m) {
            m = new class Manager;
        }

        return m;
    }
}
