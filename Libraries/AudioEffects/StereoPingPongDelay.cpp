/************************************************************************************
*                              StereoPingPongDelay.cpp                              *
*                       Copyright (c) 2025, Donald Swearingen                       *
*                                All Rights Reserved                                *
*************************************************************************************
*
* Description
*
*
* Modifications List
*
*   •
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

#include "StereoPingPongDelay.h"

/************************************************************************************
*                         M A C R O S  /  C O N S T A N T S                         *
*************************************************************************************
*/

#define INMIXER_CHAN_DELAYIN    0       // Input from "outside world"
#define INMIXER_CHAN_DELAY_FB   1       // Delay self-feedback input
#define INMIXER_CHAN_DELAY_XFB  2       // Cross-Feedback input from other delay

#define DELAYOUT_CHAN_DELAY_OUT 0       // Output to "outside world"
#define DELAYOUT_CHAN_DELAY_FB  1       // Delay self-feedback output
#define DELAYOUT_CHAN_DELAY_XFB 2       // Cross-Feedback output to other delay

/************************************************************************************
*                     D A T A / T Y P E   D E F I N I T I O N S                     *
*************************************************************************************
*/


/************************************************************************************
*                          P U B L I C   F U N C T I O N S                          *
*************************************************************************************
*/

// Constructor
//
StereoPingPongDelay::StereoPingPongDelay() {

}

StereoPingPongDelay::StereoPingPongDelay(AudioStream& srcOutLeft, AudioStream& srcOutRight) :
    _delayLeft(AUDIO_MEMORY_EXTMEM, DelayExtMemMaxTime), _delayRight(AUDIO_MEMORY_EXTMEM, DelayExtMemMaxTime) {

    _inputLeftToMixerLeft.connect(srcOutLeft, 0, _delayInMixerLeft, INMIXER_CHAN_DELAYIN);
    _inputRightToMixerRight.connect(srcOutRight, 0, _delayInMixerRight, INMIXER_CHAN_DELAYIN);

    _mixerLeftToDelayLeft.connect(_delayInMixerLeft, _delayLeft);
    _mixerRightToDelayRight.connect(_delayInMixerRight, _delayRight);

    _delayLeftFbToMixerLeft.connect(_delayLeft, DELAYOUT_CHAN_DELAY_OUT, _delayInMixerLeft, INMIXER_CHAN_DELAY_FB);
    _delayLeftXfbToMixerRight.connect(_delayLeft, DELAYOUT_CHAN_DELAY_XFB, _delayInMixerRight, INMIXER_CHAN_DELAY_XFB);

    _delayRightFbToMixerRight.connect(_delayRight, DELAYOUT_CHAN_DELAY_OUT, _delayInMixerRight, INMIXER_CHAN_DELAY_FB);
    _delayRightXfbToMixerLeft.connect(_delayRight, DELAYOUT_CHAN_DELAY_XFB, _delayInMixerLeft, INMIXER_CHAN_DELAY_XFB);

    _delayLeftToOutputLeft.connect(_delayLeft, DELAYOUT_CHAN_DELAY_OUT, _gainLeftAmp, 0);
    _delayRightToOutputRight.connect(_delayRight, DELAYOUT_CHAN_DELAY_OUT, _gainRightAmp, 0);

    // FFS: Document: VERY IMPORTANT that gain on FB channels must not be
    // or approach 1.0 unless direct input is muted. Otherwise, FB overload.
    //
    // This means the TOTAL FB gain (sum of all gain settings). In a xFB
    // setting, if both the FB and the xFB channel gains are 0.5 for both
    // the L and R channels, there will be a buildup and overload of the
    // output levels.
    //
    // Maybe the input values for a specific channel should be scaled
    // by the sum of the channel gains (all assumed 0-1)
    //
    //      chanGainX   = newVal * sumOfGains
    //                  = .3 / (.25 + .8 + .4)
    //
    _delayInMixerLeft.gain(INMIXER_CHAN_DELAYIN, 1.0);
    _delayInMixerLeft.gain(INMIXER_CHAN_DELAY_FB, 0.0);
    _delayInMixerLeft.gain(INMIXER_CHAN_DELAY_XFB, 0.0);
    _delayLeft.delay(DELAYOUT_CHAN_DELAY_OUT, 1000);

    _delayInMixerRight.gain(INMIXER_CHAN_DELAYIN, 1.0);
    _delayInMixerRight.gain(INMIXER_CHAN_DELAY_FB, 0.0);
    _delayInMixerRight.gain(INMIXER_CHAN_DELAY_XFB, 0.0);
    _delayRight.delay(DELAYOUT_CHAN_DELAY_OUT, 1000);

    setDelayLeftOutGain(0.5);   // app -6dB
    setDelayRightOutGain(0.5);  // app -6dB
}

// Destructor
//
StereoPingPongDelay::~StereoPingPongDelay() {

}

// setLeftInput / setRightInput
//
void StereoPingPongDelay::setLeftInput(AudioStream& srcOutLeft) {

    _inputLeftToMixerLeft.connect(srcOutLeft, 0, _delayInMixerLeft, INMIXER_CHAN_DELAYIN);
}

void StereoPingPongDelay::setRightInput(AudioStream& srcOutRight) {

    _inputRightToMixerRight.connect(srcOutRight, 0, _delayInMixerRight, INMIXER_CHAN_DELAYIN);
}

// getLeftOutput / getRightOutput
//
// Return pointers to final amplifier stage to be
// used for signal chain connections in derived class.
//
AudioStream* StereoPingPongDelay::getLeftOutput() {

    return &_gainLeftAmp;
}

AudioStream* StereoPingPongDelay::getRightOutput() {

    return &_gainRightAmp;
}

// setGain
//
void StereoPingPongDelay::setGain(float gain) {

//    Serial.printf("StereoPingPongDelay::setGain: %f\n", gain);
    _gainLeftAmp.gain(gain);
    _gainRightAmp.gain(gain);
}

// setLeftOutGain
//
void StereoPingPongDelay::setDelayLeftOutGain(float gain) {

//    Serial.printf("StereoPingPongDelay::setDelayLeftOutGain: %f\n", gain);
    _gainLeftAmp.gain(gain);
}

// setDelayLeftOutTime
//
void StereoPingPongDelay::setDelayLeftOutTime(float delayTime) {

//    Serial.printf("StereoPingPongDelay::setDelayLeftOutTime: %f\n", delayTime);
    _delayLeft.delay(DELAYOUT_CHAN_DELAY_OUT, delayTime);
}

// setDelayLeftFbTime
//
void StereoPingPongDelay::setDelayLeftFbTime(float delayTime) {

//    Serial.printf("StereoPingPongDelay::setDelayLeftFbTime: %f\n", delayTime);
    _delayLeft.delay(DELAYOUT_CHAN_DELAY_FB, delayTime);
}

// setDelayLeftFbAmt
//
void StereoPingPongDelay::setDelayLeftFbAmt(float amt) {

//    Serial.printf("StereoPingPongDelay::setDelayLeftFbAmt: %f\n", amt);
    _delayInMixerLeft.gain(INMIXER_CHAN_DELAY_FB, amt);
}

// setDelayLeftxFbTime
//
void StereoPingPongDelay::setDelayLeftxFbTime(float delayTime) {

//    Serial.printf("StereoPingPongDelay::setDelayLeftxFbTime: %f\n", delayTime);
    _delayLeft.delay(DELAYOUT_CHAN_DELAY_XFB, delayTime);
}

// setDelayLeftxFbAmt
//
void StereoPingPongDelay::setDelayLeftxFbAmt(float amt) {

//    Serial.printf("StereoPingPongDelay::setDelayLeftxFbAmt: %f\n", amt);
    _delayInMixerLeft.gain(INMIXER_CHAN_DELAY_XFB, amt);
}



// setRightOutGain
//
void StereoPingPongDelay::setDelayRightOutGain(float gain) {

//    Serial.printf("StereoPingPongDelay::setDelayRightOutGain: %f\n", gain);
    _gainRightAmp.gain(gain);
}

// setDelayRightOutTime
//
void StereoPingPongDelay::setDelayRightOutTime(float delayTime) {

//    Serial.printf("StereoPingPongDelay::setDelayRightOutTime: %f\n", delayTime);
    _delayRight.delay(DELAYOUT_CHAN_DELAY_OUT, delayTime);
}

// setDelayRightFbTime
//
void StereoPingPongDelay::setDelayRightFbTime(float delayTime) {

//    Serial.printf("StereoPingPongDelay::setDelayRightFbTime: %f\n", delayTime);
    _delayRight.delay(DELAYOUT_CHAN_DELAY_FB, delayTime);
}

// setDelayRightFbAmt
//
void StereoPingPongDelay::setDelayRightFbAmt(float amt) {

//    Serial.printf("StereoPingPongDelay::setDelayRightFbAmt: %f\n", amt);
    _delayInMixerRight.gain(INMIXER_CHAN_DELAY_FB, amt);
}

// setDelayRightxFbTime
//
void StereoPingPongDelay::setDelayRightxFbTime(float delayTime) {

//    Serial.printf("StereoPingPongDelay::setDelayRightxFbTime: %f\n", delayTime);
    _delayRight.delay(DELAYOUT_CHAN_DELAY_XFB, delayTime);
}

// setDelayRightxFbAmt
//
void StereoPingPongDelay::setDelayRightxFbAmt(float amt) {

//    Serial.printf("StereoPingPongDelay::setDelayRightxFbAmt: %f\n", amt);
    _delayInMixerRight.gain(INMIXER_CHAN_DELAY_XFB, amt);
}

/************************************************************************************
*                          P R I V A T E   F U N C T I O N S                        *
*************************************************************************************
*/


