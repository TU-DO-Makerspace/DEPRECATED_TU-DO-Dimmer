#include <math.h>

#include <Arduino.h>
#include <NeoPixelBus.h>

#define MAIN_POT 0
#define R_POT A5
#define G_POT A5
#define B_POT A5

#define MAIN_STRIP 1
#define RGB_STRIP 12

// Potentiometer movement detection
#define AVG_SAMPLES 100
#define MAX_POT_MOV_DEV 6

inline uint32_t uint32_pow(uint8_t base, uint8_t pow)
{
  uint32_t ret = 1;
  for (uint8_t i = 0; i < pow; i++)
  {
    ret *= base;
  }
  
  return ret;
}

inline uint8_t adc_to_rgb(uint16_t val)
{
  if (val == 0) {
    return 0;
  }
  else if (val < 4)
  {
    return 1;
  }
  return val >> 2;
}

inline uint8_t avg_pot_read(uint8_t pin, uint16_t samples)
{
  uint64_t avg = 0;

  for (uint16_t i = 0; i < samples; i++)
  {
    avg += analogRead(pin);
  }

  return adc_to_rgb(round(avg/samples));
}

inline bool parse_hex(NeoPixelBus <NeoGrbFeature, Neo800KbpsMethod> *stripe, String cmd_buffer)
{
  if (cmd_buffer.length() != 7 || cmd_buffer[0] != '#')
  {
    return false;
  }

  uint32_t hex = 0;

  for (size_t i = 1; i < cmd_buffer.length(); i++)
  {
    if (cmd_buffer[i] >= '0' && cmd_buffer[i] <= '9')
    {
      hex += (cmd_buffer[i] - 0x30) * uint32_pow(16, (cmd_buffer.length() - 1 - i));
    }

    else if (cmd_buffer[i] >= 'A' && cmd_buffer[i] <= 'F')
    {
      hex += (cmd_buffer[i] - 55) * uint32_pow(16, (cmd_buffer.length() - 1 - i));
    }

    else
    {
      return false;
    }
  }

  stripe->ClearTo(HtmlColor(hex));
  stripe->Show();
  return true;
}

NeoPixelBus <NeoGrbFeature, Neo800KbpsMethod> strip(31, RGB_STRIP);
RgbColor strip_color(0, 0, 0);

uint8_t m, r, g, b;
uint8_t r_avg, g_avg, b_avg;

String cmd_buffer = "";
bool hex_parsed = false;

// RgbColor patches[10];

void setup()
{
  pinMode(MAIN_POT, INPUT);
  pinMode(R_POT, INPUT);
  pinMode(G_POT, INPUT);
  pinMode(B_POT, INPUT);
  pinMode(MAIN_STRIP, OUTPUT);

  Serial.begin(9600);

  strip.Begin();
  
  strip_color.R = adc_to_rgb(analogRead(R_POT));
  strip_color.G = adc_to_rgb(analogRead(G_POT));
  strip_color.B = adc_to_rgb(analogRead(B_POT));
  
  strip.ClearTo(strip_color);

  analogWrite(MAIN_STRIP, analogRead(MAIN_POT));
  strip.Show();

  // Indicator that patchbay is unitialized

}

void loop()
{
  m = adc_to_rgb(analogRead(MAIN_POT));
  r = adc_to_rgb(analogRead(R_POT));
  g = adc_to_rgb(analogRead(G_POT));
  b = adc_to_rgb(analogRead(B_POT));

  analogWrite(MAIN_STRIP, m);

  if (!hex_parsed || 
      abs(r - r_avg) > MAX_POT_MOV_DEV ||
      abs(g - g_avg) > MAX_POT_MOV_DEV ||
      abs(b - b_avg) > MAX_POT_MOV_DEV
  )
  {
    hex_parsed = false;
    strip_color.R = r;
    strip_color.G = g;
    strip_color.B = b;
    strip.ClearTo(strip_color);
    strip.Show();
  }
}

void serialEvent() {
  while(Serial.available())
  {
    char c = (char)Serial.read();

    if (c == '#')
    {
      cmd_buffer = "#";
    }
    else if (cmd_buffer.length() > 6)
    {
      if (parse_hex(&strip, cmd_buffer))
      {
        // Read average of pots for potentiometer movement detection 
        r_avg = avg_pot_read(R_POT, AVG_SAMPLES);
        g_avg = avg_pot_read(G_POT, AVG_SAMPLES);
        b_avg = avg_pot_read(B_POT, AVG_SAMPLES);

        hex_parsed = true;
        
        // // Compensate for any changes in supply voltage to avoid
        // // false positive on pot movement detection
      }
      else
      {
        Serial.println("Invalid hex value!");
      }
      cmd_buffer = "";
    }
    else
    {
      cmd_buffer += c;
    }
  }
}