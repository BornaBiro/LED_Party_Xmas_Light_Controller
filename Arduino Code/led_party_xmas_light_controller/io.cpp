#include "io.h"

IO::IO()
{
    // Empty constructor
}

uint8_t IO::configPins()
{
    pinMode(BUZZER, OUTPUT);
    pinMode(ENC_A, INPUT);
    pinMode(ENC_B, INPUT);
    pinMode(ENC_SW, INPUT);
    pinMode(RTC_INT, INPUT_PULLUP);
    pinMode(RTC_CLKOUT, INPUT_PULLUP);

#ifdef USE_RELAY_OUTPUT
    pinMode(RELAY, OUTPUT);
#endif
    return 1;

    // Get inital state of the Button1
    _btn1State = getButton1State();
}

#ifdef USE_RELAY_OUTPUT
uint8_t IO::relay(int8_t _state)
{
    if (_state != -1)
        digitalWrite(RELAY, _state);
    return digitalRead(RELAY);
}
#endif

uint8_t IO::configRotaryEnc()
{
    // Config rotary encoder pins A & B in interrupt mode
    EICRA |= (1 << ISC01); // Set INT0 on falling egde
    EIMSK |= (1 << INT0);  // Enable INT0

    // Config rotary encoder SW pin in pin change mode
    PCICR |= (1 << PCIE1);    // Enable PCINT from PCINT8 to PCINT14
    PCMSK1 |= (1 << PCINT10); // Enable mask on PCINT10

    // Enable global Interrupts
    sei();
}

bool IO::rotaryEncAvaialble()
{
    return (_encVar == 0 ? false : true);
}

int IO::getRotaryEnc()
{
    int _temp = _encVar;
    _encVar = 0;
    return _temp;
}

void IO::getRotaryEnc(int *_a)
{
    *(_a) += _encVar;
    _encVar = 0;
}

bool IO::getRotaryEncSW()
{
    uint8_t _temp = _encSwVar;
    _encSwVar = 0;
    return _temp;
}

bool IO::getButton1State()
{
    int _btn = analogRead(A7);
    return ((_btn < BTN1_PRESSED_TRESHOLD)?true:false);
}

bool IO::getButton1()
{
    bool _currentState = getButton1State();
    bool _ret = false;
    if (_currentState && !_btn1State) _ret = true;
    _btn1State = _currentState;
    return _ret;
}

// Low level stuf
ISR(INT0_vect)
{
    if (digitalRead(ENC_B))
    {
        _encVar--;
    }
    else
    {
        _encVar++;
    }
}

ISR(PCINT1_vect)
{
    if (!digitalRead(ENC_SW)) _encSwVar = 1;
}