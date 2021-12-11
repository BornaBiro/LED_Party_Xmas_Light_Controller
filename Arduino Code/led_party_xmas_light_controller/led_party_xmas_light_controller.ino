#include "Adafruit_WS2801.h"
#include "SPI.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>
#include <Wire.h>

// My libs
#include "defines.h"
#include "gui.h"
#include "io.h"
#include "pcf85063.h"
#include "pitches.h"
#include "audio.h"
#include "controller.h"


// Constructors / objects
IO io;
pcf85063 rtc;
GUI gui;
Adafruit_SSD1306 display(128, 32, &Wire, -1);
Adafruit_WS2801 led1 = Adafruit_WS2801(30, WS1_DAT, WS1_CLK);
audio myAudio;
controller ctrl;

int a = 0;

void setup()
{
    Serial.begin(115200);
    Wire.begin();
    led1.begin();
    led1.show();
    myAudio.initAudio();
    ctrl.init(&led1, NULL, NULL);

    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.clearDisplay();
    display.display();
    delay(100);

    rtc.RTCinit();
    if (!rtc.isClockSet())
        rtc.setClock(1638143273);
    rtc.intOnSeconds(true);
    rtc.setAlarm(rtc.getClock() + 15);
    rtc.rtcInt(true);

    io.configPins();
    io.configRotaryEnc();

    gui.init(&rtc, &display);

    ctrl.setMode(LED_CTRL_MODE_PARTY_MUSIC_1);
    ctrl.setLedColor(0b001111110000000000111111);
}

void loop()
{
    if (io.rotaryEncAvaialble())
    {
        io.getRotaryEnc(&a);
        Serial.println(a, DEC);
    }

    if (io.getRotaryEncSW())
    {
        Serial.println("ENC SW pressed");
    }

    if (io.getButton1())
    {
        Serial.println("BTN1 pressed");
    }

    if (rtc.available())
    {
        display.clearDisplay();
        display.setTextColor(WHITE, BLACK);
        display.setCursor(0, 0);
        Serial.println(rtc.getClock(), DEC);
        gui.displayTime(GUI_RTC_TIME | GUI_RTC_DATE, rtc.getClock());
        display.display();
    }

    if (rtc.availableINT())
    {
        rtc.clearAlarm();
        digitalWrite(RELAY, HIGH);
        Serial.println("Reley is set");
    }

    ctrl.clearLeds(LED_CHANNEL_1);
    myAudio.getAudio();
    ctrl.reactLEDsToMusic(myAudio.getPeak(), LED_CHANNEL_1);
}
