/************************************************************************************
*                                    FmSynth.h                                      *
*                      Copyright (c) 2025, Donald Swearingen                        *
*                               All Rights Reserved                                 *
*************************************************************************************
*
* Description
*
*
* Copyright Notices
*
*   This version of FmSynth is based on the original by Ghost In Translation posted
*   on Git at https://github.com/ghostintranslation/FmSynth. Both versions are issued
*   under the MIT License conditions described below:
*
*   Copyright (c) 2025, Donald Swearingen
*   Copyright (c) 2019, Ghost In Translation
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
* Modifications List
*
*/

#ifndef FMSYNTH_H_
#define FMSYNTH_H_

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

// TeensyDuino Libraries
//
#include <Audio.h>

// User Arduino Libraries
//
#include "ArduinoEclipseDefs.h"
#include "AudioPanGain.h"
#include "MidiDefines.h"
#include "synth_dexed.h"

/************************************************************************************
*                         M A C R O S  /  C O N S T A N T S                         *
*************************************************************************************
*/

const uint16_t  VoicePresetDecodedLength    = 156;

/************************************************************************************
*                     D A T A / T Y P E   D E F I N I T I O N S                     *
*************************************************************************************
*/

/************************************************************************************
*                         C L A S S   D E F I N I T I O N S                         *
*************************************************************************************
*/

class FmSynth {

// Constructor/Destructor
//
public:

    // Dummy argument required (see section "Constructor Variants Issue" in heading above).
    FmSynth(const uint8_t DexedVoiceCount, const uint16_t  DexedSampleRate);
    virtual ~FmSynth() {};

// Class Variables
//
public:


private:

    AudioSynthDexed     dexSynth;
    uint8_t             maxVoices;
    unsigned int        attack;
    unsigned int        decay;
    float               sustain;
    unsigned int        release;
    int                 modFreq;
    float               modAmt;

    AudioPanGain        audioPanGain;
    MidiPanType         panType;
    uint8_t             panParam;
    uint8_t             panPosition;
    uint8_t             panPositionPrev;
    bool                holdSet;
    bool                randomSet;

    uint8_t             decoded_voice[VoicePresetDecodedLength];
    uint16_t            currentVoicePresetNum;
    uint8_t*            presetBanks;

    // Device enabled/disabled
    bool                playerEnabled;

    // Sustain pedal state
    bool                sustainPedalPressed;

    // MIDI note/key shift variables
    int                 noteShift;
    MidiNoteNumber      noteRangeStart;
    MidiNoteNumber      noteRangeEnd;
    uint8_t             heldNoteCount;


protected:


// Class Methods
//
public:

    void                initialize() {};
    void                update();
    void                setEnabled(bool enabled);

    void                setPrestBanksPtr(uint8_t* banksAddress);
    const char*         getVoicePresetName(uint16_t voicePresetNum);
    void                selectVoicePreset(uint16_t voicePresetNum);

    AudioStream*        getLeftOutput();
    AudioStream*        getRightOutput();
    void                setNoteShift(int _noteShift);
    void                setNoteRangeStart(MidiNoteNumber _noteRangeStart);
    void                setNoteRangeEnd(MidiNoteNumber _noteRangeEnd);
    void                noteOn(uint8_t note, uint8_t midiVelocity);
    void                noteOff(byte midiNote);
    void                sustainPedal(bool pressed);
    void                setPitchbendRange(uint8_t range);
    void                setPanType(MidiPanType type, uint8_t param);
    void                setPanPosition(uint8_t position);
    void                pitchBend(int16_t bend);
    void                allNotesOff();
    void                setGain(float gainValue);
    void                setAttack(float attack);
    void                setDecay(float decay);
    void                setSustain(float level);
    void                setRelease(float release);
    void                setModulatorFrequency(float freqHz);
    void                setModulatorAmplitude(float amplitude);

private:

    void                loadVoice(uint16_t voiceNum);

protected:

};

#endif // FMSYNTH_H_
