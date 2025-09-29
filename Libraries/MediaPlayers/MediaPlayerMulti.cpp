/************************************************************************************
*                                MediaPlayerMulti.cpp                               *
*                       Copyright (c) 2025, Donald Swearingen                       *
*                                All Rights Reserved                                *
*************************************************************************************
*
* Description
*
*   MediaPlayerMulti is the base class for media file type players that include
*   multiple individual instances of a specific media player type (AudioPlaySdWav,
*   AudioPlaySdWav, AudioPlaySerialflashRaw).
*
*   MediaPlayerMulti defines structures and methods that provide management and playback
*   of multiple player voices, including:
*
*       • Functions to start and stop playback of individual streams
*       • Management of voice assignment and replacement from a fixed size
*         (size defined by derived subclasses) pool of available "voices"
*       • Provision of an output and gain control mechanism common to all
*         derived subclasses
*       • Maintenance of an array (soundFileNames) and count (soundFileCount) of media
*         files available for playback, contents and size set at startup (see
*         function setSoundFiles().
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

#include <stdlib.h>

#include "ArduinoEclipseDefs.h"
#include "UtilFns.h"
#include "MediaPlayerMulti.h"

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
MediaPlayerMulti::MediaPlayerMulti(uint8_t playerCount) :
    _playerCount(playerCount), voiceMgr(playerCount) {

    // Restrict count to max
    if (_playerCount > MaxPlayers) {
        _playerCount = MaxPlayers;
    }

    // Connect internal signal chain
    _playerMixerLeftToOutAmpLeft.connect(_playerMixerLeft, 0, _outAmpLeft, 0);
    _playerMixerRightToOutAmpRight.connect(_playerMixerRight, 0, _outAmpRight, 0);

    // Voice initial held state
    for (int i = 0; i < playerCount; i++) {
        voiceHeldState[i] = VoiceNotHeld;
    }

    sustainPedalPressed = false;
    playerEnabled = true;

    noteShift       = 0;
    noteRangeStart  = MidiNoteMin;
    noteRangeEnd    = MidiNoteMax;
}

// Destructor
//
MediaPlayerMulti::~MediaPlayerMulti() {

}

// getLeftOutput / getRightOutput
//
// Return pointers to final amplifier stage to be
// used in signal chain connections of derived class.
//
AudioStream* MediaPlayerMulti::getLeftOutput() {
//AudioAmplifier* MediaPlayerMulti::getLeftOutput() {
    return &_outAmpLeft;
}

AudioStream* MediaPlayerMulti::getRightOutput() {
//AudioAmplifier* MediaPlayerMulti::getRightOutput() {
    return &_outAmpRight;
}

// setSoundFiles
//
void MediaPlayerMulti::setSoundFiles(char **_soundFileNames, uint16_t _soundFileCount) {

    soundFileNames = _soundFileNames;
    soundFileCount = _soundFileCount;

//    if (soundFileCount > 0) {
//        Serial.printf("MediaPlayerMulti::setSoundFiles: soundFileNames %x soundFileCount %d\n", soundFileNames, soundFileCount);
//        Serial.printf("MediaPlayerMulti::setSoundFiles: soundFileNames[0] %s\n", soundFileNames[0]);
//        Serial.printf("MediaPlayerMulti::setSoundFiles: soundFileNames[199] %x\n", soundFileNames[199]);
//    }
}

// setEnabled()
//
void MediaPlayerMulti::setEnabled(bool enabled) {

//    Serial.printf("MediaPlayerMulti::setEnabled: %d\n", enabled);

    playerEnabled = enabled;

//    if (!playerEnabled) {
//        allNotesOff();
//    }
}

// setGain()
//
void MediaPlayerMulti::setGain(float gainValue) {

    _outAmpLeft.gain(gainValue);
    _outAmpRight.gain(gainValue);
}

// setSoundBank()
//
void MediaPlayerMulti::setSoundBank(uint8_t bankNum) {

    soundBank = bankNum;
}

// setNoteLimits()
//
void MediaPlayerMulti::setNoteLimits(int _noteShift, MidiNoteNumber _noteRangeStart, MidiNoteNumber _noteRangeEnd) {

    setNoteShift(_noteShift);
    setNoteRangeStart(noteRangeStart);
    setNoteRangeEnd(_noteRangeEnd);
}

// setNoteShift()
//
void MediaPlayerMulti::setNoteShift(int _noteShift) {

//    Serial.printf("MediaPlayerMulti::setNoteShift: %d\n", _noteShift);

    // FFS: Quick fix to avoid hanging notes. Need a better approach that would preserve held notes.
    if ((_noteShift != noteShift) && (voiceMgr.getActiveVoiceCount() != 0)) {
//        Serial.printf("MediaPlayerMulti::setNoteShift: _noteShift %d noteShift %d\n", _noteShift, noteShift);
//        Serial.printf("MediaPlayerMulti::setNoteShift: voiceMgr.getActiveVoiceCount() %d\n", voiceMgr.getActiveVoiceCount());
//        Serial.printf("MediaPlayerMulti::setNoteShift: Calling allNotesOff\n");
        allNotesOff();
    }
    noteShift = _noteShift;
}

// setNoteRangeStart()
//
void MediaPlayerMulti::setNoteRangeStart(MidiNoteNumber _noteRangeStart) {

//    Serial.printf("MediaPlayerMulti::setNoteRangeStart: %d\n", _noteRangeStart);

    // FFS: Quick fix to avoid hanging notes. Need a better approach that would preserve held notes.
    if ((_noteRangeStart != noteRangeStart) && (voiceMgr.getActiveVoiceCount() != 0)) {
//        Serial.printf("MediaPlayerMulti::setNoteRangeStart: _noteRangeStart %d noteRangeStart %d\n", _noteRangeStart, noteRangeStart);
//        Serial.printf("MediaPlayerMulti::setNoteRangeStart: voiceMgr.getActiveVoiceCount() %d\n", voiceMgr.getActiveVoiceCount());
//        Serial.printf("MediaPlayerMulti::setNoteRangeStart: Calling allNotesOff\n");
        allNotesOff();
    }
    noteRangeStart = _noteRangeStart;
    if (noteRangeStart > noteRangeEnd) {
        MidiNoteNumber note = noteRangeStart;
        noteRangeStart = noteRangeEnd;
        noteRangeEnd = note;
//        Serial.printf("MediaPlayerMulti::setNoteRangeStart: switched to noteRangeStart %d noteRangeEnd %d\n", noteRangeStart, noteRangeEnd);
    }
}

// setNoteRangeEnd()
//
void MediaPlayerMulti::setNoteRangeEnd(MidiNoteNumber _noteRangeEnd) {

//    Serial.printf("MediaPlayerMulti::setNoteRangeEnd: (current) noteRangeEnd %d (new) _noteRangeEnd %d\n", noteRangeEnd, _noteRangeEnd);

    // FFS: Quick fix to avoid hanging notes. Need a better approach that would preserve held notes.
    if ((_noteRangeEnd != noteRangeEnd) && (voiceMgr.getActiveVoiceCount() != 0)) {
//        Serial.printf("MediaPlayerMulti::setNoteRangeEnd: _noteRangeEnd %d noteRangeEnd %d\n", _noteRangeEnd, noteRangeEnd);
//        Serial.printf("MediaPlayerMulti::setNoteRangeEnd: voiceMgr.getActiveVoiceCount() %d\n", voiceMgr.getActiveVoiceCount());
//        Serial.printf("MediaPlayerMulti::setNoteRangeEnd: Calling allNotesOff\n");
        allNotesOff();
    }
    noteRangeEnd = _noteRangeEnd;
    if (noteRangeStart > noteRangeEnd) {
        MidiNoteNumber note = noteRangeStart;
        noteRangeStart = noteRangeEnd;
        noteRangeEnd = note;
//        Serial.printf("MediaPlayerMulti::setNoteRangeEnd: switched to noteRangeStart %d noteRangeEnd %d\n", noteRangeStart, noteRangeEnd);
    }
}

// noteOn()
//
void MediaPlayerMulti::noteOn(MidiNoteNumber note, float gainValue) {

//    Serial.printf("\nMediaPlayerMulti::noteOn note %d gain %f\n", note, gainValue);

    if (playerEnabled && soundFileCount > 0) {
        int shiftedNote = note + noteShift;

        if (shiftedNote >= noteRangeStart && shiftedNote <= noteRangeEnd) {

            // Set note's pan position
            setNotePosition();

            VoiceNumber voiceNum = voiceMgr.getVoiceUsingNote(shiftedNote);

            // No voice is assigned to shifted note
            if (voiceNum == voiceMgr.VoiceNone) {
                voiceNum = voiceMgr.getFreeVoice();
                uint16_t soundFileNum = (soundBank * SoundFilesPerBank) + shiftedNote;

                if (voiceNum < _playerCount && soundFileNum < MaxSoundFileIndex) {
                    // Play only valid sound files (gaps in array are possible)
                    if (soundFileNames[soundFileNum]) {
                        voiceMgr.setVoiceActive(true, voiceNum, shiftedNote);
                        mediaPlayers[voiceNum]->startPlay(soundFileNames[soundFileNum], gainValue, UtilFns::scale(panPosition, 0, 127, 0.0, 1.0));
                        mediaPlayers[voiceNum]->currentNote = shiftedNote;
                        voiceHeldState[voiceNum] = VoiceHeld;
                    }
                }

            // A voice is assigned to shifted note
            } else {
                // Assigned voice state
                VoiceHeldState voiceState = voiceHeldState[voiceNum];

                // Trigger new notes
                if (voiceState == VoiceNotHeld) {
                    VoiceNumber voiceNum = voiceMgr.getFreeVoice();
                    uint16_t soundFileNum = (soundBank * SoundFilesPerBank) + shiftedNote;

                    if (voiceNum < _playerCount && soundFileNum < MaxSoundFileIndex) {
                        if (soundFileNames[soundFileNum]) {
                            voiceMgr.setVoiceActive(true, voiceNum, shiftedNote);
                            mediaPlayers[voiceNum]->startPlay(soundFileNames[soundFileNum], gainValue, UtilFns::scale(panPosition, 0, 127, 0.0, 1.0));
                            mediaPlayers[voiceNum]->currentNote = shiftedNote;
                            voiceHeldState[voiceNum] = VoiceHeld;
                        }
                    }

                // Retrigger notes released, but held by pedal (VoiceHeldPending) or ignorenoteoff (VoiceHeld)
                } else if (voiceState == VoiceHeldPending || voiceState == VoiceHeld) {
                    voiceHeldState[voiceNum] = VoiceNotHeld;
                    mediaPlayers[voiceNum]->stopPlay();
//                    Serial.printf("MediaPlayerMulti::noteOn calling voiceMgr.setVoiceActive(false)\n");
                    voiceMgr.setVoiceActive(false, voiceNum, shiftedNote);
                    noteOn(note, gainValue);     // safely recursive
                }
            }
        }
    }
}

// noteOff()
//
void MediaPlayerMulti::noteOff(MidiNoteNumber note) {

//    Serial.printf("\nMediaPlayerMulti::noteOff note %d\n", note);

    int shiftedNote = note + noteShift;
    if (shiftedNote >= noteRangeStart && shiftedNote <= noteRangeEnd) {
        VoiceNumber voiceNum = voiceMgr.getVoiceUsingNote(shiftedNote);
//        Serial.printf("MediaPlayerMulti::noteOff shifted note %d\n", shiftedNote);

        if (voiceNum < _playerCount) {
            if (mediaPlayers[voiceNum]->currentNote == shiftedNote) {
                if (sustainPedalPressed) {
                    if (voiceHeldState[voiceNum] == VoiceHeld) {
                        voiceHeldState[voiceNum] = VoiceHeldPending;
                    }

                } else if (voiceHeldState[voiceNum] != VoiceNotHeld){
                    voiceHeldState[voiceNum] = VoiceNotHeld;
                    mediaPlayers[voiceNum]->stopPlay();
//                    Serial.printf("MediaPlayerMulti::noteOff calling voiceMgr.setVoiceActive(false)\n");
                    voiceMgr.setVoiceActive(false, voiceNum, shiftedNote);
                }
            }
        }
    }
}

// allNotesOff()
//
void MediaPlayerMulti::allNotesOff() {

//    Serial.println("MediaPlayerMulti::allNotesOff\n");

    for (int i = 0; i < _playerCount; i++) {
        if (mediaPlayers[i]->isPlaying()) {
            mediaPlayers[i]->stopPlay();
        }
        voiceMgr.setVoiceActive(false, i, 0);
        voiceHeldState[i] = VoiceNotHeld;
    }
}

// setPanType()
//
void MediaPlayerMulti::setPanType(MidiPanType type, uint8_t param) {

//    Serial.printf("MediaPlayerMulti::setPanType: type %d param %d\n", type, param);

    panType = type;
    panParam = param;

    switch (panType) {

        case PanTypeCenter:
            // For all voices at note time
            panParam = PanParamDefault;
            break;

        case PanTypeFixed:
            // For all voices at note time
            panParam = param;
            break;

        case PanTypeHold:
            // Per-Voice Parameter passed to note
            panParam = panPosition;
            break;

        case PanTypeFollow:
            // Use current position to start, then follow setting on updates
            // Continuous for all voices
            panParam = panPosition;
            break;

        case PanTypeRandom:
            // Per-Voice Parameter passed to note
            break;

        case PanTypeAuto:
            // TODO: Generate position from wave function TBD
            break;

        default:
            break;
    }
}

// setPanPosition()
//
void MediaPlayerMulti::setPanPosition(uint8_t position) {

//    Serial.printf("MediaPlayerMulti::setPanPosition: %d\n", position);

    panPosition = position;

    for (uint8_t player = 0; player < _playerCount; player++) {
        if (panType == PanTypeFollow) {
            mediaPlayers[player]->setPan(UtilFns::scale(panPosition, 0, 127, 0.0, 1.0));
        }
    }
}

// sustainPedal()
//
void MediaPlayerMulti::sustainPedal(bool pressed) {

//    Serial.printf("BasicSynth::sustainPedal: %d\n", pressed);

    sustainPedalPressed = pressed;

    if (!sustainPedalPressed) {
        for (int i = 0; i < _playerCount; i++) {
            if (voiceHeldState[i] == VoiceHeldPending) {
                MidiNoteNumber note = voiceMgr.getVoiceActiveNote(i);
                if (note != MidiNoteNone) {
                    noteOff(note - noteShift);
                }
            }
        }
    }
}

/************************************************************************************
*                          P R I V A T E   F U N C T I O N S                        *
*************************************************************************************
*/

// setNotePosition()
//
void MediaPlayerMulti::setNotePosition() {

    switch (panType) {
        case PanTypeCenter:
            panPosition = 63;
            break;

        case PanTypeFixed:
            panPosition = panParam;
            break;

        case PanTypeHold:
            break;

        case PanTypeFollow:
            break;

        case PanTypeRandom:
            panPosition = UtilFns::getRand(PanParamMin, PanParamMax);
            break;

        case PanTypeAuto:
            // Generate position from wave function TBD
            break;

        default:
            break;
    }
}


