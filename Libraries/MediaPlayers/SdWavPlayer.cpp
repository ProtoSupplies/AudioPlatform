/************************************************************************************
*                                 SdWavPlayer.cpp                                   *
*                      Copyright (c) 2025, Donald Swearingen                        *
*                               All Rights Reserved                                 *
*************************************************************************************
*
* Description
*
*   SdWavPlayer a subclass of MediaPlayer that implements the methods of that base
*   class to play mono and stereo WAV audio files from microSD card media.
*
*   SdWavPlayer illustrates an example of implementing audio functionality in a
*   separate class from the Arduino main .ino module. SdWavPlayer passes its audio
*   output to an audio mixer (or mixers) defined in the instantiating module and
*   specified as arguments in the SdWavPlayer class constructor. The output mixer
*   is assumed connected directly or indirectly to the final audio output of the
*   application in which it resides.
*
* Copyright Notices
*
*   SdWavPlayer Copyright (c) 2025, Donald Swearingen
*   AudioPlayWav Copyright (c) 2021, Frank Bösing, f.boesing @ gmx (dot) de
*
*   MIT License
*
*   Permission is hereby granted, free of charge, to any person obtaining a copy of this
*   software and associated documentation files (the "Software"), to deal in the Software
*   without restriction, including without limitation the rights to use, copy, modify,
*   merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
*   permit persons to whom the Software is furnished to do so, subject to the following
*   conditions:
*
*   The above copyright notice and this permission notice shall be included in all copies
*   or substantial portions of the Software.
*
*   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
*   INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
*   PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
*   FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
*   OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
*   DEALINGS IN THE SOFTWARE.
*
*/

/****************
* "Pre-Defines" *
*****************
*
* Definitions that affect subsequent external references
*
*/

/************************************************************************************
*                       E X T E R N A L   R E F E R E N C E S                       *
*************************************************************************************
*/

#include "SdWavPlayer.h"

/************************************************************************************
*                         M A C R O S  /  C O N S T A N T S                         *
*************************************************************************************
*/

/************************************************************************************
*                     D A T A / T Y P E   D E F I N I T I O N S                     *
*************************************************************************************
*/

/************************************************************************************
*                          P U B L I C   F U N C T I O N S                          *
*************************************************************************************
*/

// Constructor
//
SdWavPlayer::SdWavPlayer(uint8_t id) : MediaPlayer(), _playerId(id) {

    // Connect Sd WAV player to first stage of MediaPlayer signal chain
    _playerToEnvelopeLeft.connect(_sdWavPlayer, 0, _envelopeLeft, 0);
    _playerToEnvelopeRight.connect(_sdWavPlayer, 1, _envelopeRight, 0);
}

// Destructor
//
SdWavPlayer::~SdWavPlayer() {

}

// startPlay()
//
void SdWavPlayer::startPlay(const char* soundFile, float gainValue, float panPosition) {

//    Serial.printf("SdWavPlayer::startPlay: %s\n", soundFile);

    // Set pan individually/dynamically per voice
    setPan(panPosition);

    // MIDI velocity handled as envelope's sustain value
    setSustain(gainValue);

    // Start envelope and play specified soundfile
    envNoteOn();
    _sdWavPlayer.play(soundFile);
}

// stopPlay()
//
void SdWavPlayer::stopPlay() {

//    Serial.printf("SdWavPlayer::stopPlay\n");

    if (_sdWavPlayer.isPlaying()) {
        envNoteOff();
        _sdWavPlayer.stop();
    }
}

// isPlaying()
//
bool SdWavPlayer::isPlaying() {

    return _sdWavPlayer.isPlaying();
}

// isStopped()
//
bool SdWavPlayer::isStopped() {

#if USE_PLAYWAV  // Test Frank Bösing Player
    return !isPlaying();
#else // Original Teensy SD WAV Player
    return _sdWavPlayer.isStopped();
#endif // Test Frank Bösing Player / Original Teensy SD WAV Player
}

/************************************************************************************
*                          P R I V A T E   F U N C T I O N S                        *
*************************************************************************************
*/
