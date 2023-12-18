#include "ledCtrlModes.h"

// Color LUT (toned down versoion of the colors, to look more like old school bulbs).
const uint8_t red[9] = {128, 255, 0, 255, 32, 255, 255, 8, 255};
const uint8_t green[9] = {16, 64, 192, 128, 255, 2, 128, 8, 8};
const uint8_t blue[9] = {255, 0, 255, 128, 8, 2, 8, 255, 16};
const uint8_t oldColors[5][3] =
{
    {red[COLOR_RED], green[COLOR_RED], blue[COLOR_RED]},
    {red[COLOR_ORANGE], green[COLOR_ORANGE], blue[COLOR_ORANGE]},
    {red[COLOR_BLUE], green[COLOR_BLUE], blue[COLOR_BLUE]},
    {red[COLOR_ROSE], green[COLOR_ROSE], blue[COLOR_ROSE]},
    {red[COLOR_GREEN], green[COLOR_GREEN], blue[COLOR_GREEN]}
};

// Function callbacks for various LED pattenrs.
// Static random colors.
void ledPatternStatic1(Adafruit_WS2801 *_led, ledModeHandleTypedef *_handle)
{
    for (int i = 0; i < _led->numPixels(); i++)
    {
        uint8_t _randomColorNumber = random(0, 8);
        uint32_t _color = rgbTo24Bit(red[_randomColorNumber], green[_randomColorNumber], blue[_randomColorNumber]);
        _led->setPixelColor(i, _color);
    }
    _led->show();
}

// Static warm white color.
void ledPatternStatic2(Adafruit_WS2801 *_led, ledModeHandleTypedef *_handle)
{
    for (int i = 0; i < _led->numPixels(); i++)
    {
        _led->setPixelColor(i, rgbTo24Bit(red[COLOR_YELLOW], green[COLOR_YELLOW], blue[COLOR_YELLOW]));
    }
    _led->show();
}

// Static cold white.
void ledPatternStatic3(Adafruit_WS2801 *_led, ledModeHandleTypedef *_handle)
{
    for (int i = 0; i < _led->numPixels(); i++)
    {
        _led->setPixelColor(i, rgbTo24Bit(red[COLOR_WHITE], green[COLOR_WHITE], blue[COLOR_WHITE]));
    }
    _led->show();
}

// Static with combination of warm and cold white LEDs.
void ledPatternStatic4(Adafruit_WS2801 *_led, ledModeHandleTypedef *_handle)
{
    // Display warm white LEDs (every even LED).
    for (int i = 0; i < _led->numPixels(); i+=2)
    {
        _led->setPixelColor(i, rgbTo24Bit(red[COLOR_YELLOW], green[COLOR_YELLOW], blue[COLOR_YELLOW]));
    }

    // Display cold white LEDs (every odd LED).
    for (int i = 1; i < _led->numPixels(); i+=2)
    {
        _led->setPixelColor(i, rgbTo24Bit(red[COLOR_WHITE], green[COLOR_WHITE], blue[COLOR_WHITE]));
    }
    _led->show();
}

// This static mode mimics the look of the old xmas lamps.
void ledPatternStatic5(Adafruit_WS2801 *_led, ledModeHandleTypedef *_handle)
{
    // Display LEDs in old school xmas light colors.
    for (int i = 0; i < _led->numPixels(); i++)
    {
        int _index = i % 5;
        _led->setPixelColor(i, rgbTo24Bit(oldColors[_index][0], oldColors[_index][1], oldColors[_index][2]));
    }
    _led->show();
}

// This static mode shows green, white, red, white, green, white, etc...
void ledPatternStatic6(Adafruit_WS2801 *_led, ledModeHandleTypedef *_handle)
{
    // LUT for the colors.
    uint32_t _colorLUT[] =
    {
        rgbTo24Bit(red[COLOR_GREEN], green[COLOR_GREEN], blue[COLOR_GREEN]),
        rgbTo24Bit(red[COLOR_YELLOW], green[COLOR_YELLOW], blue[COLOR_YELLOW]),
        rgbTo24Bit(red[COLOR_RED], green[COLOR_RED], blue[COLOR_RED]),
        rgbTo24Bit(red[COLOR_YELLOW], green[COLOR_YELLOW], blue[COLOR_YELLOW]),
    };

    // Display LEDs in old school xmas light colors.
    for (int i = 0; i < _led->numPixels(); i++)
    {
        int _index = i % 4;
        _led->setPixelColor(i, _colorLUT[_index]);
    }
    _led->show();
}

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
    uint8_t _shift = _handle->animationPhase & 1;

    for (int i = _shift; i < _led->numPixels(); i+=2)
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

ledModeHandleTypedef modeList[] = 
{
    {
        .animation = 0,
        .animationPhase = 0,
        .maxAnimationsPhases = 0,
        .melody = 0,
        .animationFrameDelay = 0,
        .name = "Stat Rnd"
    },
    {
        .animation = 0,
        .animationPhase = 0,
        .maxAnimationsPhases = 0,
        .melody = 0,
        .animationFrameDelay = 0,
        .name = "Stat WW"
    },
    {
        .animation = 0,
        .animationPhase = 0,
        .maxAnimationsPhases = 0,
        .melody = 0,
        .animationFrameDelay = 0,
        .name = "Stat CW"
    },
    {
        .animation = 0,
        .animationPhase = 0,
        .maxAnimationsPhases = 0,
        .melody = 0,
        .animationFrameDelay = 0,
        .name = "Stat WW&CW"
    },
    {
        .animation = 0,
        .animationPhase = 0,
        .maxAnimationsPhases = 0,
        .melody = 0,
        .animationFrameDelay = 0,
        .name = "Stat Old"
    },
    {
        .animation = 0,
        .animationPhase = 0,
        .maxAnimationsPhases = 0,
        .melody = 0,
        .animationFrameDelay = 0,
        .name = "Stat Xmas"
    },
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
        .maxAnimationsPhases = 0,
        .melody = 0,
        .animationFrameDelay = 1000,
        .name = "Mode 2"
    },
    {
        .animation = 1,
        .animationPhase = 0,
        .maxAnimationsPhases = 0,
        .melody = 1,
        .animationFrameDelay = 1000,
        .name = "Mode 2 with melody"
    },
};

// Ugh... Arduino does not support struct init with the pointer function, so I need to it this way.
void initFunctionPointers()
{
    // Add your functions to the list here.
    modeList[0].animationFunction = &ledPatternStatic1;
    modeList[1].animationFunction = &ledPatternStatic2;
    modeList[2].animationFunction = &ledPatternStatic3;
    modeList[3].animationFunction = &ledPatternStatic4;
    modeList[4].animationFunction = &ledPatternStatic5;
    modeList[5].animationFunction = &ledPatternStatic6;
    modeList[6].animationFunction = &ledPatternFunction1;
    modeList[7].animationFunction = &ledPatternFunction2;
    modeList[8].animationFunction = &ledPatternFunction2;
}

uint32_t rgbTo24Bit(uint8_t _r, uint8_t _g, uint8_t _b)
{
    uint32_t _color24;
    _color24 = _r;
    _color24 <<= 8;
    _color24 |= _g;
    _color24 <<= 8;
    _color24 |= _b;
    return _color24;
}