/************************************************************************************
*                                PresetDataStructs.h                                *
*                                                                                   *
*                       Copyright (c) 2025, Donald Swearingen                       *
*                                All Rights Reserved                                *
*************************************************************************************
*
* Description
*
*   Definitions of the data structures underlying the Player and Audio devices of the
*   AudioPlatform software application.
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

#ifndef PRESETDATASTRUCTS_H_
#define PRESETDATASTRUCTS_H_

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

// Directive that sets locations of removed values when efx presets
// are loaded in Presets::loadPreset() until the locations are reused
// by new values added later.
//
#define SET_EFX_PLACEHOLDERS_ON_LOAD    1

/************************************************************************************
*            E X P O R T E D   D A T A / T Y P E   D E F I N I T I O N S            *
*************************************************************************************
*
* Exported Data/Types referenced with namespace prefix "Presets::"
*
*/

/*************************
* Device Data Structures *
**************************
*
* Description
*
*   Parameter blocks for various devices mirroring device settings applied in
*   screen settings and other control sources. Used as containers for storage
*   and retrieval of preset data.
*
* Expansion
*
*   After initial release, add new parameters to the end of settings structures
*   in order to preserve existing settings. Eventually when new version is issued,
*   can move to more logical positions. But doing so will require a conversion
*   utility application to reorganize existing presets.
*
* Version Control
*
*/

// Preset Common Data
//
typedef struct {

    char        presetName[PresetNameSize];
    float       presetModified; // (bool 0/1) Rev 0.5.1.1 1/14/25

} PresetCommonData;

// Media Player
//
// Common to Wav and Raw players
//
typedef struct {

    float       enabled;        // Rev 0.3.3
    float       outLevel;       // Rev 0.3.3
    float       dlySend;        // Rev 0.3.3
    float       rvbSend;        // Rev 0.3.3
    float       panType;        // Rev 0.3.3
    float       noteShift;      // Rev 0.4.0.1
    float       noteRangeMin;   // Rev 0.4.0.1
    float       noteRangeMax;   // Rev 0.4.0.1
    float       soundBank;      // Rev 0.5.1.0
    float       panParameter;   // Rev 0.5.2

} MediaPlayerSettings;

// Basic Synth
//
typedef struct {

    float       enabled;        // Rev 0.3.3
    float       outLevel;       // Rev 0.3.3
    float       modFreq;        // Rev 0.3.3
    float       modAmt;         // Rev 0.3.3
    float       attack;         // Rev 0.3.3
    float       decay;          // Rev 0.3.3
    float       sustain;        // Rev 0.3.3
    float       release;        // Rev 0.3.3
    float       dlySend;        // Rev 0.3.3
    float       rvbSend;        // Rev 0.3.3
    float       synthType;      // Rev 0.3.3
    float       noteShift;      // Rev 0.4.0.1
    float       noteRangeMin;   // Rev 0.4.0.1
    float       noteRangeMax;   // Rev 0.4.0.1
    float       panType;        // Rev 0.5.2
    float       panParameter;   // Rev 0.5.2

} BasicSynthSettings;

// FM (Dexed) Synth
//
typedef struct {

    float       enabled;        // Rev 0.3.4
    float       outLevel;       // Rev 0.3.4
    float       dlySend;        // Rev 0.3.4
    float       rvbSend;        // Rev 0.3.4
    float       presetNum;      // Rev ?? (See M250125-1 and M250126-1)
    float       noteShift;      // Rev 0.4.0.1
    float       noteRangeMin;   // Rev 0.4.0.1
    float       noteRangeMax;   // Rev 0.4.0.1
    float       pitchBendRange; // Rev 0.5.2
    float       panType;        // Rev 0.5.2
    float       panParameter;   // Rev 0.5.2

// Possible additions:
//  ADSR on final output

} FmSynthSettings;

// MIDI
//
// MIDI Settings not yet active in v0.5.5.4, 7/21/25
//
typedef struct {

    float       dummy1;         // Rev 0.3.3
    float       dummy2;         // Rev 0.3.3

} MidiSettings;

// Future Effect Settings
//
// 0.5.5.5: Reverb settings moved to own structure. Efx settings now
// unused until a new efx device is added to the application.
//
typedef struct {

    float       efxGain;        // Rev 0.5.5.5
    float       param2;         // Rev 0.5.5.5
    float       param3;         // Rev 0.5.5.5
    float       param4;         // Rev 0.5.5.5

} EfxSettings;

// Reverb Settings
//
// 0.5.5.5: Reverb settings moved to own structure.
//
typedef struct {

    // 0.5.5.5 additions
    float       rvbPchShft;     // Rev 0.5.5.5
    float       rvbPchShftAmt;  // Rev 0.5.5.5
    float       rvbShimShft;    // Rev 0.5.5.5
    float       rvbShimShftRto; // Rev 0.5.5.5

    // 0.3.3.3 positioning
    float       rvbRoomSize;    // Rev 0.3.3
    float       rvbDiffusion;   // Rev 0.3.3
    float       rvbLevel;       // Rev 0.3.3

    // 0.5.5.5 additions
    float       rvbShimAmt;     // Rev 0.5.5.5

} RvbSettings;

// Stereo Delay
//
typedef struct {

    float       delayLeftDelayTime;     // Rev 0.3.4.4
    float       delayLeftLevel;         // Rev 0.3.4.4
    float       delayLeftRvbSend;       // Rev 0.3.4.4
    float       delayLeftEfx3Send;      // Rev 0.3.4.4
    float       delayLeftFbDelayAmt;    // Rev 0.3.4.4
    float       delayLeftxFbDelayTime;  // Rev 0.3.4.4
    float       delayLeftxFbDelayAmt;   // Rev 0.3.4.4

    float       delayRightDelayTime;    // Rev 0.3.4.4
    float       delayRightLevel;        // Rev 0.3.4.4
    float       delayRightRvbSend;      // Rev 0.3.4.4
    float       delayRightEfx3Send;     // Rev 0.3.4.4
    float       delayRightFbDelayAmt;   // Rev 0.3.4.4
    float       delayRightxFbDelayTime; // Rev 0.3.4.4
    float       delayRightxFbDelayAmt;  // Rev 0.3.4.4

} StereoDelaySettings;

// Audio IO
//
typedef struct {

    float       lineInEnabled;  // Rev 0.3.3
    float       lineInLevel;    // Rev 0.3.3
    float       lineOutLevel;   // Rev 0.3.3
    float       phoneLevel;     // Rev 0.3.3
    float       lineInDlySend;  // Rev 0.3.3
    float       lineInRvbSend;  // Rev 0.3.3

} AudioIoSettings;

/****************
* End Namespace *
*****************
*/
} // namespace Presets

#endif // PRESETDATASTRUCTS_H_
