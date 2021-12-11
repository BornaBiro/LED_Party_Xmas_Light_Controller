#ifndef __AUDIO_H__
#define __AUDIO_H__

#include "Adafruit_WS2801.h"
#include "Arduino.h"
#include "defines.h"

class audio
{
  public:
    audio();
    void initAudio();
    void getAudio();
    int16_t getPeak();
    void resetAudio();

  private:
    int16_t maxLevel = 0;
    int8_t adc[128];
    int k;
};

#endif