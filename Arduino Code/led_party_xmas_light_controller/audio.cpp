#include "audio.h"

audio::audio()
{
    // Empty constructor
}

void audio::initAudio()
{
    memset(adc, 0, sizeof(adc));
}

void audio::getAudio()
{
    for (int i = 0; i < 128; i++)
    {
        adc[i] = (analogRead(AUDIO_IN) >> 2) - 127;
    }

    for (int i = 0; i < 125; i++)
    {
        int32_t _movingAvg = 0;
        for (int j = 0; j < 3; j++)
        {
            _movingAvg += adc[i + j];
        }
        _movingAvg /= 3;
        adc[i] = abs(_movingAvg);
    }

    int16_t max = adc[0];
    for (int i = 0; i < 125; i++)
    {
        if (adc[i] > max)
            max = adc[i];
    }

    max *= 2;

    if (max > maxLevel)
    {
        maxLevel = max;
    }
    else
    {
        if (maxLevel > 3)
            maxLevel -= 3;
    }
}

int16_t audio::getPeak()
{
    return maxLevel;
}

void audio::resetAudio()
{
    memset(adc, 0, sizeof(adc));
    maxLevel = 0;
    k = 0;
}