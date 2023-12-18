#ifndef __LED_CTRL_MODES_H__
#define __LED_CTRL_MODES_H__

// Add Arduino header file
#include "Arduino.h"

// Include file for the defines.
#include "ledControllerDefines.h"

void ledPatternFunction1(Adafruit_WS2801 *_led, ledModeHandleTypedef *_handle);
void ledPatternFunction2(Adafruit_WS2801 *_led, ledModeHandleTypedef *_handle);

// Ugh... Arduino does not support struct init with the pointer function, so I need to it this wav.
void initFunctionPointers();

#endif