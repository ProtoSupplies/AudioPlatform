/************************************************************************************
*                               BasicSynthVoice.h                                  *
*                                                                                   *
*                      Copyright (c) 2025, Donald Swearingen                        *
*                               All Rights Reserved                                 *
*************************************************************************************
*
* Description
*
* Copyright Notices
*
*   This version of BasicSynthVoice is based on the original by Ghost In Translation
*   posted on Git at https://github.com/ghostintranslation/BasicSynth. Both versions
*   are issued under the MIT License conditions described below:
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

#ifndef SIMPLESYNTHVOICE_H_
#define SIMPLESYNTHVOICE_H_

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
#include "AudioEnvPanGain.h"
#include "MidiDefines.h"

/************************************************************************************
*                         M A C R O S  /  C O N S T A N T S                         *
*************************************************************************************
*/

/************************************************************************************
*                     D A T A / T Y P E   D E F I N I T I O N S                     *
*************************************************************************************
*/

// Custom types
//
typedef enum {
    FM,
    FMx10,
    AM,
    AMx10,
    RING
} SynthType;

/************************************************************************************
*                         C L A S S   D E F I N I T I O N S                         *
*************************************************************************************
*/

class BasicSynthVoice {

    // Constructor/Destructor
    //
    public:

    BasicSynthVoice();
    virtual ~BasicSynthVoice() {};

// Class Variables
//
public:

    MidiNoteNumber              currentNote; // The midi note currently being played.

private:

    // AM and Ring
    AudioSynthWaveformDc        dcAM;
    AudioSynthWaveform          sawtoothAM;
    AudioSynthWaveform          sineAM;
    AudioEffectMultiply         multiplyAM;
    AudioMixer4                 mixerAM;
    AudioMixer4                 mixerAM2;
    AudioMixer4                 mixerDcMod;

    // FM
    AudioSynthWaveformModulated sineFM;
    AudioSynthWaveformModulated sawtoothFM;
    AudioMixer4                 mixerFM;

    // Common
    AudioSynthWaveformSine      sineModulator;
    AudioMixer4                 outMixer;

    // Patch Cords
    AudioConnection             sineAmToMixerAm;
    AudioConnection             dcAmToDcMod;
    AudioConnection             sawtoothAToMixerAm;
    AudioConnection             sineModToSawtoothFM;
    AudioConnection             sineModToSineFm;
    AudioConnection             sineModtoMixerDcMod;
    AudioConnection             mixerDcModToMultiplyAm;
    AudioConnection             mixerAmToMultiplyAm;
    AudioConnection             mixerAmToMixerAm2;
    AudioConnection             sineFmToMixerFm;
    AudioConnection             samwoothFmToMixerFm;
    AudioConnection             multiplyAmToMixerAm2;
    AudioConnection             mixerFmToOutMixer;
    AudioConnection             mixerAm2ToOutMixer;

    // Output Chain
    AudioEnvPanGain             envPanGain;

    // Misc
    SynthType                   synthType;
    byte                        note;
    bool                        notePlayed;

protected:

// Class Methods (References)
//
public:

    AudioStream*    getLeftOutput();
    AudioStream*    getRightOutput();

    void            setSynthType(SynthType synthType);
    void            sustainPedal(bool pressed);
    void            noteOn(byte midiNote = 0);
    void            noteOn(uint8_t note, uint8_t midiVelocity, uint8_t midiPan);
    void            setPanPosition(uint8_t position);
    void            noteOff();
    bool            isActive();
    bool            isNotePlayed();
    void            setNotePlayed(bool notePlayed);
    void            setFrequency(int frequency);
    void            setModulatorFrequency(int freq);
    void            setModulatorAmplitude(float amp);
    void            setADSR(unsigned int attack, unsigned int decay, float sustain, unsigned int release);
    void            setEnvAttack(int att);
    void            setEnvDecay(int dec);
    void            setEnvSustain(float level);
    void            setEnvRelease(int rel);

private:


protected:


};

#endif // SIMPLESYNTHVOICE_H_
