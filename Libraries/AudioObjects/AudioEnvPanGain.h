/************************************************************************************
*                                 AudioEnvPanGain.h                                 *
*                      Copyright (c) 2025, Donald Swearingen                        *
*                               All Rights Reserved                                 *
*************************************************************************************
*
* Description
*
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

#ifndef AUDIOENVPANGAIN_H_
#define AUDIOENVPANGAIN_H_

// Standard C++
//
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

// Teensyduino
//
#include <Audio.h>

// User Arduino Libraries
//
#include "UtilFns.h"
#include "Teensy41_Util.h"
#include "ArduinoEclipseDefs.h"

#include "AudioPanGain.h"

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

class AudioEnvPanGain {

// Constructor/Destructor
//
public:

    AudioEnvPanGain();
    AudioEnvPanGain(AudioStream& srcOutLeft, AudioStream& srcOutRight);
    virtual ~AudioEnvPanGain();

// Class Variables
//
public:


private:

    // Audio Envelope Control
    static constexpr float DefaultEnvAttackTime        = 10.0;  // ms
    static constexpr float DefaultEnvSustainLevel      = 1.0;   // max of 0.0-1.0 range
    static constexpr float DefaultEnvReleaseTime       = 10.0;  // ms
    static constexpr float DefaultEnvNoteOnReleaseTime = 10.0;  // ms

protected:

    // Player Left/Right Envelopes
    // Used to prevent clicks when player switched on and off
    AudioEffectEnvelope _envelopeLeft;          // instance envelopes
    AudioEffectEnvelope _envelopeRight;

    // Inter-module patch cords
    AudioConnection     _inputToEnvelopeLeft;
    AudioConnection     _inputToEnvelopeRight;

    // PanGain Module
    AudioPanGain        _panGain;

// Class Methods
//
public:

    // Outputs from final amplifier stage. Functions allow for
    // connection routing in parent/owner class.
    AudioStream*        getLeftOutput();
    AudioStream*        getRightOutput();

    void                setLeftInput(AudioStream& srcOutLeft);
    void                setRightInput(AudioStream& srcOutRight);
    void                setLeftRightInput(AudioStream& srcOutLeft);

    void                setEnvelope(float delay, float attack, float hold, float decay, float sustain, float release, float releaseNoteOn);
    void                setDelay(float delay);
    void                setAttack(float attack);
    void                setHold(float hold);
    void                setDecay(float decay);
    void                setSustain(float sustain);
    void                setRelease(float release);
    void                setReleaseNoteOn(float releaseNoteOn);
    bool                envelopeIsActive();

    void                setGain(float gain);
    void                setPan(float position);

    void                envNoteOn();
    void                envNoteOff();

protected:


};

#endif // AUDIOENVPANGAIN_H_
