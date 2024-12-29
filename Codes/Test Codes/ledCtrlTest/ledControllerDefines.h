#ifndef __LED_CONTROLLER_DEFINES_H__
#define __LED_CONTROLLER_DEFINES_H__

// Add Arduino Adafruit WS2801 Library.
#include "Adafruit_WS2801.h"

// Header file with struct typedefs for each LED mode
typedef struct ledModeHandleTypedef
{
    uint8_t animation;
    uint8_t animationPhase;
    uint8_t maxAnimationsPhases;
    uint8_t melody;
    uint16_t animationFrameDelay;
    char *name;
    void (*animationFunction)(Adafruit_WS2801 *_led, ledModeHandleTypedef *_ctrl);
};

#endif