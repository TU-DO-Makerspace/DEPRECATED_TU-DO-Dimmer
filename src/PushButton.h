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
   * Description: A hardware abstraction class for push buttons 
   * 
   */

#pragma once

#include <stdint.h>

class PushButton
{
    uint8_t _pin;
    bool _pullup;
    bool _prev_state;
    
public:    
    PushButton();
    PushButton(uint8_t pin, bool pullup);
    bool state();
    bool released();
};