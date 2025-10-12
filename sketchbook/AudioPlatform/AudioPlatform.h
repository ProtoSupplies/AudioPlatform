/************************************************************************************
*                                  AudioPlatform.h                                  *
*                                                                                   *
*                       Copyright (c) 2025, Donald Swearingen                       *
*                                All Rights Reserved                                *
*************************************************************************************
*
* Description
*
*   Structures and definitions common to primary components of the main AudioPlatform
*   application.
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

#ifndef AUDIOPLATFORM_H_
#define AUDIOPLATFORM_H_

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

// Standard C++
//
#include <Math.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

// User Arduino Libraries
//
#include "AudioPlatformGlobal.h"

/************************************************************************************
*                E X P O R T E D   M A C R O S  /  C O N S T A N T S                *
*************************************************************************************
*/

/*******************************
* Application Name and Version *
********************************
*
*/

const char* ApplicationName     = "AudioPlatform";
const char* ApplicationVersion  = "Revision 0.5.5.6, 8/2/25";

const char* BuildDate           = __DATE__;
const char* BuildTime           = __TIME__;

/*****************************
* Teensy 4.1 Pin Definitions *
******************************
*
*/

#define SD_CS_PIN               BUILTIN_SDCARD  // TeensySD 4.1 Card Select
#define SERIALFLASH_CS_PIN      6               // AudioPlatform Serial Flash Chip Select

/**********************
* Application Screens *
***********************
*
* Screen names are defined here rather than in Screens.h in order to 1) make the
* names generally available to all application components; and 2) to avoid issues of
* recursion in .h files, which may prevent complete access to terms defined in
* the .h file depending on the order and placement of include statements referencing
* the .h file.
*
*/

// Screen IDs
//
// !!NOTE!!: Very important that ScreenID definitions be based at 0 and monotonically
// increasing as they will in some cases be used as array indices. Because of this, it
// is of critical importance that all arrays using these definitions as indices must
// arrange the array entries in the same order as they appear in this enumeration
// (for example, see the array "screenProfiles" definition in module Screens.ino).
//
// NOTE: Definition as uint8_t restricts maximum number of screen IDs to <= 256
//
typedef enum : uint8_t {
    ScreenMainMenu = 0,
    ScreenUtilMenu,
    ScreenWavPlayer,
    ScreenRawPlayer,
    ScreenBasicSynth,
    ScreenFmSynth,
    ScreenFmBanks,
    ScreenMidi,
    ScreenStereoDelay,
    ScreenReverb,
    ScreenEfx,
    ScreenInputOutput,
    ScreenPresets,
    ScreenMonitor,
    ScreenSystemInfo,
    ScreenColors,
    ScreenTestTextColumns,
    ScreenTestTextEdit,
    ScreenTestListSelect,
    ScreenViewMedia,
    ScreenSetTime,
    ScreenSerialFlashUtil,
    ScreenLive,
    ScreenCount,        // # of Screens. Always Next to Last Entry.
    ScreenNone          // Always Last Entry
} ScreenID;

/*****************************
* Application Preset Devices *
******************************
*
* Preset devices are those devices, screens, and structures that store their
* parameter values in presets that can be saved and recalled.
*
*/

// Preset Device IDs
//
// !!NOTA BENE!! The order of the following definitions is used to determine the position of
// each device within a given preset. Once a presets file has been written, it is essential
// that the order defined here must be preserved exactly. Any change to the order will serve
// to invalidate existing presets file(s), requiring reorganization to restore compatibility.
//
// The implication is that, for the same reason of compatibility, any new devices must be added
// to the END of the existing list. And in the event that a given device is no longer used, the
// name must remain in this list.
//
typedef enum : uint16_t {

    // Rev 0.3.4.5: made PresetCommon a device instead of header block.
    // No effect on data of existing devices since previous code skipped
    // the header portion of the preset devoted to preset common data,
    // (that is, deviceWavPlayer occupies the second slot in the preset
    // in both cases)
    //
    // Rev 0.5.5.5: renamed deviceEfx>deviceRvb. In pevious revisions
    // deviceEfx was meant to include params for goth reverb, and a future
    // effects device. So the rename preserves the position of the reverb
    // parameters and moves deviceEfx to the end of the list as a new
    // device with yet to be determined parameters.
    //
    devicePresetCommon  = 0,    // Rev 0.3.4.5

    deviceWavPlayer     = 1,    // Rev 0.3.3
    deviceBasicSynth    = 2,    // Rev 0.3.3
    deviceMidi          = 3,    // Rev 0.3.3
    deviceRvb           = 4,    // Rev 0.5.5.5
    deviceAudioIo       = 5,    // Rev 0.3.3

    deviceRawPlayer     = 6,    // Rev 0.3.4
    deviceFmSynth       = 7,    // Rev 0.3.4

    deviceStereoDelay   = 8,    // Rev 0.3.4.4
    deviceEfx           = 9,    // Rev 0.5.5.5

    deviceCount // Total number of devices

} PresetDeviceId;

// Preset Device Parameter IDs
//
typedef enum : uint16_t {

    // Effects
    Efx_None,
    Reverb_Level,
    Reverb_RoomSize,
    Reverb_Diffusion,
    Reverb_PchShft,
    Reverb_PchShftAmt,
    Reverb_ShimPchShft,
    Reverb_ShimShftRatio,
    Reverb_ShimAmt,

    FutureEfx_Level,
    FutureEfx_Param2,
    FutureEfx_Param3,
    FutureEfx_Param4,

    StereoDelay_DlyL_DlyTime,
    StereoDelay_DlyL_Gain,
    StereoDelay_DlyL_RvbSend,
    StereoDelay_DlyL_Efx3Send,
    StereoDelay_DlyL_FB_DlyAmt,
    StereoDelay_DlyL_xFB_DlyTime,
    StereoDelay_DlyL_xFB_DlyAmt,
    StereoDelay_DlyR_DlyTime,
    StereoDelay_DlyR_Gain,
    StereoDelay_DlyR_RvbSend,
    StereoDelay_DlyR_Efx3Send,
    StereoDelay_DlyR_FB_DlyAmt,
    StereoDelay_DlyR_xFB_DlyTime,
    StereoDelay_DlyR_xFB_DlyAmt,

    // WAV File Player
    MediaPlayWav_Status,
    MediaPlayWav_Level,
    MediaPlayWav_DlySend,
    MediaPlayWav_RvbSend,
    MediaPlayWav_PanType,
    MediaPlayWav_PanParameter,
    MediaPlayWav_NoteShift,
    MediaPlayWav_NoteRangeMin,
    MediaPlayWav_NoteRangeMax,
    MediaPlayWav_SoundBank,

    // RAW (mono) File Player
    MediaPlayRaw_Status,
    MediaPlayRaw_Level,
    MediaPlayRaw_DlySend,
    MediaPlayRaw_RvbSend,
    MediaPlayRaw_PanType,
    MediaPlayRaw_PanParameter,
    MediaPlayRaw_NoteShift,
    MediaPlayRaw_NoteRangeMin,
    MediaPlayRaw_NoteRangeMax,
    MediaPlayRaw_SoundBank,

    // Basic Synthesizer
    BasicSynth_Status,
    BasicSynth_Level,
    BasicSynth_ModFreq,
    BasicSynth_ModAmt,
    BasicSynth_Attack,
    BasicSynth_Decay,
    BasicSynth_Sustain,
    BasicSynth_Release,
    BasicSynth_DlySend,
    BasicSynth_RvbSend,
    BasicSynth_Type,
    BasicSynth_NoteShift,
    BasicSynth_NoteRangeMin,
    BasicSynth_NoteRangeMax,
    BasicSynth_PanType,
    BasicSynth_PanParameter,

    // DX7 Emulator FM Synthesizer
    FmSynth_Status,
    FmSynth_Level,
    FmSynth_DlySend,
    FmSynth_RvbSend,
    FmSynth_PresetNum,
    FmSynth_NoteShift,
    FmSynth_NoteRangeMin,
    FmSynth_NoteRangeMax,
    FmSynth_PitchBendRange,
    FmSynth_PanType,
    FmSynth_PanParameter,

    // Audio In/Out Codec
    AudioIo_LineIn_Status,
    AudioIo_LineIn_Level,
    AudioIo_LineOut_Level,
    AudioIo_Phone_Volume,
    AudioIo_LineIn_DlySend,
    AudioIo_LineIn_RvbSend,
    AudioIo_InputSel,
    AudioIo_MicGain,

    // General MIDI Parameters (Global)
    Midi_Input_Port,
    Midi_Input_Device,
    Midi_Chan_WavPlayer,
    Midi_Chan_RawPlayer,
    Midi_Chan_BasicSynth,
    Midi_Chan_FmSynth,

} DeviceParamId;

// Preset Device Status
//
typedef enum {
    Device_Disabled = 0,
    Device_Enabled = 1,
    Device_OffLine
} PresetDeviceStatus;


/**************************
* Application I/O Devices *
***************************
*
*/

// MID Input Port
//
typedef enum {
    MidiInPort_USB = 0,
    MidiInPort_DIN = 1
} MidiInputPort;

// MID Input Device
//
typedef enum {
    MidiInDevice_Generic    = 0,
    MidiInDevice_LK25       = 1,
    MidiInDevice_MK3        = 2,
    MidiInDevice_ML3        = 3
} MidiInputDevice;

/*************************
* Application Components *
**************************
*
* Normally, .h files are included at the beginning of a module. However, in this case AudioPlatform.h
* defines a number global types that are referenced by other core components of the AudioPlatform application.
* For that reason, the following declarations are placed near the bottom of the module.
*
*/
#include "AudioDevices.h"
#include "InputDevices.h"
#include "Screens.h"
#include "Presets.h"

/******************
* Begin Namespace *
*******************
*/
namespace AudioPlatform {

/************************************************************************************
*            E X P O R T E D   D A T A / T Y P E   D E F I N I T I O N S            *
*************************************************************************************
*
* Exported data referenced with namespace prefix "AudioPlatform::"
*
*/

// TODO: Try to move these literals to PROGMEM. Most likely they are being allocated in RAM.
//
const char* DeviceStatusLabels[]    = {"Disabled", "Enabled"};
const char* SynthTypeLabels[]       = {"FM", "FMx10", "AM", "AMx10", "RING"};
// NOTE: The order of panTypeLabels must match those in the constants "MidiPanType" enum MidiDefines.h
const char* PanTypeLabels[]         = {"Center", "Fixed", "Hold", "Follow", "Random", "Auto"};
const char* MidiPortLabels[]        = {"USB MIDI", "DIN MIDI"};
const char* MidiInDeviceLabels[]    = {"Generic", "Launchkey25", "Launchkey49", "MiniLab3"};

const char* AudioLineInPpLabels[]   = {"3.12", "2.63", "2.22", "1.87", "1.58", "1.33", "1.11", "0.94",
                                       "0.79", "0.67", "0.56", "0.48", "0.40", "0.34", "0.29", "0.24"};
const char* AudioLineOutPpLabels[]  = {"1.16", "1.22", "1.29", "1.37", "1.44", "1.53", "1.62", "1.71", "1.80", "1.91",
                                       "2.02", "2.14", "2.26", "2.39", "2.53", "2.67", "2.83", "2.98", "3.16"};

// AudioPlatform System Information
//
extern      SystemHardwareProfile   systemProfile;

/************************************************************************************
*              E X P O R T E D   F U N C T I O N   P R O T O T Y P E S              *
*************************************************************************************
*
* Exported functions referenced with namespace prefix "AudioPlatform::"
*
*/

/****************
* End Namespace *
*****************
*/
} // namespace AudioPlatform

#endif // AUDIOPLATFORM_H_
