#include "gui.h"

GUI::GUI()
{

}

void GUI::init(pcf85063 *_r, Adafruit_SSD1306 *_o)
{
    _myRtc = _r;
    _myOled = _o;
}

void GUI::displayTime(uint8_t _flags, time_t _myEpoch)
{
    char temp[20] = {0};
    struct tm _tm = _myRtc->epochToHuman(_myEpoch);
    if (_flags & GUI_RTC_TIME)
    {
        sprintf(temp, "%02d:%02d:%02d ", _tm.tm_hour, _tm.tm_min, _tm.tm_sec);
        _myOled->print(temp);
    }
    
    if (_flags & GUI_RTC_DATE)
    {
        sprintf(temp, "%02d/%02d/%04d", _tm.tm_mday, _tm.tm_mon + 1, _tm.tm_year + RTC_HUMAN_YEAR);
        _myOled->print(temp);
    }
}