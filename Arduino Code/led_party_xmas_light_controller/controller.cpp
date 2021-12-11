#include "controller.h"

controller::controller()
{
    // Empty constructor
}

void controller::init(Adafruit_WS2801 *_l1, Adafruit_WS2801 *_l2, Adafruit_WS2801 *_l3)
{
    _leds[0] = _l1;
    _leds[1] = _l2;
    _leds[2] = _l3;
}

void controller::setMode(uint8_t _mode)
{
    ledMode = _mode;
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

    switch (ledMode)
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