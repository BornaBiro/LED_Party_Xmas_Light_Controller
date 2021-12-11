#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>
#include <Wire.h>
#include "Adafruit_WS2801.h"
#include "SPI.h"

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
Adafruit_WS2801 led1 = Adafruit_WS2801(30, WS1_DAT, WS1_CLK);

int a = 0;
int16_t maxLevel = 0;

void setup()
{
    Serial.begin(115200);
    Wire.begin();
    led1.begin();
    led1.show();

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

//------------------------------------------
    int8_t adc[128];
    for(int i = 0; i < 128; i++)
    {
        adc[i] = (analogRead(AUDIO_IN) >> 2) - 127;
    }

    for(int i = 0; i < 125; i++)
    {
        int32_t _movingAvg = 0;
        for(int j = 0; j < 3; j++)
        {
            _movingAvg += adc[i + j];
        }
        _movingAvg /= 3;
        adc[i] = abs(_movingAvg);
    }

    int16_t max = adc[0];
    for(int i = 0; i < 125; i++)
    {
        if (adc[i] > max) max = adc[i];
    }

    max *= 2;

    for(int i = 0; i < 30; i++)
    {
        led1.setPixelColor(i, 0);
    }

    if (max > maxLevel)
    {
        maxLevel = max;
    }
    else
    {
        if (maxLevel > 4) maxLevel-=4;
    }

    // Effect 1-------------------------
    //int k = 30/127.0*maxLevel;
    //for(int i = 0; i < k; i++)
    //{
    //    led1.setPixelColor(i, 0b111111111111111111111111);
    //}
    //led1.show();
    // Effect 1-------------------------

    // Effect 2-------------------------
    //int k = 30/127.0*maxLevel/2;
    //for(int i = 0; i < k; i++)
    //{   
    //    led1.setPixelColor(29 - i, 0b111111111111111111111111);
    //    led1.setPixelColor(i, 0b111111111111111111111111);
    //}
    //led1.show();
    // Effect 2-------------------------

    // Effect 2-------------------------
    int k = 30/127.0*maxLevel/2;
    for(int i = 0; i < k; i++)
    {   
        led1.setPixelColor(14 - i, 0b111111111111111111111111);
        led1.setPixelColor(15 + i, 0b111111111111111111111111);
    }
    led1.show();
    // Effect 2-------------------------
//------------------------------------------
}
