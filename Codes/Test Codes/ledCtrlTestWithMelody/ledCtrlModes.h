#ifndef __LED_CTRL_MODES_H__
#define __LED_CTRL_MODES_H__

// Add Arduino header file
#include "Arduino.h"

// Include file for the defines.
#include "ledControllerDefines.h"

// Ugh... Arduino does not support struct init with the pointer function, so I need to it this wav.
void initFunctionPointers();

// Helper function to convert RGB to 24 bit RGB for LEDs.
uint32_t rgbTo24Bit(uint8_t _r, uint8_t _g, uint8_t _b);

// Convert Hue, Saturation and Value (brightness) to RGB values. Again, thanks Adafruit!
uint32_t ColorHSV(uint16_t hue, uint8_t sat, uint8_t val);

#endif