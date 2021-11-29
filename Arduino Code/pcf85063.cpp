#include "PCF85063.h"

pcf85063::pcf85063()
{
}

void pcf85063::readRegisters(uint8_t _reg, uint8_t *_data, uint8_t n)
{
    Wire.beginTransmission((uint8_t) PCF85063_ADDR);
    Wire.write(_reg);
    Wire.endTransmission(false);

    Wire.requestFrom((uint8_t) PCF85063_ADDR, n, true);

    while(Wire.available() != n);

    for (int i = 0; i < n; i++)
    {
        _data[i] = (uint8_t)(Wire.read());
    }
}

void pcf85063::writeRegisters(uint8_t _reg, uint8_t *_data, uint8_t n)
{
    Wire.beginTransmission(PCF85063_ADDR);
    Wire.write(_reg);
    for (int i = 0; i < n; i++)
    {
        Wire.write(_data[i]);
    }
    Wire.endTransmission();
}

void pcf85063::RTCinit(uint8_t resetRTC)
{
    uint8_t regData;
    if (resetRTC)
    {
        // Stop RTC
        regData = 0b00100000;
        writeRegisters(PCF85063_CTRL1, &regData, 1);
        delay(1);

        // Do a SW RST of RTC
        regData = 0b00110000;
        writeRegisters(PCF85063_CTRL1, &regData, 1);
        delay(1);
    }

    // Clear interrupts and set clockout frequency (1Hz)
    regData = PCF85063_CTRL2_MASK;
    writeRegisters(PCF85063_CTRL2, &regData, 1);

    // Start RTC normally with internal cap set to 12.5 pF
    regData = 0b00000001;
    writeRegisters(PCF85063_CTRL1, &regData, 1);
}

time_t pcf85063::getClock()
{
    uint8_t regData[7];
    struct tm _myTime;
    memset(&_myTime, 0, sizeof(_myTime));
    readRegisters(PCF85063_SECONDS, regData, sizeof(regData));

    _myTime.tm_sec = bcd2dec(regData[0] & 0x7f);
    _myTime.tm_min = bcd2dec(regData[1]  & 0x7f);
    _myTime.tm_hour = bcd2dec(regData[2]  & 0x7f);
    _myTime.tm_mday = bcd2dec(regData[3]  & 0x3f);
    _myTime.tm_wday = bcd2dec(regData[4]  & 0x07);
    _myTime.tm_mon = bcd2dec(regData[5]  & 0x1f);
    _myTime.tm_year = bcd2dec(regData[6]) + 2000 - RTC_HUMAN_YEAR; // 1870 instead of 1900 because of stupid offsets

    //char tmp[50];
    //sprintf(tmp, "%d %d %d %d %d %d %d", bcd2dec(regData[4]  & 0x07), bcd2dec(regData[3]  & 0x3f), bcd2dec(regData[5]  & 0x1f), bcd2dec(regData[6]) + 2000, bcd2dec(regData[2]  & 0x7f), bcd2dec(regData[1]  & 0x7f), bcd2dec(regData[0] & 0x7f));
    //Serial.println(tmp);
    return mktime(&_myTime);
}

void pcf85063::setClock(time_t _epoch)
{
    struct tm _myTime;
    memset(&_myTime, 0, sizeof(_myTime));
    uint8_t regData[8];
    memcpy(&_myTime, gmtime((const time_t *)&_epoch), sizeof(_myTime));
    // 0x0f sotred i RAM means that clock is alredy set
    regData[0] = 0x0f;
    regData[1] = dec2bcd(_myTime.tm_sec);
    regData[2] = dec2bcd(_myTime.tm_min);
    regData[3] = dec2bcd(_myTime.tm_hour);
    regData[4] = dec2bcd(_myTime.tm_mday);
    regData[5] = _myTime.tm_wday;
    regData[6] = dec2bcd(_myTime.tm_mon);
    regData[7] = dec2bcd((_myTime.tm_year + RTC_HUMAN_YEAR) % 100); // 1870 instead of 1900 because of stupid offsets
    writeRegisters(PCF85063_RAMBYTE, regData, sizeof(regData));
}

void pcf85063::setAlarm(time_t _epoch)
{
    uint8_t regData[5];
    struct tm _myTime;
    memset(&_myTime, 0, sizeof(_myTime));

    clearAlarm();
    disableAlarm();

    memcpy(&_myTime, localtime((const time_t *)&_epoch), sizeof(_myTime));
    regData[0] = dec2bcd(_myTime.tm_sec);
    regData[1] = dec2bcd(_myTime.tm_min);
    regData[2] = dec2bcd(_myTime.tm_hour);
    regData[3] = dec2bcd(_myTime.tm_mday);
    regData[4] = _myTime.tm_wday;

    // Set up alarm time
    writeRegisters(PCF85063_SEC_AL, regData, 5);

    // Enable alarm on INT pin
    regData[0] = PCF85063_CTRL2_MASK | 0b10000000;
    writeRegisters(PCF85063_CTRL2, regData, 1);
}

void pcf85063::disableAlarm()
{
    // Disable alarm on INT pin
    uint8_t regData = PCF85063_CTRL2_MASK | 0b00000000;
    writeRegisters(PCF85063_CTRL2, &regData, 1);

    // And also clear the alarm flag
    clearAlarm();
}

bool pcf85063::checkAlarmFlag()
{
    uint8_t regData;
    readRegisters(PCF85063_CTRL2, &regData, 1);
    return regData & 0b01000000;
}

void pcf85063::clearAlarm()
{
    uint8_t regData = PCF85063_CTRL2_MASK;
    writeRegisters(PCF85063_CTRL2, &regData, 1);
}

uint8_t pcf85063::isClockSet()
{
    uint8_t regData;
    readRegisters(PCF85063_RAMBYTE, &regData, 1);
    return regData;
}

struct tm pcf85063::epochToHuman(time_t _t)
{
    struct tm _tm;
    memset(&_tm, 0, sizeof(_tm));
    memcpy(&_tm, localtime((const time_t *)(&_t)), sizeof(_tm));
    return _tm;
}

void pcf85063::intOnSeconds(bool _en)
{
    if (_en)
    {
        PCICR |= (1 << PCIE0);   // Enable PCINT from PCINT0 to PCINT7
        PCMSK0 |= (1 << PCINT1); // Enable PCINT0
    }
    else
    {
        PCICR &= ~(1 << PCIE0);   // Disable PCINT from PCINT0 to PCINT7
        PCMSK0 &= ~(1 << PCINT1); // Disable PCINT0
    }
}

void pcf85063::rtcInt(bool _en)
{
    if (_en)
    {
        PCICR |= (1 << PCIE0);   // Enable PCINT from PCINT0 to PCINT7
        PCMSK0 |= (1 << PCINT0); // Enable PCINT0
    }
    else
    {
        PCICR &= ~(1 << PCIE0);   // Disable PCINT from PCINT0 to PCINT7
        PCMSK0 &= ~(1 << PCINT0); // Disable PCINT0
    }
}

bool pcf85063::available()
{
    bool _ret = _secondsFlag & 1;
    _secondsFlag = 0;
    return _ret;
}

bool pcf85063::availableINT()
{
    bool _ret = _rtcIntFlag & 1;
    _rtcIntFlag = 0;
    return _ret;
}

ISR(PCINT0_vect)
{
    if (PINB & (1 << PB1))
    {
        _secondsFlag = 1;
    }

    if (!digitalRead(8))
    {
        _rtcIntFlag = 1;
    }
}