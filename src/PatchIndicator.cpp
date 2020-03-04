#include <Arduino.h>
#include "PatchIndicator.h"

PatchIndicator::PatchIndicator() {

}

PatchIndicator::PatchIndicator(bool config, uint8_t common, uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint8_t e, uint8_t f, uint8_t g) :
_config(config), _common(common) {
    _segments[0] = a;
    _segments[1] = b;
    _segments[2] = c;
    _segments[3] = d;
    _segments[4] = e;
    _segments[5] = f;
    _segments[6] = g;
    
    pinMode(_common, OUTPUT);

    for (uint8_t i = 0; i < 7; i++) {
        pinMode(_segments[i], OUTPUT);
    }
    
    select(false);
}

void PatchIndicator::set(uint8_t num) {
    for (uint8_t i = 0; i < 7; i++) {
        digitalWrite(_segments[i], (_config == COMMON_CATHODE) ? digits[num][i] : !digits[num][i]);
    }
}

void PatchIndicator::select(bool select) {
    _state = select;
    digitalWrite(_common, (_config == COMMON_ANODE) ? select : !select);
}

void PatchIndicator::toggle() {
    _state = !_state;
    digitalWrite(_common, (_config == COMMON_ANODE) ? _state : !_state);
}

bool PatchIndicator::busy() {
    return _busy;
}

void PatchIndicator::show(unsigned long duration) {
    _blinks = 0;
    _show = true;
    select(true);
    _show_tstamp = millis() + duration;
    _busy = true;
}

void PatchIndicator::blink(uint8_t blinks, unsigned long interval_on, unsigned long interval_off) {
    _show = false;
    _blinks = blinks;
    _blink_interval_on = interval_on;
    _blink_interval_off = interval_off;
    select(true);
    _blink_tstamp = millis() + interval_on;
    _busy = true;
}

void PatchIndicator::update() {
    if (_show && _show_tstamp <= millis()) {
        select(false);
        _show = false;
        _busy = false;
    }
    else if (_blinks > 0 && _blink_tstamp <= millis()) {
        toggle();
        
        if (_state == false) {
            _blink_tstamp = millis() + _blink_interval_off;
            _blinks--;
        } else {
            _blink_tstamp = millis() + _blink_interval_on;
        }

        _busy = (_blinks > 0);
    }
}
