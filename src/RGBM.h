  /*
   * Copyright (C) 2020  Patrick Pedersen, The TU-DO Makespace

   * This program is free software: you can redistribute it and/or modify
   * it under the terms of the GNU General Public License as published by
   * the Free Software Foundation, either version 3 of the License, or
   * (at your option) any later version.

   * This program is distributed in the hope that it will be useful,
   * but WITHOUT ANY WARRANTY; without even the implied warranty of
   * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   * GNU General Public License for more details.

   * You should have received a copy of the GNU General Public License
   * along with this program.  If not, see <https://www.gnu.org/licenses/>.
   * 
   * Author: Patrick Pedersen <ctx.xda@gmail.com>
   * Description: Wrappers around the NeoPixel RGB objects with some additional implementations
   * 
   */

#pragma once

#include <Arduino.h>
#include <NeoPixelBus.h>

class RGBM {
    protected:
    RgbColor _rgb;
    uint8_t _m;

    public:
    RGBM();
    RGBM(RgbColor rgb, uint8_t m);

    uint8_t r();
    uint8_t g();
    uint8_t b();
    uint8_t m();
    RgbColor rgb();
    
    void set_r(uint8_t r);
    void set_g(uint8_t g);
    void set_b(uint8_t b);
    void set_rgb(RgbColor rgb);
    void set_rgb(uint8_t r, uint8_t g, uint8_t b);
    void set_rgbm(RgbColor rgb, uint8_t m);
    void set_rgbm(uint8_t r, uint8_t g, uint8_t b, uint8_t m);

    String hexcode();    
};

class PreciseRGBM : public RGBM {
    private:
        double _r_double, _g_double, _b_double, _m_double;
    
    public:
        PreciseRGBM();
        PreciseRGBM(RgbColor rgb, uint8_t m);

        void operator = (RGBM &rgbm) {
            set_rgbm(rgbm.rgb(), rgbm.m());
        }

        virtual void set_r(uint8_t r);
        virtual void set_g(uint8_t r);
        virtual void set_b(uint8_t r);
        virtual void set_rgb(RgbColor rgb);
        virtual void set_rgb(uint8_t r, uint8_t g, uint8_t b);
        virtual void set_rgbm(RgbColor rgb, uint8_t m);      
        virtual void set_rgbm(uint8_t r, uint8_t g, uint8_t b, uint8_t m);

        void inc_rgb_bright();
        void dec_rgb_bright();
};