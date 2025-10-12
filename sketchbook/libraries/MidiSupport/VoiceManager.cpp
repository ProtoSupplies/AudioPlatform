/************************************************************************************
*                                 VoiceManager.cpp                                  *
*                      Copyright (c) 2025, Donald Swearingen                        *
*                               All Rights Reserved                                 *
*************************************************************************************
*
* Description
*
*   VoiceManager provides a set of functions that support the assignment of a variable
*   number of "voices" (an abstract entity designed to play a single audio stream)
*   subject to play/stop control based on MIDI NoteOn and NoteOff messages.
*
*   Intended as a component of classes design for playback of multiple simultaneous
*   audio streams, methods are provided both for assignment of a voice channel from
*   a fixed pool of available voices, as well as assignment of a new voice by a
*   "voice-stealing" mechanism when there are no inactive voices available in the pool.
*
*
* Modifications List
*
*   •
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

#include "VoiceManager.h"

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

// Constructor/Destructor
//
VoiceManager::VoiceManager(uint8_t _voiceCount) : voiceCount(_voiceCount), activeVoiceCount(0) {

    // Restrict pool size
    if (voiceCount > MaxVoices) {
        voiceCount = MaxVoices;
    }

    // Initialize pool data
    for (VoiceNumber voice = 0; voice < voiceCount; voice++) {
        voiceInfo[voice].activeNote = MidiNoteNone;
        voiceInfo[voice].voiceActive = false;
        voiceInfo[voice].noteStartTime = 0;
    }

    for (MidiNoteNumber note = 0; note < (MidiNoteMax + 1); note++) {
        voiceUsingNote[note] = VoiceNone;
    }

    lastVoiceAssigned = voiceCount - 1;
}

VoiceManager::~VoiceManager() {

}

// getFreeVoice()
//
// Find first free voice. If none, return oldest active voice.
//
VoiceNumber VoiceManager::getFreeVoice() {

//    Serial.printf("VoiceManager::getFreeVoice: voiceCount %d\n", voiceCount);

    // Defaults
    VoiceNumber oldestVoice = VoiceNone;
    NoteStartTime oldestStartTime = MaxStartTime;
    VoiceNumber freeVoice = VoiceNone;
    MidiNoteNumber activeNote = MidiNoteNone;

    // Find an available voice
    uint8_t voicesChecked = 0;
    VoiceNumber voice = (lastVoiceAssigned + 1) % voiceCount;
    while (voicesChecked <= voiceCount) {

        // Check for inactive voice
        if (!voiceInfo[voice].voiceActive) {
            freeVoice = voice;      // found
            break;
        }

        voicesChecked++;
        voice = (voice + 1) % voiceCount;
    }

    // Track oldest (longest in use) voice in pool
    for (voice = 0; voice < voiceCount; voice++) {

        if (voiceInfo[voice].noteStartTime < oldestStartTime) {
            oldestVoice = voice;
            oldestStartTime = voiceInfo[voice].noteStartTime;
        }
    }

    // If no free voices, use oldest active voice (voice stealing)
    if (freeVoice == VoiceNone) {
        freeVoice = oldestVoice;
        activeNote = voiceInfo[freeVoice].activeNote;
//        Serial.printf("VoiceManager::getFreeVoice: (oldest voice) freeVoice %d activeNote %d\n", activeNote);
    }

#if 0 // Action was keeping activeVoiceCount at 0 or 1
    // Deactivate voice initially, leaving it to caller
    // to determine whether or not to activate.
//    Serial.printf("VoiceManager::getFreeVoice: calling setVoiceActive() false freeVoice %d activeNote %d\n", freeVoice, activeNote);
    setVoiceActive(false, freeVoice, activeNote);
    lastVoiceAssigned = freeVoice;
#endif
//    Serial.printf("VoiceManager::getFreeVoice: freeVoice %d\n", freeVoice);

    return freeVoice;
}

// getVoiceUsingNote()
//
// Return the voice (if any) currently assigned to a specific
// MIDI note. If none, the returned contents will be VoiceNone.
//
VoiceNumber VoiceManager::getVoiceUsingNote(MidiNoteNumber midiNote) {

    return voiceUsingNote[midiNote];
}

// getVoiceActiveNote()
//
// Return currently active note for a specified voice
//
MidiNoteNumber VoiceManager::getVoiceActiveNote(VoiceNumber voiceNum) {

    return voiceInfo[voiceNum].activeNote;
}

// setVoiceActive()
//
// Activate/Deactivate a specified voice, setting the voice's information
// profile as appropriate to the specified state
//
void VoiceManager::setVoiceActive(bool activate, uint8_t voiceNum, uint8_t noteNum) {

//    Serial.printf("VoiceManager::setVoiceActive activeVoiceCount %d voiceCount %d\n", activeVoiceCount, voiceCount);

    if (activate) {
//        Serial.printf("VoiceManager::setVoiceActive ACTIVATE voiceNum %d noteNum %d\n", voiceNum, noteNum);
        if (activeVoiceCount < voiceCount) {
            activeVoiceCount++;
        }
        voiceInfo[voiceNum].activeNote = noteNum;
        voiceInfo[voiceNum].voiceActive = true;
        voiceInfo[voiceNum].noteStartTime = millis();
        if (noteNum != MidiNoteNone) {
            voiceUsingNote[noteNum] = voiceNum;
        }
        lastVoiceAssigned = voiceNum;

    } else {
//        Serial.printf("VoiceManager::setVoiceActive DEACTIVATE voiceNum %d\n", voiceNum, noteNum);
        if (activeVoiceCount > 0) {
            activeVoiceCount--;
        }
        voiceInfo[voiceNum].activeNote = MidiNoteNone;
        voiceInfo[voiceNum].voiceActive = false;
        voiceInfo[voiceNum].noteStartTime = 0;
        if (noteNum != MidiNoteNone) {
            voiceUsingNote[noteNum] = VoiceNone;
        }
    }
//    Serial.printf("VoiceManager::setVoiceActive activeVoiceCount %d\n", activeVoiceCount);
}

// findOldestVoice
//
// When all voices are active, find the one that has been active the longest, to be
// replaced by a new note.
//
// Not used internally by VoiceMaganer, but provided for convenience of parent/owner
// classes which may require this information.
//
uint8_t VoiceManager::findOldestVoice() {

    uint8_t oldestVoice = VoiceNone;
    uint32_t oldestStartTime = MaxStartTime;

    for (int voice = 0; voice < voiceCount; voice++) {
        if (voiceInfo[voice].noteStartTime < oldestStartTime) {
            oldestVoice = voice;
            oldestStartTime = voiceInfo[voice].noteStartTime;
        }
    }

    return oldestVoice;
}

// getActiveVoiceCount
//
uint8_t VoiceManager::getActiveVoiceCount() {

    return activeVoiceCount;
}

/************************************************************************************
*                          P R I V A T E   F U N C T I O N S                        *
*************************************************************************************
*/

