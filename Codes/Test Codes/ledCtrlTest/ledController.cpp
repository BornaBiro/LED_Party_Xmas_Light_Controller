#include "ledController.h"

// Constructor.
LedCtrl::LedCtrl()
{
    // Empty ... for now! :)
}

void LedCtrl::begin(Adafruit_WS2801 *_ledsCh1, Adafruit_WS2801 *_ledsCh2, Adafruit_WS2801 *_ledsCh3)
{
    // Copy the object addresses into local pointer array.
    _ledObjectArray[0] = _ledsCh1;
    _ledObjectArray[1] = _ledsCh2;
    _ledObjectArray[2] = _ledsCh3;

    // Init all I/Os
    initIOPins();

    // Init typedefs.
    initFunctionPointers();

    // Set mode to the first mode.
    _currentLEDMode = 0;

    // Enable the controller.
    enable();
}

uint8_t LedCtrl::update()
{
    // Check if the LED Controlelr is enabled (if its not, there is no point in going any further).
    if (!_enabled) return;

    // Variable will hold the return value.
    // 1 = update occured.
    // 0 = no update occured.
    uint8_t _updateHappend = 0;

    if (modeList[_currentLEDMode].animation && !modeList[_currentLEDMode].melody)
    {
        // Check if the timeout has occured.
        if ((unsigned long)(millis() - _ledFrameTimer) < (unsigned long)(modeList[_currentLEDMode].animationFrameDelay * (1 + _speedOffset / 100.0))) return _updateHappend;

        // Get the new timestamp.
        _ledFrameTimer = millis();

        for (int i = 0; i < 3; i++)
        {
            // Check if the LED channel is used at all.
            if (checkLedObject(_ledObjectArray[i]))
            {
                modeList[_currentLEDMode].animationFunction(_ledObjectArray[i], &modeList[_currentLEDMode]);
            }
        }

        // Update the animation phase.
        updatePhase(&modeList[_currentLEDMode]);

        // Update the state of the return variable.
        _updateHappend = 1;
    }

    // Check if the melody also needs to be updated.
    if (modeList[_currentLEDMode].melody)
    {
        // TODO: Update the melody

        // Update the state of the return variable.
        //_retVaue = 1;
    }

    // Otherwise, check if the whole mode needs to be changed.
    if ((unsigned long)(millis() - _modeTimeoutTimer) >= _modeTimeout)
    {
        // Capture the new time.
        _modeTimeoutTimer = millis();

        // Change the mode.
        _currentLEDMode++;

        // Check if we need to roll back to the start.
        if (_currentLEDMode >= _numberOfModes) _currentLEDMode = 0;

        // Reset the animation phase to zero.
        modeList[_currentLEDMode].animationPhase = 0;

        // Force update!
        for (int i = 0; i < 3; i++)
        {
            // Check if the LED channel is used at all.
            if (checkLedObject(_ledObjectArray[i]))
            {
                modeList[_currentLEDMode].animationFunction(_ledObjectArray[i], &modeList[_currentLEDMode]);
            }
        }

        // Update the state of the return variable.
        _updateHappend = 1;
    }

    return _updateHappend;
}

// Disable LED controller.
void LedCtrl::disable()
{
    // Set the mode variable to zero.
    _enabled = 0;

    // Turn off all LEDs.
    for (int i = 0; i < 3; i++)
    {
        if (checkLedObject(_ledObjectArray[i]))
        {
            // Turn off all LEDs.
            for (int j = 0; j < _ledObjectArray[i]->numPixels(); j++)
            {
                _ledObjectArray[i]->setPixelColor(j, 0x000000);
            }

            // Send the data to them.
            _ledObjectArray[i]->show();
        }
    }
}

void LedCtrl::enable()
{
    // Set enable / disable flag to 1 (enable).
    _enabled = 1;

    // Set the current animation frame to 0.
    modeList[_currentLEDMode].animationPhase = 0;

    // Get a new timestap for frames / phase and for mode timeout.
    _ledFrameTimer = millis();
    _modeTimeoutTimer = millis();

    // Force update on all LED channels.
    for (int i = 0; i < 3; i++)
    {
        if (checkLedObject(_ledObjectArray[i]))
        {
            modeList[_currentLEDMode].animationFunction(_ledObjectArray[i], &modeList[_currentLEDMode]);
        }
    }
}

void LedCtrl::setMode(uint8_t _mode)
{
    // Check if the mode is valid at all.
    if (_mode >= _numberOfModes) return;

    // Save it.
    _currentLEDMode = _mode;

    // Update the LED controller.
    enable();
}

uint8_t LedCtrl::getMode()
{
    // Just return the current mode.
    return _currentLEDMode;
}

char* LedCtrl::getCurrentModeName()
{
    // Return the chart that holds the name of the current mode (but check if the ponter if not null).
    // In that case, return generic string.

    char *_modeName = modeList[_currentLEDMode].name;

    if (_modeName == NULL)
    {
        return _genericModeName;
    }

    return _modeName;
}

void LedCtrl::speedChange(int8_t _percent)
{
    // Limit to +/- 99%.
    if ((_percent > 99) || (_percent < -99)) return;

    // Save it to the internal variable. And also reverse it (positive numbers should speed up the pattern).
    _speedOffset = -_percent;
}

int8_t LedCtrl::getSpeed()
{
    return -_speedOffset;
}

void LedCtrl::setPatternDelay(long _patternDelay)
{
    // Check for the unvalid data (max 30 minutes).
    if (_patternDelay <= 0 || _patternDelay > 1800000LL) return;

    // Get new timestamp so it counts from the start.
    _modeTimeoutTimer = millis();

    // Copy value into class variable.
    _modeTimeout = _patternDelay;
}


unsigned long LedCtrl::getPatternDelay()
{
    // Return the value from the class.
    return _modeTimeout;
}
// Private
void LedCtrl::updatePhase(ledModeHandleTypedef *_handle)
{
    // Update the variable.
    _handle->animationPhase++;

    // Check if rollover happend (but only if this is used at all).
    if (_handle->maxAnimationsPhases != 0 && _handle->animationPhase >= _handle->maxAnimationsPhases)
    {
        _handle->animationPhase = 0;
    }
}

void LedCtrl::initIOPins()
{
    // Init all Adafruit LED objects (check if the object is not NULL).
    for (int i = 0; i < 3; i++)
    {
        if (checkLedObject(_ledObjectArray[i])) _ledObjectArray[i]->begin();
    }
}

bool LedCtrl::checkLedObject(Adafruit_WS2801 *_ledsCh)
{
    return (_ledsCh != NULL?true:false);
}