/************************************************************************************
*                                AudioEnvPanGain.cpp                                *
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

#include "AudioEnvPanGain.h"

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

// Constructors
//
AudioEnvPanGain::AudioEnvPanGain() {

    // Player attributes
    setEnvelope(0.0, DefaultEnvAttackTime, 0.0, 0.0, DefaultEnvSustainLevel, DefaultEnvReleaseTime, DefaultEnvNoteOnReleaseTime);
    setPan(0.5);
    setGain(1.0);
}

AudioEnvPanGain::AudioEnvPanGain(AudioStream& srcOutLeft, AudioStream& srcOutRight) : _panGain(_envelopeLeft, _envelopeRight){

    // Connect internal signal chain
    _inputToEnvelopeLeft.connect(srcOutLeft, 0, _envelopeLeft, 0);
    _inputToEnvelopeRight.connect(srcOutRight, 0, _envelopeRight, 0);

    // Player attributes
    setEnvelope(0.0, DefaultEnvAttackTime, 0.0, 0.0, DefaultEnvSustainLevel, DefaultEnvReleaseTime, DefaultEnvNoteOnReleaseTime);
    setPan(0.5);
    setGain(1.0);
}

// Destructor
//
AudioEnvPanGain::~AudioEnvPanGain() {

}

// setLeftInput / setRightInput
//
void AudioEnvPanGain::setLeftInput(AudioStream& srcOutLeft) {

    _inputToEnvelopeLeft.connect(srcOutLeft, 0, _envelopeLeft, 0);
}

void AudioEnvPanGain::setRightInput(AudioStream& srcOutRight) {

    _inputToEnvelopeRight.connect(srcOutRight, 0, _envelopeRight, 0);
}

void AudioEnvPanGain::setLeftRightInput(AudioStream& srcOut) {

    _inputToEnvelopeLeft.connect(srcOut, 0, _envelopeLeft, 0);
    _inputToEnvelopeRight.connect(srcOut, 1, _envelopeRight, 0);
}

// getLeftOutput / getRightOutput
//
// Return pointers to final amplifier stage to be
// used for signal chain connections in derived class.
//
AudioStream* AudioEnvPanGain::getLeftOutput() {

    return _panGain.getLeftOutput();
}

AudioStream* AudioEnvPanGain::getRightOutput() {

    return _panGain.getRightOutput();
}

// setEnvelope
//
void AudioEnvPanGain::setEnvelope(float delay, float attack, float hold, float decay, float sustain, float release, float releaseNoteOn) {

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
void AudioEnvPanGain::setDelay(float delay) {

    _envelopeLeft.delay(delay);
    _envelopeRight.delay(delay);
}

// setAttack
//
void AudioEnvPanGain::setAttack(float attack) {

    _envelopeLeft.attack(attack);
    _envelopeRight.attack(attack);
}

// setHold
//
void AudioEnvPanGain::setHold(float hold) {

    _envelopeLeft.hold(hold);
    _envelopeRight.hold(hold);
}

// setDecay
//
void AudioEnvPanGain::setDecay(float decay) {

    _envelopeLeft.decay(decay);
    _envelopeRight.decay(decay);
}

// setSustain
//
void AudioEnvPanGain::setSustain(float sustain) {

    _envelopeLeft.sustain(sustain);
    _envelopeRight.sustain(sustain);
}

// release
//
void AudioEnvPanGain::setRelease(float release) {

    _envelopeLeft.release(release);
    _envelopeRight.release(release);
}

// releaseNoteOn
//
void AudioEnvPanGain::setReleaseNoteOn(float releaseNoteOn) {

    _envelopeLeft.releaseNoteOn(releaseNoteOn);
    _envelopeRight.releaseNoteOn(releaseNoteOn);
}

// releaseNoteOn
//
bool AudioEnvPanGain::envelopeIsActive() {

    return _envelopeLeft.isActive();
//    return (_envelopeLeft.isActive() || _envelopeRight.isActive());
}

// setGain
//
void AudioEnvPanGain::setGain(float gain) {

//    Serial.printf("AudioEnvPanGain::setGain gain %f\n", gain);

    _panGain.setGain(gain);
}

// setPan()
//
// Position [0.0,1.0]
// left: 0.0 middle: 0.5 right: 1.0
//
void AudioEnvPanGain::setPan(float position) {

//    Serial.printf("AudioEnvPanGain::setPan position %f\n", position);

    _panGain.setPan(position);
}

// envNoteOn()
//
void AudioEnvPanGain::envNoteOn() {

    _envelopeLeft.noteOn();
    _envelopeRight.noteOn();
}

// envNoteOff()
//
void AudioEnvPanGain::envNoteOff() {

    _envelopeLeft.noteOff();
    _envelopeRight.noteOff();

    // When envelope is applied at voice level rather than synth level, busy
    // waiting here delays the cutoff of other note offs until the current envelope
    // is finished. The longer the decay time is set, the greater is the
    // delay before all notes are silent, though the note releases occurred
    // at the same moment.
//    while (_envelopeLeft.isActive() || _envelopeRight.isActive());
}

/************************************************************************************
*                          P R I V A T E   F U N C T I O N S                        *
*************************************************************************************
*/



