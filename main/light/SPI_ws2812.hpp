/*
 * SPI_ws2812.h
 *
 *  Created on: 02-Nov-2020
 *      Author: Dhananjay Khairnar
 */

#ifndef MAIN_SPI_WS2812_H_
#define MAIN_SPI_WS2812_H_

#include <stdio.h>
#include <string.h>
#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "common.hpp"
struct SPISettings {
    spi_host_device_t host;
    spi_device_handle_t spi;
    int dma_chan;
    spi_device_interface_config_t devcfg;
    spi_bus_config_t buscfg;
} ;

#define MAX_POWER_LIMIT 0.1

class Ws2812Spi {

private:
    int pin;
    int ledNum;
    int dmaSize;
    light::RGB *buffer;
    uint16_t *ledDMAbuffer;
    float powerLimit = MAX_POWER_LIMIT;

    static bool spiInit;

    SPISettings spiSettings{
            .host = HSPI_HOST,
            .dma_chan = SPI_DMA_CH_AUTO,
            .devcfg {
                    .command_bits = 0,
                    .address_bits = 0,
                    .mode = 0, //SPI mode 0
                    .clock_speed_hz = static_cast<int>(3.2 * 1000 * 1000), //Clock out at 3.2 MHz
                    .spics_io_num = -1, //CS pin
                    .queue_size = 1, //Not sure if needed
            },
            .buscfg {
                    .mosi_io_num = 0,
                    .miso_io_num = -1,
                    .sclk_io_num = -1,
                    .quadwp_io_num = -1,
                    .quadhd_io_num = -1,
                    .max_transfer_sz = 0,
            },
    };
public:
    Ws2812Spi(int pin, int ledNum, light::RGB *buffer);
    void led_strip_update();
    void SetPower(float powerPercent) {
        powerLimit = powerPercent * MAX_POWER_LIMIT;
    }
};


#endif /* MAIN_SPI_WS2812_H_ */
