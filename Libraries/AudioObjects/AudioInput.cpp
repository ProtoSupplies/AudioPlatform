/************************************************************************************
*                                  AudioInput.cpp                                   *
*                                                                                   *
*                      Copyright (c) 2025, Donald Swearingen                        *
*                              All Rights Reserved                                  *
*************************************************************************************
*
* Description
*
*   AudioInput is basically a wrapper for Teensy 4.1 I2S audio inputs in support of
*   line in capability, providing functions to set input gain, enable/disable input
*   signal, and access module's L/R output signals for patching connections in
*   parent/owner classes.
*
* Copyright Notices
*
*   AudioInput
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

#include "AudioInput.h"
#include "ArduinoEclipseDefs.h"

/************************************************************************************
*                         M A C R O S  /  C O N S T A N T S                         *
*************************************************************************************
*/

/************************************************************************************
*                     D A T A / T Y P E   D E F I N I T I O N S                     *
*************************************************************************************
*/


/************************************************************************************
*                    P U B L I C   C L A S S   F U N C T I O N S                    *
*************************************************************************************
*/

// Constructor / Destructor
//
AudioInput::AudioInput() {

    setGain(inputGainDefault);
    setEnabled(true);
}

AudioInput::~AudioInput() {

}

// getLeftOutput / getRightOutput
//
// Return pointers to final amplifier stage to be
// used for signal chain connections in derived class.
//
AudioStream* AudioInput::getLeftOutput() {
//AudioAmplifier* AudioInput::getLeftOutput() {
    return &gainLeft;
}

AudioStream* AudioInput::getRightOutput() {
//AudioStream* AudioInput::getRightOutput() {
    return &gainRight;
}

// setEnabled
//
// Enable/disable module's LR input signals
//
void AudioInput::setEnabled(bool state) {

    if (state) {
        enabled = true;
        audioInToGainLeft.connect(audioLineIn, 0, gainLeft, 0);
        audioInToGainRight.connect(audioLineIn, 1, gainRight, 0);

    } else {
        enabled = false;
        audioInToGainLeft.disconnect();
        audioInToGainRight.disconnect();
    }
}

// setGain
//
// Set module's LR input gain
//
void AudioInput::setGain(float gainValue) {

    inputGain = gainValue;

    gainLeft.gain(inputGain);
    gainRight.gain(inputGain);
}

/************************************************************************************
*                   P R I V A T E   C L A S S   F U N C T I O N S                   *
*************************************************************************************
*/



