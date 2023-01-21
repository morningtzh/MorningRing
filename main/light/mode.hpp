//
// Created by MorningTZH on 2023/1/21.
//

#ifndef MORNINGRING_MODE_HPP
#define MORNINGRING_MODE_HPP

#include <string>
#include <map>
#include <list>
#include <memory>
#include "light.hpp"
#include "fast_hsv2rgb.h"

class string;
namespace light {
    namespace mode {
        using Config = std::map<std::string, int>;

        class Mode {
        private:
            Config config;
            LightBuffer &lightBuffer;

        public:
            Mode(LightBuffer &lightBuffer) : lightBuffer(lightBuffer) {
            };

            virtual bool SetConfig(Config &newConfig) = 0;

            virtual std::unique_ptr<Config> GetConfig() = 0;

            virtual void CalculateNext() = 0;

            virtual ~Mode() = default;

            void SetRGB(int index, uint8_t r, uint8_t g, uint8_t b) {
                lightBuffer.inside[index * 3 + 0] = r;
                lightBuffer.inside[index * 3 + 1] = g;
                lightBuffer.inside[index * 3 + 2] = b;
            }

            void GetRGB(int index, uint8_t &r, uint8_t &g, uint8_t &b) {
                r = lightBuffer.inside[index * 3 + 0];
                g = lightBuffer.inside[index * 3 + 1];
                b = lightBuffer.inside[index * 3 + 2];
            }

            void SetHSV(int index, uint16_t h, uint8_t s, uint8_t v) {
                uint8_t r, g, b;
                fast_hsv2rgb_8bit(h, s, v, &r, &g, &b);

                lightBuffer.inside[index * 3 + 0] = r;
                lightBuffer.inside[index * 3 + 1] = g;
                lightBuffer.inside[index * 3 + 2] = b;
            }

        };

        typedef Mode *(*Creator)(light::LightBuffer &lightBuffer);

        typedef void (*Destroyer)(Mode *);

        /***************** HSLRing Mode *************/

        class HSLRing : Mode {
        public:
            HSLRing(LightBuffer lightBuffer) : Mode(lightBuffer) {
                for (int i = 0; i < 182; i++) {
                    SetHSV(i, 360 * i / 182, 255, 255);
                }
            }

            void CalculateNext() override {

                uint8_t r, g, b;
                GetRGB(0, r, g, b);

                for (int i = 0; i < 182 - 1; i++) {
                    uint8_t r, g, b;
                    GetRGB(i + 1, r, g, b);
                    SetRGB(i, r, g, b);
                }

                SetRGB(182 - 1, r, g, b);
            }
        };
    }
}


#endif //MORNINGRING_MODE_HPP
