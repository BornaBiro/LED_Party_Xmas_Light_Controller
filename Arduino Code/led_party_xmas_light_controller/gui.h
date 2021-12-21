#ifndef __GUI_H__
#define __GUI_H__

#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"

#include "pcf85063.h"
#include "controller.h"

class GUI
{
    public:
    GUI();
    void init(pcf85063 *_r, Adafruit_SSD1306 *_o, controller *_c);
    void update();
    void mainScreen(time_t _myEpoch);
    void settingsScreen();
    void resetMenu();
    void displayOff();
    void displayOn();
    char* getModeName(uint8_t _m);

    private:
    pcf85063 *_myRtc = NULL;
    Adafruit_SSD1306 *_myOled = NULL;
    controller *_myCtrl = NULL;

    unsigned long _oledTimeout = 0;
    uint8_t _oledOn = 1;
    uint8_t _currentItem = 0;
    uint8_t _currentMenu = 0;
};

#endif