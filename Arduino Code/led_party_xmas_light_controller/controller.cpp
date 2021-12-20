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
    // Check if you sholud change mode
    if (((unsigned long)(millis() - _mode._modeTimestamp) > _mode._modeTimeout) && !_forced)
    {
        _mode._mode++;
        if (_mode._mode > LED_CTRL_MODE_XMAS_3)
            _mode._mode = 0; // remove this later!!!! DO NOT FORGET TO DO THAT!
        setMode(_mode._mode, _mode._modeTimeout, _mode._patternChangeTimeout);
    }

    if (_mode._mode == LED_CTRL_MODE_STATIC_1 && _forced)
    {
        for (int i = 0; i < 3; i++)
        {
            if (_leds[i] != NULL)
            {
                randomSeed(analogRead(0));
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
                randomSeed(analogRead(0));
                for (int j = 0; j < (_leds[i]->numPixels()); j++)
                {
                    _leds[i]->setPixelColor(j, color24(oldColors[(j) % 5][0],
                                                       oldColors[(j) % 5][1],
                                                       oldColors[(j) % 5][2]));
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
}
void controller::setMode(uint8_t _m, uint32_t _tm, uint16_t _tp)
{
    _mode._mode = _m;
    _mode._modeTimestamp = millis();
    _mode._patternTimestamp = millis();
    _mode._patternSeq = 0;
    _mode._modeTimeout = _tm;
    _mode._patternChangeTimeout = _tp;
    update(1);
}

uint8_t controller::getMode()
{
    return _mode._mode;
}

void controller::setLedColor(uint32_t _c)
{
    ledColor = _c;
}

void controller::clearLeds(uint8_t _ledCh)
{
    if (_leds[_ledCh] == NULL)
        return;

    for (int i = 0; i < _leds[_ledCh]->numPixels(); i++)
    {
        _leds[_ledCh]->setPixelColor(i, 0);
    }
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