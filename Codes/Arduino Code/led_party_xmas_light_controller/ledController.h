#ifndef __LED_CONTROLLER_H__
#define __LED_CONTROLLER_H__

// Add main Arduino Herader file
#include "Arduino.h"

// Add Adafruit WS2801 LED library. Thanks Adafruit!
#include "Adafruit_WS2801.h"

// Include header for the defines
#include "ledControllerDefines.h"

// Include file for the all modes and animations
#include "ledCtrlModes.h"

// Include melody controller library.
#include "melody.h"

// Yuck...
extern ledModeHandleTypedef modeList[14];

// Class for the LED controller
class LedCtrl
{
    public:
        // Constructor.
        LedCtrl();

        // Libary / class init
        void begin(Adafruit_WS2801 *_ledsCh1, Adafruit_WS2801 *_ledsCh2, Adafruit_WS2801 *_ledsCh3, Melody *_melodyPtr);

        // Fucntion thast checks if the LEDs needs to chagne the animation frame.
        uint8_t update();

        // Disable the LED Controller.
        void disable();

        // Enable LED controller.
        void enable();

        // Get the state of the LED controller.
        uint8_t getState();

        void setMode(int8_t _mode);

        uint8_t getMode();

        char* getCurrentModeName();

        void speedChange(int8_t _percent);

        int8_t getSpeed();

        void setPatternDelay(long _patternDelay);

        unsigned long getPatternDelay();

        void enableMelody(uint8_t _melody);

        uint8_t getMelodyStatus();

        void setAutomaticChange(uint8_t _autoModeChange);

        uint8_t getAutomaticChange();

    private:
        void initIOPins();

        // Method checks if the selected LED channel / object is vailed or used.
        bool checkLedObject(Adafruit_WS2801 *_ledsCh);

        // Updates animation phase variable from typedef for ledModeHandleTypedef.
        void updatePhase(ledModeHandleTypedef *_handle);

        // Variable hold value of the current LED mode.
        uint8_t _currentLEDMode = 0;

        // Current status of the LED controller.
        uint8_t _enabled = 1;

        // Variable holding timer timeout value for the frame delay.
        unsigned long _ledFrameTimer = 0;

        // Variable for mode timeout value.
        unsigned long _modeTimeout = 60000UL;

        // Variable for mode change timestamp.
        unsigned long _modeTimeoutTimer = 0;

        // How many modes are there?
        const uint8_t _numberOfModes = sizeof(modeList) / sizeof(modeList[0]);

        // Current animation phase counter.
        uint16_t _animationPhase = 0;

        // Generic name for the LED mode (in case that one does not exists in the list).
        char _genericModeName[13] = {"No mode name"};

        // Offset variable for the pattern speed change.
        int8_t _speedOffset = 0;

        // Automatic mode update flag variable.
        // By default, update the mode automatically.
        uint8_t _automaticModeUpdate = 1;

        // Pointer array to the Adafruit WS2801 LED library.
        Adafruit_WS2801 *_ledObjectArray[3];

        // Pointer to the melody controller object.
        Melody *_melodyCtrlPtr;
};

#endif