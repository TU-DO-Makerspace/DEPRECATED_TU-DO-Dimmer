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
   * Description: A hardware abstraction class for the 7-segment patch indicator
   * 
   */

#pragma once

#include <stdint.h>

#define COMMON_ANODE 0
#define COMMON_CATHODE 1

class PatchIndicator
{
    bool digits[10][7] {
        // 0
        {
            true, true, true, true, true, true, false
        },
        // 1
        {
            false, true, true, false, false, false, false
        },
        // 2
        {
            true, true, false, true, true, false, true
        },
        // 3
        {
            true, true, true, true, false, false, true
        },
        // 4
        {
            false, true, true, false, false, true, true
        },
        // 5
        {
            true, false, true, true, false, true, true
        },
        // 6
        {
            true, false, true, true , true, true, true
        },
        // 7
        {
            true, true, true, false, false, false, false
        },
        // 8
        {
            true, true, true, true, true, true, true
        },
        // 9
        {
            true, true, true, true, false, true, true
        }
    };

    bool _state;
    bool _config;
    uint8_t _common;
    uint8_t _segments[7];

    bool _busy = false;
    
    bool _show = false;
    unsigned long _show_tstamp;
    

    uint8_t _blinks;
    unsigned long _blink_interval_on, _blink_interval_off;
    unsigned long _blink_tstamp;

    void select(bool select);
    void toggle();

    public:
    
    PatchIndicator();
    PatchIndicator(bool config, uint8_t common, uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint8_t e, uint8_t f, uint8_t g);

    void set(uint8_t num);
    bool busy();
    void update();
    void blink(uint8_t blinks, unsigned long interval_on, unsigned long interval_off);
    void show(unsigned long duration);
};
