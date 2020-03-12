#pragma once

#include <Encoder.h>

#include "PushButton.h"

enum encoder_action {
        no_action,
        pressed,
        left,
        right
};

class PatchEncoder {
        PushButton _sw;
        Encoder *_rotary_enc;
        
        unsigned long _debounce_time;
        long _pos;

        bool _debounce;
        unsigned long _debounce_tstamp;
        
        public:
        
        PatchEncoder();
        PatchEncoder(uint8_t clk, uint8_t dt, uint8_t sw, unsigned long debounce_time);
        ~PatchEncoder();

        encoder_action action();
};