#include "audio.h"

audio::audio()
{
    // Empty constructor
}

void audio::initAudio()
{
    memset(adcBuffer, 0, ADC_BUFFER_SIZE);
    bufferTail = 0;
    bufferHead = 0;
    ADMUX = 0;
    ADMUX |= 0b00000110; // ADC6

    ADCSRA = 0;
    ADCSRA |= (1 << ADEN) | (1 << ADIE) | (1 << ADIF) | (1 << ADATE) | 0b00000110;

    ADCSRB = 0;

    ADCSRA |= (1 << ADSC);
}

uint8_t audio::getAudio()
{
    if (sampleCount() > 100)
    {
        int8_t adc[128];
        uint8_t samples = sampleCount();
        for (int i = 0; i < samples; i++)
        {
            adc[i] = getSample();
        }

        for (int i = 0; i < samples - _movingAvgSamples; i++)
        {
            int32_t _movingAvg = 0;
            for (int j = 0; j < _movingAvgSamples; j++)
            {
                _movingAvg += adc[i + j];
            }
            _movingAvg /= _movingAvgSamples;
            adc[i] = abs(_movingAvg);
        }

        int16_t max = adc[0];
        for (int i = 0; i < samples - _movingAvgSamples; i++)
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
            if (maxLevel > 1)
                maxLevel -= 1;
        }
        return 1;
    }
    else
    {
        return 0;
    }
}

int16_t audio::getPeak()
{
    return maxLevel;
}

void audio::resetAudio()
{
}

void audio::pauseAudio()
{
    allowNextConversion = 0;
}

void audio::resumeAudio()
{
    allowNextConversion = 1;
    ADCSRA |= (1 << ADSC);
}

int8_t audio::getSample()
{
    if (bufferHead == bufferTail)
    {
        return 0;
    }
    else
    {
        int8_t c = adcBuffer[bufferTail];
        bufferTail = (bufferTail + 1) % ADC_BUFFER_SIZE;
        return c;
    }
}

int audio::sampleCount()
{
    return ((uint8_t)(ADC_BUFFER_SIZE + bufferHead - bufferTail)) % ADC_BUFFER_SIZE;
}

ISR(ADC_vect)
{
    int16_t _result = ADC;
    uint8_t i = (uint8_t)(bufferHead + 1) % ADC_BUFFER_SIZE;
    if (i != bufferTail)
    {
        adcBuffer[bufferHead] = (_result - 511) >> 2;
        bufferHead = i;
    }
}