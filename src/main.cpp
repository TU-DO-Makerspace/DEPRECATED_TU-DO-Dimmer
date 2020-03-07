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
   * Description: Main firmware routines for our custom TU-DO Makespace Cafe lights dimmer
   * 
   */

#include <math.h>

#include <Arduino.h>
#include <EEPROM.h>
#include <NeoPixelBus.h>

#include <KY040rotary.h>

#include "config.h"
#include "PatchIndicator.h"

#ifndef __AVR__
#error Sorry, only AVR boards are currently supported
#endif

//////////////////////////////
// Structs
//////////////////////////////

struct rgbm {
        RgbColor rgb;
        uint8_t M;
};

//////////////////////////////
// Functions
//////////////////////////////

inline uint32_t uint32_pow(uint8_t base, uint8_t pow)
{
        uint32_t ret = 1;

        for (uint8_t i = 0; i < pow; i++)
        ret *= base;

        return ret;
}

inline uint8_t adc_to_rgb(uint16_t val)
{
        if (val == 0)
        return 0;
        else if (val < 4)
        return 1;

        return (val >> 2);
}

inline uint8_t avg_pot_read(uint8_t pin, uint16_t samples)
{
        uint64_t avg = 0;

        for (uint16_t i = 0; i < samples; i++)
                avg += analogRead(pin);

        return adc_to_rgb(round(avg/samples));
}

inline rgbm read_rgbm_pots(uint8_t pot_r, uint8_t pot_g, uint8_t pot_b, uint8_t pot_m)
{
        rgbm ret;
        
        ret.rgb.R = adc_to_rgb(analogRead(R_POT));
        ret.rgb.G = adc_to_rgb(analogRead(G_POT));
        ret.rgb.B = adc_to_rgb(analogRead(B_POT));
        ret.M = adc_to_rgb(analogRead(M_POT));
        
        return ret;
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

inline bool rgbm_pot_mov_det(rgbm rgbmpots, rgbm avg, uint8_t max_dev)
{
        return (
                abs(rgbmpots.rgb.R - avg.rgb.R) > max_dev ||
                abs(rgbmpots.rgb.G - avg.rgb.G) > max_dev ||
                abs(rgbmpots.rgb.B - avg.rgb.B) > max_dev ||
                abs(rgbmpots.M - avg.M) > max_dev
        );
}

inline bool hexstr_to_uint32(uint32_t *_hex, String hexstr)
{
        uint32_t hex = 0;

        for (size_t i = 0; i < hexstr.length(); i++) {
                if (hexstr[i] >= '0' && hexstr[i] <= '9')
                        hex += (hexstr[i] - 0x30) * uint32_pow(16, (hexstr.length() - i));
                else if (hexstr[i] >= 'A' && hexstr[i] <= 'F')
                        hex += (hexstr[i] - 55) * uint32_pow(16, (hexstr.length() - i));
                else if (hexstr[i] >= 'a' && hexstr[i] <= 'f')
                        hex += (hexstr[i] - 78) * uint32_pow(16, (hexstr.length() - i));
                else
                        return false;
        }

        *_hex = hex;
        return true;
}

//////////////////////////////
// Global vars & Objects
//////////////////////////////

// LED Strips
uint8_t mainstrp_bright;

NeoPixelBus <NeoGrbFeature, Neo800KbpsMethod> rgbstrp(RGB_STRIP_LEDS , RGB_STRIP);
RgbColor rgbstrp_color;

rgbm rgbmpots;
rgbm avg;

rgbm patches[10];
uint8_t current_patch;

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

// Rotary Encoder
KY040 bright_enc(BRIGHT_ENC_CLK, BRIGHT_ENC_DT, BRIGHT_ENC_SW);

// External color programming

// When set to true, the device will maintain its current color
// until potentiometer movement is detected
bool programmed = false;

///////////////////////
// Color via serial
///////////////////////

void serialEvent()
{
        while(Serial.available()) {
                char c = (char)Serial.read();

                if (c == 'g') {
                        String rgb_hex[3] { String(rgbstrp_color.R, HEX), String(rgbstrp_color.G, HEX), String(rgbstrp_color.B, HEX) };

                        for (uint8_t i = 0; i < 3; i++) {
                                if (rgb_hex[i].length() == 1)
                                        rgb_hex[i] = "0" + rgb_hex[i];
                        }

                        Serial.println("Current Color: #" + rgb_hex[0] + rgb_hex[1] + rgb_hex[2]);
                        Serial.println("R: " + String(rgbstrp_color.R));
                        Serial.println("G: " + String(rgbstrp_color.G));
                        Serial.println("B: " + String(rgbstrp_color.B));
                        cmdbuf = "";
                } else if (c == '#') {
                        cmdbuf = "#";
                } else if (cmdbuf.length() == 7 || cmdbuf.length() == 9) {
                        uint32_t rgb_hex;
                        uint32_t m_hex;

                        String rgb;
                        String m;
                        
                        if (cmdbuf[0] != '#')
                                goto INVALID_HEX;

                        rgb = cmdbuf.substring(1, 7);

                        if (!hexstr_to_uint32(&rgb_hex, rgb))
                                goto INVALID_HEX;

                        if (cmdbuf.length() == 9) {
                                m = cmdbuf.substring(7, 9);
                                
                                if (!hexstr_to_uint32(&m_hex, m))
                                        goto INVALID_HEX;
                                
                                mainstrp_bright = m_hex;
                                analogWrite(m_hex, MAIN_STRIP);
                        }
                        
                        rgbstrp_color = HtmlColor(rgb_hex);
                        rgbstrp.ClearTo(rgbstrp_color);
                        rgbstrp.Show();

                        // Read average of pots for potentiometer movement detection 
                        avg = avg_rgbm_pot_read(R_POT, G_POT, B_POT, M_POT, AVG_SAMPLES);
                        programmed = true;

                        cmdbuf = "";
                        return;

                        INVALID_HEX:
                        Serial.println("Invalid hex value!");
                        cmdbuf = "";
                } else {
                        cmdbuf += c;
                }
        }
}

//////////////////////////////
// Rotary Encoder Interrupts
//////////////////////////////

void change_patch(bool up)
{
        bool invalid = false;

        if (up && current_patch < 9)
                current_patch++;
        else if (!up && current_patch > 0)
                current_patch--;
        else
                invalid = true;
  
        if (!invalid)
        {
                rgbstrp_color = patches[current_patch].rgb;
                rgbstrp.ClearTo(rgbstrp_color);
                rgbstrp.Show();
                mainstrp_bright = patches[current_patch].M;
                analogWrite(MAIN_STRIP, mainstrp_bright);
                avg = avg_rgbm_pot_read(R_POT, G_POT, B_POT, M_POT, AVG_SAMPLES);
                programmed = true;
                patch_indicator.set(current_patch);
        }

        patch_indicator.show(PATCH_DISPLAY_TIME);
}

void patch_up()
{
        change_patch(true);
}

void patch_dwn()
{
        change_patch(false);
}

void save_patch()
{
        patches[current_patch].rgb = rgbstrp_color;
        patches[current_patch].M = mainstrp_bright;
        EEPROM.put(EEPROM_PATCH_ADDR + (sizeof(rgbm) * current_patch), patches[current_patch]);
        patch_indicator.blink(NUM_SAVE_BLINKS, BLINK_INTERVAL_ON, BLINK_INTERVAL_OFF);
}


//////////////////////////////
// Initialization
//////////////////////////////

void setup()
{
        pinMode(M_POT, INPUT);
        pinMode(R_POT, INPUT);
        pinMode(G_POT, INPUT);
        pinMode(B_POT, INPUT);

        pinMode(MAIN_STRIP, OUTPUT);

        Serial.begin(9600);
        Serial.println(String(BOOT_MSG_ASCII_ART) + "\n");
        Serial.println("Author(s): " + String(BOOT_MSG_AUTHORS));
        Serial.println("License: " + String(BOOT_MSG_LICENSE));
        Serial.println("Build date: " + String(__DATE__));
        Serial.println("Documentation: " + String(BOOT_MSG_SRC));
        rgbstrp.Begin();

        // Load patches from EEPROM into ram
        EEPROM.get(EEPROM_PATCH_ADDR, patches);

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

        // Rotary Encoder Initialization
        bright_enc.Begin();
        bright_enc.OnButtonRight(patch_up);
        bright_enc.OnButtonLeft(patch_dwn);
        bright_enc.OnButtonClicked(save_patch);
}

//////////////////////////////
// Main loop
//////////////////////////////

// Avoid implementing time intensive
// instructions/operations, as delays
// will reduce the smoothness of the
// color transisitons

void loop()
{
        rgbmpots = read_rgbm_pots(R_POT, G_POT, B_POT, M_POT);

        if (!programmed || rgbm_pot_mov_det(rgbmpots, avg, MAX_POT_MOV_DEV)) {
                rgbstrp_color = rgbmpots.rgb;
                mainstrp_bright = rgbmpots.M;
                rgbstrp.ClearTo(rgbstrp_color);
                rgbstrp.Show();
                analogWrite(MAIN_STRIP, mainstrp_bright);
                programmed = false;
        }

        bright_enc.Process(millis());

        if (patch_indicator.busy())
                patch_indicator.update();
}