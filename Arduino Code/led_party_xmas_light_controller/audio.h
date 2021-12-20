#ifndef __AUDIO_H__
#define __AUDIO_H__

#include "Adafruit_WS2801.h"
#include "Arduino.h"
#include "defines.h"

#define ADC_BUFFER_SIZE            64

static volatile uint8_t allowNextConversion = 0;
static volatile uint8_t adcBuffer[ADC_BUFFER_SIZE];
static volatile uint16_t bufferTail = 0;
static volatile uint16_t bufferHead = 0;

class audio
{
  public:
    audio();
    void initAudio();
    uint8_t getAudio();
    int16_t getPeak();
    void resetAudio();
    void pauseAudio();
    void resumeAudio();
    int8_t getSample();
    int sampleCount();


  private:
  int16_t maxLevel = 0;
  const uint8_t _movingAvgSamples = 7;
};

#endif