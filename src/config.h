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
   * Description: Configuration file for the TU-DO custom light dimmer firmware
   * 
   */

#pragma once

#include "PatchIndicator.h"

///////////////////////////
// Hardware Parameters
///////////////////////////

// Pots
#define R_POT A4
#define G_POT A3
#define B_POT A2
#define M_POT A5 // Main light

// Rotary encoder
#define ROTARY_ENC_DT  2 // INT 0
#define ROTARY_ENC_CLK 3 // INT 1
#define ROTARY_ENC_SW  A0

// LED Strips
#define MAIN_STRIP     5
#define RGB_STRIP      A1
#define RGB_STRIP_LEDS 30 // Number of LEDs/Pixels on the RGB strip

// 7-Segment Patch Indicator
#define SEV_SEG_COMMON_MODE COMMON_ANODE
#define SEV_SEG_A      11
#define SEV_SEG_B      12
#define SEV_SEG_C      7
#define SEV_SEG_D      6
#define SEV_SEG_E      4
#define SEV_SEG_F      9
#define SEV_SEG_G      8
#define SEV_SEG_DP     13
#define SEV_SEG_COMMON 10

///////////////////////////
// Firmware parameters
///////////////////////////

// Lights
// #define NO_MAIN_STRIP // No main LED strip present,
                         // defining this will prevent noise from toggling potentiometer movement detection

// Rotary Encoder
#define ROTARY_ENC_DEBOUCE_TIME 100

// Potentiometer movement detection
#define AVG_SAMPLES     100
#define MAX_POT_MOV_DEV 6
// #define POTS_INVERTED // Inverts the potentiometer readings in case of wiring screw ups...

// 7-Segment Patch Indicator
#define NUM_SAVE_BLINKS    3
#define BLINK_INTERVAL_ON  250  // ms
#define BLINK_INTERVAL_OFF 250  // ms
#define PATCH_DISPLAY_TIME 5000 // Time (ms) for 7-seg to remain on after changing patches

// Patches
#define EEPROM_PATCH_ADDR  0x0 // Start of patches array in EEPROM

// Boot message
#define BOOT_MSG_AUTHORS "Patrick Pedersen <ctx.xda@gmail.com>"
#define BOOT_MSG_LICENSE "GPLv3"
#define BOOT_MSG_SRC "https://github.com/TU-DO-Makerspace/TU-DO-Disco"
#define BOOT_MSG_ASCII_ART \
"Created by the:\n"\
"\n"\
" ______   __  __\n"\
"/\\__  _\\ /\\ \\/\\ \\\n"\
"\\/_/\\ \\/ \\ \\ \\_\\ \\\n"\
"   \\ \\_\\  \\ \\_____\\\n"\
"    \\/_/   \\/_____/\n"\
"\n"\                    
" _____     ______\n"\    
"/\\  __-.  /\\  __ \\\n"\   
"\\ \\ \\/\\ \\ \\ \\ \\/\\ \\\n"\  
" \\ \\____-  \\ \\_____\\\n"\ 
"  \\/____/   \\/_____/\n"\
"\n"\
"\n"\
"Makerspace | Do it yourself!"

// Authors Credit
#define MORSE_SHORT 200
#define MORSE_LONG 3 * MORSE_SHORT
#define MORSE_BTWN MORSE_SHORT
#define MORSE_LETTER MORSE_LONG