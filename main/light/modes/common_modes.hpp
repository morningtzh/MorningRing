//
// Created by MorningTZH on 2023/1/22.
//

#include "../light.hpp"
#include "../mode.hpp"

namespace light::mode {

    [[maybe_unused]] static const char *MODULE = "LightMode";

/***************** HSVRing Mode *************/
    class HSVRing : public Mode {

        static ConfigData defaultConfig;

    public:
        [[maybe_unused]] explicit HSVRing(LightBuffer &lightBuffer, bool start = true) : Mode(lightBuffer, "HSVRing", defaultConfig) {
            if (start) {
                for (int i = 0; i < LIGHT_INSIDE_POINTS; i++) {
                    lightBuffer.SetHSV(INSIDE_RING, i, HSV_HUE_MAX * i / LIGHT_INSIDE_POINTS, 255, 255);
                }

                for (int i = LIGHT_OUTSIDE_POINTS - 1; i >= 0; i--) {
                    lightBuffer.SetHSV(OUTSIDE_RING, i, HSV_HUE_MAX * i / LIGHT_OUTSIDE_POINTS, 255, 255);
                }
            }
        }

        void CalculateNext() override {
            lightBuffer.Rotate(INSIDE_RING, config.get<int>("InsideRingDirect"), config.get<int>("InsideRingStep"));
            lightBuffer.Rotate(OUTSIDE_RING, config.get<int>("OutsideRingDirect"), config.get<int>("OutsideRingStep"));
        }
    };

    ConfigData HSVRing::defaultConfig{
            {"InsideRingDirect",  {CLOCKWISE}},
            {"OutsideRingDirect", {COUNTERCLOCKWISE}},
            {"InsideRingStep",    {1}},
            {"OutsideRingStep",   {2}},
    };

/***************** Test Mode *************/
    class Test : public Mode {
    private:
        int hue = 0;

    public:
        [[maybe_unused]] explicit Test(LightBuffer &lightBuffer, bool start) : Mode(lightBuffer) {
            for (int i = 0; i < LIGHT_INSIDE_POINTS; i++) {
                lightBuffer.SetHSV(INSIDE_RING, i, hue, 255, 255);
            }
        }

        void CalculateNext() override {
            for (int i = 0; i < LIGHT_INSIDE_POINTS; i++) {
                lightBuffer.SetHSV(INSIDE_RING, i, hue++, 255, 255);
            }
        }
    };

    class Single : public Mode {
    private:
        int rgb = 0;
        bool dir = true;

    public:
        [[maybe_unused]] explicit Single(LightBuffer &lightBuffer, bool start) : Mode(lightBuffer) {
            if (start) {
                for (int i = 0; i < LIGHT_INSIDE_POINTS; i++) {
                    lightBuffer.SetRGB(INSIDE_RING, i, 0, 0, 0);
                }
            }
        }

        void CalculateNext() override {

            rgb++;
            if (rgb > 255) {
                rgb = 0;
            }

            for (int i = 0; i < LIGHT_INSIDE_POINTS; i++) {
                lightBuffer.SetRGB(INSIDE_RING, i, rgb, rgb, rgb);
            }
        }
    };
}  // namespace light::mode