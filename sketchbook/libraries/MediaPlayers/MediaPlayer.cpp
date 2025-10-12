/************************************************************************************
*                                 MediaPlayer.cpp                                   *
*                      Copyright (c) 2025, Donald Swearingen                        *
*                               All Rights Reserved                                 *
*************************************************************************************
*
* Description
*
*   MediaPlayer is the base class for a library of classes that are designed to play the
*   contents of various audio media file types, including Mono and Stereo WAV files,
*   generally located on a microSD card; and raw (headerless) MONO audio files,
*   located either on microSD or in a NOR Flash storage device directly connected
*   to the system's MPU.
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

#include "MediaPlayer.h"

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
MediaPlayer::MediaPlayer() {

    // Connect internal signal chain
    _envelopeLeftToPanLeft.connect(_envelopeLeft, _panLeftAmp);
    _envelopeRightToPanRight.connect(_envelopeRight, _panRightAmp);
    _panLeftToGainLeft.connect(_panLeftAmp, 0, _gainLeftAmp, 0);
    _panRightToGainRight.connect(_panRightAmp, 0, _gainRightAmp, 0);

    // Player attributes
    setEnvelope(0.0, DefaultEnvAttackTime, 0.0, 0.0, DefaultEnvSustainTime, DefaultEnvReleaseTime, DefaultEnvNoteOnReleaseTime);
    setPan(0.5);
    setGain(1.0);
}

// Destructor
//
MediaPlayer::~MediaPlayer() {

}

// getLeftOutput / getRightOutput
//
// Return pointers to final amplifier stage to be
// used for signal chain connections in derived class.
//
AudioStream* MediaPlayer::getLeftOutput() {

    return &_gainLeftAmp;
}

AudioStream* MediaPlayer::getRightOutput() {

    return &_gainRightAmp;
}

// setGain
//
void MediaPlayer::setGain(float gain) {

    _gainLeftAmp.gain(gain);
    _gainRightAmp.gain(gain);
}

// setPan()
//
// Position [0.0,1.0]
// left: 0.0 middle: 0.5 right: 1.0
//
void MediaPlayer::setPan(float position) {

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

// setEnvelope
//
void MediaPlayer::setEnvelope(float delay, float attack, float hold, float decay, float sustain, float release, float releaseNoteOn) {

    _envelopeLeft.delay(delay);
    _envelopeLeft.attack(attack);
    _envelopeLeft.hold(hold);
    _envelopeLeft.decay(decay);
    _envelopeLeft.sustain(sustain);
    _envelopeLeft.release(release);
    _envelopeLeft.releaseNoteOn(releaseNoteOn);

    _envelopeRight.delay(delay);
    _envelopeRight.attack(attack);
    _envelopeRight.hold(hold);
    _envelopeRight.decay(decay);
    _envelopeRight.sustain(sustain);
    _envelopeRight.release(release);
    _envelopeRight.releaseNoteOn(releaseNoteOn);
}

// setDelay
//
void MediaPlayer::setDelay(float delay) {

    _envelopeLeft.delay(delay);
    _envelopeRight.delay(delay);
}

// setAttack
//
void MediaPlayer::setAttack(float attack) {

    _envelopeLeft.attack(attack);
    _envelopeRight.attack(attack);
}

// setHold
//
void MediaPlayer::setHold(float hold) {

    _envelopeLeft.hold(hold);
    _envelopeRight.hold(hold);
}

// setDecay
//
void MediaPlayer::setDecay(float decay) {

    _envelopeLeft.decay(decay);
    _envelopeRight.decay(decay);
}

// setSustain
//
void MediaPlayer::setSustain(float sustain) {

    _envelopeLeft.sustain(sustain);
    _envelopeRight.sustain(sustain);
}

// release
//
void MediaPlayer::setRelease(float release) {

    _envelopeLeft.release(release);
    _envelopeRight.release(release);
}

// releaseNoteOn
//
void MediaPlayer::setReleaseNoteOn(float releaseNoteOn) {

    _envelopeLeft.releaseNoteOn(releaseNoteOn);
    _envelopeRight.releaseNoteOn(releaseNoteOn);
}

// envNoteOn()
//
void MediaPlayer::envNoteOn() {

    _envelopeLeft.noteOn();
    _envelopeRight.noteOn();
}

// envNoteOff()
//
void MediaPlayer::envNoteOff() {

    _envelopeLeft.noteOff();
    _envelopeRight.noteOff();
    while (_envelopeRight.isActive());
}

/************************************************************************************
*                          P R I V A T E   F U N C T I O N S                        *
*************************************************************************************
*/

