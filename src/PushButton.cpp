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
   * Description: Method/Function definitions for the PushButton class
   * 
   */
    
    #include <Arduino.h>
    #include "PushButton.h"

    PushButton::PushButton(uint8_t pin, bool pullup) : _pin(pin) , _pullup(pullup) {
        pinMode(pin, (pullup) ? INPUT_PULLUP : INPUT);
        _prev_state = state();
    }

    bool PushButton::state() {
        return _pullup ? !digitalRead(_pin) : digitalRead(_pin); 
    }

    bool PushButton::released() {
        bool current_state = state();
        if (current_state == false && _prev_state == true) {
           _prev_state = false;
           return true;
        }
        _prev_state = current_state;
        return false;
    }