#define __AVR__

#include "Adafruit_WS2801.h"
#include "SPI.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>
#include <Wire.h>

// My libs
#include "audio.h"
#include "controller.h"
#include "defines.h"
#include "gui.h"
#include "io.h"
#include "pcf85063.h"
#include "pitches.h"


// Constructors / objects
IO io;
pcf85063 rtc;
GUI gui;
Adafruit_SSD1306 display(128, 32, &Wire, -1);
Adafruit_WS2801 led1 = Adafruit_WS2801(30, WS1_DAT, WS1_CLK);
Adafruit_WS2801 led2 = Adafruit_WS2801(20, WS2_DAT, WS2_CLK);
audio myAudio;
controller ctrl;

int a = 0;

void setup()
{
    Serial.begin(115200);
    Wire.begin();
    //myAudio.initAudio();
    ctrl.init(&myAudio, &led1, &led2, NULL);

    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.clearDisplay();
    display.display();
    delay(100);

    rtc.RTCinit();
    if (!rtc.isClockSet())
        rtc.setClock(1639266187);
    rtc.intOnSeconds(true);
    rtc.setAlarm(rtc.getClock() + 15);
    rtc.rtcInt(true);

    io.configPins();
    io.configRotaryEnc();

    gui.init(&rtc, &display, &ctrl);

    // ctrl.setMode(LED_CTRL_MODE_PARTY_MUSIC_2);
    //ctrl.setMode(LED_CTRL_MODE_STATIC_1, 20000, 500);
    ctrl.setMode(LED_CTRL_MODE_STATIC_1, 120000, 400);
    //ctrl.setLedColor(0b001111110000000000111111);
}

void loop()
{
    if (io.rotaryEncAvaialble())
    {
        io.getRotaryEnc(&a);
        gui.displayOn();
        gui.update();
        Serial.println(a, DEC);
    }

    if (io.getRotaryEncSW())
    {
        Serial.println("ENC SW pressed");
        gui.displayOn();
        gui.update();
    }

    if (io.getButton1())
    {
        Serial.println("BTN1 pressed");
        gui.displayOn();
        gui.update();
    }

    if (rtc.availableINT())
    {
        rtc.clearAlarm();
        // digitalWrite(RELAY, HIGH);
        Serial.println("Reley is set");
    }

    //if (myAudio.getAudio())
    //{
    //    myAudio.pauseAudio();
    //    ctrl.reactLEDsToMusic(myAudio.getPeak(), LED_CHANNEL_1);
    //    myAudio.resumeAudio();
    //}

    gui.update();
    ctrl.update();
}