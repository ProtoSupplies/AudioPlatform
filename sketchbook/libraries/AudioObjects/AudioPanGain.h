/************************************************************************************
*                                  AudioPanGain.h                                   *
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

#ifndef AUDIOPANGAIN_H_
#define AUDIOPANGAIN_H_

// Standard C++
//
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>

// Teensyduino
//
#include <Audio.h>

// User Arduino Libraries
//
#include "UtilFns.h"
#include "Teensy41_Util.h"
#include "ArduinoEclipseDefs.h"
#include "MidiDefines.h"

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

class AudioPanGain {

// Constructor/Destructor
//
public:

    AudioPanGain();
    AudioPanGain(AudioStream& srcOutLeft, AudioStream& srcOutRight);
    virtual ~AudioPanGain();

// Class Variables
//
public:


private:


protected:

    // Amplifiers used to set Player pan position
    AudioAmplifier      _panLeftAmp;
    AudioAmplifier      _panRightAmp;

    // Amplifiers used to set Player output gain
    AudioAmplifier      _gainLeftAmp;
    AudioAmplifier      _gainRightAmp;

    // Inter-module patch cords
    AudioConnection     _inputToPanLeft;
    AudioConnection     _inputToPanRight;
    AudioConnection     _panLeftToGainLeft;
    AudioConnection     _panRightToGainRight;

// Class Methods
//
public:

    // Outputs from final amplifier stage. Functions allow for
    // connection routing in parent/owner class.
    AudioStream*        getLeftOutput();
    AudioStream*        getRightOutput();

    void                setLeftInput(AudioStream& srcOutLeft);
    void                setRightInput(AudioStream& srcOutRight);

    void                setGain(float gain);
    void                setPan(float position);

protected:


};

#endif // AUDIOPANGAIN_H_
