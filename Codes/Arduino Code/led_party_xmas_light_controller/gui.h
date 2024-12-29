#ifndef __GUI_H__
#define __GUI_H__

#include "pcf85063.h"
#include "LiquidCrystal_I2C.h"
#include "defines.h"

// Include LED Controller library.
#include "ledController.h"

// Include EEPROM library.
#include <EEPROM.h>

// EEPROM start addresses
#define GUI_EEPROM_ADDR_MODE            0x00
#define GUI_EEPROM_ADDR_ANIM_DELAY      0x01
#define GUI_EEPROM_ADDR_ANIM_DUR        0x03
#define GUI_EEPROM_ADDR_AUTO_CHANGE     0x05
#define GUI_EEPROM_ADDR_MELODY_EN       0x06
#define GUI_EEPROM_ADDR_AUTOSTART_LED   0x07
#define GUI_EEPROM_ADDR_AUTOSTART_RLY   0x08
#define GUI_EEPROM_ADDR_AUTOTIME1       0x09
#define GUI_EEPROM_ADDR_AUTOTIME2       0x0E
#define GUI_EEPROM_ADDR_AUTOTIME3       0x13
#define GUI_EEPROM_ADDR_SECRET_KEY      0x18

// Key for the EEPROM validity.
#define GUI_EEPROM_SECRET_KEY_VALUE     65

class GUI
{
    public:
    GUI();
    void init(LiquidCrystal_I2C &lcd, pcf85063 &rtc, LedCtrl *_ctrl);
    void update(LiquidCrystal_I2C &lcd, pcf85063 &rtc);
    void mainScreen(LiquidCrystal_I2C &lcd, pcf85063 &rtc, time_t _myEpoch);
    void settingsScreen(LiquidCrystal_I2C &lcd, pcf85063 &rtc);
    uint8_t updateMenu(int _enc, int _encSw, int _sw, pcf85063 &rtc);
    void displayOff(LiquidCrystal_I2C &lcd);
    void displayOn(LiquidCrystal_I2C &lcd);

    private:
    LedCtrl *_myCtrl = NULL;

    void drawItemSelector(LiquidCrystal_I2C &lcd, int8_t _pos);
    void drawSetting(LiquidCrystal_I2C &lcd, char *_title, char *_setting, int _startPosition, int _menuPosition);
    void checkLimits(int8_t _min, int8_t _max, int8_t *_var);
    void checkActiveTime(pcf85063 &rtc);
    int restoreSettings(deviceSettings *_s);

    struct deviceSettings settings;
    const char *_autoSettingStr[3] = {"Off", "On", "Auto"};
    char lcdBuffer[20];
    unsigned long _lcdTimeout = 0;
    uint8_t _lcdOn = 1;
    int8_t _currentItem = 0;
    int8_t _currentMenu = 0;
    uint8_t updateNeeded = 0;
    int8_t _menuCursor = -1;
    uint8_t _relayState = 0;
};

#endif