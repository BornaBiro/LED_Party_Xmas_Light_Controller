#include "melodies.h"
#include "melody.h"

Melody melody;

void setup()
{
    // Debug output.
    Serial.begin(115200);
    Serial.println("Code has started");

    melody.begin(A3, (uint16_t*)currentMelodyNotes, (uint16_t*)currentMelodyDurations, (uint8_t*)melodyElements, sizeof(melodyElements) / sizeof(melodyElements[0]));
    //melody.play();
}

void loop()
{
    int note = melody.update();

    if (note)
    {
        if (note == 1) Serial.println("New note");
        if (note == 2) Serial.println("New melody");
    }

    if (Serial.available())
    {
        char a = Serial.read();

        if (a == '+') melody.next();

        if (a == '-') melody.prev();

        if (a == 'u') melody.setTempoOffset(melody.getTempoOffset() + 1);

        if (a == 'd') melody.setTempoOffset(melody.getTempoOffset() - 1);

        if (a == 's') melody.stop();

        if (a == 'p') melody.play();
    }
}