/************************************************************************************
*                              BasicSynthVoice.cpp                                 *
*                                                                                   *
*                      Copyright (c) 2025, Donald Swearingen                        *
*                               All Rights Reserved                                 *
*************************************************************************************
*
* Description
*
* Copyright Notices
*
*   This version of BasicSynthVoice is based on the original by Ghost In Translation
*   posted on Git at https://github.com/ghostintranslation/BasicSynth. Both versions
*   are issued under the MIT License conditions described below:
*
*   Copyright (c) 2025, Donald Swearingen
*   Copyright (c) 2019, Ghost In Translation
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
* Modifications List
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

// Class Definitions
//
#include "BasicSynthVoice.h"

// User Arduino Libraries
//
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
*                          P U B L I C   F U N C T I O N S                          *
*************************************************************************************
*/

// Constructor
//
BasicSynthVoice::BasicSynthVoice() : envPanGain(outMixer, outMixer) {

    // AM and Ring
    sineAM.amplitude(1);
    dcAM.amplitude(0.5);
    sawtoothAM.begin(WAVEFORM_SAWTOOTH);
    sawtoothAM.amplitude(1);
    mixerAM.gain(0, 0.5); // sineAM TODO Link to a potentiometer
    mixerAM.gain(1, 0.5); // swatoothAM TODO Link to a potentiometer
    mixerAM2.gain(0, 0);
    mixerAM2.gain(1, 1);
    mixerDcMod.gain(0, 0);
    mixerDcMod.gain(1, 1);

    // FM
    sineFM.amplitude(1);
    sawtoothFM.begin(WAVEFORM_SAWTOOTH);
    sawtoothFM.amplitude(1);
    mixerFM.gain(0, 1.0); // TODO Link to a potentiometer
    mixerFM.gain(1, 0.05); // TODO Link to a potentiometer

    // Common
    outMixer.gain(0, 1);
    envPanGain.setSustain(1.0);
    envPanGain.setGain(1.0);

    // Patching
    sineAmToMixerAm.connect(sineAM, 0,              mixerAM, 0);
    sawtoothAToMixerAm.connect(sawtoothAM, 0,       mixerAM, 1);
    dcAmToDcMod.connect(dcAM, 0,                    mixerDcMod, 0);
    sineModtoMixerDcMod.connect(sineModulator, 0,   mixerDcMod, 1);
    sineModToSineFm.connect(sineModulator, 0,       sineFM, 0);
    sineModToSawtoothFM.connect(sineModulator, 0,   sawtoothFM, 0);
    mixerAmToMixerAm2.connect(mixerAM, 0,           mixerAM2, 0);
    mixerAmToMultiplyAm.connect(mixerAM, 0,         multiplyAM, 0);
    mixerDcModToMultiplyAm.connect(mixerDcMod, 0,   multiplyAM, 1);
    sineFmToMixerFm.connect(sineFM, 0,              mixerFM, 0);
    samwoothFmToMixerFm.connect(sawtoothFM, 0,      mixerFM, 1);
    multiplyAmToMixerAm2.connect(multiplyAM, 0,     mixerAM2, 1);
    mixerAm2ToOutMixer.connect(mixerAM2, 0,         outMixer, 0);
    mixerFmToOutMixer.connect(mixerFM, 0,           outMixer, 1);

    // Initial parameters
    setSynthType(FM);
    setEnvSustain(1.0);
    notePlayed = false;
}

// getLeftOutput / getRightOutput
//
AudioStream* BasicSynthVoice::getLeftOutput() {

    return envPanGain.getLeftOutput();
}

AudioStream* BasicSynthVoice::getRightOutput() {

    return envPanGain.getRightOutput();
}

// setSynthType()
//
// Set the synthType
//
void BasicSynthVoice::setSynthType(SynthType type){

    if (synthType != type) {
//        Serial.printf("BasicSynthVoice::setSynthType: Changing to synthType %d\n", synthType);
        synthType = type;

        // Init the mixers gain to 0
        outMixer.gain(0, 0);
        outMixer.gain(1, 0);
        mixerDcMod.gain(0, 0);

        // Set the values according to the synthType
        switch (synthType){

            case FM:
            case FMx10:
                outMixer.gain(1, 1);
                break;

            case AM:
            case AMx10:
                mixerDcMod.gain(0, 1);
                outMixer.gain(0, 1);
                break;

            case RING:
                outMixer.gain(0, 1);
                break;
        }
    }
}

// noteOn()
//
void BasicSynthVoice::noteOn(byte midiNote) {

//    Serial.printf("BasicSynthVoice::noteOn1 note %d\n", midiNote);

    float freq = 440.0 * powf(2.0, (float)(midiNote - 69) * 0.08333333);
    sawtoothAM.frequency(freq);
    sineAM.frequency(freq);
    sawtoothFM.frequency(freq);
    sineFM.frequency(freq);

    envPanGain.envNoteOn();
    currentNote = midiNote;
//    last_played = millis();
    notePlayed = true;
}

void BasicSynthVoice::noteOn(uint8_t note, uint8_t midiVelocity, uint8_t midiPan) {

#if 0   // constant velocity for tests
    midiVelocity = 90;
#endif  // constant velocity for tests

//    Serial.printf("BasicSynthVoice::noteOn note %d veloc %d pan %d\n", note, midiVelocity, midiPan);

    envPanGain.setPan(UtilFns::scale(midiPan, 0, 127, 0.0, 1.0));
    envPanGain.setGain(UtilFns::scale(midiVelocity, 0, 127, 0.03125, 1.0));

    noteOn(note);
}

// setPanPosition()
//
void BasicSynthVoice::setPanPosition(uint8_t position) {

//    Serial.printf("BasicSynthVoice::setPanPosition pan %d\n", position);

    envPanGain.setPan(UtilFns::scale(position, 0, 127, 0.0, 1.0));
}

// noteOff()
//
void BasicSynthVoice::noteOff() {

//    Serial.printf("BasicSynthVoice::noteOff \n");

    envPanGain.envNoteOff();
}

// isActive()
//
// Is the voice active
//
bool BasicSynthVoice::isActive(){

    return envPanGain.envelopeIsActive();
}

// isNotePlayed()
//
bool BasicSynthVoice::isNotePlayed(){

    return notePlayed;
}

// setNotePlayed()
//
void BasicSynthVoice::setNotePlayed(bool played){

    notePlayed = played;
}

// setFrequency()
//
// Set the base frequency
//
void BasicSynthVoice::setFrequency(int freq){

    sineAM.frequency(freq);
    sawtoothAM.frequency(freq);
    sineFM.frequency(freq);
    sawtoothFM.frequency(freq);
}

// setModulatorFrequency()
//
// Set the modulator frequency
//
// TODO: argument should be float
//
void BasicSynthVoice::setModulatorFrequency(int freq){

//    Serial.printf("BasicSynthVoice::setModulatorFrequency: %d\n", freq);

    switch (synthType){

        case FM:
        case AM:
            sineModulator.frequency(freq);
            break;

        case FMx10:
        case AMx10:
        case RING:
            sineModulator.frequency(freq*10);
            break;
    }
}
// setModulatorAmplitude()
//
// Set the modulator amplitude
//
void BasicSynthVoice::setModulatorAmplitude(float amp){

//    Serial.printf("BasicSynthVoice::setModulatorAmplitude: %f\n", amp);

    // In AM sineModulator amplitude should be fixed at 0.5 and dcAM too
    switch (synthType){

        case AM:
        case AMx10:
            sineModulator.amplitude(0.5);
            break;

        default:
            sineModulator.amplitude(amp);
            break;
    }

    // Turns out that the original version assumed a 12-bit converter and then divided inputs by 4096:
    //
    //    // Modulator amplitude
    //    // TODO This should send a standardized value from 0 to 1023
    //    float modulatorAmplitude = (float)analogRead(this->modulatorAmplitudePin)/(float)4096;
    //    if(this->modulatorAmplitude != modulatorAmplitude){
    //      this->modulatorAmplitude = modulatorAmplitude;
    //      for (int i = 0; i < voiceCount ; i++) {
    //        this->voices[i]->setModulatorAmplitude(modulatorAmplitude);
    //      }
    //    }
    //
    // So, value sent to setModulatorAmplitude() was in range [0,1]
    //
    // Changing this back now would affect existing presets; but would then be correct.
    //
#if 1 // Correct range
    mixerAM2.gain(0, 1 - amp);
    mixerAM2.gain(1, amp);
#else // Old; incorrect range
    mixerAM2.gain(0, 1 - amp*4);
    mixerAM2.gain(1, amp*4);
#endif
}

// setADSR()
//
// Set Envelope attack, decay, sustain, release
//
void BasicSynthVoice::setADSR(unsigned int attack, unsigned int decay, float sustain, unsigned int release){

//    Serial.printf("BasicSynthVoice::setADSR attack %d decay %d sustain %f release %d\n", attack, decay, sustain, release);

    envPanGain.setAttack(attack);
    envPanGain.setDecay(decay);
    envPanGain.setSustain(sustain);
    envPanGain.setRelease(release);
}

// setEnvAttack()
//
// Set envelope attack time [0.0,3000.0] ms
//
void BasicSynthVoice::setEnvAttack(int attackValue){

    envPanGain.setAttack(attackValue);
}

// setEnvDecay()
//
// Set envelope decay time [0.0,3000.0] ms
//
void BasicSynthVoice::setEnvDecay(int decayValue){

    envPanGain.setDecay(decayValue);
}

// setEnvSustain()
//
// Set envelope sustain level [0.0,1.0]
//
void BasicSynthVoice::setEnvSustain(float sustainLevel){

    envPanGain.setSustain(sustainLevel);
}

// setEnvRelease()
//
// Set envelope release time [0.0,3000.0] ms
//
void BasicSynthVoice::setEnvRelease(int releaseValue){

    envPanGain.setRelease(releaseValue);
}

/************************************************************************************
*                        P R O T E C T E D   F U N C T I O N S                      *
*************************************************************************************
*/

/************************************************************************************
*                          P R I V A T E   F U N C T I O N S                        *
*************************************************************************************
*/

