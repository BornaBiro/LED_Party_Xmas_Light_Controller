#ifndef __PCF85063_H___
#define __PCF85063_H___

#include <Arduino.h>
#include <Wire.h>
#include <time.h>
// NOTE: This AVR time.h C library uses some stupid offsets. Also it starts from Y2K, not form 1970!

#define PCF85063_ADDR 0x51

#define PCF85063_CTRL1   0x00
#define PCF85063_CTRL2   0x01
#define PCF85063_OFFSET  0x02
#define PCF85063_RAMBYTE 0x03
#define PCF85063_SECONDS 0x04
#define PCF85063_MINUTES 0x05
#define PCF85063_HOURS   0x06
#define PCF85063_MDAYS   0x07
#define PCF85063_WDAYS   0x08
#define PCF85063_MONTHS  0x09
#define PCF85063_YEARS   0x0A

#define PCF85063_SEC_AL  0x0B
#define PCF85063_MIN_AL  0x0C
#define PCF85063_HOUR_AL 0x0D
#define PCF85063_MDAY_AL 0x0E
#define PCF85063_WDAY_AL 0x0F

#define PCF85063_CTRL2_MASK 0b00000110

#define RTC_HUMAN_YEAR 1870

static inline uint8_t bcd2dec(uint8_t _bcd)
{
    return ((_bcd & 0x0f) + ((_bcd >> 4) * 10));
};

static inline uint8_t dec2bcd(uint8_t _dec)
{
    return ((_dec % 10) | ((_dec / 10) << 4));
}

static volatile uint8_t _secondsFlag = 0;
static volatile uint8_t _rtcIntFlag = 0;

class pcf85063
{
  public:
    pcf85063();
    void RTCinit(uint8_t resetRTC = 0);
    time_t getClock();
    void setClock(time_t _epoch);
    void setAlarm(time_t _epoch);
    void disableAlarm();
    bool checkAlarmFlag();
    void clearAlarm();
    uint8_t isClockSet();
    struct tm pcf85063::epochToHuman(time_t _t);
    void intOnSeconds(bool _en);
    void rtcInt(bool _en);
    bool available();
    bool availableINT();

  private:
    void readRegisters(uint8_t _reg, uint8_t *_data, uint8_t n);
    void writeRegisters(uint8_t _reg, uint8_t *_data, uint8_t n);
};

#endif