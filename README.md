<!-- omit in toc -->
# TU-DO Disco

A custom DIY RGB dimmer for the TU-DO cafe.

- [Features](#features)
- [Building](#building)
- [Usage](#usage)
  - [General usage](#general-usage)
  - [Patch bank](#patch-bank)
    - [Loading/Selecting patches](#loadingselecting-patches)
    - [Saving patches](#saving-patches)
  - [Setting via USB](#setting-via-usb)
    - [Setting RGB only](#setting-rgb-only)
    - [Setting RGB and the main light](#setting-rgb-and-the-main-light)
    - [Retrieving the current color](#retrieving-the-current-color)
- [Documentation](#documentation)

## Features

The TU-DO dimmer can control all three color channels of an RGB LED strip as well as the brightness of a main 12V white LED strip using a set of 4 rotary potentiometers. In addition to satisfying the basic features a lights dimmer, the device also offers a 10-slot patch bank to store light any desired "patches"/configuration. The stored patches may then be loaded. Finally, one last touch that has been added is the option to program the LED strips do a desired HTML color code via USB serial communication.

## Building

TUDO :)

## Usage

### General usage

The device offers a total of 4 potentiometers to control the lights, three for the RGB strip and one to control the brightness of the white main light.

### Patch bank

A total of 10 save slots are provided to permanently store a desired light patch/configuration. **Upon device boot, the first (0th) patch is always loaded.**


#### Loading/Selecting patches

Loading the next or previous patch is achieved using a rotary encoder. By turning the rotary encoder to the right, the next patch is loaded, contrary, turning it to the left will select the previous patch. When a new patch has been selected, the patch is applied to the lights and the 7-Segment LED indicator will display the currently selected save slot for a brief time. As soon as the potentiometers are turned, the loaded configuration is discarded and the LEDs are once again set by the potentiometer values.

#### Saving patches

To save the current lights configuration to the currently selected save slot, press down the rotary encoder. The 7-Segment patch indicator will flash the currently selected save slot, confirming that the current patch has been saved.


### Setting via USB

As mentioned above, the lights may also be programmed via USB serial communication by providing a HTML color code.

#### Setting RGB only

To only program the RGB lights, a typical HTML/Hex color code in the form of `#AABBCC` is send via the Serial console.

Example, setting the RGB strip to red:
```
#FF000
```

The hex characters are case insensitive and the preceding `#` sign must be provided!

#### Setting RGB and the main light

Should one desire to also set the brightness of the main light, an additional byte (two hex character) is added to the HTML color code, controlling the brightness.

Example, disabling RGB lights and setting the Main light to full brightness:
```
#000000FF
```

Once again, the hex characters are case insensitive and the preceding `#` sign must be provided!

The light will maintain their programmed value until potentiometer movement is detected.

#### Retrieving the current color

One may also retrieve information regarding the current lights configuration by simply sending the `g` command via the serial console.

Example:
```
g
Current Color: #AABBCCDD
R: 170
G: 187
B: 204
M: 221
```

## Documentation

TUDO :)