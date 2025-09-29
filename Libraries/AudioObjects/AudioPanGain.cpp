/************************************************************************************
*                                 AudioPanGain.cpp                                  *
*                      Copyright (c) 2025, Donald Swearingen                        *
*                               All Rights Reserved                                 *
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
*                          P U B L I C   F U N C T I O N S                          *
*************************************************************************************
*/

// Constructor
//
AudioPanGain::AudioPanGain() {

    // Connect internal signal chain
    _panLeftToGainLeft.connect(_panLeftAmp, 0, _gainLeftAmp, 0);
    _panRightToGainRight.connect(_panRightAmp, 0, _gainRightAmp, 0);

    // Player attributes
    setPan(0.5);
    setGain(1.0);
}

AudioPanGain::AudioPanGain(AudioStream& srcOutLeft, AudioStream& srcOutRight) {

    // Connect internal signal chain
    _inputToPanLeft.connect(srcOutLeft, 0, _panLeftAmp, 0);
    _inputToPanRight.connect(srcOutRight, 0, _panRightAmp, 0);
    _panLeftToGainLeft.connect(_panLeftAmp, 0, _gainLeftAmp, 0);
    _panRightToGainRight.connect(_panRightAmp, 0, _gainRightAmp, 0);

    // Player attributes
    setPan(0.5);
    setGain(1.0);
}

// Destructor
//
AudioPanGain::~AudioPanGain() {

}

// setLeftInput / setRightInput
//
void AudioPanGain::setLeftInput(AudioStream& srcOutLeft) {

    _inputToPanLeft.connect(srcOutLeft, 0, _panLeftAmp, 0);
}

void AudioPanGain::setRightInput(AudioStream& srcOutRight) {

    _inputToPanRight.connect(srcOutRight, 0, _panRightAmp, 0);
}

// getLeftOutput / getRightOutput
//
// Return pointers to final amplifier stage to be
// used for signal chain connections in derived class.
//
AudioStream* AudioPanGain::getLeftOutput() {

    return &_gainLeftAmp;
}

AudioStream* AudioPanGain::getRightOutput() {

    return &_gainRightAmp;
}

// setGain
//
void AudioPanGain::setGain(float gain) {

    _gainLeftAmp.gain(gain);
    _gainRightAmp.gain(gain);
}

// setPan()
//
// Position [0.0,1.0]
// left: 0.0 middle: 0.5 right: 1.0
//
void AudioPanGain::setPan(float position) {

    if (position > 1.0) {
        position = 1.0;
    } else if (position < 0.0) {
        position = 0.0;
    }

#if PAN_USE_LINEAR_XFADE // speaker-to-speaker crossfade
    _panLeftAmp.gain(1.0 - position);
    _panRightAmp.gain(position);
#elif PAN_USE_SPKR_TO_SPKR_XFADE
    float angle = (PI / 2.0) * position;
    float leftGain = cos(angle);
    float rightGain = cos(PI_DIV_2 - angle);
    _panLeftAmp.gain(leftGain);
    _panRightAmp.gain(rightGain);
#endif // USE_LINEAR_XFADE / USE_SPKR_TO_SPKR_XFADE
}

/************************************************************************************
*                          P R I V A T E   F U N C T I O N S                        *
*************************************************************************************
*/


