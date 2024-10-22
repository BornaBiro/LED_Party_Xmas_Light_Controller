#define __AVR__

#include "Adafruit_WS2801.h"
#include "LiquidCrystal_I2C.h"
#include <SPI.h>
#include <Wire.h>
#include <EEPROM.h>

// My libs
#include "audio.h"
#include "controller.h"
#include "defines.h"
#include "gui.h"
#include "io.h"
#include "pcf85063.h"
#include "pitches.h"


// Constructors / objects
Adafruit_WS2801 led1 = Adafruit_WS2801(30, WS1_DAT, WS1_CLK);
Adafruit_WS2801 led2 = Adafruit_WS2801(20, WS2_DAT, WS2_CLK);
LiquidCrystal_I2C lcd(0x27, 16, 2);
IO io;
pcf85063 rtc;
GUI gui;
audio myAudio;
controller ctrl;

void setup()
{
    // make a tone, so we know that device is alive.
    tone(BUZZER_PIN, BUZZER_CLICK_FREQ, BUZZER_LONG_LEN);
    Serial.begin(115200);
    Wire.begin();
    Wire.setWireTimeout(25000, true);

    lcd.begin();
    lcd.clear();

    myAudio.initAudio();
    ctrl.init(&myAudio, &led1, &led2, NULL);

    rtc.RTCinit();
    if (!rtc.isClockSet())
        rtc.setClock(1639266187);
    rtc.intOnSeconds(true);
    rtc.setAlarm(rtc.getClock() + 15);
    rtc.rtcInt(true);

    io.configPins();
    io.configRotaryEnc();    

    gui.init(lcd, rtc, &ctrl);

    sei();

    ctrl.setMode(LED_CTRL_MODE_PARTY_2, 8000, 50);
}

void loop()
{
    int encoder = 0;
    int encoderButton = 0;

    encoderButton = io.getRotaryEncSW();
    if (io.rotaryEncAvaialble() || encoderButton)
    {
        io.getRotaryEnc(&encoder);
        gui.updateMenu(encoder, encoderButton, 0, rtc);
        gui.displayOn(lcd);
        if (!ctrl.getCurrentMelody()) tone(BUZZER_PIN, BUZZER_CLICK_FREQ, BUZZER_CLICK_LEN);
    }

    //if (myAudio.getAudio())
    //{
        // myAudio.pauseAudio();
    //    ctrl.reactLEDsToMusic(myAudio.getPeak(), LED_CHANNEL_1);
        // myAudio.resumeAudio();
    //}

    gui.update(lcd, rtc);
    ctrl.update();
}