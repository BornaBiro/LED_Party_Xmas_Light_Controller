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


// Constructors / objects
IO io;
pcf85063 rtc;
GUI gui;
Adafruit_SSD1306 display(128, 32, &Wire, -1);

int a = 0;

void setup()
{
    Serial.begin(115200);
    Wire.begin();

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
}
