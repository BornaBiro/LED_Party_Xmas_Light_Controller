#include "gui.h"

GUI::GUI()
{

}

void GUI::init(pcf85063 *_r, Adafruit_SSD1306 *_o, controller *_c)
{
    _myRtc = _r;
    _myOled = _o;
    _myCtrl = _c;
    
    mainScreen(_r->getClock());
}

void GUI::update()
{
    if (!_oledOn) return;

    if (!_currentItem && !_currentMenu && _myRtc->available())
    {
        mainScreen(_myRtc->getClock());
    }

    if (_currentMenu)
    {
        settingsScreen();
    }

    if(((unsigned long)(millis() - _oledTimeout) > 20000) && _oledOn) displayOff();

    _myOled->display();
}

void GUI::mainScreen(time_t _myEpoch)
{
    char temp[20] = {0};
    
    _myOled->clearDisplay();
    _myOled->setTextColor(WHITE, BLACK);
    _myOled->setCursor(0, 0);

    struct tm _tm = _myRtc->epochToHuman(_myEpoch);
    sprintf(temp, "%02d:%02d:%02d %02d/%02d/%04d", _tm.tm_hour, _tm.tm_min, _tm.tm_sec, _tm.tm_mday, _tm.tm_mon + 1, _tm.tm_year + RTC_HUMAN_YEAR);
    _myOled->print(temp);
    
    sprintf(temp, "Mode:%s", getModeName(_myCtrl->getMode()));
    _myOled->setCursor(0, 8);
    _myOled->print(temp);

    sprintf(temp, "Dur:%dmin  Speed:%dsec", 10, 2);
    _myOled->setCursor(0, 16);
    _myOled->print(temp);
}

void GUI::settingsScreen()
{

}

void GUI::displayOff()
{
    if (_oledOn)
    {
        _oledOn = 0;
        _myOled->ssd1306_command(SSD1306_DISPLAYOFF);
    }
}

void GUI::displayOn()
{
    _oledTimeout = millis();
    if (!_oledOn)
    {
        _oledOn = 1;
        _myOled->ssd1306_command(SSD1306_DISPLAYON);
    }
}

void GUI::resetMenu()
{
    _currentItem = 0;
    _currentMenu = 0;
}

char* GUI::getModeName(uint8_t _m)
{
    static char _s[20];
    if (_m >= LED_CTRL_MODE_STATIC_1 && _m <= LED_CTRL_MODE_STATIC_4)
    {
        sprintf(_s, "Xmas static %d", _m + 1);
    }

    if (_m >= LED_CTRL_MODE_XMAS_1 && _m <= LED_CTRL_MODE_XMAS_5)
    {
        sprintf(_s, "Xmas dynam. %d", _m - LED_CTRL_MODE_XMAS_1 + 1);
    }

    if (_m >= LED_CTRL_MODE_PARTY_1 && _m <= LED_CTRL_MODE_PARTY_4)
    {
        sprintf(_s, "Party. %d", _m - LED_CTRL_MODE_PARTY_1 + 1);
    }

    if (_m >= LED_CTRL_MODE_PARTY_MUSIC_1 && _m <= LED_CTRL_MODE_PARTY_MUSIC_3)
    {
        sprintf(_s, "Party dynam. %d", _m - LED_CTRL_MODE_PARTY_MUSIC_1 + 1);
    }
    return _s;
}