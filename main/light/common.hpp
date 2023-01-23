//
// Created by MorningTZH on 2023/1/21.
//

#ifndef MORNINGRING_COMMON_HPP
#define MORNINGRING_COMMON_HPP

#include <list>

#include "esp_log.h"
#include "fast_hsv2rgb.h"

#define INSIDE_RING 0
#define OUTSIDE_RING 1

#define CLOCKWISE 0
#define COUNTERCLOCKWISE 1

namespace light {

    const int LIGHT_INSIDE_POINTS = 182;
    const int LIGHT_OUTSIDE_POINTS = 184;

    struct RGB {
        uint8_t r,g,b;
    };

    struct LightBuffer {
        // RGB三倍buffer
        RGB inside[LIGHT_INSIDE_POINTS];
        RGB outside[LIGHT_OUTSIDE_POINTS];

        void SetRGB(int side, int index, uint8_t r, uint8_t g, uint8_t b) {
            RGB *buffer = side == INSIDE_RING ? inside : outside;

            buffer[index].r = r;
            buffer[index].g = g;
            buffer[index].b = b;
        }

        void GetRGB(int side, int index, uint8_t &r, uint8_t &g, uint8_t &b) {
            RGB *buffer = side == INSIDE_RING ? inside : outside;

            r = buffer[index].r;
            g = buffer[index].g;
            b = buffer[index].b;
        }

        void SetHSV(int side, int index, uint16_t h, uint8_t s, uint8_t v) {
            uint8_t r, g, b;
            fast_hsv2rgb_8bit(h, s, v, &r, &g, &b);

            RGB *buffer = side == INSIDE_RING ? inside : outside;

            buffer[index].r = r;
            buffer[index].g = g;
            buffer[index].b = b;
        }

        // 旋转buffer，但step不为1的情况下视觉效果较差。
        void Rotate(int side, int direction = CLOCKWISE, int step = 1) {
            RGB *buffer = side == INSIDE_RING ? inside : outside;
            std::list<RGB> pool;
            int len = side == INSIDE_RING ? LIGHT_INSIDE_POINTS : LIGHT_OUTSIDE_POINTS;

            if (direction == CLOCKWISE) {

                for (int i = len - 1; i > 0; i--) {

                    if (pool.size() < step) {
                        pool.push_back(buffer[i]);
                        buffer[i] = buffer[i-step];
                    }else {
                        buffer[i] = buffer[i-step];

                    }
                }

                for (int i = step - 1; i >= 0; i--) {
                    buffer[i] = pool.front();
                    pool.pop_front();
                }
            } else {
                for (int i = 0; i < len - step; i++) {
                    if (pool.size() < step) {
                        pool.push_back(buffer[i]);
                        buffer[i] = buffer[i+step];
                    } else {
                        buffer[i] = buffer[i+step];
                    }
                }

                for (int i = len - step; i < len; i++) {
                    buffer[i] = pool.front();
                    pool.pop_front();
                }
            }
        }
    };
}


#endif //MORNINGRING_COMMON_HPP
