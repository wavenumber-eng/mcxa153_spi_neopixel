#ifndef WS2812_H
#define WS2812_H

#include "stdint.h"

typedef struct{
    uint8_t r;
    uint8_t g;
    uint8_t b;
} rgb_pixel_t;

extern void ws2812_serialize_strip(rgb_pixel_t * led_strip, uint32_t strip_length, uint8_t *spi_data);

#endif