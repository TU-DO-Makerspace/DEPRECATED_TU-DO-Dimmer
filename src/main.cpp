  /*
   * TU-DO Disco - Firmware for our custom TU-DO Makespace Cafe lights dimmer 
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
   */

 // TODO: 
 //  - RgbColor -> rgbm
 //  - Implement Control for main light via hex
 //  - Load EEPROM into patch array
 //  - Boot MSG
 //  - RGB Brightness controll

#include <math.h>

#include <Arduino.h>
#include <NeoPixelBus.h>

#include "PatchIndicator.h"
#include "PushButton.h"

#ifndef __AVR__
#error Sorry, only AVR boards are currently supported
#endif

////////////////////////////
// Pins/Hardware Parameters
////////////////////////////

// Pots
#define R_POT A5
#define G_POT A4
#define B_POT A3
#define M_POT A2 // Main light

// Rotary encoder
#define BRIGHT_ENC A1

// LED Strips
#define MAIN_STRIP 3
#define RGB_STRIP 2
#define RGB_STRIP_LEDS 8

// Buttons (Must share same Pin Bank!)

/* For more information regarding the pin change
 * interrupt values, consult chapter 12 of the
 * Atmega328 datasheet, as well as the following
 * Arduino to AVR conversion diagrams:
 *  - https://www.arduino.cc/en/Hacking/PinMapping168
 *  - http://marcusjenkins.com/arduino-pinout-diagrams/
 * */

#define PATCH_UP_BTN 13
#define PATCH_DWN_BTN 12
#define PATCH_SAVE_BTN 11

#define PULLUP true

// 7-Segment Patch Indicator

#define SEV_SEG_COMMON 10

#define SEV_SEG_A 9
#define SEV_SEG_B 8
#define SEV_SEG_C 7
#define SEV_SEG_D 6
#define SEV_SEG_E 5
#define SEV_SEG_F 4
#define SEV_SEG_G A0

////////////////////////
// Firmware parameters
////////////////////////

// Potentiometer movement detection
#define AVG_SAMPLES 100
#define MAX_POT_MOV_DEV 6

// 7-Segment Patch Indicator

#define BLINK_INTERVAL_ON 1000
#define BLINK_INTERVAL_OFF 500
#define NUM_SAVE_BLINKS 3
#define PATCH_DISPLAY_TIME 5000

////////////////////////
// Patches
////////////////////////

struct rgbm {
  RgbColor rgb;
  uint8_t M;
};

rgbm patches[10];
uint8_t current_patch;

////////////////////////
// Functions
////////////////////////

inline uint32_t uint32_pow(uint8_t base, uint8_t pow)
{
  uint32_t ret = 1;
  for (uint8_t i = 0; i < pow; i++) {
    ret *= base;
  }
  
  return ret;
}

inline uint8_t adc_to_rgb(uint16_t val) {
  if (val == 0) {
    return 0;
  }
  else if (val < 4) {
    return 1;
  }
  return val >> 2;
}

inline uint8_t avg_pot_read(uint8_t pin, uint16_t samples)
{
  uint64_t avg = 0;

  for (uint16_t i = 0; i < samples; i++) {
    avg += analogRead(pin);
  }

  return adc_to_rgb(round(avg/samples));
}

inline rgbm avg_rgbm_pot_read(uint8_t pot_r, uint8_t pot_g, uint8_t pot_b, uint8_t pot_m, uint16_t samples)
{
  rgbm ret;
  ret.rgb.R = avg_pot_read(pot_r, samples);
  ret.rgb.G = avg_pot_read(pot_g, samples);
  ret.rgb.B = avg_pot_read(pot_b, samples);
  ret.M = avg_pot_read(pot_m, samples);
  return ret;
}

inline bool parse_hex(uint32_t *_hex, String cmdbuf)
{
  if (cmdbuf.length() != 7 || cmdbuf[0] != '#') {
    return false;
  }

  uint32_t hex = 0;

  for (size_t i = 1; i < cmdbuf.length(); i++) {
    if (cmdbuf[i] >= '0' && cmdbuf[i] <= '9') {
      hex += (cmdbuf[i] - 0x30) * uint32_pow(16, (cmdbuf.length() - 1 - i));
    }
    else if (cmdbuf[i] >= 'A' && cmdbuf[i] <= 'F') {
      hex += (cmdbuf[i] - 55) * uint32_pow(16, (cmdbuf.length() - 1 - i));
    }
    else if (cmdbuf[i] >= 'a' && cmdbuf[i] <= 'f') {
      hex += (cmdbuf[i] - 78) * uint32_pow(16, (cmdbuf.length() - 1 - i));
    }
    else {
      return false;
    }
  }

  *_hex = hex;
  return true;
}

//////////////////////////
// Global vars & Objects
//////////////////////////

// LED Strips
uint8_t mainstrp_bright;

NeoPixelBus <NeoGrbFeature, Neo800KbpsMethod> rgbstrp(RGB_STRIP_LEDS , RGB_STRIP);
RgbColor rgbstrp_color;

rgbm rgbmpots;
rgbm avg;

// Serial commandline buffer
String cmdbuf = "";

// 7 Segment patch indicator
PatchIndicator patch_indicator (
  COMMON_ANODE, 
  SEV_SEG_COMMON,
  SEV_SEG_A, 
  SEV_SEG_B, 
  SEV_SEG_C, 
  SEV_SEG_D, 
  SEV_SEG_E, 
  SEV_SEG_F, 
  SEV_SEG_G
);

// Buttons
PushButton patch_up_btn(PATCH_UP_BTN, PULLUP);
PushButton patch_dwn_btn(PATCH_DWN_BTN, PULLUP);
PushButton patch_save_btn(PATCH_SAVE_BTN, PULLUP);

// External color programming

// When set to true, the device will maintain its current color
// until potentiometer movement is detected
bool programmed = false;

///////////////////////
// Button Interrupts
///////////////////////

///////////////////////
// Color via serial
///////////////////////

void serialEvent() {
  while(Serial.available()) {
    char c = (char)Serial.read();

    if (c == 'g') {
      String rgb_hex[3] { String(rgbstrp_color.R, HEX), String(rgbstrp_color.G, HEX), String(rgbstrp_color.B, HEX) };
      
      for (uint8_t i = 0; i < 3; i++) {
        if (rgb_hex[i].length() == 1) {
          rgb_hex[i] = "0" + rgb_hex[i];
        }
      }

      Serial.println("Current Color: #" + rgb_hex[0] + rgb_hex[1] + rgb_hex[2]);
      Serial.println("R: " + String(rgbstrp_color.R));
      Serial.println("G: " + String(rgbstrp_color.G));
      Serial.println("B: " + String(rgbstrp_color.B));
      cmdbuf = "";
    }
    else if (c == '#') {
      cmdbuf = "#";
    }
    else if (cmdbuf.length() > 6) {
      uint32_t hex;
      if (parse_hex(&hex, cmdbuf)) {
        rgbstrp_color = HtmlColor(hex);
        rgbstrp.ClearTo(rgbstrp_color);
        rgbstrp.Show();

        // Read average of pots for potentiometer movement detection 
        avg = avg_rgbm_pot_read(R_POT, G_POT, B_POT, M_POT, AVG_SAMPLES);
        programmed = true;        
      }
      else {
        Serial.println("Invalid hex value!");
      }
      cmdbuf = "";
    }
    else {
      cmdbuf += c;
    }
  }
}

//////////////////////////
// Initialization
//////////////////////////

void setup()
{
  pinMode(M_POT, INPUT);
  pinMode(R_POT, INPUT);
  pinMode(G_POT, INPUT);
  pinMode(B_POT, INPUT);
  pinMode(BRIGHT_ENC, INPUT);

  pinMode(PATCH_UP_BTN, INPUT_PULLUP);
  pinMode(PATCH_DWN_BTN, INPUT_PULLUP);
  pinMode(PATCH_SAVE_BTN, INPUT_PULLUP);

  pinMode(MAIN_STRIP, OUTPUT);

  Serial.begin(9600);

  rgbstrp.Begin();

  // Load 0th patch on boot
  current_patch = 0;
  rgbstrp_color = patches[current_patch].rgb;
  mainstrp_bright = patches[current_patch].M;
  
  rgbstrp.ClearTo(rgbstrp_color);
  rgbstrp.Show();
  analogWrite(MAIN_STRIP, mainstrp_bright);

  // 7-Segment Initialization
  patch_indicator.set(0);
  patch_indicator.show(PATCH_DISPLAY_TIME);

  avg = avg_rgbm_pot_read(R_POT, G_POT, B_POT, M_POT, AVG_SAMPLES);
  programmed = true;
}

///////////////////////
// Main loop
///////////////////////

// Avoid implementing time intensive
// instructions/operations, as delays
// will reduce the smoothness of the
// color transisitons

void loop()
{
  rgbmpots.rgb.R = adc_to_rgb(analogRead(R_POT));
  rgbmpots.rgb.G = adc_to_rgb(analogRead(G_POT));
  rgbmpots.rgb.B = adc_to_rgb(analogRead(B_POT));
  rgbmpots.M = adc_to_rgb(analogRead(M_POT));

  if (!programmed || 
      abs(rgbmpots.rgb.R - avg.rgb.R) > MAX_POT_MOV_DEV ||
      abs(rgbmpots.rgb.G - avg.rgb.G) > MAX_POT_MOV_DEV ||
      abs(rgbmpots.rgb.B - avg.rgb.B) > MAX_POT_MOV_DEV ||
      abs(rgbmpots.M - avg.M) > MAX_POT_MOV_DEV) {
    rgbstrp_color = rgbmpots.rgb;
    mainstrp_bright = rgbmpots.M;
    rgbstrp.ClearTo(rgbstrp_color);
    rgbstrp.Show();
    analogWrite(MAIN_STRIP, mainstrp_bright);
    programmed = false;
  }

  bool patch_up_released = patch_up_btn.released();
  bool patch_dwn_released = patch_dwn_btn.released();

  if (patch_up_released || patch_dwn_released) {
    
    bool invalid = false;

    if (patch_up_released && current_patch < 9) {
      current_patch++;
    }
    else if (patch_dwn_released && current_patch > 0) {
      current_patch--;
    }
    else {
      invalid = true;
    }
    
    if (!invalid) {
      rgbstrp.ClearTo(patches[current_patch].rgb);
      rgbstrp.Show();
      analogWrite(MAIN_STRIP, patches[current_patch].M);
      avg = avg_rgbm_pot_read(R_POT, G_POT, B_POT, M_POT, AVG_SAMPLES);
      programmed = true;
      patch_indicator.set(current_patch);
    }

    patch_indicator.show(PATCH_DISPLAY_TIME);
  }
  else if (patch_save_btn.released()) {
    patches[current_patch].rgb = rgbstrp_color;
    patches[current_patch].M = mainstrp_bright;
    patch_indicator.blink(NUM_SAVE_BLINKS, BLINK_INTERVAL_ON, BLINK_INTERVAL_OFF);
    // TODO EEPROM STUFF
  }

  if (patch_indicator.busy()) {
    patch_indicator.update();
  }
}