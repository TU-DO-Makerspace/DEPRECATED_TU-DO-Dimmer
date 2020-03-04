    #include <Arduino.h>
    #include "PushButton.h"

    PushButton::PushButton(uint8_t pin, bool pullup) : _pin(pin) , _pullup(pullup) {   
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