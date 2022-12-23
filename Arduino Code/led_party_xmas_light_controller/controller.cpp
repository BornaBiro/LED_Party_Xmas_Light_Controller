#include "controller.h"

controller::controller()
{
    // Empty constructor
}

void controller::init(audio *_a, Adafruit_WS2801 *_l1, Adafruit_WS2801 *_l2, Adafruit_WS2801 *_l3)
{
    _leds[0] = _l1;
    _leds[1] = _l2;
    _leds[2] = _l3;

    for (int i = 0; i < 3; i++)
    {
        if (_leds[i] != NULL)
        {
            _leds[i]->begin();
            _leds[i]->show();
        }
    }
}

void controller::update(uint8_t _forced)
{
    // If the LEDs are disabled, do not update the LED contoller
    if (!_mode._enabled) return;

    // Check if you sholud change mode
    if (((unsigned long)(millis() - _mode._modeTimestamp) > _mode._modeTimeout) && !_forced)
    {
        if (_mode._autoChange)
        {
            _mode._mode++;
            if (_mode._mode > LED_CTRL_MODE_LAST_MODE)
                _mode._mode = LED_CTRL_MODE_STATIC_1;
            
            setMode(_mode._mode, _mode._modeTimeout, _mode._patternChangeTimeout);
        }
    }

    if (_mode._mode == LED_CTRL_MODE_STATIC_1 && _forced)
    {
        for (int i = 0; i < 3; i++)
        {
            if (_leds[i] != NULL)
            {
                randomSeed(_audioPtr->getSample());
                for (int j = 0; j < (_leds[i]->numPixels()); j++)
                {
                    uint8_t c = random(0, 7);
                    _leds[i]->setPixelColor(j, color24(red[c], green[c], blue[c]));
                }
                _leds[i]->show();
            }
        }
    }
    
    if (_mode._mode == LED_CTRL_MODE_STATIC_2 && _forced)
    {
        for (int i = 0; i < 3; i++)
        {
            if (_leds[i] != NULL)
            {
                for (int j = 0; j < (_leds[i]->numPixels()); j++)
                {
                    _leds[i]->setPixelColor(j, color24(red[ZUTA], green[ZUTA], blue[ZUTA]));
                }
                _leds[i]->show();
            }
        }
    }
    
    if (_mode._mode == LED_CTRL_MODE_STATIC_3 && _forced)
    {
        for (int i = 0; i < 3; i++)
        {
            if (_leds[i] != NULL)
            {
                for (int j = 0; j < (_leds[i]->numPixels()); j++)
                {
                    _leds[i]->setPixelColor(j, color24(red[BIJELA], green[BIJELA], blue[BIJELA]));
                }
                _leds[i]->show();
            }
        }
    }

    if (_mode._mode == LED_CTRL_MODE_STATIC_4 && _forced)
    {
        for (int i = 0; i < 3; i++)
        {
            if (_leds[i] != NULL)
            {
                randomSeed(_audioPtr->getSample());
                for (int j = 0; j < (_leds[i]->numPixels()); j++)
                {
                    _leds[i]->setPixelColor(
                        j, color24(oldColors[(j) % 5][0], oldColors[(j) % 5][1], oldColors[(j) % 5][2]));
                }
                _leds[i]->show();
            }
        }
    }

    if (_mode._mode == LED_CTRL_MODE_XMAS_1)
    {
        if (((unsigned long)(millis() - _mode._patternTimestamp) > _mode._patternChangeTimeout) || _forced)
        {
            _mode._patternTimestamp = millis();
            for (int i = 0; i < 3; i++)
            {
                if (_leds[i] != NULL)
                {
                    for (int j = 0; j < (_leds[i]->numPixels()); j++)
                    {
                        _leds[i]->setPixelColor(j, color24(oldColors[(j + _mode._patternSeq) % 5][0],
                                                           oldColors[(j + _mode._patternSeq) % 5][1],
                                                           oldColors[(j + _mode._patternSeq) % 5][2]));
                    }
                    _leds[i]->show();
                }
            }
            _mode._patternSeq++;
        }
    }

    if (_mode._mode == LED_CTRL_MODE_XMAS_2)
    {
        if (((unsigned long)(millis() - _mode._patternTimestamp) > _mode._patternChangeTimeout) || _forced)
        {
            _mode._patternTimestamp = millis();
            for (int i = 0; i < 3; i++)
            {
                if (_leds[i] != NULL)
                {
                    if (_mode._patternSeq % 2)
                    {
                        for (int j = 0; j < (_leds[i]->numPixels()); j += 4)
                        {
                            _leds[i]->setPixelColor(j, color24(red[ROZA], green[ROZA], blue[ROZA]));
                            _leds[i]->setPixelColor(j + 1,
                                                    color24(red[NARANCASTA], green[NARANCASTA], blue[NARANCASTA]));
                            _leds[i]->setPixelColor(j + 2, color24(0, 0, 0));
                            _leds[i]->setPixelColor(j + 3, color24(0, 0, 0));
                        }
                    }
                    else
                    {
                        for (int j = 0; j < (_leds[i]->numPixels()); j += 4)
                        {
                            _leds[i]->setPixelColor(j, color24(0, 0, 0));
                            _leds[i]->setPixelColor(j + 1, color24(0, 0, 0));
                            _leds[i]->setPixelColor(j + 2, color24(red[ZELENA], green[ZELENA], blue[ZELENA]));
                            _leds[i]->setPixelColor(j + 3, color24(red[PLAVA], green[PLAVA], blue[PLAVA]));
                        }
                    }
                    _leds[i]->show();
                }
            }
            _mode._patternSeq++;
        }
    }

    if (_mode._mode == LED_CTRL_MODE_XMAS_3)
    {
        if (((unsigned long)(millis() - _mode._patternTimestamp) > _mode._patternChangeTimeout) || _forced)
        {
            _mode._patternTimestamp = millis();
            for (int i = 0; i < 3; i++)
            {
                if (_leds[i] != NULL)
                {
                    for (int j = 0; j < (_leds[i]->numPixels()); j += 2)
                    {
                        if (_mode._patternSeq % 2)
                        {
                            _leds[i]->setPixelColor(j, color24(255, 0, 0));
                            _leds[i]->setPixelColor(j + 1, color24(0, 255, 0));
                        }
                        else
                        {
                            _leds[i]->setPixelColor(j, color24(0, 255, 0));
                            _leds[i]->setPixelColor(j + 1, color24(255, 0, 0));
                        }
                    }
                    _leds[i]->show();
                }
            }
            _mode._patternSeq++;
        }
    }

    if (_mode._mode == LED_CTRL_MODE_XMAS_4)
    {
        if (((unsigned long)(millis() - _mode._patternTimestamp) > _mode._patternChangeTimeout) || _forced)
        {
            _mode._patternTimestamp = millis();
            for (int i = 0; i < 3; i++)
            {
                if (_leds[i] != NULL)
                {
                    uint32_t _tmpColor = ColorHSV((uint16_t)(_mode._patternSeq) << 8, 255, 255);
                    for (int j = 0; j < (_leds[i]->numPixels()); j++)
                    {
                        _leds[i]->setPixelColor(j, _tmpColor);
                    }
                    _leds[i]->show();
                }
            }
            _mode._patternSeq+=4;
        }
    }

    // This mode has music!
    if (_mode._mode == LED_CTRL_MODE_XMAS_5)
    {
        if (((unsigned long)(millis() - _mode._patternTimestamp) > _mode._patternChangeTimeout) || _forced)
        {
            _mode._patternTimestamp = millis();

            if (!_mode._currentMelody) _mode._currentMelody = 1;

            if (_mode._melody) tone(BUZZER_PIN, pgm_read_word(currentMelodyNotes[_mode._currentMelody - 1] + _mode._patternSeq));
            _mode._patternChangeTimeout = pgm_read_word(currentMelodyDurations[_mode._currentMelody - 1] + _mode._patternSeq);

            for (int i = 0; i < 3; i++)
            {
                if (_leds[i] != NULL)
                {
                    for (int j = 0; j < (_leds[i]->numPixels()); j += 2)
                    {
                        if (_mode._patternSeq % 2)
                        {
                            _leds[i]->setPixelColor(j, color24(255, 0, 0));
                            _leds[i]->setPixelColor(j + 1, color24(0, 255, 0));
                        }
                        else
                        {
                            _leds[i]->setPixelColor(j, color24(0, 255, 0));
                            _leds[i]->setPixelColor(j + 1, color24(255, 0, 0));
                        }
                    }
                    _leds[i]->show();
                }
            }
            _mode._patternSeq++;
            if (_mode._patternSeq > pgm_read_byte(&melodyElements[_mode._currentMelody - 1]))
            {
                _mode._patternSeq = 0;
                _mode._currentMelody++;

                if (_mode._currentMelody > 8) _mode._currentMelody = 0;
            }
        }
    }
}
void controller::setMode(int16_t _m, uint32_t _tm, uint16_t _tp)
{
    // Do not allow wrong modes
    if (_m > LED_CTRL_MODE_LAST_MODE) _m = LED_CTRL_MODE_STATIC_1;
    if (_m < LED_CTRL_MODE_STATIC_1) _m = LED_CTRL_MODE_LAST_MODE;

    _mode._mode = _m;
    _mode._modeTimestamp = millis();
    _mode._patternTimestamp = millis();
    _mode._patternSeq = 0;
    _mode._modeTimeout = _tm;
    _mode._patternChangeTimeout = _tp;
    _mode._currentMelody = 0;
    noTone(BUZZER_PIN);
    update(1);
}

uint8_t controller::getMode()
{
    return _mode._mode;
}

void controller::setAnimationDelay(uint16_t _tp)
{
    _mode._patternTimestamp = millis();
    _mode._patternChangeTimeout = _tp;
}

void controller::setLedColor(uint32_t _c)
{
    ledColor = _c;
}

void controller::clearLeds()
{
    for (int j = 0; j < 3; j++)
    {
        if (_leds[j] != NULL)
        {
            for (int i = 0; i < _leds[j]->numPixels(); i++)
            {
                _leds[j]->setPixelColor(i, 0);
            }
        }
        _leds[j]->show();
    }
}

void controller::setState(uint8_t _enable)
{
    _mode._enabled = _enable;
}

uint8_t controller::getState()
{
    return _mode._enabled;
}

void controller::setAutoChange(uint8_t _autoChange)
{
    _mode._autoChange = _autoChange;
}

void controller::setMelody(uint8_t _melody)
{
    _mode._melody = _melody;
}

uint8_t controller::getCurrentMelody()
{
    return _mode._currentMelody;
}

void controller::reactLEDsToMusic(int16_t _maxValue, uint8_t _ledCh)
{
    if (_leds[_ledCh] == NULL)
        return;

    int8_t k = 0;
    int8_t _noLeds = _leds[_ledCh]->numPixels();

    for (int i = 0; i < _noLeds; i++)
    {
        _leds[0]->setPixelColor(i, 0);
    }

    switch (_mode._mode)
    {
    case LED_CTRL_MODE_PARTY_MUSIC_1:
        k = _noLeds / 127.0 * _maxValue;
        for (int i = 0; i < k; i++)
        {
            _leds[0]->setPixelColor(i, ledColor);
        }
        break;

    case LED_CTRL_MODE_PARTY_MUSIC_2:
        k = _noLeds / 127.0 * _maxValue / 2;
        for (int i = 0; i < k; i++)
        {
            _leds[0]->setPixelColor(29 - i, ledColor);
            _leds[0]->setPixelColor(i, ledColor);
        }
        break;

    case LED_CTRL_MODE_PARTY_MUSIC_3:
        k = _noLeds / 127.0 * _maxValue / 2;
        for (int i = 0; i < k; i++)
        {
            _leds[0]->setPixelColor(14 - i, ledColor);
            _leds[0]->setPixelColor(15 + i, ledColor);
        }
        break;
    }
    _leds[0]->show();
}

uint32_t controller::color24(byte r, byte g, byte b)
{
    uint32_t c;
    c = r;
    c <<= 8;
    c |= g;
    c <<= 8;
    c |= b;
    return c;
}

uint32_t controller::ColorHSV(uint16_t hue, uint8_t sat, uint8_t val)
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