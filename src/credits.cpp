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
   * Description: Authors credit ;)
   * 
   */

#include "config.h"
#include "credits.h"

/*
 * flash
 * -----------
 * Parameters:
 *      rgbstrp - Pointer to a NeoPixel RGB strip driver object
 *      color - Color to be flashed
 *      duration - Duration for color to be flashed 
 * Description:
 *      "Flashes"/Displays a provided color for a provided duration.
 *      The RGB strip must still be turned off manually after this function
 *      has been called.
 */

void flash(NeoPixelBus <NeoGrbFeature, Neo800KbpsMethod> *rgbstrp, RgbColor color, unsigned long duration)
{
        rgbstrp->ClearTo(color);
        rgbstrp->Show();
        delay(duration);
}

/*
 * flash
 * -----------
 * Parameters:
 *      rgbstrp - Pointer to a NeoPixel RGB strip driver object
 * Description:
 *      Author credits
 */

void authors_credit(NeoPixelBus <NeoGrbFeature, Neo800KbpsMethod> *rgbstrp)
{
        RgbColor off(0, 0, 0);
        RgbColor red(255, 0, 0);
        RgbColor green(0, 0, 255);
        RgbColor blue(0, 255, 0);
        RgbColor white(255, 255, 255);

        flash(rgbstrp, RgbColor(0, 0, 0), 1000);

        // C
        flash(rgbstrp, red, MORSE_LONG);
        flash(rgbstrp, off, MORSE_BTWN);
        flash(rgbstrp, red, MORSE_SHORT);     
        flash(rgbstrp, off, MORSE_BTWN);
        flash(rgbstrp, red, MORSE_LONG);
        flash(rgbstrp, off, MORSE_BTWN);
        flash(rgbstrp, red, MORSE_SHORT);

        flash(rgbstrp, off, MORSE_LETTER);

        // T
        flash(rgbstrp, green, MORSE_LONG);

        flash(rgbstrp, off, MORSE_LETTER);

        // X
        flash(rgbstrp, off, MORSE_LONG);
        flash(rgbstrp, off, MORSE_BTWN);
        flash(rgbstrp, blue, MORSE_SHORT);     
        flash(rgbstrp, off, MORSE_BTWN);
        flash(rgbstrp, blue, MORSE_SHORT);     
        flash(rgbstrp, off, MORSE_BTWN);

        // z
        flash(rgbstrp, off, MORSE_LONG);
        flash(rgbstrp, off, MORSE_BTWN);
        flash(rgbstrp, white, MORSE_SHORT);     
        flash(rgbstrp, off, MORSE_BTWN);
        flash(rgbstrp, white, MORSE_SHORT);     
        flash(rgbstrp, off, MORSE_BTWN);

        flash(rgbstrp, RgbColor(0, 0, 0), 1000);
}