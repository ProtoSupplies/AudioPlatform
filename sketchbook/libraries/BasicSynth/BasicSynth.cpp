/************************************************************************************
*                                  BasicSynth.cpp                                   *
*                                                                                   *
*                      Copyright (c) 2025, Donald Swearingen                        *
*                               All Rights Reserved                                 *
*************************************************************************************
*
* Description
*
* Copyright Notices
*
*   This version of BasicSynth is based on the original by Ghost In Translation posted
*   on Git at https://github.com/ghostintranslation/BasicSynth. Both versions are issued
*   under the MIT License conditions described below:
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
* Modifications List (TODO)
*
*   • Implement MIDI channel
*   • Implement Pan control
*   • Define MIDI control for mixerAM and mixerFM gains, other params (using faders 3-8 on MiniLab3_
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
#include "BasicSynth.h"

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
// Dummy argument required (see section "Constructor Variants Issue" in BasicSynth.h).
//
BasicSynth::BasicSynth() : outChain(outAmpLeft, outAmpRight), voiceMgr(VoiceCount) {

    // Global voice settings
    synthType = FM;
    attack = 50;
    decay = 50;
    sustain = 1.0;
    release = 50;

    // Out Mixer and Amp
    outAmpLeft.gain(1.0);
    outAmpRight.gain(1.0);

    // Out Mixer to Out Amp
    outMixerLeftToOutAmpLeft = new AudioConnection(outMixerLeft, 0, outAmpLeft, 0);
    outMixerRightToOutAmpRight = new AudioConnection(outMixerRight, 0, outAmpRight, 0);

    // Keep gain proportional to count to prevent distortion
    float voiceGain = 4.0 / (float)VoiceCount;
//    float voiceGain = 2.0 * (1.0 / (float)VoiceCount);

    // Create 1 mixer for every VoicesPerMixerCount voices
    for (int i = 0; i < VoiceCount / VoicesPerMixerCount; i++) {

        voiceLeftMixers[i] = new AudioMixer4();

        voiceLeftMixers[i]->gain(0, voiceGain);
        voiceLeftMixers[i]->gain(1, voiceGain);
        voiceLeftMixers[i]->gain(2, voiceGain);
        voiceLeftMixers[i]->gain(3, voiceGain);

        voiceRightMixers[i] = new AudioMixer4();

        voiceRightMixers[i]->gain(0, voiceGain);
        voiceRightMixers[i]->gain(1, voiceGain);
        voiceRightMixers[i]->gain(2, voiceGain);
        voiceRightMixers[i]->gain(3, voiceGain);

        // Voice Mixer to out Mixer
        voiceMixerToOutMixerLeft[i] = new AudioConnection(*voiceLeftMixers[i], 0, outMixerLeft, i % VoicesPerMixerCount);
        voiceMixerToOutMixerRight[i] = new AudioConnection(*voiceRightMixers[i], 0, outMixerRight, i % VoicesPerMixerCount);
    }

    // Create Voices
    for (int i = 0; i < VoiceCount; i++) {

        voices[i] = new BasicSynthVoice();
        voices[i]->setADSR(attack, decay, sustain, release);

        // Voice to Voice Mixer (VoicesPerMixerCount per mixer)
        voiceToVoiceMixerLeft[i] = new AudioConnection(*voices[i]->getLeftOutput(), 0, *voiceLeftMixers[i / VoicesPerMixerCount], i % VoicesPerMixerCount);
        voiceToVoiceMixerRight[i] = new AudioConnection(*voices[i]->getRightOutput(), 0, *voiceRightMixers[i / VoicesPerMixerCount], i % VoicesPerMixerCount);

        // Voice initial held state
        voiceHeldState[i] = VoiceNotHeld;
    }

    sustainPedalPressed = false;
    playerEnabled = true;

    noteShift       = 0;
    noteRangeStart  = MidiNoteMin;
    noteRangeEnd    = MidiNoteMax;
}

// updateControls()
//
void BasicSynth::updateControls() {

}

// getLeftOutput()
//
// Return the left audio output
//
AudioStream* BasicSynth::getLeftOutput() {

    return outChain.getLeftOutput();
}

// getRightOutput()
//
// Return the right audio output
//
AudioStream* BasicSynth::getRightOutput() {

    return outChain.getRightOutput();
}

// setNoteShift()
//
void BasicSynth::setNoteShift(int _noteShift) {

//    Serial.printf("BasicSynth::setNoteShift: %d\n", _noteShift);

    // FFS: Quick fix to avoid hanging notes. Need a better approach that would preserve held notes.
    if ((_noteShift != noteShift) && (voiceMgr.getActiveVoiceCount() != 0)) {
        allNotesOff();
    }
    noteShift = _noteShift;
}

// setNoteRangeStart()
//
void BasicSynth::setNoteRangeStart(MidiNoteNumber _noteRangeStart) {

//    Serial.printf("BasicSynth::setNoteRangeStart: %d\n", _noteRangeStart);

    // FFS: Quick fix to avoid hanging notes. Need a better approach that would preserve held notes.
    if ((_noteRangeStart != noteRangeStart) && (voiceMgr.getActiveVoiceCount() != 0)) {
        allNotesOff();
    }
    noteRangeStart = _noteRangeStart;
    if (noteRangeStart > noteRangeEnd) {
        MidiNoteNumber note = noteRangeStart;
        noteRangeStart = noteRangeEnd;
        noteRangeEnd = note;
    }
}

// setNoteRangeEnd()
//
void BasicSynth::setNoteRangeEnd(MidiNoteNumber _noteRangeEnd) {

//    Serial.printf("BasicSynth::setNoteRangeEnd: %d\n", _noteRangeEnd);

    // FFS: Quick fix to avoid hanging notes. Need a better approach that would preserve held notes.
    if ((_noteRangeEnd != noteRangeEnd) && (voiceMgr.getActiveVoiceCount() != 0)) {
        allNotesOff();
    }
    noteRangeEnd = _noteRangeEnd;
    if (noteRangeStart > noteRangeEnd) {
        MidiNoteNumber note = noteRangeStart;
        noteRangeStart = noteRangeEnd;
        noteRangeEnd = note;
    }
}

// noteOn()
//
void BasicSynth::noteOn(uint8_t note, uint8_t midiVelocity) {

//    Serial.printf("BasicSynth::noteOn note %d velocity %d\n", note, midiVelocity);

    if (playerEnabled) {
        int shiftedNote = note + noteShift;

        if (shiftedNote >= noteRangeStart && shiftedNote <= noteRangeEnd) {

            // Set note's pan position
            setNotePosition();

            VoiceNumber voiceNum = voiceMgr.getVoiceUsingNote(shiftedNote);

            // No voice is assigned to shifted note
            if (voiceNum == voiceMgr.VoiceNone) {
                voiceNum = voiceMgr.getFreeVoice();
                voiceMgr.setVoiceActive(true, voiceNum, shiftedNote);
                voices[voiceNum]->noteOn(shiftedNote, midiVelocity, panPosition);
                voices[voiceNum]->currentNote = shiftedNote;
                voiceHeldState[voiceNum] = VoiceHeld;

            // A voice is assigned to shifted note
            } else {

                VoiceHeldState voiceState = voiceHeldState[voiceNum];

                // Trigger new notes
                if (voiceState == VoiceNotHeld) {
                    VoiceNumber voiceNum = voiceMgr.getFreeVoice();
                    voiceMgr.setVoiceActive(true, voiceNum, shiftedNote);
                    voices[voiceNum]->noteOn(shiftedNote, midiVelocity, panPosition);
                    voices[voiceNum]->currentNote = shiftedNote;
                    voiceHeldState[voiceNum] = VoiceHeld;

                // Retrigger notes released, but held by pedal (VoiceHeldPending) or ignorenoteoff (VoiceHeld)
                } else if (voiceState == VoiceHeldPending || voiceState == VoiceHeld) {
                    voiceHeldState[voiceNum] = VoiceNotHeld;
                    voices[voiceNum]->noteOff();
                    voiceMgr.setVoiceActive(false, voiceNum, shiftedNote);
                    noteOn(shiftedNote, midiVelocity);     // safely recursive
                }
            }
        }
    }
}

// noteOff()
//
void BasicSynth::noteOff(byte note) {

//    Serial.printf("BasicSynth::noteOff note %d on voice %d voiceHeldState %d\n", note, voiceNum, voiceHeldState[voiceNum]);

    int shiftedNote = note + noteShift;
    if (shiftedNote >= noteRangeStart && shiftedNote <= noteRangeEnd) {
        VoiceNumber voiceNum = voiceMgr.getVoiceUsingNote(shiftedNote);

        if (voiceNum < VoiceCount) {

            if (voices[voiceNum]->currentNote == shiftedNote) {
                if (sustainPedalPressed) {
                    if (voiceHeldState[voiceNum] == VoiceHeld) {
                        voiceHeldState[voiceNum] = VoiceHeldPending;
                    }

                } else if (voiceHeldState[voiceNum] != VoiceNotHeld){
                    voiceHeldState[voiceNum] = VoiceNotHeld;
                    voices[voiceNum]->noteOff();
                    voiceMgr.setVoiceActive(false, voiceNum, shiftedNote);
                }
            }
        }
    }
}

// setPanType()
//
void BasicSynth::setPanType(MidiPanType type, uint8_t param) {

//    Serial.printf("BasicSynth::setPanType: type %d param %d\n", type, param);

    panType = type;
    panParam = param;

    switch (panType) {

        case PanTypeCenter:
            // For all voices at note time
            panParam = PanParamDefault;
            break;

        case PanTypeFixed:
            // For all voices at note time
            panParam = param;
            break;

        case PanTypeHold:
            // Per-Voice Parameter passed to note
            panParam = panPosition;
            break;

        case PanTypeFollow:
            // Use current position to start, then follow setting on updates
            // Continuous for all voices
            panParam = panPosition;
            break;

        case PanTypeRandom:
            // Per-Voice Parameter passed to note
            break;

        case PanTypeAuto:
            // TODO: Generate position from wave function TBD
            break;

        default:
            break;
    }
}

// setPanPosition()
//
void BasicSynth::setPanPosition(uint8_t position) {

//    Serial.printf("BasicSynth::setPanPosition: %d\n", position);

    panPosition = position;

    for (uint8_t voiceNum = 0; voiceNum < VoiceCount; voiceNum++) {
        if (panType == PanTypeFollow) {
            voices[voiceNum]->setPanPosition(panPosition);
        }
    }
}

// allNotesOff()
//
// Stop all the voices
//
void BasicSynth::allNotesOff() {

//    Serial.printf("BasicSynth::stop\n");

    for (int i = 0; i < VoiceCount; i++) {
        voices[i]->noteOff();
        voiceHeldState[i] = VoiceNotHeld;
        voiceMgr.setVoiceActive(false, i, 0);
    }
}

// sustainPedal()
//
void BasicSynth::sustainPedal(bool pressed) {

//    Serial.printf("BasicSynth::sustainPedal: %d\n", pressed);

    sustainPedalPressed = pressed;

    if (!sustainPedalPressed) {
        for (int i = 0; i < VoiceCount; i++) {
            if (voiceHeldState[i] == VoiceHeldPending) {
                MidiNoteNumber note = voiceMgr.getVoiceActiveNote(i);
                if (note != MidiNoteNone) {
                    noteOff(note - noteShift);
                }
            }
        }
    }
}

// setEnabled()
//
void BasicSynth::setEnabled(bool enabled) {

//    Serial.printf("BasicSynth::setEnabled %d\n", enabled);

    playerEnabled = enabled;

//    if (!playerEnabled) {
//        allNotesOff();
//    }
}

// setGain()
//
void BasicSynth::setGain(float gainValue) {

//    Serial.printf("BasicSynth::setGain %f\n", gainValue);

    outAmpLeft.gain(gainValue);
    outAmpRight.gain(gainValue);
}

// setSynthType()
//
void BasicSynth::setSynthType(SynthType type) {

    if (synthType != type) {
//        Serial.printf("Setting synth type %d\n", type);
        synthType = type;
        for (int i = 0; i < VoiceCount ; i++) {
            voices[i]->setSynthType(type);
        }
    }
}

// setAttack()
//
void BasicSynth::setAttack(float attackValue) {

//    Serial.printf("BasicSynth::setAttack %f\n", attack);
    attack = attackValue;
    for (int i = 0; i < VoiceCount ; i++) {
        voices[i]->setEnvAttack(attack);
    }
}

// setDecay()
//
void BasicSynth::setDecay(float decayValue) {

//    Serial.printf("BasicSynth::setDecay %f\n", decay);
    decay = decayValue;
    for (int i = 0; i < VoiceCount ; i++) {
        voices[i]->setEnvDecay(decay);
    }
}

// setSustain()
//
void BasicSynth::setSustain(float sustainLevel) {

//    Serial.printf("BasicSynth::setSustain %f\n", sustainLevel);
    sustain = sustainLevel;
    for (int i = 0; i < VoiceCount ; i++) {
        voices[i]->setEnvSustain(sustain);
    }
}

// setRelease()
//
void BasicSynth::setRelease(float releaseValue) {

//    Serial.printf("BasicSynth::setRelease %f\n", release);
    release = releaseValue;
    for (int i = 0; i < VoiceCount ; i++) {
        voices[i]->setEnvRelease(release);
    }
}

// setModulatorFrequency()
//
void BasicSynth::setModulatorFrequency(float freqHz) {

//    Serial.printf("BasicSynth::setModulatorFrequency %f\n", freqHz);
    modFreq = freqHz;
    for (int i = 0; i < VoiceCount ; i++) {
        voices[i]->setModulatorFrequency((int)freqHz);
    }
}

// setModulatorAmplitude()
//
// FFS: Called with MIDI control value [0,127] and from Synth screen
// with value [0,127].
//
void BasicSynth::setModulatorAmplitude(float amplitude) {

//    Serial.printf("BasicSynth::setModulatorAmplitude %f\n", amplitude);
    modAmt = amplitude;
    for (int i = 0; i < VoiceCount ; i++) {
        voices[i]->setModulatorAmplitude(amplitude);
    }
}

/************************************************************************************
*                        P R O T E C T E D   F U N C T I O N S                      *
*************************************************************************************
*/

/************************************************************************************
*                          P R I V A T E   F U N C T I O N S                        *
*************************************************************************************
*/

// setNotePosition()
//
void BasicSynth::setNotePosition() {

    switch (panType) {
        case PanTypeCenter:
            panPosition = 63;
            break;

        case PanTypeFixed:
            panPosition = panParam;
            break;

        case PanTypeHold:
            break;

        case PanTypeFollow:
            break;

        case PanTypeRandom:
            panPosition = UtilFns::getRand(PanParamMin, PanParamMax);
            break;

        case PanTypeAuto:
            // Generate position from wave function TBD
            break;

        default:
            break;
    }
}
