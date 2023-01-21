//
// Created by MorningTZH on 2023/1/21.
//

#ifndef MORNINGRING_COMMON_HPP
#define MORNINGRING_COMMON_HPP

#include "fast_hsv2rgb.h"

namespace light {
    struct LightBuffer {
        uint8_t inside[182];
        uint8_t outside[184];

        void SetRGB(int index, uint8_t r, uint8_t g, uint8_t b) {
            inside[index * 3 + 0] = r;
            inside[index * 3 + 1] = g;
            inside[index * 3 + 2] = b;
        }

        void GetRGB(int index, uint8_t &r, uint8_t &g, uint8_t &b) {
            r = inside[index * 3 + 0];
            g = inside[index * 3 + 1];
            b = inside[index * 3 + 2];
        }

        void SetHSV(int index, uint16_t h, uint8_t s, uint8_t v) {
            uint8_t r, g, b;
            fast_hsv2rgb_8bit(h, s, v, &r, &g, &b);

            inside[index * 3 + 0] = r;
            inside[index * 3 + 1] = g;
            inside[index * 3 + 2] = b;
        }
    };


}



#endif //MORNINGRING_COMMON_HPP
