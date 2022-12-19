#include "gui.h"

GUI::GUI()
{
}

void GUI::init(LiquidCrystal_I2C &lcd, pcf85063 &rtc, controller *_ctrl)
{
    // Copy the conntroller object pointer. -> TODO: Use references!
    _myCtrl = _ctrl;

    if (restoreSettings(&settings) == 0)
    {
        // Set default settings (TODO: get everything from the EEPROM)
        settings.currentSetting = 0; // Xmas mode
        settings.currentMode = 0;
        settings.animationDelay = 5; // 500ms
        settings.autoChange = 1;
        settings.melody = 0;
        settings.autoStartLeds = 0;
        settings.autoStartRelay = 0;
        settings.animationDuration = 30;
        settings.activationTimes[0] = {0, 0, 0, 0, 0};
        settings.activationTimes[1] = {0, 0, 0, 0, 0};
        settings.activationTimes[2] = {0, 0, 0, 0, 0};

        // Save settings to EEPROM
        EEPROM.put(GUI_EEPROM_ADDR_MODE, settings.currentMode);
        EEPROM.put(GUI_EEPROM_ADDR_ANIM_DELAY, settings.animationDelay);
        EEPROM.put(GUI_EEPROM_ADDR_ANIM_DUR, settings.animationDuration);
        EEPROM.put(GUI_EEPROM_ADDR_AUTO_CHANGE, settings.autoChange);
        EEPROM.put(GUI_EEPROM_ADDR_MELODY_EN, settings.melody);
        EEPROM.put(GUI_EEPROM_ADDR_AUTOSTART_LED, settings.autoStartLeds);
        EEPROM.put(GUI_EEPROM_ADDR_AUTOSTART_RLY, settings.autoStartRelay);
        EEPROM.put(GUI_EEPROM_ADDR_AUTOTIME1, settings.activationTimes[0]);
        EEPROM.put(GUI_EEPROM_ADDR_AUTOTIME2, settings.activationTimes[1]);
        EEPROM.put(GUI_EEPROM_ADDR_AUTOTIME3, settings.activationTimes[2]);

        // Write down EEPROM KEY
        uint8_t _key = GUI_EEPROM_ADDR_SECRET_KEY;
        EEPROM.put(GUI_EEPROM_ADDR_MODE, _key);
    }

    // Show the main screen.
    mainScreen(lcd, rtc, rtc.getClock());
}

void GUI::update(LiquidCrystal_I2C &lcd, pcf85063 &rtc)
{
    // Save the status of the RTC.
    uint8_t _rtcTick = rtc.available();

    // Check if the state of the LEDs or relay output needs to be changed.
    if (_rtcTick)
        checkActiveTime(rtc);

    // If the LCD is off, there is no need for updating it.
    if (!_lcdOn)
        return;

    // Main menu only needs to be updated if no menu is selected and if there is update on the time or forced updated is
    // requested.
    if (!_currentItem && !_currentMenu && (_rtcTick || updateNeeded))
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
                settings.animationDelay = 1;
            if (settings.animationDelay < 1)
                settings.animationDelay = 600;

            EEPROM.put(GUI_EEPROM_ADDR_ANIM_DELAY, settings.animationDelay);
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
                settings.animationDuration = 1;
            if (settings.animationDuration < 1)
                settings.animationDuration = 600;

            EEPROM.put(GUI_EEPROM_ADDR_ANIM_DUR, settings.animationDuration);
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
            EEPROM.put(GUI_EEPROM_ADDR_AUTO_CHANGE, settings.autoChange);
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
            EEPROM.put(GUI_EEPROM_ADDR_MELODY_EN, settings.melody);
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
            _menuCursor = 0;
            settings.autoStartLeds += _enc;
            checkLimits(0, 2, &(settings.autoStartLeds));
            EEPROM.put(GUI_EEPROM_ADDR_AUTOSTART_LED, settings.autoStartLeds);
            updateNeeded = 1;
            // LED controller needs to be updated.
        }

        if (_currentItem == 2)
        {
            _menuCursor = 9;
            settings.autoStartRelay += _enc;
            checkLimits(0, 2, &(settings.autoStartRelay));

            if (settings.autoStartRelay < 2)
            {
                digitalWrite(RELAY, settings.autoStartRelay & 1);
                _relayState = settings.autoStartRelay & 1;
            }

            EEPROM.put(GUI_EEPROM_ADDR_AUTOSTART_RLY, settings.autoStartRelay);
            updateNeeded = 1;
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
                EEPROM.put(GUI_EEPROM_ADDR_AUTOTIME1 + ((_currentSetting / 5) * 5),
                           settings.activationTimes[_currentSetting / 5]);
                _menuCursor = 1;
            }
            if (_currentSetting % 5 == 1)
            {
                settings.activationTimes[_currentSetting / 5].minutesOn += _enc;
                checkLimits(0, 59, &(settings.activationTimes[_currentSetting / 5].minutesOn));
                EEPROM.put(GUI_EEPROM_ADDR_AUTOTIME1 + ((_currentSetting / 5) * 5),
                           settings.activationTimes[_currentSetting / 5]);
                _menuCursor = 4;
            }
            if (_currentSetting % 5 == 2)
            {
                settings.activationTimes[_currentSetting / 5].hoursOff += _enc;
                checkLimits(0, 23, &(settings.activationTimes[_currentSetting / 5].hoursOff));
                EEPROM.put(GUI_EEPROM_ADDR_AUTOTIME1 + ((_currentSetting / 5) * 5),
                           settings.activationTimes[_currentSetting / 5]);
                _menuCursor = 7;
            }
            if (_currentSetting % 5 == 3)
            {
                settings.activationTimes[_currentSetting / 5].minutesOff += _enc;
                checkLimits(0, 59, &(settings.activationTimes[_currentSetting / 5].minutesOff));
                EEPROM.put(GUI_EEPROM_ADDR_AUTOTIME1 + ((_currentSetting / 5) * 5),
                           settings.activationTimes[_currentSetting / 5]);
                _menuCursor = 10;
            }
            if (_currentSetting % 5 == 4)
            {
                settings.activationTimes[_currentSetting / 5].isEnabled += _enc;
                settings.activationTimes[_currentSetting / 5].isEnabled &= 1;
                EEPROM.put(GUI_EEPROM_ADDR_AUTOTIME1 + ((_currentSetting / 5) * 5),
                           settings.activationTimes[_currentSetting / 5]);
                _menuCursor = 13;
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
        sprintf(lcdBuffer, "%d.%01d sec", settings.animationDelay / 10, abs(settings.animationDelay % 10));
        drawSetting(lcd, "Animation delay", lcdBuffer, 1, _menuCursor);
        break;
    }
    case 4: {
        sprintf(lcdBuffer, "%d sec", settings.animationDuration);
        drawSetting(lcd, "Animation dur.", lcdBuffer, 1, _menuCursor);
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
        sprintf(lcdBuffer, "L:%4s S:%4s", _autoSettingStr[settings.autoStartLeds],
                _autoSettingStr[settings.autoStartRelay]);
        drawSetting(lcd, "Auto start", lcdBuffer, 1, _menuCursor);
        break;
    }
    case 8: {
        struct tm _myTime = rtc.epochToHuman(rtc.getClock());
        sprintf(lcdBuffer, "%02d:%02d", _myTime.tm_hour, _myTime.tm_min);
        drawSetting(lcd, "Set time", lcdBuffer, 1, _menuCursor);
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
        sprintf(lcdBuffer, "%d %02d:%02d-%02d:%02d %c", (unsigned int)(_selectedTimer) + 1,
                settings.activationTimes[_selectedTimer].hoursOn, settings.activationTimes[_selectedTimer].minutesOn,
                settings.activationTimes[_selectedTimer].hoursOff, settings.activationTimes[_selectedTimer].minutesOff,
                settings.activationTimes[_selectedTimer].isEnabled ? 'Y' : 'N');

        drawSetting(lcd, "Activation time", lcdBuffer, 0, _menuCursor);
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
        lcd.clear();
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

void GUI::checkActiveTime(pcf85063 &rtc)
{
    struct tm _currentTime = rtc.epochToHuman(rtc.getClock());
    uint8_t relayOn = 0;


    for (int i = 0; i < 3; i++)
    {
        if ((settings.activationTimes[i].hoursOn != settings.activationTimes[i].hoursOff) ||
            (settings.activationTimes[i].minutesOn != settings.activationTimes[i].minutesOff))
        {
            // "Shift" the time at the start of the current day to avoid issues with turn off time in the next day (aka.
            // day overlap issue). First, convert everytinh into minutes
            int minutesConvOn = (settings.activationTimes[i].hoursOn * 60) + settings.activationTimes[i].minutesOn;
            int minutesConvOff = (settings.activationTimes[i].hoursOff * 60) + settings.activationTimes[i].minutesOff;
            int minutesConvCurrent = (_currentTime.tm_hour * 60) + _currentTime.tm_min;

            // Shift everything so that "0 minutes" on timeline is onTime (or minutesConvOn)
            int16_t offTimeHours = ((minutesConvOff - minutesConvOn) + 1440) % 1440;
            int16_t currentHours = ((minutesConvCurrent - minutesConvOn) + 1440) % 1440;
            int16_t onTimeHours = 0;

            // Check wheater relay needs to be turned off or on.
            if (currentHours >= onTimeHours && currentHours <= offTimeHours && settings.activationTimes[i].isEnabled)
            {
                if (settings.autoStartRelay == 2)
                {
                    relayOn = 1;
                }
            }

            // if (currentHours >= offTimeHours && currentHours >= onTimeHours && settings.activationTimes[i].isEnabled
            // && settings.autoStartRelay == 2)
            // {
            //     if (_relayState)
            //     {
            //         digitalWrite(RELAY, LOW);
            //         _relayState = 0;
            //     }
            // }
        }
    }

    // If the flag is set, that means the relay needs to be turned on
    if (relayOn)
    {
        // Check if the relay is not already on
        if (!_relayState)
        {
            _relayState = 1;
            digitalWrite(RELAY, HIGH);
        }
    }
    else
    {
        // If relay do not need to be turned on, keep it off. But first check if the relay is not already off.
        if (_relayState)
        {
            _relayState = 0;
            digitalWrite(RELAY, LOW);
        }
    }
}

int GUI::restoreSettings(deviceSettings *_s)
{
    // First check the key (for the EEPROM data validity). If the key is not correct, return and use default settings.
    uint8_t _key;
    EEPROM.get(GUI_EEPROM_ADDR_MODE, _key);
    if (_key != GUI_EEPROM_ADDR_SECRET_KEY)
        return 0;

    EEPROM.get(GUI_EEPROM_ADDR_MODE, (_s->currentMode));
    EEPROM.get(GUI_EEPROM_ADDR_ANIM_DELAY, (_s->animationDelay));
    EEPROM.get(GUI_EEPROM_ADDR_ANIM_DUR, (_s->animationDuration));
    EEPROM.get(GUI_EEPROM_ADDR_AUTO_CHANGE, (_s->autoChange));
    EEPROM.get(GUI_EEPROM_ADDR_MELODY_EN, (_s->melody));
    EEPROM.get(GUI_EEPROM_ADDR_AUTOSTART_LED, (_s->autoStartLeds));
    EEPROM.get(GUI_EEPROM_ADDR_AUTOSTART_RLY, (_s->autoStartRelay));
    EEPROM.get(GUI_EEPROM_ADDR_AUTOTIME1, (_s->activationTimes[0]));
    EEPROM.get(GUI_EEPROM_ADDR_AUTOTIME2, (_s->activationTimes[1]));
    EEPROM.get(GUI_EEPROM_ADDR_AUTOTIME3, (_s->activationTimes[2]));

    return 1;
}


void GUI::drawSetting(LiquidCrystal_I2C &lcd, char *_title, char *_setting, int _startPosition, int _menuPosition)
{
    lcd.print(_title);
    lcd.setCursor(_startPosition, 1);
    lcd.print(_setting);
    drawItemSelector(lcd, _menuPosition);
}