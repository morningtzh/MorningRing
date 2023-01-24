//
// Created by MorningTZH on 2023/1/22.
//

#include "light.hpp"
#include "mode.hpp"

namespace light::mode {

    static const char *MODULE = "LightMode";

/***************** HSVRing Mode *************/
    class HSVRing : public Mode {

        static Config defaultConfig;

    public:
        explicit HSVRing(LightBuffer &lightBuffer, bool start = true) : Mode(lightBuffer, "HSVRing", defaultConfig) {
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
            lightBuffer.Rotate(INSIDE_RING, GetConfig("InsideRingDirect"), GetConfig("InsideRingStep"));
            lightBuffer.Rotate(OUTSIDE_RING, GetConfig("OutsideRingDirect"), GetConfig("OutsideRingStep"));
        }
    };

    Config HSVRing::defaultConfig{
            {"InsideRingDirect", {CLOCKWISE}},
            {"OutsideRingDirect", {COUNTERCLOCKWISE}},
            {"InsideRingStep", {1}},
            {"OutsideRingStep", {2}},
    };

/***************** Test Mode *************/
    class Test : public Mode {
    private:
        int hue = 0;

    public:
        explicit Test(LightBuffer &lightBuffer, bool start) : Mode(lightBuffer) {
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
        explicit Single(LightBuffer &lightBuffer, bool start) : Mode(lightBuffer) {
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