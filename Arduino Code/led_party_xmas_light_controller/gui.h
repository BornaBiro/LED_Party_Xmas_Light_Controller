#ifndef __GUI_H__
#define __GUI_H__

#include "pcf85063.h"
#include "controller.h"
#include "LiquidCrystal_I2C.h"
#include "defines.h"

class GUI
{
    public:
    GUI();
    void init(LiquidCrystal_I2C &lcd, pcf85063 &rtc, controller *_ctrl);
    void update(LiquidCrystal_I2C &lcd, pcf85063 &rtc);
    void mainScreen(LiquidCrystal_I2C &lcd, pcf85063 &rtc, time_t _myEpoch);
    void settingsScreen(LiquidCrystal_I2C &lcd, pcf85063 &rtc);
    uint8_t updateMenu(int _enc, int _encSw, int _sw, pcf85063 &rtc);
    void displayOff(LiquidCrystal_I2C &lcd);
    void displayOn(LiquidCrystal_I2C &lcd);
    char* getModeName(uint8_t _m);

    private:
    controller *_myCtrl = NULL;
    struct deviceSettings settings;

    void drawItemSelector(LiquidCrystal_I2C &lcd, int8_t _pos);
    void drawSetting(LiquidCrystal_I2C &lcd, char *_title, char *_setting);
    void checkLimits(int8_t _min, int8_t _max, int8_t *_var);

    char lcdBuffer[20];
    unsigned long _lcdTimeout = 0;
    uint8_t _lcdOn = 1;
    int8_t _currentItem = 0;
    int8_t _currentMenu = 0;
    uint8_t updateNeeded = 0;
    int8_t _menuCursor = -1;
    const char *_autoSettingStr[3] = {"Off", "On", "Auto"};
};

#endif