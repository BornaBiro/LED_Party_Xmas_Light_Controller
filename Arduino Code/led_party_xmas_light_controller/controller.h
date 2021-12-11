#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#include "Adafruit_WS2801.h"
#include "defines.h"

#define LED_CTRL_MODE_STATIC        0
#define LED_CTRL_MODE_XMAS_CYCLE    1
#define LED_CTRL_MODE_PARTY_CYCLE   32
#define LED_CTRL_MODE_PARTY_MUSIC_1 64
#define LED_CTRL_MODE_PARTY_MUSIC_2 65
#define LED_CTRL_MODE_PARTY_MUSIC_3 66

#define LED_CHANNEL_1 0
#define LED_CHANNEL_2 1
#define LED_CHANNEL_3 2

class controller
{
  public:
    controller();
    void init(Adafruit_WS2801 *_l1, Adafruit_WS2801 *_l2, Adafruit_WS2801 *_l3);
    void setMode(uint8_t _mode);
    void setLedColor(uint32_t _c);
    void clearLeds(uint8_t _ledCh);
    void reactLEDsToMusic(int16_t _maxValue, uint8_t _ledCh);

  private:
    Adafruit_WS2801 *_leds[3] = {NULL, NULL, NULL};
    uint8_t ledMode = LED_CTRL_MODE_STATIC;
    uint8_t songs = 0;
    uint8_t relayOut = 0;
    uint32_t ledColor = 0b000011110000111100001111;
};

#endif