#include "PatchEncoder.h"

PatchEncoder::PatchEncoder()
{

}

PatchEncoder::PatchEncoder(uint8_t dt, uint8_t clk, uint8_t sw, unsigned long debounce_time) : _debounce_time(debounce_time)
{
        _debounce = false;
        _sw = PushButton(sw, true);
        _rotary_enc = new Encoder(dt, clk);
        _pos = _rotary_enc->read();
}

PatchEncoder::~PatchEncoder()
{
        delete _rotary_enc;
        _rotary_enc = NULL;
}

encoder_action PatchEncoder::action()
{
        if (_debounce && millis() >= _debounce_tstamp) {
                encoder_action ret = no_action;
                _debounce = false;

                long read = _rotary_enc->read();
                
                if (read > _pos)
                        ret = right;
                else if (read < _pos)
                        ret = left;

                _pos = read;

                return ret;
        } else if (!_debounce && _rotary_enc->read() != _pos) {
                _debounce = true;
                _debounce_tstamp = millis() + _debounce_time;
        } else if (_sw.released()) {
                return pressed;
        }

        return no_action;
}