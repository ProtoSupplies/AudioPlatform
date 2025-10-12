/************************************************************************************
*                               MediaPlayerMulti.h                              *
*                       Copyright (c) 2025, Donald Swearingen                       *
*                                All Rights Reserved                                *
*************************************************************************************
*
* Description
*
*   MediaPlayerMulti is the base class for media file type players that supports
*   multiple individual instances of a specific media player type.
*
*   See MediaPlayerMulti.cpp for further details.
*
* Copyright Notices
*
*   Copyright (c) 2025, Donald Swearingen
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

/************************************************************************************
*                       E X T E R N A L   R E F E R E N C E S                       *
*************************************************************************************
*/

#ifndef MEDIAPLAYERMULTI_H_
#define MEDIAPLAYERMULTI_H_

#include <stdint.h>

#include <Audio.h>

#include "MediaPlayer.h"
#include "VoiceManager.h"
#include "AudioMixer8.h"

/************************************************************************************
*                         M A C R O S  /  C O N S T A N T S                         *
*************************************************************************************
*/

/************************************************************************************
*                     D A T A / T Y P E   D E F I N I T I O N S                     *
*************************************************************************************
*/

/************************************************************************************
*                         C L A S S   D E F I N I T I O N S                         *
*************************************************************************************
*/

class MediaPlayerMulti {

// Constructor/Destructor
//
public:

    MediaPlayerMulti(uint8_t playerCount);
    virtual ~MediaPlayerMulti();

// Class Variables
//
public:


private:

    // Maximum number of players available.
    static const uint8_t    MaxPlayers = 16;

    // Maximum sound file constants
    // TODO: Duplicates AudioPlatformGlobal.h (place in TeensyUtil?)
    static const int        MaxSoundBanks = 4;
    static const int        SoundFilesPerBank = 100;
    static const int        MaxSoundFileIndex = (MaxSoundBanks * SoundFilesPerBank) - 1;    // [0,399]

protected:

    // Number of players in voice pool
    uint8_t                 _playerCount;

    // Player Mixers
    AudioMixer8             _playerMixerLeft;   // Left channel mixer for 4 or 8 players
    AudioMixer8             _playerMixerRight;  // Right channel mixer for 4 or 8 players

    // Output Amplifiers (final output)
    AudioAmplifier          _outAmpLeft;
    AudioAmplifier          _outAmpRight;

    // Inter-module patch cords
    AudioConnection         _playerMixerLeftToOutAmpLeft;
    AudioConnection         _playerMixerRightToOutAmpRight;

    // Intra-module patch cords
    AudioConnection         _playersToPlayerMixerLeft[MaxPlayers];
    AudioConnection         _playersToPlayerMixerRight[MaxPlayers];

    // Media player pool (voices)
    // Media type defined and array elements assigned in derived class
    MediaPlayer             *mediaPlayers[MaxPlayers];

    // Device enabled/disabled
    bool                    playerEnabled;

    // Media sources (Mono/Stereo WAV files; Mono Raw audio files)
    char                    **soundFileNames;
    uint16_t                soundFileCount;

    // Voice allocation/replacement manager
    VoiceManager            voiceMgr;
    VoiceHeldState          voiceHeldState[MaxPlayers];

    // Sustain pedal state
    bool                    sustainPedalPressed;

    // MIDI note/key shift variables
    int                     noteShift;
    MidiNoteNumber          noteRangeStart;
    MidiNoteNumber          noteRangeEnd;

    // Sound Bank (0-3; 100 samples/bank)
    uint8_t                 soundBank;

    // MIDI pan
    MidiPanType             panType;
    uint8_t                 panParam;
    uint8_t                 panPosition;

// Class Methods
//
public:

    // Outputs from final amplifier stage. Functions allow for
    // connection routing in parent/owner class.
    AudioStream*    getLeftOutput();
    AudioStream*    getRightOutput();

    void            setSoundFiles(char **soundFileNames, uint16_t soundFileCount);
    void            setEnabled(bool enabled);
    void            setGain(float gainValue);

    void            setSoundBank(uint8_t bankNum);
    void            setNoteLimits(int _noteShift, MidiNoteNumber _noteRangeStart, MidiNoteNumber _noteRangeEnd);
    void            setNoteShift(int _noteShift);
    void            setNoteRangeStart(MidiNoteNumber _noteRangeStart);
    void            setNoteRangeEnd(MidiNoteNumber _noteRangeEnd);
    void            noteOn(MidiNoteNumber note, float gainValue);
    void            noteOff(MidiNoteNumber note);
    void            allNotesOff();
    void            setPanType(MidiPanType type, uint8_t param);
    void            setPanPosition(uint8_t position);
    void            sustainPedal(bool pressed);

    // TODO:
//    void    IgnoreNoteOff(bool ignore); // also called for pedal down/up

protected:

private:

    void            setNotePosition();
};

#endif // MEDIAPLAYERMULTI_H__
