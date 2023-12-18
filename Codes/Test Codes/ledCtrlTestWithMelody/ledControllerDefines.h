#ifndef __LED_CONTROLLER_DEFINES_H__
#define __LED_CONTROLLER_DEFINES_H__

// Add Arduino Adafruit WS2801 Library.
#include "Adafruit_WS2801.h"

// Color defines for color LUT
#define COLOR_PURPLE        0
#define COLOR_ORANGE        1
#define COLOR_LIGHT_BLUE    2
#define COLOR_WHITE         3
#define COLOR_GREEN         4
#define COLOR_RED           5
#define COLOR_YELLOW        6
#define COLOR_BLUE          7
#define COLOR_ROSE          8

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