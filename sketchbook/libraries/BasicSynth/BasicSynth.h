/************************************************************************************
*                                   BasicSynth.h                                    *
*                      Copyright (c) 2025, Donald Swearingen                        *
*                               All Rights Reserved                                 *
*************************************************************************************
*
* Description
*
* Copyright Notices
*
*   This version of BasicSynth is based on the original by Ghost In Translation posted
*   on Git at https://github.com/ghostintranslation/BasicSynth. Both versions are issued
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

#ifndef SIMPLESYNTH_H_
#define SIMPLESYNTH_H_

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
#include "VoiceManager.h"
#include "ArduinoEclipseDefs.h"
#include "AudioPanGain.h"

#include "BasicSynthVoice.h"

/************************************************************************************
*                         M A C R O S  /  C O N S T A N T S                         *
*************************************************************************************
*/

const byte VoiceCount           = 12;   // max = 16
const byte VoicesPerMixerCount  = 4;    // 8 when AudioMixer8 used

/************************************************************************************
*                     D A T A / T Y P E   D E F I N I T I O N S                     *
*************************************************************************************
*/

/************************************************************************************
*                         C L A S S   D E F I N I T I O N S                         *
*************************************************************************************
*/

class BasicSynth {

// Constructor/Destructor
//
public:

    BasicSynth();
    virtual ~BasicSynth() {};

// Class Variables
//
public:


private:

    BasicSynthVoice*    voices[VoiceCount];
    VoiceHeldState      voiceHeldState[VoiceCount];

    SynthType           synthType;
    unsigned int        attack;
    unsigned int        decay;
    float               sustain;
    unsigned int        release;
    int                 modFreq;
    float               modAmt;

    AudioConnection*    voiceToVoiceMixerLeft[VoiceCount];
    AudioConnection*    voiceMixerToOutMixerLeft[VoiceCount / VoicesPerMixerCount];
    AudioConnection*    outMixerLeftToOutAmpLeft;
    AudioConnection*    voiceToVoiceMixerRight[VoiceCount];
    AudioConnection*    voiceMixerToOutMixerRight[VoiceCount / VoicesPerMixerCount];
    AudioConnection*    outMixerRightToOutAmpRight;

    AudioMixer4*        voiceLeftMixers[VoiceCount / 2];
    AudioMixer4         outMixerLeft;
    AudioAmplifier      outAmpLeft;
    AudioMixer4*        voiceRightMixers[VoiceCount / 2];
    AudioMixer4         outMixerRight;
    AudioAmplifier      outAmpRight;

    AudioPanGain        outChain;

    // Voice allocation/replacement manager
    VoiceManager        voiceMgr;

    // Device enabled/disabled
    bool                playerEnabled;

    // Sustain pedal state
    bool                sustainPedalPressed;

    // MIDI note/key shift variables
    int                 noteShift;
    MidiNoteNumber      noteRangeStart;
    MidiNoteNumber      noteRangeEnd;

    // MIDI pan
    MidiPanType         panType;
    uint8_t             panParam;
    uint8_t             panPosition;

protected:


// Class Methods
//
public:

    void                initialize() {};
    void                updateControls();
    AudioStream*        getLeftOutput();
    AudioStream*        getRightOutput();
    void                setNoteShift(int _noteShift);
    void                setNoteRangeStart(MidiNoteNumber _noteRangeStart);
    void                setNoteRangeEnd(MidiNoteNumber _noteRangeEnd);
    void                noteOn(uint8_t note, uint8_t midiVelocity);
    void                noteOff(byte midiNote);
    void                setPanType(MidiPanType type, uint8_t param);
    void                setPanPosition(uint8_t position);
    void                allNotesOff();
    void                sustainPedal(bool pressed);
    void                setEnabled(bool enabled);
    void                setGain(float gainValue);
    void                setSynthType(SynthType type);
    void                setAttack(float attackValue);
    void                setDecay(float decayValue);
    void                setSustain(float sustainLevel);
    void                setRelease(float releaseValue);
    void                setModulatorFrequency(float freqHz);
    void                setModulatorAmplitude(float amplitude);

private:

    void                setNotePosition();

protected:

};

#endif  // SIMPLESYNTH_H_
