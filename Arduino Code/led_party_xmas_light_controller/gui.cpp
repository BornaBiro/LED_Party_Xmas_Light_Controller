#include "gui.h"

GUI::GUI()
{
}

void GUI::init(LiquidCrystal_I2C &lcd, pcf85063 &rtc, controller *_ctrl)
{
    // Copy the conntroller object pointer. -> TODO: Use references!
    _myCtrl = _ctrl;

    // Set default settings (TODO: get everything from the EEPROM)
    settings.currentSetting = 0; // Xmas mode
    settings.currentMode = _ctrl->getMode();
    settings.animationDelay = 5; // 500ms
    settings.autoChange = 1;
    settings.melody = 0;
    settings.autoStartLeds = 0;
    settings.autoStartRelay = 0;
    settings.animationDuration = 30;
    settings.activationTimes[0] = {0, 0, 0, 0, 0};
    settings.activationTimes[1] = {0, 0, 0, 0, 0};
    settings.activationTimes[2] = {0, 0, 0, 0, 0};

    // Show the main screen.
    mainScreen(lcd, rtc, rtc.getClock());
}

void GUI::update(LiquidCrystal_I2C &lcd, pcf85063 &rtc)
{
    // If the LCD is off, there is no need for updating it.
    if (!_lcdOn)
        return;

    // Main menu only needs to be updated if no menu is selected and if there is update on the time or forced updated is
    // requested.
    if (!_currentItem && !_currentMenu && (rtc.available() || updateNeeded))
    {
        // Update the main screen.
        mainScreen(lcd, rtc, rtc.getClock());
    }

    // If the menu has been selected and there is request for the screen update, update the LCD.
    if (_currentMenu && updateNeeded)
    {
        // Send the updated screen.
        settingsScreen(lcd, rtc);
    }

    // If timeout time has passed, turn of the LCD.
    if (((unsigned long)(millis() - _lcdTimeout) > 20000) && _lcdOn)
        displayOff(lcd);

    // Clear the variable for the LCD update request.
    updateNeeded = 0;
}

void GUI::mainScreen(LiquidCrystal_I2C &lcd, pcf85063 &rtc, time_t _myEpoch)
{
    // On the main screen, it shoud be only time, mode and output status.

    // Get the time from the RTC
    struct tm _tm = rtc.epochToHuman(_myEpoch);

    // Write it to the screen.
    sprintf(lcdBuffer, "%02d:%02d:%02d", _tm.tm_hour, _tm.tm_min, _tm.tm_sec);
    lcd.setCursor(0, 0);
    lcd.print(lcdBuffer);

    // Write the selected mode.
    lcd.setCursor(0, 1);
    lcd.print(getModeName(_myCtrl->getMode()));
}

uint8_t GUI::updateMenu(int _enc, int _encSw, int _sw, pcf85063 &rtc)
{
    // No update on the encoder and buttons? Go back!
    if (_enc == 0 && _encSw == 0)
        return 0;

    // LCD is off? Return, because menu is not visable on the screen, but request screen update.
    if (!_lcdOn)
    {
        updateNeeded = 1;
        return 0;
    }

    if (_currentMenu != 0 && _currentItem == 0)
    {
        _currentMenu += _enc;

        if (_currentMenu > 10)
            _currentMenu = 1;
        if (_currentMenu < 1)
            _currentMenu = 10;

        updateNeeded = 1;
    }

    if (_encSw)
    {
        if (_currentMenu == 0)
        {
            _currentMenu = 1;
        }
        else
        {
            _currentItem++;
        }
        updateNeeded = 1;
    }

    switch (_currentMenu)
    {
    case 1: {
        _currentItem = 0;
        _menuCursor = -1;
        break;
    }

    case 2: {
        _currentItem = 0;
        _menuCursor = -1;
        break;
    }

    case 3: {
        if (_currentItem > 1)
        {
            _currentItem = 0;
            _menuCursor = -1;
        }

        if (_currentItem == 1)
        {
            _menuCursor = 0;
            settings.animationDelay += _enc;
            if (settings.animationDelay > 600)
                settings.animationDelay = 0;
            if (settings.animationDelay < 0)
                settings.animationDelay = 600;
        }
        updateNeeded = 1;
        break;
    }

    case 4: {
        if (_currentItem > 1)
        {
            _currentItem = 0;
            _menuCursor = -1;
        }

        if (_currentItem == 1)
        {
            _menuCursor = 0;
            settings.animationDuration += _enc;
            if (settings.animationDuration > 600)
                settings.animationDuration = 0;
            if (settings.animationDuration < 0)
                settings.animationDuration = 600;
        }
        updateNeeded = 1;
        break;
    }

    case 5: {
        if (_currentItem > 1)
        {
            _currentItem = 0;
            _menuCursor = -1;
        }
        if (_currentItem == 1)
        {
            _menuCursor = 0;
            settings.autoChange += _enc;
            settings.autoChange &= 1;
        }
        updateNeeded = 1;
        break;
    }

    case 6: {
        if (_currentItem > 1)
        {
            _currentItem = 0;
            _menuCursor = -1;
        }

        if (_currentItem == 1)
        {
            _menuCursor = 0;
            settings.melody += _enc;
            settings.melody &= 1;
        }
        updateNeeded = 1;
        break;
    }

    case 7: {
        if (_currentItem > 2)
        {
            _currentItem = 0;
            _menuCursor = -1;
        }

        if (_currentItem == 1)
        {
            settings.autoStartLeds += _enc;
            checkLimits(0, 2, &(settings.autoStartLeds));
            updateNeeded = 1;
            _menuCursor = 0;
            // LED controller needs to be updated.
        }

        if (_currentItem == 2)
        {
            settings.autoStartRelay += _enc;
            checkLimits(0, 2, &(settings.autoStartRelay));
            updateNeeded = 1;
            _menuCursor = 9;
            // Relay controller needs to be updated.
        }
        break;
    }
    case 8: {
        if (_currentItem > 2)
        {
            _currentItem = 0;
            _menuCursor = -1;
        }

        if (_currentItem == 1)
        {
            struct tm _myTime = rtc.epochToHuman(rtc.getClock());
            _myTime.tm_hour += _enc;
            _myTime.tm_sec = 0;
            checkLimits(0, 23, &(_myTime.tm_hour));
            rtc.setClock(mktime(&_myTime));
            updateNeeded = 1;
            _menuCursor = 0;
        }

        if (_currentItem == 2)
        {
            struct tm _myTime = rtc.epochToHuman(rtc.getClock());
            _myTime.tm_min += _enc;
            _myTime.tm_sec = 0;
            checkLimits(0, 59, &(_myTime.tm_min));
            rtc.setClock(mktime(&_myTime));
            updateNeeded = 1;
            _menuCursor = 3;
        }
        break;
    }
    case 9: {
        if (_currentItem > 15)
        {
            _currentItem = 0;
            _menuCursor = -1;
        }

        if (_currentItem)
        {
            uint8_t _currentSetting = _currentItem - 1;

            if (_currentSetting % 5 == 0)
            {
                settings.activationTimes[_currentSetting / 5].hoursOn += _enc;
                checkLimits(0, 23, &(settings.activationTimes[_currentSetting / 5].hoursOn));
                _menuCursor = 2;
            }
            if (_currentSetting % 5 == 1)
            {
                settings.activationTimes[_currentSetting / 5].minutesOn += _enc;
                checkLimits(0, 59, &(settings.activationTimes[_currentSetting / 5].minutesOn));
                _menuCursor = 5;
            }
            if (_currentSetting % 5 == 2)
            {
                settings.activationTimes[_currentSetting / 5].hoursOff += _enc;
                checkLimits(0, 23, &(settings.activationTimes[_currentSetting / 5].hoursOff));
                _menuCursor = 8;
            }
            if (_currentSetting % 5 == 3)
            {
                settings.activationTimes[_currentSetting / 5].minutesOff += _enc;
                checkLimits(0, 59, &(settings.activationTimes[_currentSetting / 5].minutesOff));
                _menuCursor = 11;
            }
            if (_currentSetting % 5 == 4)
            {
                settings.activationTimes[_currentSetting / 5].isEnabled += _enc;
                settings.activationTimes[_currentSetting / 5].isEnabled &= 1;
                _menuCursor = 14;
            }
        }
        updateNeeded = 1;
        break;
    }

    default: {
        if (_currentItem)
        {
            _currentItem = 0;
            _currentMenu = 0;
            updateNeeded = 1;
            _menuCursor = -1;
        }
        break;
    }
    }

    return 0;
}

void GUI::settingsScreen(LiquidCrystal_I2C &lcd, pcf85063 &rtc)
{
    if (updateNeeded)
    {
        lcd.clear();
        lcd.setCursor(0, 0);
    }
    else
    {
        return;
    }

    switch (_currentMenu)
    {
    case 1: {
        lcd.print("Current setting");
        break;
    }
    case 2: {
        lcd.print("Current mode");
        break;
    }
    case 3: {
        lcd.print("Animation delay");
        lcd.setCursor(1, 1);
        lcd.print(settings.animationDelay / 10.0);
        lcd.print('s');
        drawItemSelector(lcd, _menuCursor);
        break;
    }
    case 4: {
        lcd.print("Animation dur.");
        lcd.setCursor(1, 1);
        lcd.print(settings.animationDuration);
        lcd.print('s');
        drawItemSelector(lcd, _menuCursor);
        break;
    }
    case 5: {
        lcd.print("Auto change");
        lcd.setCursor(1, 1);
        lcd.print(settings.autoChange ? stringEnabled : stringDisabled);
        drawItemSelector(lcd, _menuCursor);
        break;
    }
    case 6: {
        lcd.print("Melody");
        lcd.setCursor(1, 1);
        lcd.print(settings.melody ? stringEnabled : stringDisabled);
        drawItemSelector(lcd, _menuCursor);
        break;
    }
    case 7: {
        lcd.print("Auto start");
        lcd.setCursor(1, 1);
        sprintf_P(lcdBuffer, "LED:%4s RLY:%4s", _autoSettingStr[settings.autoStartLeds], _autoSettingStr[settings.autoStartRelay]);
        lcd.print(lcdBuffer);
        drawItemSelector(lcd, _menuCursor);
        break;
    }
    case 8: {
        struct tm _myTime = rtc.epochToHuman(rtc.getClock());
        lcd.print("Set time");
        lcd.setCursor(1, 1);
        sprintf(lcdBuffer, "%02d:%02d", _myTime.tm_hour, _myTime.tm_min);
        lcd.print(lcdBuffer);
        drawItemSelector(lcd, _menuCursor);
        break;
    }
    case 9: {
        uint8_t _selectedTimer;
        if (_currentItem)
        {
            _selectedTimer = ((_currentItem - 1) / 5);
        }
        else
        {
            _selectedTimer = 0;
        }

        lcd.print("Activation time");
        lcd.setCursor(1, 1);
        sprintf(lcdBuffer, "%d %02d:%02d-%02d:%02d %d", (unsigned int)(_selectedTimer) + 1,
                settings.activationTimes[_selectedTimer].hoursOn, settings.activationTimes[_selectedTimer].minutesOn,
                settings.activationTimes[_selectedTimer].hoursOff, settings.activationTimes[_selectedTimer].minutesOff,
                settings.activationTimes[_selectedTimer].isEnabled);
        lcd.print(lcdBuffer);
        drawItemSelector(lcd, _menuCursor);
        break;
    }
    default: {
        lcd.print("Exit");
        break;
    }
    }
}

void GUI::displayOff(LiquidCrystal_I2C &lcd)
{
    if (_lcdOn)
    {
        _lcdOn = 0;
        _currentItem = 0;
        _currentMenu = 0;
        _menuCursor = -1;
        lcd.noBacklight();
        lcd.noDisplay();
    }
}

void GUI::displayOn(LiquidCrystal_I2C &lcd)
{
    _lcdTimeout = millis();
    if (!_lcdOn)
    {
        _lcdOn = 1;
        lcd.backlight();
        lcd.display();
    }
}

char *GUI::getModeName(uint8_t _m)
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


// Private functions
void GUI::drawItemSelector(LiquidCrystal_I2C &lcd, int8_t _pos)
{
    if (_pos < 0)
        return;
    lcd.setCursor(_pos, 1);
    lcd.print('>');
}

void GUI::checkLimits(int8_t _min, int8_t _max, int8_t *_var)
{
    if (*_var > _max)
        *_var = _min;
    if (*_var < _min)
        *_var = _max;
}

void GUI::drawSetting(LiquidCrystal_I2C &lcd, char *_title, char *_setting)
{
}