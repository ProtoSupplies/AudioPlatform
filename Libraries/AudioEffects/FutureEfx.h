/************************************************************************************
*                                    FutureEfx.h                                    *
*                       Copyright (c) 2025, Donald Swearingen                       *
*                                All Rights Reserved                                *
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

#ifndef FUTUREEFX_H_
#define FUTUREEFX_H_

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

/************************************************************************************
*                         M A C R O S  /  C O N S T A N T S                         *
*************************************************************************************
*/

// Use External PSRAM
//
#ifndef EFX_AUDIOMEM_USE_EXT
#define EFX_AUDIOMEM_USE_EXT    1
#endif

const float FutureEfxMemMaxTime  = 11000.0f;     // Maximum delay time in ms when using external memory

/************************************************************************************
*                     D A T A / T Y P E   D E F I N I T I O N S                     *
*************************************************************************************
*/

/************************************************************************************
*                         C L A S S   D E F I N I T I O N S                         *
*************************************************************************************
*/

class FutureEfx {

// Constructor/Destructor
//
public:

    FutureEfx();
    FutureEfx(AudioStream& srcOutLeft, AudioStream& srcOutRight);

    virtual ~FutureEfx();

// Class Variables
//
public:


private:


protected:

    // Amplifiers used to set Player output gain
    AudioAmplifier      _gainLeftAmp;
    AudioAmplifier      _gainRightAmp;

#if EFX_AUDIOMEM_USE_EXT
#else // INTERNAL RAM
#endif // EFX_AUDIOMEM_USE_EXT / STANDARD_DELAY

#if 0
    // Inter-module patch cords
    AudioConnection     _inputLeftToMixerLeft;
    AudioConnection     _inputRightToMixerRight;
    AudioConnection     _mixerLeftToDelayLeft;
    AudioConnection     _mixerRightToDelayRight;

    AudioConnection     _delayLeftFbToMixerLeft;
    AudioConnection     _delayLeftXfbToMixerRight;
    AudioConnection     _delayRightFbToMixerRight;
    AudioConnection     _delayRightXfbToMixerLeft;

    AudioConnection     _delayLeftToOutputLeft;
    AudioConnection     _delayRightToOutputRight;
#endif

// Class Methods
//
public:

    AudioStream*        getLeftOutput();
    AudioStream*        getRightOutput();

    void                setLeftInput(AudioStream& srcOutLeft);
    void                setRightInput(AudioStream& srcOutRight);

    void                setGain(float gain);
    void                setLeftOutGain(float gain);
    void                setRightOutGain(float gain);

    // Dummy parameters for testing
    void                setParam2(float paramVal);
    void                setParam3(float paramVal);
    void                setParam4(float paramVal);

protected:


};

#endif // FUTUREEFX_H_
