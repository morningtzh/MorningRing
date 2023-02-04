/*
 * SPI_ws2812.c
 *
 *  Created on: 02-Nov-2020
 *      Author: Dhananjay Khairnar
 */

#include "SPI_ws2812.hpp"

#define POWER_LIMIT 0.1

int SPI_USED = 0;

Ws2812Spi::Ws2812Spi(int pin, int ledNum, light::RGB *buffer) : pin(pin), ledNum(ledNum),
                                                                dmaSize(((ledNum * 16 * (24 / 4))) + 1),
                                                                buffer(buffer) {

    spiSettings.buscfg.mosi_io_num = pin;
    spiSettings.buscfg.max_transfer_sz = dmaSize;
    spiSettings.host = SPI_USED == 0 ? SPI2_HOST : SPI3_HOST;
    spiSettings.dma_chan = SPI_USED == 0 ? SPI_DMA_CH1 : SPI_DMA_CH2;
    SPI_USED++;

    esp_err_t err;

    err = spi_bus_initialize(spiSettings.host, &spiSettings.buscfg, spiSettings.dma_chan);
    ESP_ERROR_CHECK(err);

    //Attach the Accel to the SPI bus
    err = spi_bus_add_device(spiSettings.host, &spiSettings.devcfg, &spiSettings.spi);
    ESP_ERROR_CHECK(err);

    ledDMAbuffer = (uint16_t *) heap_caps_malloc(dmaSize, MALLOC_CAP_DMA); // Critical to be DMA memory.
}

void Ws2812Spi::led_strip_update() {
    uint16_t LedBitPattern[16] = {
            0x8888,
            0x8C88,
            0xC888,
            0xCC88,
            0x888C,
            0x8C8C,
            0xC88C,
            0xCC8C,
            0x88C8,
            0x8CC8,
            0xC8C8,
            0xCCC8,
            0x88CC,
            0x8CCC,
            0xC8CC,
            0xCCCC
    };
    uint32_t i;
    esp_err_t err;

    memset(ledDMAbuffer, 0, dmaSize);
    int n = 0;
    for (i = 0; i < ledNum; i++) {

        auto r = (uint8_t)(buffer[i].r * POWER_LIMIT);
        auto g = (uint8_t)(buffer[i].g * POWER_LIMIT);
        auto b = (uint8_t)(buffer[i].b * POWER_LIMIT);

        //R
        ledDMAbuffer[n++] = LedBitPattern[0x0f & (r >> 4)];
        ledDMAbuffer[n++] = LedBitPattern[0x0f & (r)];

        //G
        ledDMAbuffer[n++] = LedBitPattern[0x0f & (g >> 4)];
        ledDMAbuffer[n++] = LedBitPattern[0x0f & (g)];

        //B
        ledDMAbuffer[n++] = LedBitPattern[0x0f & (b >> 4)];
        ledDMAbuffer[n++] = LedBitPattern[0x0f & (b)];

    }

    spi_transaction_t t;
    memset(&t, 0, sizeof(t));
    t.length = dmaSize * 8; //length is in bits
    t.tx_buffer = ledDMAbuffer;

    err = spi_device_transmit(spiSettings.spi, &t);
    ESP_ERROR_CHECK(err);
}