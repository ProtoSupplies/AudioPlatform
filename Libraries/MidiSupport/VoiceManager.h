/************************************************************************************
*                                 VoiceManager.h                                    *
*                      Copyright (c) 2025, Donald Swearingen                        *
*                               All Rights Reserved                                 *
*************************************************************************************
*
* Description
*
*   VoiceManager provides a set of functions that support the assignment of a variable
*   number of "voices" to classes designed for the simultaneous playback of multiple
*   audio streams.
*
*   See VoiceManager.cpp for further details.
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

#ifndef VOICEMANAGER_H_
#define VOICEMANAGER_H_

#include <stdint.h>
#include "elapsedMillis.h"

#include "ArduinoEclipseDefs.h"
#include "MidiDefines.h"

/************************************************************************************
*                         M A C R O S  /  C O N S T A N T S                         *
*************************************************************************************
*/

const uint32_t      MaxUint32       = 4294967295;   // maximum unsigned 32-bit number

/************************************************************************************
*                     D A T A / T Y P E   D E F I N I T I O N S                     *
*************************************************************************************
*/

typedef uint8_t     VoiceNumber;
typedef uint32_t    NoteStartTime;

typedef struct {

    bool            voiceActive;        // true when voice is playing / in use
    MidiNoteNumber  activeNote;         // active voice's MIDI note [MidiNoteMin, MidiNoteMax]
    NoteStartTime   noteStartTime;      // active voice's start time (ms since startup)

} VoiceState;

typedef enum {

    VoiceNotHeld,                       // Voice not held / key not pressed
    VoiceHeld,                          // Voice held / key pressed
    VoiceHeldPending                    // Voice held / key released with pedal down

} VoiceHeldState;

/************************************************************************************
*                         C L A S S   D E F I N I T I O N S                         *
*************************************************************************************
*/

class VoiceManager {

// Constructor/Destructor
//
public:

    VoiceManager(uint8_t _voiceCount);
    virtual ~VoiceManager();

// Class Variables
//
public:

    static const uint8_t    MaxVoices = 16;
    static const uint8_t    VoiceNone = 255;                // "NULL" voice ID

private:

    uint8_t                 voiceCount;                     // # voices in pool (owner-defined)
    uint8_t                 activeVoiceCount;               // # voices currently in use
    VoiceState              voiceInfo[MaxVoices];
    VoiceNumber             voiceUsingNote[MidiNoteMax + 1];
    uint8_t                 lastVoiceAssigned;

    static const uint32_t   MaxStartTime = MaxUint32;      // maximum unsigned 32-bit number

protected:


// Class Methods
//
public:

    VoiceNumber             getFreeVoice();
    VoiceNumber             getVoiceUsingNote(MidiNoteNumber midiNote);
    MidiNoteNumber          getVoiceActiveNote(VoiceNumber voiceNum);
    void                    setVoiceActive(bool voiceActive, VoiceNumber voiceNum, MidiNoteNumber noteNum);
    VoiceNumber             findOldestVoice();
    uint8_t                 getActiveVoiceCount();

private:


protected:


};
#endif // VOICEMANAGER_H_
