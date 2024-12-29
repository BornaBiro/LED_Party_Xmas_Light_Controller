#define __AVR__

#include "Adafruit_WS2801.h"
#include "LiquidCrystal_I2C.h"
#include <SPI.h>
#include <Wire.h>
#include <EEPROM.h>

// My libs
#include "defines.h"
#include "gui.h"
#include "io.h"
#include "pcf85063.h"

// Include library fot the LED controller (animations, melody control, etc).
#include "ledController.h"

// Include library for the LED controller.
#include "melody.h"

// Include list of  the melodies.
#include "melodies.h"

// Constructors / objects
Adafruit_WS2801 led1 = Adafruit_WS2801(20, WS1_DAT, WS1_CLK);
Adafruit_WS2801 led2 = Adafruit_WS2801(30, WS2_DAT, WS2_CLK);
LiquidCrystal_I2C lcd(0x27, 16, 2);
IO io;
pcf85063 rtc;
GUI gui;

// Create LED Controller object.
LedCtrl ctrl;

// Create an object for the melody controller.
Melody melody;

void setup()
{
    // make a tone, so we know that device is alive.
    tone(BUZZER_PIN, BUZZER_CLICK_FREQ, BUZZER_LONG_LEN);
    Serial.begin(115200);
    Wire.begin();
    Wire.setWireTimeout(25000, true);

    lcd.begin(16, 2);
    lcd.clear();
    lcd.backlight();

    // Init library for the melody contoller.
    melody.begin(BUZZER_PIN, (uint16_t*)currentMelodyNotes, (uint16_t*)currentMelodyDurations, (uint8_t*)melodyElements, sizeof(melodyElements) / sizeof(melodyElements[0]));

    // Init LED controller libary.
    ctrl.begin(&led1, &led2, NULL, &melody);

    rtc.RTCinit();
    if (!rtc.isClockSet())
        rtc.setClock(1639266187);
    rtc.intOnSeconds(true);
    //rtc.setAlarm(rtc.getClock() + 15);
    //rtc.rtcInt(true);

    io.configPins();
    io.configRotaryEnc();    

    gui.init(lcd, rtc, &ctrl);

    sei();
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
        if (!melody.isPlaying()) tone(BUZZER_PIN, BUZZER_CLICK_FREQ, BUZZER_CLICK_LEN);
    }

    gui.update(lcd, rtc);
    ctrl.update();
}