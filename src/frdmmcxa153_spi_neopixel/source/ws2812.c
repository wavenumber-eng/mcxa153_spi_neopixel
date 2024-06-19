#include "ws2812.h"



#define ZERO_FRAME  0b11000000
#define ONE_FRAME   0b11110000
#define BIT(X)      (1<<X)

// Serialize an 8-bit color into an equivalent sequence
static inline void ws2812_serialize_color(uint8_t buf[8], uint8_t color)
{
	for (uint8_t i = 0; i < 8; i++) {
		buf[i] = color & BIT(7 - i) ? ONE_FRAME : ZERO_FRAME;
	}
}


void ws2812_serialize_strip(rgb_pixel_t * led_strip, uint32_t strip_length, uint8_t *spi_data){
    uint32_t offset = 0;

    for(uint8_t i = 0; i < strip_length; i++){

        // color order GRB
        ws2812_serialize_color(spi_data + (offset), led_strip[i].g);
        ws2812_serialize_color(spi_data + (offset + 8), led_strip[i].r);
        ws2812_serialize_color(spi_data + (offset + 16), led_strip[i].b);
        offset+=24;
    }
}