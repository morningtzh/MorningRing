//
// Created by MorningTZH on 2023/1/22.
//

#include "light.hpp"
#include "mode.hpp"

namespace light::mode {

    static const char *MODULE = "Light";

    /***************** HSVRing Mode *************/
    class HSVRing : public Mode {
    public:
        explicit HSVRing(LightBuffer &lightBuffer) : Mode(lightBuffer) {
            for (int i = 0; i < LIGHT_INSIDE_POINTS; i++) {
                lightBuffer.SetHSV(i, HSV_HUE_MAX * i / LIGHT_INSIDE_POINTS, 255, 255);
            }

            // int i = 0;
            // ESP_LOGI(MODULE, "HSVRing Start buffer %d %2x%2x%2x", i / 3, lightBuffer.inside[i], lightBuffer.inside[i+1], lightBuffer.inside[i+2]);
        }

        void CalculateNext() override {

            uint8_t r, g, b;
            lightBuffer.GetRGB(0, r, g, b);

            for (int i = 0; i < LIGHT_INSIDE_POINTS - 1; i++) {
                uint8_t r, g, b;
                lightBuffer.GetRGB(i + 1, r, g, b);
                lightBuffer.SetRGB(i, r, g, b);
            }

            lightBuffer.SetRGB(LIGHT_INSIDE_POINTS - 1, r, g, b);

            // int i = 0;
            // ESP_LOGI(MODULE, "HSVRing buffer %d %2x%2x%2x", i / 3, lightBuffer.inside[i], lightBuffer.inside[i+1], lightBuffer.inside[i+2]);

        }
    };


    /***************** HSVRing Mode *************/
    class Test : public Mode {
    private:
        int hue = 0;
    public:
        explicit Test(LightBuffer &lightBuffer) : Mode(lightBuffer) {
            for (int i = 0; i < LIGHT_INSIDE_POINTS; i++) {
                lightBuffer.SetHSV(i, hue, 255, 255);
            }

            // int i = 0;
            // ESP_LOGI(MODULE, "HSVRing Start buffer %d %2x%2x%2x", i / 3, lightBuffer.inside[i], lightBuffer.inside[i+1], lightBuffer.inside[i+2]);
        }

        void CalculateNext() override {

            for (int i = 0; i < LIGHT_INSIDE_POINTS; i++) {
                lightBuffer.SetHSV(i, hue++, 255, 255);
            }
            // int i = 0;
            // ESP_LOGI(MODULE, "HSVRing buffer %d %2x%2x%2x", i / 3, lightBuffer.inside[i], lightBuffer.inside[i+1], lightBuffer.inside[i+2]);

        }
    };

    class Single : public Mode {
    private:
        int rgb = 0;
        bool dir = true;
    public:
        explicit Single(LightBuffer &lightBuffer) : Mode(lightBuffer) {
            for (int i = 0; i < LIGHT_INSIDE_POINTS; i++) {
                lightBuffer.SetRGB(i, 0, 0, 0);
            }

            // int i = 0;
            // ESP_LOGI(MODULE, "HSVRing Start buffer %d %2x%2x%2x", i / 3, lightBuffer.inside[i], lightBuffer.inside[i+1], lightBuffer.inside[i+2]);
        }

        void CalculateNext() override {
            rgb++;
            if (rgb > 50) {
                rgb = 0;
            }

            for (int i = 0; i < LIGHT_INSIDE_POINTS; i++) {
                lightBuffer.SetRGB(i, rgb, rgb, rgb);
            }


            // int i = 0;
            // ESP_LOGI(MODULE, "HSVRing buffer %d %2x%2x%2x", i / 3, lightBuffer.inside[i], lightBuffer.inside[i+1], lightBuffer.inside[i+2]);

        }
    };
}