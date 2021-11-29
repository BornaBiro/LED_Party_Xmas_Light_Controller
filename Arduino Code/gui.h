#ifndef __GUI_H__
#define __GUI_H__

#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"

#include "pcf85063.h"

#define GUI_RTC_TIME 0x01
#define GUI_RTC_DATE 0x02

class GUI
{
    public:
    GUI();
    void init(pcf85063 *_r, Adafruit_SSD1306 *_o);
    void displayTime(uint8_t _flags, time_t _myEpoch);

    private:
    pcf85063 *_myRtc = NULL;
    Adafruit_SSD1306 *_myOled = NULL;
};

#endif