/************************************************************************************
*                                   FutureEfx.cpp                                   *
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

#include "FutureEfx.h"

/************************************************************************************
*                         M A C R O S  /  C O N S T A N T S                         *
*************************************************************************************
*/

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
FutureEfx::FutureEfx() {

}

FutureEfx::FutureEfx(AudioStream& srcOutLeft, AudioStream& srcOutRight) {

    setLeftInput(srcOutLeft);
    setLeftInput(srcOutRight);
}

// Destructor
//
FutureEfx::~FutureEfx() {

}

// setLeftInput / setRightInput
//
void FutureEfx::setLeftInput(AudioStream& srcOutLeft) {

}

void FutureEfx::setRightInput(AudioStream& srcOutRight) {

}

// getLeftOutput / getRightOutput
//
// Return pointers to final amplifier stage to be
// used for signal chain connections in derived class.
//
AudioStream* FutureEfx::getLeftOutput() {

    return &_gainLeftAmp;
}

AudioStream* FutureEfx::getRightOutput() {

    return &_gainRightAmp;
}

// setGain
//
void FutureEfx::setGain(float gain) {

//    Serial.printf("FutureEfx::setGain: %f\n", gain);

    _gainLeftAmp.gain(gain);
    _gainRightAmp.gain(gain);
}

// setLeftOutGain
//
void FutureEfx::setLeftOutGain(float gain) {

//    Serial.printf("FutureEfx::setLeftOutGain: %f\n", gain);
    _gainLeftAmp.gain(gain);
}

// setRightOutGain
//
void FutureEfx::setRightOutGain(float gain) {

//    Serial.printf("FutureEfx::setRightOutGain: %f\n", gain);
    _gainRightAmp.gain(gain);
}

// setParam2
//
void FutureEfx::setParam2(float paramVal) {

//    Serial.printf("FutureEfx::setParam2: %f\n", paramVal);
}

// setParam3
//
void FutureEfx::setParam3(float paramVal) {

//    Serial.printf("FutureEfx::setParam3: %f\n", paramVal);
}


// setParam4
//
void FutureEfx::setParam4(float paramVal) {

//    Serial.printf("FutureEfx::setParam4: %f\n", paramVal);
}


/************************************************************************************
*                          P R I V A T E   F U N C T I O N S                        *
*************************************************************************************
*/


