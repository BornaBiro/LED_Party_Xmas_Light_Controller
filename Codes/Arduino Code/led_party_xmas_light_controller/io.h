#ifndef __IO_H__
#define __I0_H__

#define BTN1_PRESSED_TRESHOLD 100

#include "Arduino.h"
#include "defines.h"

static volatile int _encVar = 0;
static volatile uint8_t _encSwVar = 0;

class IO
{   
    public:
    IO();
    uint8_t configPins();
    uint8_t configRotaryEnc();
    bool rotaryEncAvaialble();
    int getRotaryEnc();
    void getRotaryEnc(int *_a);
    bool getRotaryEncSW();
    bool getButton1State();
    bool getButton1();

    #ifdef USE_RELAY_OUTPUT
    uint8_t relay(int8_t _state);
    #endif

    private:
    bool _btn1State;
};

#endif