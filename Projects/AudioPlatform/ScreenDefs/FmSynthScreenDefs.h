/************************************************************************************
*                                FmSynthScreenDefs.h                                *
*                                                                                   *
*                       Copyright (c) 2025, Donald Swearingen                       *
*                                All Rights Reserved                                *
*************************************************************************************
*
* Description
*
*   Screen layout and definitions for the FmSynthScreen of the AudioPlatform application.
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
#ifndef FMSYNTHSCREENDEFS_H_
#define FMSYNTHSCREENDEFS_H_

/******************
* FM Synth Screen *
*******************
*
*/

DisplayScreenInfo fmSynthSettingsDisplayInfo = (DisplayScreenInfo) {
    DISPLAY_BGD_COLOR_DEFAULT,  // displayBgdColor
    PARAMBTN_SCRN_MARGIN_TOP,   // marginTop
    PARAMBTN_SCRN_MARGIN_BOTTOM,// marginBottom
    PARAMBTN_SCRN_MARGIN_LEFT,  // marginLeft
    PARAMBTN_SCRN_MARGIN_RIGHT, // marginRight
    HDR_FONT_DEFAULT,           // headerFont
    HDR_TEXT_COLOR_DEFAULT,     // headerFontColor
    SUBHDR_FONT_DEFAULT,        // subHeaderFont
    SUBHDR_TEXT_COLOR_DEFAULT,  // subHeaderFontColor
    "FM Synthesizer",           // screenHeader
    "8-Note Polyphony",         // screenSubHeader
    TextAlignLeft,              // textAlign
    true,                       // showPrevBtn
    "Home"                      // prevBtnText
};

#define SCRN_FMSYNTH_BTN_COUNT  12

ButtonPanelInfo fmSynthSettingsPanelInfo = (ButtonPanelInfo) {
    SCRN_FMSYNTH_BTN_COUNT,     // btnCount
    BTNS_PER_ROW_DEFAULT,       // btnsPerRow
    PARAMBTN_SCRN_BTN_WIDTH,    // btnWidth
    PARAMBTN_SCRN_BTN_HEIGHT,   // btnHeight
    PARAMBTN_SCRN_GAP_HORIZ,    // btnGapHorizontal
    PARAMBTN_SCRN_GAP_VERT,     // btnGapVertical
    BTN_PANEL_BGD_COLOR_DEFAULT,// panelBgdColor
    BTN_COLOR_DEFAULT,          // btnColor
    BTN_SEL_COLOR_DEFAULT,      // btnActiveColor
    BTN_FONT_DEFAULT,           // btnFont
    BTN_TEXT_COLOR_DEFAULT,     // btnTextColor
    BTN_SEL_TEXT_COLOR_DEFAULT, // btnActiveTextColor
    BTN_OUTLINE_COLOR_DEFAULT   // btnOutlineColor
};

// Parameter information structures associated with the screen buttons
ParamButtonInfo     fmSynthStatParams           = (ParamButtonInfo) {FmSynth_Status, BtnParam_Type_Text, "Synth Status", "", Device_Disabled, Device_Enabled, AudioPlatform::DeviceStatusLabels};
ParamButtonInfo     fmSynthLvlParams            = (ParamButtonInfo) {FmSynth_Level, BtnParam_Type_Float, "Synth Level", "dB", FMSYNTH_LEVEL_MIN, FMSYNTH_LEVEL_MAX};
ParamButtonInfo     fmSynthDlyParams            = (ParamButtonInfo) {FmSynth_DlySend, BtnParam_Type_Int, "Synth -> Delay", "%", PCT_TO_FLOAT(FMSYNTH_DLY_SEND_MIN), PCT_TO_FLOAT(FMSYNTH_DLY_SEND_MAX)};
ParamButtonInfo     fmSynthRvbParams            = (ParamButtonInfo) {FmSynth_RvbSend, BtnParam_Type_Int, "Synth -> Reverb", "%", PCT_TO_FLOAT(FMSYNTH_RVB_SEND_MIN), PCT_TO_FLOAT(FMSYNTH_RVB_SEND_MAX)};

ParamButtonInfo     fmSynthPresets              = (ParamButtonInfo) {FmSynth_PresetNum, BtnParam_Type_Text, "Voice Name", "", 0, FmSynthPresetCount-1, Presets::fmSynthPresetNames};
ParamButtonInfo     fmSynthNoteShiftParams      = (ParamButtonInfo) {FmSynth_NoteShift, BtnParam_Type_Int, "Note Shift",  "", NOTE_SHIFT_MIN_SYNTH, NOTE_SHIFT_MAX_SYNTH};
ParamButtonInfo     fmSynthNoteRangeMinParams   = (ParamButtonInfo) {FmSynth_NoteRangeMin, BtnParam_Type_Int, "Note Min", "", NOTE_RANGE_MIN_SYNTH, NOTE_RANGE_MAX_SYNTH};
ParamButtonInfo     fmSynthNoteRangeMaxParams   = (ParamButtonInfo) {FmSynth_NoteRangeMax, BtnParam_Type_Int, "Note Max", "", NOTE_RANGE_MIN_SYNTH, NOTE_RANGE_MAX_SYNTH};

ParamButtonInfo     fmSynthBendRange            = (ParamButtonInfo) {FmSynth_PitchBendRange, BtnParam_Type_Int, "PitchBend Range", "Semitones", 1.0, 12.0};
// NOTE: Pan-related constants defined in MidiDefines.h
ParamButtonInfo     fmSynthPanType              = (ParamButtonInfo) {FmSynth_PanType, BtnParam_Type_Text, "Pan Type", "", PanTypeMin, PanTypeMax, AudioPlatform::PanTypeLabels};
ParamButtonInfo     fmSynthPanParameter         = (ParamButtonInfo) {FmSynth_PanParameter, BtnParam_Type_Float, "Pan Parameter", "", PanParamMin, PanParamMax};

// Array of parameter information structures for screen buttons
ParamButtonInfo     fmSynthSettingsBtnParams[SCRN_FMSYNTH_BTN_COUNT] = {

                    fmSynthStatParams,
                    fmSynthLvlParams,
                    fmSynthDlyParams,
                    fmSynthRvbParams,

                    fmSynthPresets,
                    fmSynthNoteShiftParams,
                    fmSynthNoteRangeMinParams,
                    fmSynthNoteRangeMaxParams,

                    fmSynthBendRange,
                    fmSynthPanType,
                    fmSynthPanParameter,
                    dummyBtnParams,
};

// Encoder parameters for FM Synthesizer
RotaryParams        fmSynthStatRotaryParams         = {FMSYNTH_STATUS_DEFAULT, FMSYNTH_STATUS_MIN, FMSYNTH_STATUS_MAX, {1,1}, 2, 0, Presets::setFmSynthParam};
RotaryParams        fmSynthLvlRotaryParams          = {FMSYNTH_LEVEL_DEFAULT, DB_TO_ROTARY(FMSYNTH_LEVEL_MIN), DB_TO_ROTARY(FMSYNTH_LEVEL_MAX), {1, 2, 12, 24}, 4, 0, Presets::setFmSynthParam};
RotaryParams        fmSynthDlyRotaryParams          = {FMSYNTH_DLY_SEND_DEFAULT, FMSYNTH_DLY_SEND_MIN, FMSYNTH_DLY_SEND_MAX, {1, 5, 10}, 3, 0, Presets::setFmSynthParam};
RotaryParams        fmSynthRvbRotaryParams          = {FMSYNTH_RVB_SEND_DEFAULT, FMSYNTH_RVB_SEND_MIN, FMSYNTH_RVB_SEND_MAX, {1, 5, 10}, 3, 0, Presets::setFmSynthParam};

RotaryParams        fmSynthPresetsRotaryParams      = {0, 0, FMSYNTH_PRESET_NUM_MAX, {1, 5}, 2, 0, Presets::setFmSynthParam};
RotaryParams        fmSynthNoteShiftRotaryParams    = {NOTE_SHIFT_DEFAULT, NOTE_SHIFT_MIN_SYNTH, NOTE_SHIFT_MAX_SYNTH, {1,5,10}, 3, 0, Presets::setFmSynthParam};
RotaryParams        fmSynthNoteRangeMinRotaryParams = {NOTE_RANGE_MIN_SYNTH, NOTE_RANGE_MIN_SYNTH, NOTE_RANGE_MAX_SYNTH, {1,5,10}, 3, 0, Presets::setFmSynthParam};
RotaryParams        fmSynthNoteRangeMaxRotaryParams = {NOTE_RANGE_MAX_SYNTH, NOTE_RANGE_MIN_SYNTH, NOTE_RANGE_MAX_SYNTH, {1,5,10}, 3, 0, Presets::setFmSynthParam};

RotaryParams        fmSynthBendRotaryParams         = {FMSYNTH_PITCH_BEND_DEFAULT, FMSYNTH_PITCH_BEND_MIN, FMSYNTH_PITCH_BEND_MAX, {1,2}, 2, 0, Presets::setFmSynthParam};
// NOTE: Pan-related constants defined in MidiDefines.h
RotaryParams        fmSynthPanRotaryParams          = {PanTypeDefault, PanTypeMin, PanTypeMax, {1,1}, 2, 0, Presets::setFmSynthParam};
RotaryParams        fmSynthPanParamRotaryParams     = {PanParamDefault, PanParamMin, PanParamMax, {1,5,10}, 3, 0, Presets::setFmSynthParam};

// Array of rotary encoder parameter information for screen buttons
RotaryParams        fmSynthSettingsRotaryParams[SCRN_FMSYNTH_BTN_COUNT] = {

                    fmSynthStatRotaryParams,
                    fmSynthLvlRotaryParams,
                    fmSynthDlyRotaryParams,
                    fmSynthRvbRotaryParams,

                    fmSynthPresetsRotaryParams,
                    fmSynthNoteShiftRotaryParams,
                    fmSynthNoteRangeMinRotaryParams,
                    fmSynthNoteRangeMaxRotaryParams,

                    fmSynthBendRotaryParams,
                    fmSynthPanRotaryParams,
                    fmSynthPanParamRotaryParams,
                    dummyRotaryParams,
};

// FM Synth screen instance
FmSynthScreen       fmSynthScreen(fmSynthSettingsDisplayInfo, fmSynthSettingsPanelInfo, fmSynthSettingsBtnParams, fmSynthSettingsRotaryParams, InputDevices::encoders, deviceFmSynth, Presets::presetsCallback);

#endif // FMSYNTHSCREENDEFS_H_
