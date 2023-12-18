#include "ledController.h"

Adafruit_WS2801 led(30, 4, 5);
LedCtrl ledCtrl;

unsigned long messageTimer = 0;

void setup()
{
    Serial.begin(115200);
    Serial.setTimeout(50);
    ledCtrl.begin(&led, NULL, NULL);
}

void loop()
{
    if (ledCtrl.update())
    {
        Serial.println("Update happend!");
    }

    if ((unsigned long)(millis() - messageTimer) > 2000UL)
    {
        messageTimer = millis();
        Serial.println("I'm alive!");
        Serial.flush();
    }

    if (Serial.available())
    {
        int a = Serial.parseInt();

        switch (a)
        {
            case 1:
            {
                ledCtrl.disable();
                Serial.println("Disabled");
                break;
            }
            case 2:
            {
                ledCtrl.enable();
                Serial.println("Enabled");
                break;
            }
            case 3:
            {
                int mode = ledCtrl.getMode();
                Serial.print("Current mode:");
                Serial.println(mode, DEC);
                break;
            }
            case 4:
            {
                ledCtrl.getCurrentModeName();
                Serial.print("Current mode name:");
                Serial.println(ledCtrl.getCurrentModeName());
                break;
            }
            case 5:
            {
                ledCtrl.speedChange(ledCtrl.getSpeed() + 5);
                Serial.print("Speed up. Current speed ");
                Serial.print(ledCtrl.getSpeed(), DEC);
                Serial.println("%");
                break;
            }
            case 6:
            {
                ledCtrl.speedChange(ledCtrl.getSpeed() - 5);
                Serial.print("Speed down. Current speed ");
                Serial.print(ledCtrl.getSpeed(), DEC);
                Serial.println("%");
                break;
            }
        }
    }
}