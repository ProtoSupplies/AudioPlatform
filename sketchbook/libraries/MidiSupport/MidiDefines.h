/************************************************************************************
*                                  MidiDefines.h                                    *
*                      Copyright (c) 2025, Donald Swearingen                        *
*                               All Rights Reserved                                 *
*************************************************************************************
*
* Description
*
*   Definitions of basic MIDI standard terms, and items specific to AudioPlatform
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

#ifndef MIDIDEFINES_H_
#define MIDIDEFINES_H_

// Select MIDI Input Device (mutually exclusive, must select one and only one)
//
#define USING_LAUNCHKEY_MK3     0       // Novation LaunchKey MK3 (see "Docs/Components Info/Launchkey MK3 User Guide v6 - EN.pdf")
#define USING_LAUNCHKEY_25      1       // Novation LaunchKey 25 (see "Docs/Components Info/Launchkey MK3 User Guide v6 - EN.pdf")
#define USING_MINILAB3          0       // Arturia MiniLab3 (see "Docs/Components Info/Arturia Minilab3 Manual 1_0_5.pdf"
#define USING_GENERIC_MIDI      0       // Undefined Generic MIDI keyboard controller

#if ((USING_LAUNCHKEY_MK3 + USING_LAUNCHKEY_25 + USING_MINILAB3 + USING_GENERIC_MIDI) > 1)
#error "Must select ONLY ONE of USING_LAUNCHKEY_MK3, USING_LAUNCHKEY_25, USING_MINILAB3, USING_GENERIC_MIDI"
#endif // ((USING_LAUNCHKEY_MK3 + USING_MINILAB3 + USING_GENERIC_MIDI) > 1)

#if ((USING_LAUNCHKEY_MK3 + USING_LAUNCHKEY_25 + USING_MINILAB3 + USING_GENERIC_MIDI) == 0)
#error "Must select one of USING_LAUNCHKEY_MK3, USING_LAUNCHKEY_25, USING_MINILAB3, USING_GENERIC_MIDI"
#endif // ((USING_LAUNCHKEY_MK3 + USING_MINILAB3 + USING_GENERIC_MIDI) == 0)

// Select one only
#define PAN_USE_LINEAR_XFADE        0
#define PAN_USE_SPKR_TO_SPKR_XFADE  1

#if PAN_USE_SPKR_TO_SPKR_XFADE
constexpr double PI_DIV_2 = (PI / 2.0);
#endif // USE_SPKR_TO_SPKR_XFADE

/************************************************************************************
*                         M A C R O S  /  C O N S T A N T S                         *
*************************************************************************************
*/

// Miscellaneous MIDI Constants
//
// MIDI values are 7-bit unsigned values in the range [0,127]. Values > 127
// defined here (e.g. MidiNoteNone) outside that range and used to indicate
// the absence of MIDI data, or intended to be ignored in functions that
// operate on MIDI values.
//
const uint8_t   MidiNoteMin         = 0;
const uint8_t   MidiNoteMax         = 127;
const uint8_t   MidiDataMin         = 0;
const uint8_t   MidiDataMax         = 127;

// Values defined beyond 7-bit MIDI range
// used to indicate values to be ignored
//
typedef enum : uint8_t {

    MidiNoteNone = 200,
    MidiDataNone,
    MidiSustainNone,
    MidiModWheelNone,
    MidiAllOffNone,
    MidiVolumeNone,
    MidiShift1None,
    MidiShift2None,
    MidiBendCtrlNone,
    MidiPanControlNone,
    ModFreqCtrlNone,
    ModAmpCtrlNone,
    ModAttackCtrlNone,
    ModDecayCtrlNone,
    ModSustainCtrlNone,
    ModReleaseCtrlNone,

} MidiIgnoreFlags;

// MIDI Message Types
//
typedef enum {

    MidiNoteOn,
    MidiNoteOff,
    MidiControlChange,
    MIDIProgramChange,
    MidiPitchBend

} MidiMessageType;

// MIDI Message Source
//
typedef enum {

    MidiSourceUSB,      // AudioPlatform USB Hub Input
    MidiSourceDIN       // AudioPlatform 5-pin MIDI DIN Input

} MidiMessageSource;

// Pan Data Type and Constants
//
// Various pan modes applied in audio devices
//
typedef enum : uint8_t {

    PanTypeCenter   = 0,
    PanTypeFixed    = 1,
    PanTypeHold     = 2,
    PanTypeFollow   = 3,
    PanTypeRandom   = 4,
    PanTypeCount    = 5,
    // Note (2/7/25): PanTypeAuto (A250205-2) disabled until implementation
    // Define dummy value that cannot occur in switch/case statements
    PanTypeAuto     = 255

} MidiPanType;

// Pan Constants
//
const MidiPanType   PanTypeDefault  = PanTypeCenter;
const MidiPanType   PanTypeMin      = PanTypeCenter;
const MidiPanType   PanTypeMax      = PanTypeRandom;
// Note (2/7/25): PanTypeAuto (A250205-2) disabled until implementation
// const MidiPanType   PanTypeMax      = PanTypeAuto;
const uint8_t       PanParamMin     = 0;
const uint8_t       PanParamMax     = 127;
const uint8_t       PanParamDefault = 63;

/************************************************************************************
*                     D A T A / T Y P E   D E F I N I T I O N S                     *
*************************************************************************************
*/

/****************************
* Standard MIDI Definitions *
*****************************
*/

typedef uint8_t     MidiNoteNumber;

// Standard MIDI Control Numbers
typedef enum {

    // Continuous Controllers
    MidiBankSelect                  = 0,
    MidiModulationWheel             = 1,
    MidiBreathController            = 2,
    // CC3 undefined
    MidiFootController              = 4,
    MidiPortamentoTime              = 5,
    MidiDataEntryMSB                = 6,
    MidiChannelVolume               = 7,
    MidiBalance                     = 8,
    // CC9 undefined
    MidiPanControl                  = 10,
    MidiExpressionController        = 11,
    MidiEffectControl1              = 12,
    MidiEffectControl2              = 13,
    // CC14 undefined
    // CC15 undefined
    MidiGeneralPurposeController1   = 16,
    MidiGeneralPurposeController2   = 17,
    MidiGeneralPurposeController3   = 18,
    MidiGeneralPurposeController4   = 19,

    MidiDataEntryLSB                = 38,

    // Switches
    MidiSustain                     = 64,
    MidiPortamento                  = 65,
    MidiSostenuto                   = 66,
    MidiSoftPedal                   = 67,
    MidiLegato                      = 68,
    MidiHold                        = 69,

    MidiSoundController1            = 70,   ///< Synth: Sound Variation   FX: Exciter On/Off
    MidiSoundController2            = 71,   ///< Synth: Harmonic Content  FX: Compressor On/Off
    MidiSoundController3            = 72,   ///< Synth: Release Time      FX: Distortion On/Off
    MidiSoundController4            = 73,   ///< Synth: Attack Time       FX: EQ On/Off
    MidiSoundController5            = 74,   ///< Synth: Brightness        FX: Expander On/Off
    MidiSoundController6            = 75,   ///< Synth: Decay Time        FX: Reverb On/Off
    MidiSoundController7            = 76,   ///< Synth: Vibrato Rate      FX: Delay On/Off
    MidiSoundController8            = 77,   ///< Synth: Vibrato Depth     FX: Pitch Transpose On/Off
    MidiSoundController9            = 78,   ///< Synth: Vibrato Delay     FX: Flange/Chorus On/Off
    MidiSoundController10           = 79,   ///< Synth: Undefined         FX: Special Effects On/Off
    MidiGeneralPurposeController5   = 80,
    MidiGeneralPurposeController6   = 81,
    MidiGeneralPurposeController7   = 82,
    MidiGeneralPurposeController8   = 83,
    MidiPortamentoControl           = 84,
    // CC85 to CC90 undefined
    MidiEffects1                    = 91,   ///< Reverb send level
    MidiEffects2                    = 92,   ///< Tremolo depth
    MidiEffects3                    = 93,   ///< Chorus send level
    MidiEffects4                    = 94,   ///< Celeste depth
    MidiEffects5                    = 95,   ///< Phaser depth
    MidiDataIncrement               = 96,
    MidiDataDecrement               = 97,
    MidiNRPNLSB                     = 98,   ///< Non-Registered Parameter Number (LSB)
    MidiNRPNMSB                     = 99,   ///< Non-Registered Parameter Number (MSB)
    MidiRPNLSB                      = 100,  ///< Registered Parameter Number (LSB)
    MidiRPNMSB                      = 101,  ///< Registered Parameter Number (MSB)

    // Channel Mode messages
    MidiAllSoundOff                 = 120,
    MidiResetAllControllers         = 121,
    MidiLocalControl                = 122,
    MidiAllNotesOff                 = 123,
    MidiOmniModeOff                 = 124,
    MidiOmniModeOn                  = 125,
    MidiMonoModeOn                  = 126,
    MidiPolyModeOn                  = 127

} MidiControlNumber;

/*****************************************
* Controller/Device-Specific Definitions *
******************************************
*/

// Novation LaunchKey25 Control/Button Numbers
// (see "Docs/Components Info/Launchkey MK3 User Guide v6 - EN.pdf")
//
// MIDI values assigned to various knobs, sliders, and
// buttons on Novation LaunchKey49 MK3 MIDI keyboard controller.
//
// Note: These values assume the original, unmodified settings
// on the MK3, which are user-modifiable using a PC/Mac based
// application. If user presets are in effect, some or all of
// the following may not apply.
//
typedef enum {

    // Vertical Sliders
    Lk25Fader       = 7,    // Ctrl # (0-127)

    // Rotational potentiometers
    Lk25Knob1       = 21,   // Ctrl # (0-127)
    Lk25Knob2       = 22,   // Ctrl # (0-127)
    Lk25Knob3       = 23,   // Ctrl # (0-127)
    Lk25Knob4       = 24,   // Ctrl # (0-127)
    Lk25Knob5       = 25,   // Ctrl # (0-127)
    Lk25Knob6       = 26,   // Ctrl # (0-127)
    Lk25Knob7       = 27,   // Ctrl # (0-127)
    Lk25Knob8       = 28,   // Ctrl # (0-127)

    // Transport buttons
    Lk25FRev        = 112,   // Ctrl # (127 press, 0 release)
    Lk25FFwd        = 113,   // Ctrl # (127 press, 0 release)
    Lk25Stop        = 114,   // Ctrl # (127 press, 0 release)
    Lk25Play        = 115,   // Ctrl # (127 press, 0 release)
    Lk25Loop        = 116,   // Ctrl # (127 press, 0 release)
    Lk25Recd        = 117,   // Ctrl # (127 press, 0 release)

    // Pads (When User Custom4 in effect)
    Lk25Pad1        = 40,    // Prog Change
    Lk25Pad2        = 41,    // Prog Change
    Lk25Pad3        = 42,    // Prog Change
    Lk25Pad4        = 43,    // Prog Change
    Lk25Pad5        = 48,    // Prog Change
    Lk25Pad6        = 49,    // Prog Change
    Lk25Pad7        = 50,    // Prog Change
    Lk25Pad8        = 51,    // Prog Change
    Lk25Pad9        = 36,    // Prog Change
    Lk25Pad10       = 37,    // Prog Change
    Lk25Pad11       = 38,   // Prog Change
    Lk25Pad12       = 39,   // Prog Change
    Lk25Pad13       = 44,   // Prog Change
    Lk25Pad14       = 45,   // Prog Change
    Lk25Pad15       = 46,   // Prog Change
    Lk25Pad16       = 47,   // Prog Change

    // Misc
    Lk25TrkLftArrow = 103,  // Ctrl # (127 press, 0 release)
    Lk25TrkRgtArrow = 102,  // Ctrl # (127 press, 0 release)
    Lk25Pad18Arrow  = 104,  // Ctrl # (127 press, 0 release)
    Lk25Pad916Arrow = 105   // Ctrl # (127 press, 0 release)

} LaunchKey25Controls;

// Novation LaunchKey49 MK3 Control/Button Numbers
// (see "Docs/Components Info/Launchkey MK3 User Guide v6 - EN.pdf")
//
// MIDI values assigned to various knobs, sliders, and
// buttons on Novation LaunchKey49 MK3 MIDI keyboard controller.
//
// Note: These values assume the original, unmodified settings
// on the MK3, which are user-modifiable using a PC/Mac based
// application. If user presets are in effect, some or all of
// the following may not apply.
//
typedef enum {

    // Vertical Sliders
    Mk3Fader1       = 71,   // Ctrl # (0-127)
    Mk3Fader2       = 72,   // Ctrl # (0-127)
    Mk3Fader3       = 73,   // Ctrl # (0-127)
    Mk3Fader4       = 74,   // Ctrl # (0-127)
    Mk3Fader5       = 75,   // Ctrl # (0-127)
    Mk3Fader6       = 76,   // Ctrl # (0-127)
    Mk3Fader7       = 77,   // Ctrl # (0-127)
    Mk3Fader8       = 78,   // Ctrl # (0-127)
    Mk3Fader9       = 79,   // Ctrl # (0-127) Master

    // 2-State buttons below sliders
    Mk3Toggle1      = 11,   // Ctrl # (0/127 on successive presses)
    Mk3Toggle2      = 12,   // Ctrl # (0/127 on successive presses)
    Mk3Toggle3      = 13,   // Ctrl # (0/127 on successive presses)
    Mk3Toggle4      = 14,   // Ctrl # (0/127 on successive presses)
    Mk3Toggle5      = 15,   // Ctrl # (0/127 on successive presses)
    Mk3Toggle6      = 16,   // Ctrl # (0/127 on successive presses)
    Mk3Toggle7      = 17,   // Ctrl # (0/127 on successive presses)
    Mk3Toggle8      = 18,   // Ctrl # (0/127 on successive presses)
    Mk3Toggle9      = 19,   // Ctrl # (0/127 on successive presses)

    // Rotational potentiometers
    Mk3Knob1        = 21,   // Ctrl # (0-127)
    Mk3Knob2        = 22,   // Ctrl # (0-127)
    Mk3Knob3        = 23,   // Ctrl # (0-127)
    Mk3Knob4        = 24,   // Ctrl # (0-127)
    Mk3Knob5        = 25,   // Ctrl # (0-127)
    Mk3Knob6        = 26,   // Ctrl # (0-127)
    Mk3Knob7        = 27,   // Ctrl # (0-127)
    Mk3Knob8        = 28,   // Ctrl # (0-127)

    // Transport buttons
    Mk3Play         = 115,   // Ctrl # (127 press, 0 release)
    Mk3Stop         = 116,   // Ctrl # (127 press, 0 release)
    Mk3Recd         = 117,   // Ctrl # (127 press, 0 release)
    Mk3Loop         = 118,   // Ctrl # (127 press, 0 release)

    // Pads (When User Custom4 in effect)
    Mk3Pad1         = 0,    // Prog Change
    Mk3Pad2         = 1,    // Prog Change
    Mk3Pad3         = 2,    // Prog Change
    Mk3Pad4         = 3,    // Prog Change
    Mk3Pad5         = 4,    // Prog Change
    Mk3Pad6         = 5,    // Prog Change
    Mk3Pad7         = 6,    // Prog Change
    Mk3Pad8         = 7,    // Prog Change
    Mk3Pad9         = 8,    // Prog Change
    Mk3Pad10        = 9,    // Prog Change
    Mk3Pad11        = 10,   // Prog Change
    Mk3Pad12        = 11,   // Prog Change
    Mk3Pad13        = 12,   // Prog Change
    Mk3Pad14        = 13,   // Prog Change
    Mk3Pad15        = 14,   // Prog Change
    Mk3Pad16        = 15,   // Prog Change

} LaunchKeyMk3Controls;

// Arturia MiniLab3 Control/Button Numbers
// (see "Docs/Components Info/Arturia Minilab3 Manual 1_0_5.pdf"
//
// MIDI values assigned to various knobs, sliders, and
// buttons on Arturia MiniLab3 MIDI controller.
//
// Note: These values assume the original, unmodified settings
// on the MiniLab3, which are user-modifiable using a PC/Mac based
// application. If user presets are in effect, some or all of
// the following may not apply.
//
typedef enum {

    // Vertical Sliders
    ML3Fader1       = 82,   // Ctrl #
    ML3Fader2       = 83,   // Ctrl #
    ML3Fader3       = 85,   // Ctrl #
    ML3Fader4       = 17,   // Ctrl #

    // Rotational potentiometers
    ML3Knob1        = 74,   // Ctrl #
    ML3Knob2        = 71,   // Ctrl #
    ML3Knob3        = 76,   // Ctrl #
    ML3Knob4        = 77,   // Ctrl #
    ML3Knob5        = 93,   // Ctrl #
    ML3Knob6        = 18,   // Ctrl #
    ML3Knob7        = 19,   // Ctrl #
    ML3Knob8        = 16,   // Ctrl #

    // Pads (When User Custom4 in effect)
    ML3Pad1         = 36,   // MIDI Note
    ML3Pad2         = 37,   // MIDI Note
    ML3Pad3         = 38,   // MIDI Note
    ML3Pad4         = 39,   // MIDI Note
    ML3Pad5         = 40,   // MIDI Note
    ML3Pad6         = 41,   // MIDI Note
    ML3Pad7         = 42,   // MIDI Note
    ML3Pad8         = 43,   // MIDI Note

} MiniLab3Controls;

/*******************************************
* Controller/Device Selection and Mappings *
********************************************
*
*  MIDI Input Device to MIDI Mappings
*  Device-Specific MIDI Settings for Selected Input Device
*
*/

// NOTE: When no controller is available for assignment, the constant
// MidiDataMax is added to the default assignment in order to 1)
// distinguish it from the default and 2) so that it will be ignored
// and not produce duplication errors in switch statements where it
// is used (for example if PAN_CONTROL and SUSTAIN control were both
// defined as -1 if when device control was available for these functions).
//
#if USING_LAUNCHKEY_MK3
    // MIDI Standard Control Mappings
    #define MIDI_CHANNEL        1
    #define MODWHEEL_CTRL       MidiDataNone    // Ignored
    #define VOLUME_CTRL         Mk3Fader9
    #define PAN_CTRL            MidiModulationWheel
    #define SUSTAIN_CTRL        MidiSustain
    #define SUSTAIN_CTRL_ALT    Mk3Toggle1
    #define ALLOFF_CTRL         MidiAllNotesOff
    #define ALLOFF_CTRL_ALT     Mk3Stop

    // Application-Specific Control Mappings
    #define SYNTH_MODFREQ_CTRL  Mk3Fader3
    #define SYNTH_MODAMT_CTRL   Mk3Fader4
    #define SYNTH_ATTACK_CTRL   Mk3Fader5
    #define SYNTH_DECAY_CTRL    Mk3Fader6
    #define SYNTH_SUSTAIN_CTRL  Mk3Fader7
    #define SYNTH_RELEASE_CTRL  Mk3Fader8

    // MK3 Pads Send Program Change 0-15 when using user preset 4
    #define MIDI_PADS_MIN       0
    #define MIDI_PADS_MAX       15

    // Application-Specific Program Change Mappings
    #define MIDI_PROG_MIN        0
    #define MIDI_PROG_MAX        15

    // Program Change shifted up in increments
    // of 16 when shift keys are pressed
    #define MIDI_PROG_SHIFT1_CTRL MidiShift1None
    #define MIDI_PROG_SHIFT2_CTRL MidiShift2None

#elif USING_LAUNCHKEY_25
    // MIDI Standard Mappings
    #define MIDI_CHANNEL        1
    #define MODWHEEL_CTRL       MidiModWheelNone
    #define VOLUME_CTRL         Lk25Fader
    #define PAN_CTRL            MidiModulationWheel
    #define SUSTAIN_CTRL        MidiSustain
    #define SUSTAIN_CTRL_ALT    MidiSustainNone
    #define ALLOFF_CTRL         Lk25Stop
    #define ALLOFF_CTRL_ALT     MidiAllOffNone
    #define IGNORE_NOTEOFF_CTRL Lk25Play

    // Application-Specific Mappings
    #define SYNTH_MODFREQ_CTRL  Lk25Knob1
    #define SYNTH_MODAMT_CTRL   Lk25Knob2
    #define SYNTH_ATTACK_CTRL   Lk25Knob3
    #define SYNTH_DECAY_CTRL    Lk25Knob4
    #define SYNTH_SUSTAIN_CTRL  Lk25Knob5
    #define SYNTH_RELEASE_CTRL  Lk25Knob6

    // Lk25 Pads Send Notes 36-51 on Channel 10
    #define MIDI_PADS_MIN       36
    #define MIDI_PADS_MAX       51

    // Application-Specific Program Change Mappings
    // Maximum of 64 presets (in AudioPlatform, only
    // 32 supported in rev 0.3.4.5; 64 eventually)
    #define MIDI_PROG_MIN       0
    #define MIDI_PROG_MAX       63

    // Program Change shifted up in increments
    // of 16 when "pad arrows" are pressed
    #define MIDI_PROG_SHIFT1_CTRL Lk25Pad18Arrow
    #define MIDI_PROG_SHIFT2_CTRL Lk25Pad916Arrow

#elif USING_MINILAB3
    // MIDI Standard Mappings
    #define MIDI_CHANNEL        1
    #define MODWHEEL_CTRL       MidiModWheelNone
    #define VOLUME_CTRL         MidiVolumeNone
    #define PAN_CTRL            MidiModulationWheel
    #define SUSTAIN_CTRL        MidiSustain
    #define SUSTAIN_CTRL_ALT    MidiSustainNone
    #define ALLOFF_CTRL         MidiAllNotesOff
    #define ALLOFF_CTRL_ALT     MidiAllOffNone

    // Application-Specific Mappings
    #define SYNTH_MODFREQ_CTRL  ML3Knob1
    #define SYNTH_MODAMT_CTRL   ML3Knob2
    #define SYNTH_ATTACK_CTRL   ML3Fader1
    #define SYNTH_DECAY_CTRL    ML3Fader2
    #define SYNTH_SUSTAIN_CTRL  ML3Fader3
    #define SYNTH_RELEASE_CTRL  ML3Fader4

    // Lk25 Pads Send Notes 36-51 on Channel 10
    #define MIDI_PADS_MIN        36
    #define MIDI_PADS_MAX        43

    // Application-Specific Program Change Mappings
    // ML3 pad values are sent as MIDI notes on channel 10,
    // and must be handled as a special case and remapped
    // to MIDI program change values.
    #define MIDI_PROG_MIN        ML3Pad1    // MIDI Note on Ch 10
    #define MIDI_PROG_MAX        ML3Pad8    // MIDI Note on Ch 10

    // Program Change shifted up in increments
    // of 16 when "pad arrows" are pressed
    #define MIDI_PROG_SHIFT1_CTRL MidiShift1None
    #define MIDI_PROG_SHIFT2_CTRL MidiShift2None

#elif USING_GENERIC_MIDI
    // MIDI Standard Mappings
    #define MIDI_CHANNEL        1
    #define MODWHEEL_CTRL       MidiModulationWheel
    #define VOLUME_CTRL         MidiChannelVolume
    #define PAN_CTRL            MidiModulationWheel
    #define SUSTAIN_CTRL        MidiSustain
    #define SUSTAIN_CTRL_ALT    MidiSustainNone
    #define ALLOFF_CTRL         MidiAllNotesOff
    #define ALLOFF_CTRL_ALT     MidiAllOffNone

    // Application-Specific Mappings
    #define SYNTH_MODFREQ_CTRL  ModFreqCtrlNone
    #define SYNTH_MODAMT_CTRL   ModAmpCtrlNone
    #define SYNTH_ATTACK_CTRL   ModAttackCtrlNone
    #define SYNTH_DECAY_CTRL    ModDecayCtrlNone
    #define SYNTH_SUSTAIN_CTRL  ModSustainCtrlNone
    #define SYNTH_RELEASE_CTRL  ModReleaseCtrlNone

    // MK3 Pads Send Program Change 0-15 when using user preset 4
    #define MIDI_PADS_MIN       0
    #define MIDI_PADS_MAX       15

    // Application-Specific Program Change Mappings
    #define MIDI_PROG_MIN       0
    #define MIDI_PROG_MAX       127

    // Program Change shifted up in increments
    // of 16 when "pad arrows" are pressed
    #define MIDI_PROG_SHIFT1_CTRL MidiShift1None
    #define MIDI_PROG_SHIFT2_CTRL MidiShift2None

#endif // USING_LAUNCHKEY_MK3 || USING_MINILAB3 || USING_GENERIC_MIDI

/********************
* MIDI Control Maps *
*********************
*/
/*
uint8_t MidiMapTemplate[256] = {

    MidiDataNone,   // 0 MidiBankSelect
    MidiDataNone,   // 1 MidiModulationWheel
    MidiDataNone,   // 2 MidiBreathController
    MidiDataNone,   // 3
    MidiDataNone,   // 4 MidiFootController
    MidiDataNone,   // 5 MidiPortamentoTime
    MidiDataNone,   // 6 MidiDataEntryMSB
    MidiDataNone,   // 7 MidiChannelVolume
    MidiDataNone,   // 8 MidiBalance
    MidiDataNone,   // 9
    MidiDataNone,   // 10 MidiPanControl
    MidiDataNone,   // 11 MidiExpressionController
    MidiDataNone,   // 12 MidiEffectControl1
    MidiDataNone,   // 13 MidiEffectControl2
    MidiDataNone,   // 14
    MidiDataNone,   // 15
    MidiDataNone,   // 16 MidiGeneralPurposeController1
    MidiDataNone,   // 17 MidiGeneralPurposeController2
    MidiDataNone,   // 18 MidiGeneralPurposeController3
    MidiDataNone,   // 19 MidiGeneralPurposeController4
    MidiDataNone,   // 20
    MidiDataNone,   // 21
    MidiDataNone,   // 22
    MidiDataNone,   // 23
    MidiDataNone,   // 24
    MidiDataNone,   // 25
    MidiDataNone,   // 26
    MidiDataNone,   // 27
    MidiDataNone,   // 28
    MidiDataNone,   // 29
    MidiDataNone,   // 20
    MidiDataNone,   // 30
    MidiDataNone,   // 31
    MidiDataNone,   // 32
    MidiDataNone,   // 33
    MidiDataNone,   // 34
    MidiDataNone,   // 35
    MidiDataNone,   // 36
    MidiDataNone,   // 37
    MidiDataNone,   // 38 MidiDataEntryLSB
    MidiDataNone,   // 39
    MidiDataNone,   // 40
    MidiDataNone,   // 41
    MidiDataNone,   // 42
    MidiDataNone,   // 43
    MidiDataNone,   // 44
    MidiDataNone,   // 45
    MidiDataNone,   // 46
    MidiDataNone,   // 47
    MidiDataNone,   // 48
    MidiDataNone,   // 49
    MidiDataNone,   // 50
    MidiDataNone,   // 51
    MidiDataNone,   // 52
    MidiDataNone,   // 53
    MidiDataNone,   // 54
    MidiDataNone,   // 55
    MidiDataNone,   // 56
    MidiDataNone,   // 57
    MidiDataNone,   // 58
    MidiDataNone,   // 59
    MidiDataNone,   // 60
    MidiDataNone,   // 61
    MidiDataNone,   // 62
    MidiDataNone,   // 63
    MidiDataNone,   // 64 MidiSustain
    MidiDataNone,   // 65 MidiPortamento
    MidiDataNone,   // 66 MidiSostenuto
    MidiDataNone,   // 67 MidiSoftPedal
    MidiDataNone,   // 68 MidiLegato
    MidiDataNone,   // 69 MidiHold
    MidiDataNone,   // 70 MidiSoundController1
    MidiDataNone,   // 71 MidiSoundController2
    MidiDataNone,   // 72 MidiSoundController3
    MidiDataNone,   // 73 MidiSoundController4
    MidiDataNone,   // 74 MidiSoundController5
    MidiDataNone,   // 75 MidiSoundController6
    MidiDataNone,   // 76 MidiSoundController7
    MidiDataNone,   // 77 MidiSoundController8
    MidiDataNone,   // 78 MidiSoundController9
    MidiDataNone,   // 79 MidiSoundController10
    MidiDataNone,   // 80 MidiGeneralPurposeController5
    MidiDataNone,   // 81 MidiGeneralPurposeController6
    MidiDataNone,   // 82 MidiGeneralPurposeController7
    MidiDataNone,   // 83 MidiGeneralPurposeController8
    MidiDataNone,   // 84 MidiPortamentoControl
    MidiDataNone,   // 85
    MidiDataNone,   // 86
    MidiDataNone,   // 87
    MidiDataNone,   // 88
    MidiDataNone,   // 89
    MidiDataNone,   // 90
    MidiDataNone,   // 91 MidiEffects1
    MidiDataNone,   // 92 MidiEffects2
    MidiDataNone,   // 93 MidiEffects3
    MidiDataNone,   // 94 MidiEffects4
    MidiDataNone,   // 95 MidiEffects5
    MidiDataNone,   // 96 MidiDataIncrement
    MidiDataNone,   // 97 MidiDataDecrement
    MidiDataNone,   // 98 MidiNRPNLSB
    MidiDataNone,   // 99 MidiNRPNMSB
    MidiDataNone,   // 100 MidiRPNLSB
    MidiDataNone,   // 101 MidiRPNMSB
    MidiDataNone,   // 102
    MidiDataNone,   // 103
    MidiDataNone,   // 104
    MidiDataNone,   // 105
    MidiDataNone,   // 106
    MidiDataNone,   // 107
    MidiDataNone,   // 108
    MidiDataNone,   // 109
    MidiDataNone,   // 110
    MidiDataNone,   // 111
    MidiDataNone,   // 112
    MidiDataNone,   // 113
    MidiDataNone,   // 114
    MidiDataNone,   // 115
    MidiDataNone,   // 116
    MidiDataNone,   // 117
    MidiDataNone,   // 118
    MidiDataNone,   // 119
    MidiDataNone,   // 120 MidiAllSoundOff
    MidiDataNone,   // 121 MidiResetAllControllers
    MidiDataNone,   // 122 MidiLocalControl
    MidiDataNone,   // 123 MidiAllNotesOff
    MidiDataNone,   // 124 MidiOmniModeOff
    MidiDataNone,   // 125 MidiOmniModeOn
    MidiDataNone,   // 126 MidiMonoModeOn
    MidiDataNone,   // 127 MidiPolyModeOn
    MidiDataNone,   // 128
};

uint8_t MidiMapLk25[256] = {

    MidiDataNone,   // 0 MidiBankSelect
    MidiDataNone,   // 1 MidiModulationWheel
    MidiDataNone,   // 2 MidiBreathController
    MidiDataNone,   // 3
    MidiDataNone,   // 4 MidiFootController
    MidiDataNone,   // 5 MidiPortamentoTime
    MidiDataNone,   // 6 MidiDataEntryMSB
    MidiDataNone,   // 7 MidiChannelVolume
    MidiDataNone,   // 8 MidiBalance
    MidiDataNone,   // 9
    MidiDataNone,   // 10 MidiPanControl
    MidiDataNone,   // 11 MidiExpressionController
    MidiDataNone,   // 12 MidiEffectControl1
    MidiDataNone,   // 13 MidiEffectControl2
    MidiDataNone,   // 14
    MidiDataNone,   // 15
    MidiDataNone,   // 16 MidiGeneralPurposeController1
    MidiDataNone,   // 17 MidiGeneralPurposeController2
    MidiDataNone,   // 18 MidiGeneralPurposeController3
    MidiDataNone,   // 19 MidiGeneralPurposeController4
    MidiDataNone,   // 20
    MidiDataNone,   // 21
    MidiDataNone,   // 22
    MidiDataNone,   // 23
    MidiDataNone,   // 24
    MidiDataNone,   // 25
    MidiDataNone,   // 26
    MidiDataNone,   // 27
    MidiDataNone,   // 28
    MidiDataNone,   // 29
    MidiDataNone,   // 20
    MidiDataNone,   // 30
    MidiDataNone,   // 31
    MidiDataNone,   // 32
    MidiDataNone,   // 33
    MidiDataNone,   // 34
    MidiDataNone,   // 35
    MidiDataNone,   // 36
    MidiDataNone,   // 37
    MidiDataNone,   // 38 MidiDataEntryLSB
    MidiDataNone,   // 39
    MidiDataNone,   // 40
    MidiDataNone,   // 41
    MidiDataNone,   // 42
    MidiDataNone,   // 43
    MidiDataNone,   // 44
    MidiDataNone,   // 45
    MidiDataNone,   // 46
    MidiDataNone,   // 47
    MidiDataNone,   // 48
    MidiDataNone,   // 49
    MidiDataNone,   // 50
    MidiDataNone,   // 51
    MidiDataNone,   // 52
    MidiDataNone,   // 53
    MidiDataNone,   // 54
    MidiDataNone,   // 55
    MidiDataNone,   // 56
    MidiDataNone,   // 57
    MidiDataNone,   // 58
    MidiDataNone,   // 59
    MidiDataNone,   // 60
    MidiDataNone,   // 61
    MidiDataNone,   // 62
    MidiDataNone,   // 63
    MidiDataNone,   // 64 MidiSustain
    MidiDataNone,   // 65 MidiPortamento
    MidiDataNone,   // 66 MidiSostenuto
    MidiDataNone,   // 67 MidiSoftPedal
    MidiDataNone,   // 68 MidiLegato
    MidiDataNone,   // 69 MidiHold
    MidiDataNone,   // 70 MidiSoundController1
    MidiDataNone,   // 71 MidiSoundController2
    MidiDataNone,   // 72 MidiSoundController3
    MidiDataNone,   // 73 MidiSoundController4
    MidiDataNone,   // 74 MidiSoundController5
    MidiDataNone,   // 75 MidiSoundController6
    MidiDataNone,   // 76 MidiSoundController7
    MidiDataNone,   // 77 MidiSoundController8
    MidiDataNone,   // 78 MidiSoundController9
    MidiDataNone,   // 79 MidiSoundController10
    MidiDataNone,   // 80 MidiGeneralPurposeController5
    MidiDataNone,   // 81 MidiGeneralPurposeController6
    MidiDataNone,   // 82 MidiGeneralPurposeController7
    MidiDataNone,   // 83 MidiGeneralPurposeController8
    MidiDataNone,   // 84 MidiPortamentoControl
    MidiDataNone,   // 85
    MidiDataNone,   // 86
    MidiDataNone,   // 87
    MidiDataNone,   // 88
    MidiDataNone,   // 89
    MidiDataNone,   // 90
    MidiDataNone,   // 91 MidiEffects1
    MidiDataNone,   // 92 MidiEffects2
    MidiDataNone,   // 93 MidiEffects3
    MidiDataNone,   // 94 MidiEffects4
    MidiDataNone,   // 95 MidiEffects5
    MidiDataNone,   // 96 MidiDataIncrement
    MidiDataNone,   // 97 MidiDataDecrement
    MidiDataNone,   // 98 MidiNRPNLSB
    MidiDataNone,   // 99 MidiNRPNMSB
    MidiDataNone,   // 100 MidiRPNLSB
    MidiDataNone,   // 101 MidiRPNMSB
    MidiDataNone,   // 102
    MidiDataNone,   // 103
    MidiDataNone,   // 104
    MidiDataNone,   // 105
    MidiDataNone,   // 106
    MidiDataNone,   // 107
    MidiDataNone,   // 108
    MidiDataNone,   // 109
    MidiDataNone,   // 110
    MidiDataNone,   // 111
    MidiDataNone,   // 112
    MidiDataNone,   // 113
    MidiDataNone,   // 114
    MidiDataNone,   // 115
    MidiDataNone,   // 116
    MidiDataNone,   // 117
    MidiDataNone,   // 118
    MidiDataNone,   // 119
    MidiDataNone,   // 120 MidiAllSoundOff
    MidiDataNone,   // 121 MidiResetAllControllers
    MidiDataNone,   // 122 MidiLocalControl
    MidiDataNone,   // 123 MidiAllNotesOff
    MidiDataNone,   // 124 MidiOmniModeOff
    MidiDataNone,   // 125 MidiOmniModeOn
    MidiDataNone,   // 126 MidiMonoModeOn
    MidiDataNone,   // 127 MidiPolyModeOn
    MidiDataNone,   // 128
};

uint8_t MidiMapMk3[256] = {

    MidiDataNone,   // 0 MidiBankSelect
    MidiDataNone,   // 1 MidiModulationWheel
    MidiDataNone,   // 2 MidiBreathController
    MidiDataNone,   // 3
    MidiDataNone,   // 4 MidiFootController
    MidiDataNone,   // 5 MidiPortamentoTime
    MidiDataNone,   // 6 MidiDataEntryMSB
    MidiDataNone,   // 7 MidiChannelVolume
    MidiDataNone,   // 8 MidiBalance
    MidiDataNone,   // 9
    MidiDataNone,   // 10 MidiPanControl
    MidiDataNone,   // 11 Mk3Toggle1
    MidiDataNone,   // 12 Mk3Toggle2
    MidiDataNone,   // 13 Mk3Toggle3
    MidiDataNone,   // 14 Mk3Toggle4
    MidiDataNone,   // 15 Mk3Toggle5
    MidiDataNone,   // 16 Mk3Toggle6
    MidiDataNone,   // 17 Mk3Toggle7
    MidiDataNone,   // 18 Mk3Toggle8
    MidiDataNone,   // 19 Mk3Toggle9
    MidiDataNone,   // 20
    MidiDataNone,   // 21 Mk3Knob1
    MidiDataNone,   // 22 Mk3Knob2
    MidiDataNone,   // 23 Mk3Knob3
    MidiDataNone,   // 24 Mk3Knob4
    MidiDataNone,   // 25 Mk3Knob5
    MidiDataNone,   // 26 Mk3Knob6
    MidiDataNone,   // 27 Mk3Knob7
    MidiDataNone,   // 28 Mk3Knob8
    MidiDataNone,   // 29
    MidiDataNone,   // 20
    MidiDataNone,   // 30
    MidiDataNone,   // 31
    MidiDataNone,   // 32
    MidiDataNone,   // 33
    MidiDataNone,   // 34
    MidiDataNone,   // 35
    MidiDataNone,   // 36
    MidiDataNone,   // 37
    MidiDataNone,   // 38 MidiDataEntryLSB
    MidiDataNone,   // 39
    MidiDataNone,   // 40
    MidiDataNone,   // 41
    MidiDataNone,   // 42
    MidiDataNone,   // 43
    MidiDataNone,   // 44
    MidiDataNone,   // 45
    MidiDataNone,   // 46
    MidiDataNone,   // 47
    MidiDataNone,   // 48
    MidiDataNone,   // 49
    MidiDataNone,   // 50
    MidiDataNone,   // 51
    MidiDataNone,   // 52
    MidiDataNone,   // 53
    MidiDataNone,   // 54
    MidiDataNone,   // 55
    MidiDataNone,   // 56
    MidiDataNone,   // 57
    MidiDataNone,   // 58
    MidiDataNone,   // 59
    MidiDataNone,   // 60
    MidiDataNone,   // 61
    MidiDataNone,   // 62
    MidiDataNone,   // 63
    MidiDataNone,   // 64 MidiSustain
    MidiDataNone,   // 65 MidiPortamento
    MidiDataNone,   // 66 MidiSostenuto
    MidiDataNone,   // 67 MidiSoftPedal
    MidiDataNone,   // 68 MidiLegato
    MidiDataNone,   // 69 MidiHold
    MidiDataNone,   // 70 MidiSoundController1
    MidiDataNone,   // 71 Mk3Fader1
    MidiDataNone,   // 72 Mk3Fader2
    MidiDataNone,   // 73 Mk3Fader3
    MidiDataNone,   // 74 Mk3Fader4
    MidiDataNone,   // 75 Mk3Fader5
    MidiDataNone,   // 76 Mk3Fader6
    MidiDataNone,   // 77 Mk3Fader7
    MidiDataNone,   // 78 Mk3Fader8
    MidiDataNone,   // 79 Mk3Fader9
    MidiDataNone,   // 80 MidiGeneralPurposeController5
    MidiDataNone,   // 81 MidiGeneralPurposeController6
    MidiDataNone,   // 82 MidiGeneralPurposeController7
    MidiDataNone,   // 83 MidiGeneralPurposeController8
    MidiDataNone,   // 84 MidiPortamentoControl
    MidiDataNone,   // 85
    MidiDataNone,   // 86
    MidiDataNone,   // 87
    MidiDataNone,   // 88
    MidiDataNone,   // 89
    MidiDataNone,   // 90
    MidiDataNone,   // 91 MidiEffects1
    MidiDataNone,   // 92 MidiEffects2
    MidiDataNone,   // 93 MidiEffects3
    MidiDataNone,   // 94 MidiEffects4
    MidiDataNone,   // 95 MidiEffects5
    MidiDataNone,   // 96 MidiDataIncrement
    MidiDataNone,   // 97 MidiDataDecrement
    MidiDataNone,   // 98 MidiNRPNLSB
    MidiDataNone,   // 99 MidiNRPNMSB
    MidiDataNone,   // 100 MidiRPNLSB
    MidiDataNone,   // 101 MidiRPNMSB
    MidiDataNone,   // 102
    MidiDataNone,   // 103
    MidiDataNone,   // 104
    MidiDataNone,   // 105
    MidiDataNone,   // 106
    MidiDataNone,   // 107
    MidiDataNone,   // 108
    MidiDataNone,   // 109
    MidiDataNone,   // 110
    MidiDataNone,   // 111
    MidiDataNone,   // 112
    MidiDataNone,   // 113
    MidiDataNone,   // 114
    MidiDataNone,   // 115 Mk3Play
    MidiDataNone,   // 116 Mk3Stop
    MidiDataNone,   // 117 Mk3Recd
    MidiDataNone,   // 118 Mk3Loop
    MidiDataNone,   // 119
    MidiDataNone,   // 120 MidiAllSoundOff
    MidiDataNone,   // 121 MidiResetAllControllers
    MidiDataNone,   // 122 MidiLocalControl
    MidiDataNone,   // 123 MidiAllNotesOff
    MidiDataNone,   // 124 MidiOmniModeOff
    MidiDataNone,   // 125 MidiOmniModeOn
    MidiDataNone,   // 126 MidiMonoModeOn
    MidiDataNone,   // 127 MidiPolyModeOn
    MidiDataNone,   // 128
};
*/

#endif // MIDIDEFINES_H_
