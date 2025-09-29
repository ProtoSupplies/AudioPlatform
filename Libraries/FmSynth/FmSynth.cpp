/************************************************************************************
*                                   FmSynth.cpp                                     *
*                                                                                   *
*                      Copyright (c) 2025, Donald Swearingen                        *
*                               All Rights Reserved                                 *
*************************************************************************************
*
* Description
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

// Class Definitions
//
#include "FmSynth.h"

// User Arduino Libraries
//
#include "ArduinoEclipseDefs.h"

/************************************************************************************
*                         M A C R O S  /  C O N S T A N T S                         *
*************************************************************************************
*/

// FFS: All below duplicate (incompletely) values in AppData/FmSynthPresetConst.h.
// Need to find a way to factor to a common place.
const uint16_t  VoicePresetBankedLength     = 128;

const uint8_t   VoicePresetsBankCount       = 16;
const uint8_t   VoicePresetsPerBank         = 32;
const uint16_t  VoicePresetCount            = (VoicePresetsBankCount * VoicePresetsPerBank);

const uint16_t  VoicePresetNameLength       = 10;
const uint16_t  VoicePresetBankNameLength   = 16;

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
// Dummy argument required (see section "Constructor Variants Issue" in FmSynth.h).
//
FmSynth::FmSynth(const uint8_t _dexedVoiceCount, const uint16_t _dexedSampleRate) :
    dexSynth(_dexedVoiceCount, _dexedSampleRate), maxVoices(_dexedVoiceCount),
    audioPanGain(dexSynth, dexSynth) {

    // Global voice settings
    attack = 50;
    decay = 50;
    sustain = 1.0;
    release = 50;

    audioPanGain.setGain(1.0);
    audioPanGain.setPan(PanParamDefault);
    panType = PanTypeDefault;
    panPosition = PanParamDefault;
    panPositionPrev = panPosition;
    setPanType(PanTypeDefault, PanParamDefault);
    holdSet = false;
    randomSet = false;

    dexSynth.setPitchbendRange(1);

    presetBanks = 0;

    sustainPedalPressed = false;
    playerEnabled = true;

    noteShift       = 0;
    noteRangeStart  = MidiNoteMin;
    noteRangeEnd    = MidiNoteMax;
    heldNoteCount   = 0;
}

// update()
//
// Perform "update" on regular intervals (called from polling loop)
// (Not called in rev 0.5.3 2/8/35)
//
void FmSynth::update() {

}

// setEnabled()
//
void FmSynth::setEnabled(bool enabled) {

    playerEnabled = enabled;

//    if (!playerEnabled) {
//        allNotesOff();
//    }
}

// setPrestBanksPtr()
//
void FmSynth::setPrestBanksPtr(uint8_t* banksAddress) {

//    Serial.printf("FmSynth::setPrestBanksPtr: banksAddress %x\n", banksAddress);

    presetBanks = banksAddress;
    selectVoicePreset(1);
}

// getVoicePresetName()
//
const char* FmSynth::getVoicePresetName(uint16_t voicePresetNum) {

//    Serial.printf("FmSynth::getVoicePresetName: voicePresetNum %d\n", voicePresetNum);

    char voiceName[VoicePresetNameLength + 1];
    char *voiceNamePtr = 0;

    if (presetBanks) {
        voiceNamePtr = (char *)&presetBanks[voicePresetNum * VoicePresetBankedLength + 118];
        strncpy(voiceName, voiceNamePtr, VoicePresetNameLength);
        voiceName[VoicePresetNameLength] = '\0';
    } else {
//        Serial.printf("FmSynth::getVoicePresetName: External Banks Undefined\n");
    }

//    Serial.printf("FmSynth::getVoicePresetName: voiceName %s\n", voiceName);

    return voiceNamePtr;
}

// selectVoicePreset()
//
void FmSynth::selectVoicePreset(uint16_t voicePresetNum) {

//    Serial.printf("FmSynth::selectVoicePreset: %d\n", voicePresetNum);

    voicePresetNum--;       // convert to 0-based index

    if (voicePresetNum >= 0 && voicePresetNum < VoicePresetCount) {
        if (voicePresetNum != currentVoicePresetNum) {
            dexSynth.notesOff();
            heldNoteCount = 0;
            currentVoicePresetNum = voicePresetNum;
            loadVoice(currentVoicePresetNum);
        }
    }
}

// getLeftOutput()
//
// Return the left audio output
//
AudioStream* FmSynth::getLeftOutput() {

    return audioPanGain.getLeftOutput();
}

// getRightOutput()
//
// Return the right audio output
//
AudioStream* FmSynth::getRightOutput() {

    return audioPanGain.getRightOutput();
}

// setNoteShift()
//
void FmSynth::setNoteShift(int _noteShift) {

//    Serial.printf("FmSynth::setNoteShift: %d\n", _noteShift);

    // FFS: Quick fix to avoid hanging notes. Need a better approach that would preserve held notes.
    if ((_noteShift != noteShift) && (dexSynth.getNumNotesPlaying() != 0)) {
        allNotesOff();
    }
    noteShift = _noteShift;
}

// setNoteRangeStart()
//
void FmSynth::setNoteRangeStart(MidiNoteNumber _noteRangeStart) {

//    Serial.printf("FmSynth::setNoteRangeStart: %d\n", _noteRangeStart);

    // FFS: Quick fix to avoid hanging notes. Need a better approach that would preserve held notes.
    if ((_noteRangeStart != noteRangeStart) && (dexSynth.getNumNotesPlaying() != 0)) {
        allNotesOff();
    }
    noteRangeStart = _noteRangeStart;
    if (noteRangeStart > noteRangeEnd) {
        MidiNoteNumber note = noteRangeStart;
        noteRangeStart = noteRangeEnd;
        noteRangeEnd = note;
//        Serial.printf("FmSynth::setNoteRangeStart: switched to noteRangeStart %d noteRangeEnd %d\n", noteRangeStart, noteRangeEnd);
    }
}

// setNoteRangeEnd()
//
void FmSynth::setNoteRangeEnd(MidiNoteNumber _noteRangeEnd) {

//    Serial.printf("FmSynth::setNoteRangeEnd: %d\n", _noteRangeEnd);

    // FFS: Quick fix to avoid hanging notes. Need a better approach that would preserve held notes.
    if ((_noteRangeEnd != noteRangeEnd) && (dexSynth.getNumNotesPlaying() != 0)) {
        allNotesOff();
    }
    noteRangeEnd = _noteRangeEnd;
    if (noteRangeStart > noteRangeEnd) {
        MidiNoteNumber note = noteRangeStart;
        noteRangeStart = noteRangeEnd;
        noteRangeEnd = note;
//        Serial.printf("FmSynth::setNoteRangeEnd: switched to noteRangeStart %d noteRangeEnd %d\n", noteRangeStart, noteRangeEnd);
    }
}

// noteOn()
//
void FmSynth::noteOn(uint8_t note, uint8_t midiVelocity) {

//    Serial.printf("FmSynth::noteOn panPosition: %d\n", panPosition);
    if (playerEnabled) {

        // Ensure shifted note is in current note range
        int shiftedNote = note + noteShift;
        if (shiftedNote >= noteRangeStart && shiftedNote <= noteRangeEnd) {

            // Handle pan assignment based on current pan type and position
            bool setPan = false;    // Set pan for note if true

            switch (panType) {

                case PanTypeHold:
                    // If pan hold position is not set, set hold position.
                    // Hold position will remain in effect until all held
                    // notes are released.
                    if (!holdSet) {
                        holdSet = true;
                        setPan = true;
                    }
                    break;

                case PanTypeRandom:
                    // If random pan position is not set, set new position.
                    // Random position will remain in effect until all held
                    // notes are released.
                    if (!randomSet) {
                        panPosition = UtilFns::getRand(PanParamMin, PanParamMax);
                        randomSet = true;
                        setPan = true;
                    }
                    break;

                case PanTypeFollow:
                    // Set note position to current value. Position will of all
                    // notes will track current value of pan controller until
                    // all held notes are released.
                    setPan = true;
                    break;

                case PanTypeAuto:
                    // Generate position from wave function TBD
                    break;

                default:
                    break;
            }

            // Set note's pan position if indicated
            if (setPan) {
//                Serial.printf("FmSynth::noteOn setting pan to: %d\n", panPosition);
                audioPanGain.setPan(UtilFns::scale((float)panPosition, (float)PanParamMin, PanParamMax, 0.0, 1.0));
                panPositionPrev = panPosition;
            }

            // Play the note
            dexSynth.keydown(shiftedNote, midiVelocity);

            // Keep track of note count in order to determine
            // when all held notes have been released (in noteOff())
            heldNoteCount++;
            if (heldNoteCount > maxVoices) {
                heldNoteCount = maxVoices;
            }

//            Serial.printf("FmSynth::noteOn heldNoteCount %d\n", heldNoteCount);
        }
    }
}

// noteOff()
//
void FmSynth::noteOff(byte note) {

    // Ensure shifted note is in current note range
    int shiftedNote = note + noteShift;
    if (shiftedNote >= noteRangeStart && shiftedNote <= noteRangeEnd) {

        // Release the note
        dexSynth.keyup(shiftedNote);

        // Adjust count of held notes
        if (heldNoteCount > 0) {
            heldNoteCount--;
        }
//        Serial.printf("FmSynth::noteOff heldNoteCount %d\n", heldNoteCount);

        // Reset hold and random flags when all held notes are released
        if (heldNoteCount == 0) {
//            Serial.printf("FmSynth::noteOff setting holdSet false\n");
            holdSet = false;
            randomSet = false;
        }
    }
}

// sustainPedal()
//
void FmSynth::sustainPedal(bool pressed) {

//    Serial.printf("FmSynth::sustainPedal: %d\n", pressed);

    dexSynth.setSustain(pressed);
}

// pitchBend()
//
void FmSynth::pitchBend(int16_t bend) {

//    Serial.printf("FmSynth::pitchBend: %d\n", bend);

    dexSynth.setPitchbend(bend);
}

// setPitchbendRange()
//
void FmSynth::setPitchbendRange(uint8_t range) {

//    Serial.printf("FmSynth::setPitchbendRange: %d\n", range);
    dexSynth.setPitchbendRange(range);
}

// setPanType()
//
void FmSynth::setPanType(MidiPanType type, uint8_t param) {

//    Serial.printf("FmSynth::setPanType: type %d param %d\n", type, param);

    bool setPos = false;

    panType = type;
    panParam = param;

    switch (panType) {

        case PanTypeCenter:
            panParam = PanParamDefault;
            panPosition = panParam;
            setPos = true;
            break;

        case PanTypeFixed:
            panParam = param;
            panPosition = panParam;
//            Serial.printf("FmSynth::setPanType: PanTypeFixed panPosition %d\n", panPosition);
            setPos = true;
            break;

        case PanTypeHold:
            panPosition = panParam;
            holdSet = false;
            break;

        case PanTypeFollow:
            // Use current position to start, then follow setting on updates
            setPos = true;
            break;

        case PanTypeRandom:
            panPosition = UtilFns::getRand(PanParamMin, PanParamMax);
            randomSet = true;
            setPos = true;
            break;

        case PanTypeAuto:
            // TODO: Generate position from wave function TBD
            break;

        default:
            break;
    }

    if (setPos) {
//        Serial.printf("FmSynth::setPanType setting position to: %d\n", panPosition);
        audioPanGain.setPan(UtilFns::scale((float)panPosition, (float)PanParamMin, PanParamMax, 0.0, 1.0));
        panPositionPrev = panPosition;
    }
}

// setPanPosition()
//
void FmSynth::setPanPosition(uint8_t position) {

//    Serial.printf("FmSynth::setPanPosition: %d\n", position);

    bool setPos = false;

    switch (panType) {

        case PanTypeFollow:
            panPosition = position;
            setPos = true;
            break;

        case PanTypeHold:
            panPosition = position;
            if (!holdSet) {
                setPos = true;
            }
            break;

        case PanTypeAuto:
            // TODO: Generate position from wave function TBD
           break;

        default:
            break;
    }

    if (setPos && panPosition != panPositionPrev) {
//        Serial.printf("FmSynth::setPanPosition position to: %d\n", panPosition);
        audioPanGain.setPan(UtilFns::scale((float)panPosition, (float)PanParamMin, PanParamMax, 0.0, 1.0));
        panPositionPrev = panPosition;
    }
}

// allNotesOff()
//
// Stop all the voices
//
void FmSynth::allNotesOff() {

//    Serial.printf("FmSynth::allNotesOff\n");

    dexSynth.notesOff();

}

// setGain()
//
void FmSynth::setGain(float gainValue) {

//    Serial.printf("FmSynth::setGain %f\n", gainValue);

    audioPanGain.setGain(gainValue);
}

// setAttack()
//
void FmSynth::setAttack(float attack) {

//    Serial.printf("FmSynth::setAttack %f\n", attack);
    this->attack = attack;
}

// setDecay()
//
void FmSynth::setDecay(float decay) {

//    Serial.printf("FmSynth::setDecay %f\n", decay);
    this->decay = decay;
}

// setSustain()
//
void FmSynth::setSustain(float level) {

//    Serial.printf("FmSynth::setSustain %f\n", level);
    this->sustain = level;
}

// setRelease()
//
void FmSynth::setRelease(float release) {

//    Serial.printf("FmSynth::setRelease %f\n", release);
    this->release = release;
}

// setModulatorFrequency()
//
void FmSynth::setModulatorFrequency(float freqHz) {

//    Serial.printf("FmSynth::setModulatorFrequency %f\n", freqHz);
    this->modFreq = freqHz;
}

// setModulatorAmplitude()
//
// FFS: Called with MIDI control value [0,127] and from Synth screen
// with value [0,127].
//
void FmSynth::setModulatorAmplitude(float amplitude) {

//    Serial.printf("FmSynth::setModulatorAmplitude %f\n", amplitude);
    this->modAmt = amplitude;
}

/************************************************************************************
*                        P R O T E C T E D   F U N C T I O N S                      *
*************************************************************************************
*/

/************************************************************************************
*                          P R I V A T E   F U N C T I O N S                         *
*************************************************************************************
*/

// loadVoice()
//
void FmSynth::loadVoice(uint16_t voicePresetNum) {

//    Serial.printf("FmSynth::loadVoice: Voice %3d\n", voicePresetNum);

    if (presetBanks) {
        uint8_t* voicePtr = (uint8_t *)&presetBanks[voicePresetNum * VoicePresetBankedLength];
        dexSynth.decodeVoice(decoded_voice, voicePtr);
        dexSynth.loadVoiceParameters(decoded_voice);
    } else {
//        Serial.printf("FmSynth::loadVoice: External Banks Undefined\n");
    }
}

