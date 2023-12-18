#ifndef __DEFINES_H__
#define __DEFINES_H__

// Select only one of these two
#define USE_RELAY_OUTPUT
//#define USE_WS3_OUTPUT

#if defined(USE_RELAY_OUTPUT) && defined(USE_WS3_OUTPUT)
#error "Both relay output and WS3_CLK are selected! Pick only one!"
#endif

// Sound
#define BUZZER_PIN          A3
#define BUZZER_CLICK_LEN    10UL
#define BUZZER_LONG_LEN     250UL
#define BUZZER_CLICK_FREQ   2500UL
#define AUDIO_IN A6

// Lights
#define WS1_DAT 4
#define WS1_CLK 5
#define WS2_DAT 6
#define WS2_CLK 7
#define WS3_DAT A0
#define WS3_CLK A1 // If it's used

// Number of addressable LEDs on each WS output
#define STRAND1     20
#define STRAND2     20
#define SRRAND3     20

// Inputs/Outputs
#define ENC_A      2  // It's also INT0
#define ENC_B      3  // It's also INT1
#define ENC_SW     A2 // It's also PCINT10
#define RELAY      A1 // If it's used
#define BTN_1      A7 // NOTE: It's on analog in only!
#define SD_CS      10
#define RTC_INT    8  // It's also PCINT0
#define RTC_CLKOUT 9  // It's also PCINT1

struct timerTime
{
    int8_t hoursOn;
    int8_t minutesOn;
    int8_t hoursOff;
    int8_t minutesOff;
    int8_t isEnabled;
};

struct deviceSettings
{
    int8_t currentSetting;
    int8_t currentMode;
    int16_t animationDelay;
    int8_t autoChange;
    int8_t melody;
    int8_t autoStartLeds;
    int8_t autoStartRelay;
    int16_t animationDuration;
    struct timerTime activationTimes[3];
};

struct ledControllerHandle
{
    uint8_t animation;
    
};

const char stringEnabled[] = "Enabled";
const char stringDisabled[] = "Disabled";

#endif