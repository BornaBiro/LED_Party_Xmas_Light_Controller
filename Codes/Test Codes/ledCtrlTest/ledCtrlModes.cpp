#include "ledCtrlModes.h"

void ledPatternFunction1(Adafruit_WS2801 *_led, ledModeHandleTypedef *_handle)
{
    uint32_t _colorFill;
    
    switch (_handle->animationPhase % 3)
    {
        case 0:
            _colorFill = 0xFF0000;
            break;
        
        case 1:
            _colorFill = 0x00FF00;
            break;

        case 2:
            _colorFill = 0x0000FF;
            break;
    }

    for (int i = 0; i < _led->numPixels(); i++)
    {
        _led->setPixelColor(i, _colorFill);
    }
    _led->show();
}

void ledPatternFunction2(Adafruit_WS2801 *_led, ledModeHandleTypedef *_handle)
{
    uint8_t _shift = _handle->animationPhase % 2;

    for (int i = (_shift & 1); i < _led->numPixels(); i+=2)
    {
        _led->setPixelColor(i, 0XFF0000);
    }
    _shift++;
    for (int i = (_shift & 1); i < _led->numPixels(); i+=2)
    {
        _led->setPixelColor(i, 0X00FF00);
    }
    _led->show();
}

ledModeHandleTypedef modeList[10] = 
{
    {
        .animation = 1,
        .animationPhase = 0,
        .maxAnimationsPhases = 3,
        .melody = 0,
        .animationFrameDelay = 1000,
        .name = "Mode 1"
    },
    {
        .animation = 1,
        .animationPhase = 0,
        .maxAnimationsPhases = 3,
        .melody = 0,
        .animationFrameDelay = 1000,
        .name = "Mode 2"
    },
    {},
    {},
    {},
    {},
    {},
    {},
    {},
    {},
};

// Ugh... Arduino does not support struct init with the pointer function, so I need to it this way.
void initFunctionPointers()
{
    // Add your functions to the list here.
    modeList[0].animationFunction = &ledPatternFunction1;
    modeList[1].animationFunction = &ledPatternFunction2;
}