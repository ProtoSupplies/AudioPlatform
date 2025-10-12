/************************************************************************************
*                                   AudioMixer8.h                                   *
*                                                                                   *
*                       Copyright (c) 2025, Donald Swearingen                       *
*                                All Rights Reserved                                *
*************************************************************************************
*
* Description
*
*   AudioMixer8 is based on the original Teensy Audio Library AudioMixer4 class
*   (see PJRC copyright declaration below), modified, extended, and reformatted to
*   support an additional 4 input channels, for a total of 8 inputs.
*
* Copyright Notices
*
*   AudioMixer8
*   Copyright (c) 2025, Donald Swearingen
*
*   Audio Library for Teensy 3.X
*   Copyright (c) 2014, Paul Stoffregen, paul@pjrc.com
*
*   Development of this audio library was funded by PJRC.COM, LLC by sales of
*   Teensy and Audio Adaptor boards.  Please support PJRC's efforts to develop
*   open source software by purchasing Teensy or other PJRC products.
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

#ifndef AUDIOMIXER8_H_
#define AUDIOMIXER8_H_

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

#include <Arduino.h>     // github.com/PaulStoffregen/cores/blob/master/teensy4/Arduino.h
#include <AudioStream.h> // github.com/PaulStoffregen/cores/blob/master/teensy4/AudioStream.h

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

class AudioMixer8 : public AudioStream {

// Constructor/Destructor
//
public:

    AudioMixer8(void);

// Class Variables
//
public:


private:

    int32_t         multiplier[8];
    audio_block_t   *inputQueueArray[8];

protected:


// Class Methods
//
public:

    void            gain(unsigned int channel, float gain);
    virtual void    update(void);

private:

    static void     applyGain(int16_t *data, int32_t mult);
    static void     applyGainThenAdd(int16_t *data, const int16_t *in, int32_t mult);

protected:


};
#endif // AUDIOMIXER8_H_
