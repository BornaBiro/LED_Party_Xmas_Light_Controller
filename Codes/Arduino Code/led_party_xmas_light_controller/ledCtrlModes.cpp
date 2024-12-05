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

// Animation pattern in which LED color pattern is green, red, green, red.
void ledPatternFunction1(Adafruit_WS2801 *_led, ledModeHandleTypedef *_handle)
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

// LED pattern where LEDs are in green, red, blue and yellow color,
// but only green and blue or red and yellow are active at the same time.
void ledPatternFunction2(Adafruit_WS2801 *_led, ledModeHandleTypedef *_handle)
{
    uint8_t _shift = _handle->animationPhase & 1;

    // Turn off all LEDs.
    for (int i = 0; i < _led->numPixels(); i++)
    {
        _led->setPixelColor(i, 0x000000);
    }

    // Turn on or green and blue LEDs or red and yellow (orange) LEDs.
    for (int i = _shift; i < _led->numPixels(); i+=4)
    {
        if (_shift == 0)
        {
            _led->setPixelColor(i, rgbTo24Bit(red[COLOR_GREEN], green[COLOR_GREEN], blue[COLOR_GREEN]));
            _led->setPixelColor(i + 2, rgbTo24Bit(red[COLOR_BLUE], green[COLOR_BLUE], blue[COLOR_BLUE]));
        }
        else
        {
            _led->setPixelColor(i, rgbTo24Bit(red[COLOR_RED], green[COLOR_RED], blue[COLOR_RED]));
            _led->setPixelColor(i + 2, rgbTo24Bit(red[COLOR_ORANGE], green[COLOR_ORANGE], blue[COLOR_ORANGE]));
        }
    }

    _led->show();
}

// Old school xmas lights color but the colors are "moving".
void ledPatternFunction3(Adafruit_WS2801 *_led, ledModeHandleTypedef *_handle)
{
    // Display LEDs in old school xmas light colors.
    for (int i = 0; i < _led->numPixels(); i++)
    {
        // In every new step, move them around.
        uint8_t _index = (i + _handle->animationPhase) % 5;

        // Send the new color for every LEDs.
        _led->setPixelColor(i, rgbTo24Bit(oldColors[_index][0], oldColors[_index][1], oldColors[_index][2]));
    }
    _led->show();
}

// Simple color wipe with a color wheel effect.
void ledPatternFunction4(Adafruit_WS2801 *_led, ledModeHandleTypedef *_handle)
{
    // Get the color for all LEDs using hue.
    uint32_t _color = ColorHSV(_handle->animationPhase << 8, 255, 255);

    // Send the new color to the all LEDs.
    for (int i = 0; i < _led->numPixels(); i++)
    {
        _led->setPixelColor(i, _color);
    }

    // Send the color to the LEDs.
    _led->show();
}

void ledPatternFunction5(Adafruit_WS2801 *_led, ledModeHandleTypedef *_handle)
{
    // At the first phase, fill the LEDs with defined color.
    const uint16_t _hue = 5280;
    const uint8_t _sat = 244;
    const uint8_t _val = 127;

    if (_handle->animationPhase == 0)
    {
        for (int i = 0; i < _led->numPixels(); i++)
        {
            _led->setPixelColor(i, ColorHSV(_hue, _sat, _val));
        }
    }
    else
    {
        // If the phase is divisable by 10, generate bright LED at random position.
        if (_handle->animationPhase % 10)
        {
            // Generate random LED position.
            int _ledPos = random(0, _led->numPixels());

            // Generate random brightness offset.
            int _brightnessOffset = random(-75, 75);

            // Set higher brightness
            _led->setPixelColor(_ledPos, ColorHSV(_hue, _sat, _val + _brightnessOffset));
        }
        else
        {
            // Go trough the loop and check if the current LED HSV brightness is 127.
            // If not, increment it or decrement it until is 127.
            for (int i = 0; i < _led->numPixels(); i++)
            {
                // Get the pixel HSV value.
                uint16_t _h;
                uint8_t _s;
                uint8_t _v;
                RGBToHSV(_led->getPixelColor(i), &_h, &_s, &_v);

                // Check if the value is corrent.
                if (_v != _val)
                {
                    // Check the direction of the increment.
                    if (_v > _val)
                    {
                        _v--;
                    }
                    else
                    {
                        _v++;
                    }

                    // Update the LED HSV.
                    _led->setPixelColor(i, ColorHSV(_hue, _sat, _v));
                }
            }
        }
    }

    // Send the color to the LEDs.
    _led->show();
}

void ledPatternFunction6(Adafruit_WS2801 *_led, ledModeHandleTypedef *_handle)
{
    // At the first phase, fill the LEDs with defined color.
    const uint16_t _hue = 5280;
    const uint8_t _sat = 244;
    const uint8_t _val = 127;

    if (_handle->animationPhase == 0)
    {
        for (int i = 0; i < _led->numPixels(); i++)
        {
            _led->setPixelColor(i, ColorHSV(_hue, _sat, _val));
        }
        _handle->animationPhase++;
    }
    else
    {
        uint8_t _selectedLed = random(0, _led->numPixels());
            uint16_t _h;
            uint8_t _s;
            uint8_t _v;
            RGBToHSV(_led->getPixelColor(_selectedLed), &_h, &_s, &_v);

            // Generate random brightness offset.
            int _brightnessOffset = random(-50, 50);

            int _newValue = _brightnessOffset + _v;
            if (_newValue > 255) _newValue = 255;
            if (_newValue < 25) _newValue = 25;

            _led->setPixelColor(_selectedLed, ColorHSV(_hue, _sat, _newValue));
        
                _handle->animationPhase = 1;

                _handle->animationFrameDelay = random(50, 500);
        
        }

    // Send the color to the LEDs.
    _led->show();
}

// Array with all supported modes and their definitions.
ledModeHandleTypedef modeList[] = 
{
    {
        .animation = 0,
        .animationPhase = 0,
        .maxAnimationsPhases = 0,
        .melody = 0,
        .animationFrameDelay = 0,
        .name = "Sta Rnd"
    },
    {
        .animation = 0,
        .animationPhase = 0,
        .maxAnimationsPhases = 0,
        .melody = 0,
        .animationFrameDelay = 0,
        .name = "Sta WW"
    },
    {
        .animation = 0,
        .animationPhase = 0,
        .maxAnimationsPhases = 0,
        .melody = 0,
        .animationFrameDelay = 0,
        .name = "Sta CW"
    },
    {
        .animation = 0,
        .animationPhase = 0,
        .maxAnimationsPhases = 0,
        .melody = 0,
        .animationFrameDelay = 0,
        .name = "Sta WW&CW"
    },
    {
        .animation = 0,
        .animationPhase = 0,
        .maxAnimationsPhases = 0,
        .melody = 0,
        .animationFrameDelay = 0,
        .name = "Sta Old"
    },
    {
        .animation = 0,
        .animationPhase = 0,
        .maxAnimationsPhases = 0,
        .melody = 0,
        .animationFrameDelay = 0,
        .name = "Sta Xmas"
    },
    {
        .animation = 1,
        .animationPhase = 0,
        .maxAnimationsPhases = 0,
        .melody = 0,
        .animationFrameDelay = 1000,
        .name = "Dyn Xmas"
    },
    {
        .animation = 1,
        .animationPhase = 0,
        .maxAnimationsPhases = 0,
        .melody = 1,
        .animationFrameDelay = 1000,
        .name = "Dyn Xmas mel."
    },
    {
        .animation = 1,
        .animationPhase = 0,
        .maxAnimationsPhases = 0,
        .melody = 0,
        .animationFrameDelay = 1000,
        .name = "Dyn Old"
    },
    {
        .animation = 1,
        .animationPhase = 0,
        .maxAnimationsPhases = 0,
        .melody = 1,
        .animationFrameDelay = 1000,
        .name = "Dyn Old Mel"
    },
    {
        .animation = 1,
        .animationPhase = 0,
        .maxAnimationsPhases = 0,
        .melody = 0,
        .animationFrameDelay = 1000,
        .name = "Dyn Old Xmas"
    },
    {
        .animation = 1,
        .animationPhase = 0,
        .maxAnimationsPhases = 255,
        .melody = 0,
        .animationFrameDelay = 200,
        .name = "Dyn C Wheel"
    },
    {
        .animation = 1,
        .animationPhase = 0,
        .maxAnimationsPhases = 0,
        .melody = 0,
        .animationFrameDelay = 75,
        .name = "Dyn Candle"
    },
    {
        .animation = 1,
        .animationPhase = 0,
        .maxAnimationsPhases = 0,
        .melody = 0,
        .animationFrameDelay = 50,
        .name = "Dyn Candle 2"
    },
};

// Ugh... Arduino does not support struct init with the pointer function, so I need to do it this way.
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
    modeList[7].animationFunction = &ledPatternFunction1;
    modeList[8].animationFunction = &ledPatternFunction2;
    modeList[9].animationFunction = &ledPatternFunction2;
    modeList[10].animationFunction = &ledPatternFunction3;
    modeList[11].animationFunction = &ledPatternFunction4;
    modeList[12].animationFunction = &ledPatternFunction5;
    modeList[13].animationFunction = &ledPatternFunction6;
}

// Helper function for LEDs.
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

// Thanks Adafruit, you save me a lot of time (and headache)!
uint32_t ColorHSV(uint16_t hue, uint8_t sat, uint8_t val)
{

    uint8_t r, g, b;

    // Remap 0-65535 to 0-1529. Pure red is CENTERED on the 64K rollover;
    // 0 is not the start of pure red, but the midpoint...a few values above
    // zero and a few below 65536 all yield pure red (similarly, 32768 is the
    // midpoint, not start, of pure cyan). The 8-bit RGB hexcone (256 values
    // each for red, green, blue) really only allows for 1530 distinct hues
    // (not 1536, more on that below), but the full unsigned 16-bit type was
    // chosen for hue so that one's code can easily handle a contiguous color
    // wheel by allowing hue to roll over in either direction.
    hue = (hue * 1530L + 32768) / 65536;
    // Because red is centered on the rollover point (the +32768 above,
    // essentially a fixed-point +0.5), the above actually yields 0 to 1530,
    // where 0 and 1530 would yield the same thing. Rather than apply a
    // costly modulo operator, 1530 is handled as a special case below.

    // So you'd think that the color "hexcone" (the thing that ramps from
    // pure red, to pure yellow, to pure green and so forth back to red,
    // yielding six slices), and with each color component having 256
    // possible values (0-255), might have 1536 possible items (6*256),
    // but in reality there's 1530. This is because the last element in
    // each 256-element slice is equal to the first element of the next
    // slice, and keeping those in there this would create small
    // discontinuities in the color wheel. So the last element of each
    // slice is dropped...we regard only elements 0-254, with item 255
    // being picked up as element 0 of the next slice. Like this:
    // Red to not-quite-pure-yellow is:        255,   0, 0 to 255, 254,   0
    // Pure yellow to not-quite-pure-green is: 255, 255, 0 to   1, 255,   0
    // Pure green to not-quite-pure-cyan is:     0, 255, 0 to   0, 255, 254
    // and so forth. Hence, 1530 distinct hues (0 to 1529), and hence why
    // the constants below are not the multiples of 256 you might expect.

    // Convert hue to R,G,B (nested ifs faster than divide+mod+switch):
    if (hue < 510)
    { // Red to Green-1
        b = 0;
        if (hue < 255)
        { //   Red to Yellow-1
            r = 255;
            g = hue; //     g = 0 to 254
        }
        else
        {                  //   Yellow to Green-1
            r = 510 - hue; //     r = 255 to 1
            g = 255;
        }
    }
    else if (hue < 1020)
    { // Green to Blue-1
        r = 0;
        if (hue < 765)
        { //   Green to Cyan-1
            g = 255;
            b = hue - 510; //     b = 0 to 254
        }
        else
        {                   //   Cyan to Blue-1
            g = 1020 - hue; //     g = 255 to 1
            b = 255;
        }
    }
    else if (hue < 1530)
    { // Blue to Red-1
        g = 0;
        if (hue < 1275)
        {                   //   Blue to Magenta-1
            r = hue - 1020; //     r = 0 to 254
            b = 255;
        }
        else
        { //   Magenta to Red-1
            r = 255;
            b = 1530 - hue; //     b = 255 to 1
        }
    }
    else
    { // Last 0.5 Red (quicker than % operator)
        r = 255;
        g = b = 0;
    }

    // Apply saturation and value to R,G,B, pack into 32-bit result:
    uint32_t v1 = 1 + val;  // 1 to 256; allows >>8 instead of /255
    uint16_t s1 = 1 + sat;  // 1 to 256; same reason
    uint8_t s2 = 255 - sat; // 255 to 0
    return ((((((r * s1) >> 8) + s2) * v1) & 0xff00) << 8) | (((((g * s1) >> 8) + s2) * v1) & 0xff00) |
           (((((b * s1) >> 8) + s2) * v1) >> 8);
}

// Helper function that converts RGB value into HSV. Thanks ChatGPT!
void RGBToHSV(uint32_t _rgb, uint16_t *h, uint8_t *s, uint8_t *v)
{
    // Extract R, G, and B components from the 24-bit RGB value
    uint8_t r = (_rgb >> 16) & 0xFF;  // Extract R component
    uint8_t g = (_rgb >> 8) & 0xFF;   // Extract G component
    uint8_t b = _rgb & 0xFF;          // Extract B component

    double red = r / 255.0;
    double green = g / 255.0;
    double blue = b / 255.0;

    double cmax = fmax(red, fmax(green, blue));
    double cmin = fmin(red, fmin(green, blue));
    double delta = cmax - cmin;

    double hue = 0, saturation = 0, value = 0;

    // Calculate Hue
    if (delta == 0)
        hue = 0;
    else if (cmax == red)
        hue = 60 * fmod(((green - blue) / delta), 6);
    else if (cmax == green)
        hue = 60 * (((blue - red) / delta) + 2);
    else
        hue = 60 * (((red - green) / delta) + 4);

    if (hue < 0) hue += 360;

    // Calculate Saturation
    if (cmax == 0)
        saturation = 0;
    else
        saturation = (delta / cmax) * 255;

    // Calculate Value
    value = cmax * 255;

    *h = (uint16_t)hue;
    *s = (uint8_t)saturation;
    *v = (uint8_t)value;
}