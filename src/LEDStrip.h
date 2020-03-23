#pragma once

#include <NeoPixelBus.h>
#include "config.h"

#define ADDRESSABLE 0
#define NON_ADDRESSABLE 1

#ifndef RGB_STRIP_TYPE
#error "RGB_STRIP_TYPE is undefined. Please specify whether a addressable or non-addressable strip is used!"
#endif

class LEDStrip {
        uint8_t _pin;
        uint8_t _brightness;

public:
        LEDStrip();
        LEDStrip(uint8_t pin);

        void set(uint8_t brightness);
        uint8_t get();
};

class RGBStrip {
#if RGB_STRIP_TYPE == ADDRESSABLE
        NeoPixelBus <NeoGrbFeature, Neo800KbpsMethod> *_rgbstrp; // Driver for RGB light strip (See https://github.com/Makuna/NeoPixelBus/wiki)

public:
        RGBStrip(unsigned int leds, uint8_t din);
        ~RGBStrip();
#else
        uint8_t _pin_r, _pin_g, _pin_b;
        RgbColor _rgb;

public:
        RGBStrip(uint8_t pin_r, uint8_t pin_g, uint8_t b);
#endif

public:
        RGBStrip();
        void set(RgbColor rgb);
        RgbColor get();   
};