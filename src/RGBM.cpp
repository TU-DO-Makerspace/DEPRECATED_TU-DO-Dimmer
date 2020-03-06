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
   * Description: Methods/Function definitions for the RGBM and PreciseRGBM classes
   * 
   */

#include <RGBM.h>

#define UNINITIALIZED_DOUBLE -1

RGBM::RGBM() {

}

RGBM::RGBM(RgbColor rgb, uint8_t m) {
    _rgb = rgb;
    _m = m;
}

RgbColor RGBM::rgb() {
    return _rgb;
}

uint8_t RGBM::m() {
    return _m;
}

uint8_t RGBM::r() {
    return _rgb.R;
}

uint8_t RGBM::b() {
    return _rgb.B;
}

uint8_t RGBM::g() {
    return _rgb.G;
}

void RGBM::set_r(uint8_t r) {
    _rgb.R = r;
}

void RGBM::set_g(uint8_t g) {
    _rgb.G = g;
}

void RGBM::set_b(uint8_t b) {
    _rgb.G = b;
}

void RGBM::set_rgb(RgbColor rgb) {
    _rgb = rgb;
}

void RGBM::set_rgb(uint8_t r, uint8_t g, uint8_t b) {
    _rgb.R = r;
    _rgb.G = g;
    _rgb.B = b;
}

void RGBM::set_rgbm(RgbColor rgb, uint8_t m) {
    _rgb = rgb;
    _m = m;
}

void RGBM::set_rgbm(uint8_t r, uint8_t g, uint8_t b, uint8_t m) {
    _rgb.R = r;
    _rgb.G = g;
    _rgb.B = b;
    _m = m;
}

String RGBM::hexcode() {
    String rgbm_hex[4] { String(r(), HEX), String(g(), HEX), String(b(), HEX), String(_m, HEX) };
        
    for (uint8_t i = 0; i < 3; i++) {
        if (rgbm_hex[i].length() == 1) {
            rgbm_hex[i] = "0" + rgbm_hex[i];
        }
    }
    

    return '#' + rgbm_hex[0] + rgbm_hex[1] + rgbm_hex[2] + rgbm_hex[3];
}

PreciseRGBM::PreciseRGBM() {
    _r_double = UNINITIALIZED_DOUBLE;
    _g_double = UNINITIALIZED_DOUBLE;
    _b_double = UNINITIALIZED_DOUBLE;
}

PreciseRGBM::PreciseRGBM(RgbColor rgb, uint8_t m) : RGBM(rgb, m) {
    _r_double = rgb.R;
    _g_double = rgb.G;
    _b_double = rgb.B;
    _m_double = m;
}

void PreciseRGBM::set_r(uint8_t r) {
    _r_double = _rgb.R = r;
}

void PreciseRGBM::set_g(uint8_t g) {
    _g_double = _rgb.G = g;
}

void PreciseRGBM::set_b(uint8_t b) {
    _b_double = _rgb.B = b;
}

void PreciseRGBM::set_rgb(RgbColor rgb) {
    _rgb = rgb;
    _r_double = rgb.R;
    _g_double = rgb.G;
    _b_double = rgb.B;
}

void PreciseRGBM::set_rgb(uint8_t r, uint8_t g, uint8_t b) {
    _r_double = _rgb.R = r;
    _g_double = _rgb.G = g;
    _b_double = _rgb.B = b;
}

void PreciseRGBM::set_rgbm(RgbColor rgb, uint8_t m) {
    _rgb = rgb;
    _m = m;
    _rgb = rgb;
    _r_double = rgb.R;
    _g_double = rgb.G;
    _b_double = rgb.B;
    _m_double = m;
}

void PreciseRGBM::set_rgbm(uint8_t r, uint8_t g, uint8_t b, uint8_t m) {
    _r_double = _rgb.R = r;
    _g_double = _rgb.G = g;
    _b_double = _rgb.B = b;
    _m_double = _m = m;
}

void PreciseRGBM::inc_rgb_bright() {
    if (_r_double == UNINITIALIZED_DOUBLE || 
        _g_double == UNINITIALIZED_DOUBLE ||
        _b_double == UNINITIALIZED_DOUBLE ||
        _m_double == UNINITIALIZED_DOUBLE) {
            set_rgbm(_rgb, _m);
    }
    _rgb.R = (_r_double *= double(1) + double(1/255));
    _rgb.G = (_g_double *= double(1) + double(1/255));
    _rgb.B = (_b_double *= double(1) + double(1/255));    
}

void PreciseRGBM::dec_rgb_bright() {
    if (_r_double == UNINITIALIZED_DOUBLE || 
        _g_double == UNINITIALIZED_DOUBLE ||
        _b_double == UNINITIALIZED_DOUBLE ||
        _m_double == UNINITIALIZED_DOUBLE) {
            set_rgbm(_rgb, _m);
    }
    _rgb.R = (_r_double *= double(1) - double(1/255));
    _rgb.G = (_g_double *= double(1) - double(1/255));
    _rgb.B = (_b_double *= double(1) - double(1/255));    
}