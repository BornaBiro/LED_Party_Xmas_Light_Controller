#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#include <EEPROM.h>

#include "Adafruit_WS2801.h"
#include "defines.h"
#include "audio.h"
#include "melodies.h"

#define LJUBICASTA 0
#define NARANCASTA 1
#define MODRO_PLAVA 2
#define BIJELA 3
#define ZELENA 4
#define CRVENA 5
#define ZUTA 6
#define PLAVA 7
#define ROZA  8

#define LED_CTRL_MODE_STATIC_1      0
#define LED_CTRL_MODE_STATIC_2      1
#define LED_CTRL_MODE_STATIC_3      2
#define LED_CTRL_MODE_STATIC_4      3
#define LED_CTRL_MODE_XMAS_1        4
#define LED_CTRL_MODE_XMAS_2        5
#define LED_CTRL_MODE_XMAS_3        6
#define LED_CTRL_MODE_XMAS_4        7
#define LED_CTRL_MODE_XMAS_5        8
#define LED_CTRL_MODE_LAST_MODE     LED_CTRL_MODE_XMAS_5

#define LED_CTRL_MODE_PARTY_1       32
#define LED_CTRL_MODE_PARTY_2       33
#define LED_CTRL_MODE_PARTY_3       34
#define LED_CTRL_MODE_PARTY_4       35
#define LED_CTRL_MODE_PARTY_MUSIC_1 36
#define LED_CTRL_MODE_PARTY_MUSIC_2 37
#define LED_CTRL_MODE_PARTY_MUSIC_3 38

#define LED_CHANNEL_1 0
#define LED_CHANNEL_2 1
#define LED_CHANNEL_3 2

class controller
{
  public:
    controller();
    void init(audio *_a, Adafruit_WS2801 *_l1, Adafruit_WS2801 *_l2, Adafruit_WS2801 *_l3);
    void update(uint8_t _forced = 0);
    void setMode(int16_t _m, unsigned long _tm, unsigned long _tp);
    uint8_t getMode();
    void setAnimationDelay(uint16_t _tp);
    void setLedColor(uint32_t _c);
    void clearLeds();
    void setState(uint8_t _enable);
    uint8_t getState();
    void setAutoChange(uint8_t _autoChange);
    void setMelody(uint8_t _melody);
    uint8_t getCurrentMelody();
    void reactLEDsToMusic(int16_t _maxValue, uint8_t _ledCh);
    uint32_t color24(byte r, byte g, byte b);
    uint32_t ColorHSV(uint16_t hue, uint8_t sat, uint8_t val);

  private:
    Adafruit_WS2801 *_leds[3] = {NULL, NULL, NULL};
    audio *_audioPtr = NULL;
    uint32_t ledColor = 0b000011110000111100001111;

    const uint8_t red[9] = {128, 255, 0, 255, 32, 255, 255, 8, 255};
    const uint8_t green[9] = {16, 64, 192, 128, 255, 2, 128, 8, 8};
    const uint8_t blue[9] = {255, 0, 255, 128, 8, 2, 8, 255, 16};
    const uint8_t oldColors[5][3] = {{red[CRVENA], green[CRVENA], blue[CRVENA]}, {red[NARANCASTA], green[NARANCASTA], blue[NARANCASTA]}, {red[PLAVA], green[PLAVA], blue[PLAVA]}, {red[ROZA], green[ROZA], blue[ROZA]}, {red[ZELENA], green[ZELENA], blue[ZELENA]}};

    struct _currentMode
    {
      uint8_t _mode = LED_CTRL_MODE_STATIC_1;
      unsigned long _modeTimeout = 0;
      unsigned long _modeTimestamp = 0;
      unsigned long _patternChangeTimeout = 0;
      unsigned long _patternTimestamp = 0;
      uint8_t _patternSeq = 0;
      uint8_t _enabled = 0;
      uint8_t _autoChange = 1;
      uint8_t _melody = 0;
      uint8_t _currentMelody = 0;
    }_mode;
};

#endif