#ifndef __MELODY_H__
#define __MELODY_H__

// Include Arduino Header file.
#include <Arduino.h>

class Melody
{
    public:
        Melody();
        void begin(uint8_t _pin, uint16_t *_notes, uint16_t *_durations, uint8_t *_elements, uint8_t _noSongs);
        void play();
        uint8_t update();
        void next();
        void prev();
        void stop();
        void setTempoOffset(int8_t _offset);
        int8_t getTempoOffset();
        void setMute(uint8_t _mute);
        uint8_t getMute();
        uint8_t isPlaying();

    private:
        uint8_t _currentSongElement = 0;
        uint8_t _playing = 0;
        int8_t _currentSong = 0;
        uint8_t _maxElements = 0;
        unsigned long _noteTimer = 0;
        uint8_t _noteState = 0;
        int8_t _tempoOffset = 0;
        uint16_t _currentNoteDuration = 0;

        // Disable mute by default.
        uint8_t _muteStatus = 0;

        uint16_t *_notesInternalArray = NULL;
        uint16_t *_durationsInternalArray = NULL;
        uint8_t *_elementsInternalArray = NULL;
        uint8_t _numberOfSongs = NULL;
        uint8_t _buzzerPin = -1;

};

#endif