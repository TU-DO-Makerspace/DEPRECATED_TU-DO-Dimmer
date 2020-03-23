#include <LEDStrip.h>

LEDStrip::LEDStrip()
{

}

LEDStrip::LEDStrip(uint8_t pin) : _pin(pin)
{
        pinMode(_pin, OUTPUT);
        set(0);
}

void LEDStrip::set(uint8_t brightness)
{
        _brightness = brightness;
        analogWrite(_pin, _brightness);
}

uint8_t LEDStrip::get()
{
        return _brightness;
}

RGBStrip::RGBStrip()
{

}

#if RGB_STRIP_TYPE == ADDRESSABLE 

RGBStrip::RGBStrip(unsigned int leds, uint8_t din)
{
        _rgbstrp = new NeoPixelBus <NeoGrbFeature, Neo800KbpsMethod> (leds , din);
        _rgbstrp->Begin();
}

RGBStrip::~RGBStrip()
{
        delete _rgbstrp;
        _rgbstrp = NULL;
}

void RGBStrip::set(RgbColor rgb)
{
        _rgbstrp->ClearTo(rgb);
        _rgbstrp->Show();
}

RgbColor RGBStrip::get()
{
        return _rgbstrp->GetPixelColor(0);
}

#else

RGBStrip::RGBStrip(uint8_t pin_r, uint8_t pin_g, uint8_t pin_b) : _pin_r(pin_r), _pin_g(pin_g), _pin_b(pin_b)
{
        set(0, 0, 0);        
}

RGBStrip::~RGBStrip()
{
        
}

void RGBStrip::set(RgbColor rgb)
{
        _rgb = rgb;
        
        analogWrite(_pin_r, _rgb.R);
        analogWrite(_pin_g, _rgb.G);
        analogWrite(_pin_b, _rgb.B);
} 

#endif