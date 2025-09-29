/************************************************************************************
*                                PresetDefaultData.h                                *
*                                                                                   *
*                       Copyright (c) 2025, Donald Swearingen                       *
*                                All Rights Reserved                                *
*************************************************************************************
*
* Description
*
*   Definitions of default values for data structures underlying the Player and Audio
*   devices of the AudioPlatform software application.
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

#ifndef PRESETDEFAULTDATA_H_
#define PRESETDEFAULTDATA_H_

/************************************************************************************
*                       E X T E R N A L   R E F E R E N C E S                       *
*************************************************************************************
*/

#include "PresetDataStructs.h"

/******************
* Begin Namespace *
*******************
*/
namespace Presets {

/****************
* "Pre-Defines" *
*****************
*
* Definitions that affect subsequent external references
*
*/

/************************************************************************************
*            E X P O R T E D   D A T A / T Y P E   D E F I N I T I O N S            *
*************************************************************************************
*
* Exported Data/Types referenced with namespace prefix "Presets::"
*
*/

/**********************
* Default Device Data *
***********************
*
* Device data structure default "factory" settings. Applied when creating default preset
* for populating individual presets, and for creating a default preset bank when no
* preset banks are found on startup.
*
*/

PROGMEM PresetCommonData presetCommonPresetDefault = {
    "Default",  // presetName
    0           // preset modified (0/1)
};

PROGMEM MediaPlayerSettings wavPlayerPresetDefault = {
    0.0,        // enabled
    0.0,        // outLevel
    0.0,        // dlySend
    0.0,        // rvbSend
    0.0,        // panType
    0.0,        // noteShift
    0.0,        // noteRangeMin
    127.0,      // noteRangeMax
    0.0,        // soundBank
    63.0        // panParameter
};

PROGMEM MediaPlayerSettings rawPlayerPresetDefault = {
    0.0,        // enabled
    0.0,        // outLevel
    0.0,        // dlySend
    0.0,        // rvbSend
    0.0,        // panType
    0.0,        // noteShift
    0.0,        // noteRangeMin
    127.0,      // noteRangeMax
    0.0,        // soundBank
    63.0        // panParameter
};

PROGMEM BasicSynthSettings basicSynthPresetDefault = {
    0.0,        // enabled;
    0.0,        // outLevel;
    0.0,        // modFreq;
    0.0,        // modAmt;
    0.0,        // attack;
    0.0,        // decay;
    0.0,        // sustain;
    0.0,        // release;
    0.0,        // dlySend;
    0.0,        // rvbSend;
    0.0,        // synthType;
    0.0,        // noteShift
    0.0,        // noteRangeMin
    127.0       // noteRangeMax
};

PROGMEM FmSynthSettings fmSynthPresetDefault = {
    0.0,        // enabled
    0.0,        // outLevel
    0.0,        // dlySend
    0.0,        // rvbSend
    0.0,        // presetNum  Rev ?? (See M250125-1 and M250126-1)
    0.0,        // noteShift
    0.0,        // noteRangeMin
    127.0,      // noteRangeMax
    0.0,        // pitchBendRange (semitones)
    0.0,        // panType
    63.0        // panParameter
};

PROGMEM MidiSettings midiPresetDefault = {
    0.0,        // dummy1
    0.0,        // dummy2
};

PROGMEM EfxSettings efxPresetDefault = {
    0.0,        // efxGain
    0.0,        // param2
    0.0,        // param3
    0.0,        // param4
};

PROGMEM RvbSettings rvbPresetDefault = {
    0.0,        // rvbPchShft
    0.0,        // rvbPchShftAmt
    0.0,        // rvbShimShft
    0.0,        // rvbShimShftRto
    REVERB_ROOMSIZE_DEFAULT,    // rvbRoomSize
    REVERB_DIFFUSION_DEFAULT,   // rvbDiffusion
    REVERB_LEVEL_DEFAULT,       // rvbLevel
    0.0,        // rvbShimAmt
};

PROGMEM StereoDelaySettings stereoDelayPresetDefault = {
    DELAY_TIME_DEFAULT,        // delayLeftDelayTime
    0.0,        // delayLeftLevel
    0.0,        // delayLeftRvbSend
    0.0,        // delayLeftEfx3Send
    0.0,        // delayLeftFbDelayAmt
    0.0,        // delayLeftxFbDelayTime
    0.0,        // delayLeftxFbDelayAmt
    DELAY_TIME_DEFAULT,        // delayRightDelayTime
    0.0,        // delayRightLevel
    0.0,        // delayRightRvbSend
    0.0,        // delayRightEfx3Send
    0.0,        // delayRightFbDelayAmt
    0.0,        // delayRightxFbDelayTime
    0.0,        // delayRightxFbDelayAmt
};

PROGMEM AudioIoSettings audioIoPresetDefault = {
    0.0,        // lineInEnabled
    0.0,        // lineInLevel
    0.0,        // lineOutLevel
    0.0,        // phoneLevel
    0.0,        // lineInDlySend
    0.0,        // lineInRvbSend
};

/****************
* End Namespace *
*****************
*/
} // namespace Presets

#endif // PRESETDEFAULTDATA_H_
