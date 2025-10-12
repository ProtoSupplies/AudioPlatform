/************************************************************************************
*                                   AudioInput.h                                    *
*                                                                                   *
*                      Copyright (c) 2025, Donald Swearingen                        *
*                              All Rights Reserved                                  *
*************************************************************************************
*
* Description
*
*   AudioInput is basically a wrapper for Teensy 4.1 I2S audio inputs in support of
*   line in capability.
*
*   For further details, see the header of AudioInput.cpp.
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

#ifndef AUDIOINPUT_H_
#define AUDIOINPUT_H_

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

// Teensyduino
//
#include <Audio.h>

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

class AudioInput {

// Constructor/Destructor
//
public:

    AudioInput();
    virtual ~AudioInput();

// Class Variables
//
public:


private:

    // I2S Signal In
    AudioInputI2S       audioLineIn;

    // Gain amplifiers set output gain
    AudioAmplifier      gainLeft;
    AudioAmplifier      gainRight;

    // Line In gain [0.0,1.0]
    float               inputGain;
    const float         inputGainDefault = 1.0;  // Unity gain (0 dB)

    // Inter-module patch cords
    AudioConnection     audioInToGainLeft;
    AudioConnection     audioInToGainRight;

    // Module state
    bool                enabled;

protected:


// Class Methods
//
public:

    // Outputs from final amplifier stage. Functions allow for
    // connection routing in parent/owner class.
    AudioStream*        getLeftOutput();
    AudioStream*        getRightOutput();

    void                setEnabled(bool state);
    void                setGain(float gainValue);

private:


protected:


};

#endif // AUDIOINPUT_H_
