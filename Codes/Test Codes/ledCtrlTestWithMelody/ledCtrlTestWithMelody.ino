#include "ledController.h"
#include "melodies.h"

Adafruit_WS2801 led(30, 4, 5);
LedCtrl ledCtrl;
Melody melody;

unsigned long messageTimer = 0;

void setup()
{
    Serial.begin(115200);
    Serial.setTimeout(50);

    randomSeed(analogRead(A6));

    melody.begin(A3, (uint16_t*)currentMelodyNotes, (uint16_t*)currentMelodyDurations, (uint8_t*)melodyElements, sizeof(melodyElements) / sizeof(melodyElements[0]));
    ledCtrl.begin(&led, NULL, NULL, &melody);

    //ledCtrl.setMode(5);
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

            case 7:
            {
                ledCtrl.enableMelody(0);
                Serial.println("Melody disabled");
                break;
            }

            case 8:
            {
                ledCtrl.enableMelody(1);
                Serial.println("Melody enabled");
                break;
            }
        }
    }
}