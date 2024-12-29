#include "melody.h"

Melody::Melody()
{
    // Empty...for now. :)
}

void Melody::begin(uint8_t _pin, uint16_t *_notes, uint16_t *_durations, uint8_t *_elements, uint8_t _noSongs)
{
    // Copy to the internal private variables and pointers.
    // Copy the pointer to the array of all notes of all melodies.
    _notesInternalArray = _notes;

    // Copy list of all note duratins of all melodies.
    _durationsInternalArray = _durations;

    // Copy list the number of the elements of  every and each melody.
    _elementsInternalArray = _elements;

    // Copy the number of all melodies.
    _numberOfSongs = _noSongs;

    // Copy the used pin fo buzzer.
    _buzzerPin = _pin;

    // Set the used pin as output (without it, generating melodies won't work!).
    pinMode(_buzzerPin, OUTPUT);


    // Debug and testing data.
    Serial.print("First note: ");
    Serial.println((uint16_t)pgm_read_word((uint16_t*)_notesInternalArray[_currentSong] + 5), DEC);

    Serial.print("First duration: ");
    Serial.println((uint16_t)pgm_read_word((uint16_t*)_durationsInternalArray[_currentSong] + 5), DEC);

    Serial.print("First melody elements: ");
    Serial.println((uint8_t)pgm_read_byte((uint8_t*)_elementsInternalArray + 1), DEC);

    Serial.print("Number of melodies: ");
    Serial.println(_noSongs, DEC);
}

void Melody::play()
{
    // Set the current song elememt to the 0, so the melody can start from the beginning.
    _currentSongElement = 0;

    // Set the first song.
    _currentSong = 0;

    // Get the current timestamp.
    _noteTimer = millis();

    // Generate a first note.
    if (!_muteStatus) tone(_buzzerPin, pgm_read_word((uint16_t*)_notesInternalArray[_currentSong] + _currentSongElement));

    // Set flag for status of the melody player.
    _playing = 1;

    // Get the current note duration (delay). Also calculate the tempo offset.
    _currentNoteDuration = (uint16_t)(((uint16_t)pgm_read_word((uint16_t*)_durationsInternalArray[_currentSong] + _currentSongElement)) * (1 + _tempoOffset / 100.0));
}

uint8_t Melody::update()
{
    // Check if the code needs to produce melody. If not, go back!
    if (!_playing) return;

    // Variable for the return value. 0 - no change, 1 - note has changed, 2 - song has changed.
    // By default, set it to 0 (no note change).
    _noteState = 0;

    // Check if the note needs to be changed.
    if ((unsigned long)(millis() - _noteTimer) >= _currentNoteDuration)
    {
        // Increment song element.
        _currentSongElement++;

        // Set the variable for the note change to 1.
        _noteState = 1;

        // Capture new time.
        _noteTimer = millis();

        // Chech if the melody needs to be changed.
        if (_currentSongElement >= pgm_read_byte((uint8_t*)_elementsInternalArray + _currentSong))
        {
            // Set the next song.
            next();

            // If the song / melody needs to be changed (it got till the end of the song / melody), set variable to 2.
            _noteState = 2;
        }
        else
        {
            // Play the note.
            if (!_muteStatus) tone(_buzzerPin, pgm_read_word((uint16_t*)_notesInternalArray[_currentSong] + _currentSongElement));
        }

        // Get the current note duration (delay). Also calculate the tempo offset.
        _currentNoteDuration = (uint16_t)(((uint16_t)pgm_read_word((uint16_t*)_durationsInternalArray[_currentSong] + _currentSongElement)) * (1 + _tempoOffset / 100.0));
    }

    // Return the state.
    return _noteState;
}

void Melody::next()
{
    // First, stop it!
    stop();

    // Wait a little bit.
    delay(200);

    // Go to the next melody.
    _currentSong++;

    // Check if is the last melody. If it is, wrap it aroud.
    if (_currentSong >= _numberOfSongs) _currentSong = 0;

    // Reset the element couter.
    _currentSongElement = 0;

    // Get the new timestamp.
    _noteTimer = millis();

    // Generate the new note.
    if (!_muteStatus) tone(_buzzerPin, pgm_read_word((uint16_t*)_notesInternalArray[_currentSong] + _currentSongElement));

    // Get the current note duration (delay). Also calculate the tempo offset.
    _currentNoteDuration = (uint16_t)(((uint16_t)pgm_read_word((uint16_t*)_durationsInternalArray[_currentSong] + _currentSongElement)) * (1 + _tempoOffset / 100.0));

    // Set new state of the flag for the melody player.
    _playing = 1;
}

void Melody::prev()
{
    // First, stop it!
    stop();

    // Wait a little bit.
    delay(200);

    // Go to the next melody.
    _currentSong--;

    // Check if is the last melody. If it is, wrap it aroud.
    if (_currentSong <= _numberOfSongs) _currentSong = _numberOfSongs;

    // Reset the element couter.
    _currentSongElement = 0;

    // Get the new timestamp.
    _noteTimer = millis();

    // Generate the new note.
    if (!_muteStatus) tone(_buzzerPin, pgm_read_word((uint16_t*)_notesInternalArray[_currentSong] + _currentSongElement));

    // Get the current note duration (delay). Also calculate the tempo offset.
    _currentNoteDuration = (uint16_t)(((uint16_t)pgm_read_word((uint16_t*)_durationsInternalArray[_currentSong] + _currentSongElement)) * (1 + _tempoOffset / 100.0));

    // Set new state of the flag for the melody player.
    _playing = 1;
}

void Melody::stop()
{
    // To stop the melody, mute the buzzer pin.
    noTone(_buzzerPin);

    // Reset the element counter.
    _currentSongElement = 0;

    // Set new state of the flag for the melody player.
    _playing = 0;
}

void Melody::setTempoOffset(int8_t _offset)
{
    // Limit to +/- 75%.
    if ((_offset > 75) || (_offset < -75)) return;

    // Save it to the internal variable. And also reverse is (positive tempo numbers should speed up the melody).
    _tempoOffset = -_offset;
}

int8_t Melody::getTempoOffset()
{
    return -_tempoOffset;
}

void Melody::setMute(uint8_t _mute)
{
    // Mute can only be enablec or disabled.
    _mute &= 1;

    // Store it into internal variable.
    _muteStatus = _mute;

    // If mute is enabled, stop any ongoing sound.
    noTone(_buzzerPin);
}

uint8_t Melody::getMute()
{
    // Return the local variable.
    return _muteStatus;
}