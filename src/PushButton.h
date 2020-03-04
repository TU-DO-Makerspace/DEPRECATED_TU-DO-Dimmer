#pragma once

#include <stdint.h>

class PushButton
{
    uint8_t _pin;
    bool _pullup;
    bool _prev_state;
    
    public:
    
    PushButton(uint8_t pin, bool pullup);
    bool state();
    bool released();
};