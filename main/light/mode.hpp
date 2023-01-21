//
// Created by MorningTZH on 2023/1/21.
//

#ifndef MORNINGRING_MODE_HPP
#define MORNINGRING_MODE_HPP

#include <string>
#include <map>
#include <list>
#include <memory>
#include "common.hpp"
#include "fast_hsv2rgb.h"

class string;
namespace light::mode {
        using Config = std::map<std::string, int>;

        class Mode {
        private:
            Config config;

        protected:
            LightBuffer &lightBuffer;
        public:
            Mode(LightBuffer &lightBuffer) : lightBuffer(lightBuffer) {
            };

            virtual bool SetConfig(Config &newConfig) = 0;

            virtual std::unique_ptr<Config> GetConfig() = 0;

            virtual void CalculateNext() = 0;

            virtual ~Mode() = default;

        };

        typedef Mode *(*Creator)(light::LightBuffer &);

        typedef void (*Destroyer)(Mode *);

        /***************** HSLRing Mode *************/

        class HSLRing : Mode {
        public:
            HSLRing(LightBuffer lightBuffer) : Mode(lightBuffer) {
                for (int i = 0; i < 182; i++) {
                    lightBuffer.SetHSV(i, 360 * i / 182, 255, 255);
                }
            }

            void CalculateNext() override {

                uint8_t r, g, b;
                lightBuffer.GetRGB(0, r, g, b);

                for (int i = 0; i < 182 - 1; i++) {
                    uint8_t r, g, b;
                    lightBuffer.GetRGB(i + 1, r, g, b);
                    lightBuffer.SetRGB(i, r, g, b);
                }

                lightBuffer.SetRGB(182 - 1, r, g, b);
            }
        };

    }


#endif //MORNINGRING_MODE_HPP
