/************************************************************************************
*                              BasicSynthScreenDefs.h                               *
*                                                                                   *
*                       Copyright (c) 2025, Donald Swearingen                       *
*                                All Rights Reserved                                *
*************************************************************************************
*
* Description
*
*   Screen layout and definitions for the BasicSynthScreen of the AudioPlatform application.
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
#ifndef BASICSYNTHSCREENDEFS_H_
#define BASICSYNTHSCREENDEFS_H_

/********************
* BasicSynth Screen *
*********************
*
*/

DisplayScreenInfo basicSynthDisplayInfo = (DisplayScreenInfo) {
    DISPLAY_BGD_COLOR_DEFAULT,  // displayBgdColor
    PARAMBTN_SCRN_MARGIN_TOP,   // marginTop
    PARAMBTN_SCRN_MARGIN_BOTTOM,// marginBottom
    PARAMBTN_SCRN_MARGIN_LEFT,  // marginLeft
    PARAMBTN_SCRN_MARGIN_RIGHT, // marginRight
    HDR_FONT_DEFAULT,           // headerFont
    HDR_TEXT_COLOR_DEFAULT,     // headerFontColor
    SUBHDR_FONT_DEFAULT,        // subHeaderFont
    SUBHDR_TEXT_COLOR_DEFAULT,  // subHeaderFontColor
    "Basic Synthesizer",        // screenHeader
    "12 Voices",                // screenSubHeader
    TextAlignLeft,              // textAlign
    true,                       // showPrevBtn
    "Home"                      // prevBtnText
};

#define SCRN_BASICSYNTH_BTN_COUNT        20

ButtonPanelInfo basicSynthPanelInfo = (ButtonPanelInfo) {
    SCRN_BASICSYNTH_BTN_COUNT,  // btnCount
    BTNS_PER_ROW_DEFAULT,       // btnsPerRow
    PARAMBTN_SCRN_BTN_WIDTH,    // btnWidth
    45,                         // btnHeight
//    PARAMBTN_SCRN_BTN_HEIGHT,   // btnHeight
    PARAMBTN_SCRN_GAP_HORIZ,    // btnGapHorizontal
    8,                          // btnGapVertical
//    PARAMBTN_SCRN_GAP_VERT,     // btnGapVertical
    BTN_PANEL_BGD_COLOR_DEFAULT,// panelBgdColor
    BTN_COLOR_DEFAULT,          // btnColor
    BTN_SEL_COLOR_DEFAULT,      // btnActiveColor
    BTN_FONT_DEFAULT,           // btnFont
    BTN_TEXT_COLOR_DEFAULT,     // btnTextColor
    BTN_SEL_TEXT_COLOR_DEFAULT, // btnActiveTextColor
    BTN_OUTLINE_COLOR_DEFAULT   // btnOutlineColor
};

// Parameter information structures associated with the screen buttons
ParamButtonInfo     basicSynthStatParams            = (ParamButtonInfo) {BasicSynth_Status, BtnParam_Type_Text, "Synth Status", "", Device_Disabled, Device_Enabled, AudioPlatform::DeviceStatusLabels};
ParamButtonInfo     basicSynthLvlParams             = (ParamButtonInfo) {BasicSynth_Level, BtnParam_Type_Float, "Synth Level", "dB", BASICSYNTH_LEVEL_MIN, BASICSYNTH_LEVEL_MAX};
ParamButtonInfo     basicSynthDlyParams             = (ParamButtonInfo) {BasicSynth_DlySend, BtnParam_Type_Int, "Synth -> Delay", "%", PCT_TO_FLOAT(BASICSYNTH_DLY_SEND_MIN), PCT_TO_FLOAT(BASICSYNTH_DLY_SEND_MAX)};
ParamButtonInfo     basicSynthRvbParams             = (ParamButtonInfo) {BasicSynth_RvbSend, BtnParam_Type_Int, "Synth -> Reverb", "%", PCT_TO_FLOAT(BASICSYNTH_RVB_SEND_MIN), PCT_TO_FLOAT(BASICSYNTH_RVB_SEND_MAX)};

ParamButtonInfo     basicSynthTypeParams            = (ParamButtonInfo) {BasicSynth_Type, BtnParam_Type_Text, "Synth Type", "", 0.0, 4.0, AudioPlatform::SynthTypeLabels};
ParamButtonInfo     basicSynthNoteShiftParams       = (ParamButtonInfo) {BasicSynth_NoteShift, BtnParam_Type_Int, "Note Shift",  "", NOTE_SHIFT_MIN_SYNTH, NOTE_SHIFT_MAX_SYNTH};
ParamButtonInfo     basicSynthNoteRangeMinParams    = (ParamButtonInfo) {BasicSynth_NoteRangeMin, BtnParam_Type_Int, "Note Min", "", NOTE_RANGE_MIN_SYNTH, NOTE_RANGE_MAX_SYNTH};
ParamButtonInfo     basicSynthNoteRangeMaxParams    = (ParamButtonInfo) {BasicSynth_NoteRangeMax, BtnParam_Type_Int, "Note Max", "", NOTE_RANGE_MIN_SYNTH, NOTE_RANGE_MAX_SYNTH};

ParamButtonInfo     basicSynthAttackParams          = (ParamButtonInfo) {BasicSynth_Attack, BtnParam_Type_Int, "Attack", "ms", (float)BASICSYNTH_ATTACK_MIN, (float)BASICSYNTH_ATTACK_MAX};
ParamButtonInfo     basicSynthDecayParams           = (ParamButtonInfo) {BasicSynth_Decay, BtnParam_Type_Int, "Decay", "ms", (float)BASICSYNTH_DECAY_MIN, (float)BASICSYNTH_DECAY_MAX};
ParamButtonInfo     basicSynthSustainParams         = (ParamButtonInfo) {BasicSynth_Sustain, BtnParam_Type_Int, "Sustain", "%", PCT_TO_FLOAT(BASICSYNTH_SUSTAIN_MIN), PCT_TO_FLOAT(BASICSYNTH_SUSTAIN_MAX)};
ParamButtonInfo     basicSynthReleaseParams         = (ParamButtonInfo) {BasicSynth_Release, BtnParam_Type_Int, "Release", "ms", (float)BASICSYNTH_RELEASE_MIN, (float)BASICSYNTH_RELEASE_MAX};

ParamButtonInfo     basicSynthModFreqParams         = (ParamButtonInfo) {BasicSynth_ModFreq, BtnParam_Type_Int, "Mod Freq", "Hz", (float)BASICSYNTH_MOD_FREQ_MIN, (float)BASICSYNTH_MOD_FREQ_MAX};
ParamButtonInfo     basicSynthModAmtParams          = (ParamButtonInfo) {BasicSynth_ModAmt, BtnParam_Type_Int, "Mod Amt", "%", PCT_TO_FLOAT(BASICSYNTH_MOD_AMT_MIN), PCT_TO_FLOAT(BASICSYNTH_MOD_AMT_MAX)};
// NOTE: Pan-related constants defined in MidiDefines.h
ParamButtonInfo     basicSynthPanType               = (ParamButtonInfo) {BasicSynth_PanType, BtnParam_Type_Text, "Pan Type", "", PanTypeMin, PanTypeMax, AudioPlatform::PanTypeLabels};
ParamButtonInfo     basicSynthPanParameter          = (ParamButtonInfo) {BasicSynth_PanParameter, BtnParam_Type_Float, "Pan Parameter", "", PanParamMin, PanParamMax};

// Array of parameter information structures for screen buttons
ParamButtonInfo     basicSynthBtnParams[SCRN_BASICSYNTH_BTN_COUNT] = {
                    basicSynthStatParams,
                    basicSynthLvlParams,
                    basicSynthDlyParams,
                    basicSynthRvbParams,

                    basicSynthTypeParams,
                    basicSynthNoteShiftParams,
                    basicSynthNoteRangeMinParams,
                    basicSynthNoteRangeMaxParams,

                    basicSynthAttackParams,
                    basicSynthDecayParams,
                    basicSynthSustainParams,
                    basicSynthReleaseParams,

                    basicSynthModFreqParams,
                    basicSynthModAmtParams,
                    basicSynthPanType,
                    basicSynthPanParameter,

                    dummyBtnParams,
                    dummyBtnParams,
                    dummyBtnParams,
                    dummyBtnParams
};

// Encoder parameters for Basic Synthesizer
RotaryParams        basicSynthStatRotaryParams          = {BASICSYNTH_STATUS_DEFAULT, BASICSYNTH_STATUS_MIN, BASICSYNTH_STATUS_MAX, {1,1}, 2, 0, Presets::setBasicSynthParam};
RotaryParams        basicSynthLvlRotaryParams           = {BASICSYNTH_LEVEL_DEFAULT, DB_TO_ROTARY(BASICSYNTH_LEVEL_MIN), DB_TO_ROTARY(BASICSYNTH_LEVEL_MAX), {1, 2, 12, 24}, 4, 0, Presets::setBasicSynthParam};
RotaryParams        basicSynthDlyRotaryParams           = {BASICSYNTH_DLY_SEND_DEFAULT, BASICSYNTH_DLY_SEND_MIN, BASICSYNTH_DLY_SEND_MAX, {1, 5, 10}, 3, 0, Presets::setBasicSynthParam};
RotaryParams        basicSynthRvbRotaryParams           = {BASICSYNTH_RVB_SEND_DEFAULT, BASICSYNTH_RVB_SEND_MIN, BASICSYNTH_RVB_SEND_MAX, {1, 5, 10}, 3, 0, Presets::setBasicSynthParam};

RotaryParams        basicSynthTypeRotaryParams          = {BASICSYNTH_TYPE_DEFAULT, BASICSYNTH_TYPE_MIN, BASICSYNTH_TYPE_MAX, {1,1}, 2, 0, Presets::setBasicSynthParam};
RotaryParams        basicSynthNoteShiftRotaryParams     = {NOTE_SHIFT_DEFAULT, NOTE_SHIFT_MIN_SYNTH, NOTE_SHIFT_MAX_SYNTH, {1,5,10}, 3, 0, Presets::setBasicSynthParam};
RotaryParams        basicSynthNoteRangeMinRotaryParams  = {NOTE_RANGE_MIN_SYNTH, NOTE_RANGE_MIN_SYNTH, NOTE_RANGE_MAX_SYNTH, {1,5,10}, 3, 0, Presets::setBasicSynthParam};
RotaryParams        basicSynthNoteRangeMaxRotaryParams  = {NOTE_RANGE_MAX_SYNTH, NOTE_RANGE_MIN_SYNTH, NOTE_RANGE_MAX_SYNTH, {1,5,10}, 3, 0, Presets::setBasicSynthParam};

RotaryParams        basicSynthAttackRotaryParams        = {BASICSYNTH_ATTACK_DEFAULT, BASICSYNTH_ATTACK_MIN, BASICSYNTH_ATTACK_MAX, {1, 5, 10, 50, 100}, 5, 0, Presets::setBasicSynthParam};
RotaryParams        basicSynthDecayRotaryParams         = {BASICSYNTH_DECAY_DEFAULT, BASICSYNTH_DECAY_MIN, BASICSYNTH_DECAY_MAX, {1, 5, 10, 50, 100}, 5, 0, Presets::setBasicSynthParam};
RotaryParams        basicSynthSustainRotaryParams       = {BASICSYNTH_SUSTAIN_DEFAULT, BASICSYNTH_SUSTAIN_MIN, BASICSYNTH_SUSTAIN_MAX, {1, 5, 10}, 3, 0, Presets::setBasicSynthParam};
RotaryParams        basicSynthReleaseRotaryParams       = {BASICSYNTH_RELEASE_DEFAULT, BASICSYNTH_RELEASE_MIN, BASICSYNTH_RELEASE_MAX, {1, 5, 10, 50, 100}, 5, 0, Presets::setBasicSynthParam};

RotaryParams        basicSynthModFreqRotaryParams       = {BASICSYNTH_MOD_FREQ_DEFAULT, BASICSYNTH_MOD_FREQ_MIN, BASICSYNTH_MOD_FREQ_MAX, {1, 5, 10}, 3, 0, Presets::setBasicSynthParam};
RotaryParams        basicSynthModAmtRotaryParams        = {BASICSYNTH_MOD_AMT_DEFAULT, BASICSYNTH_MOD_AMT_MIN, BASICSYNTH_MOD_AMT_MAX, {1, 5, 10}, 3, 0, Presets::setBasicSynthParam};
// NOTE: Pan-related constants defined in MidiDefines.h
RotaryParams        basicSynthPanRotaryParams           = {PanTypeDefault, PanTypeMin, PanTypeMax, {1,1}, 2, 0, Presets::setBasicSynthParam};
RotaryParams        basicSynthPanParamRotaryParams      = {PanParamDefault, PanParamMin, PanParamMax, {1,5,10}, 3, 0, Presets::setBasicSynthParam};

// Array of rotary encoder parameter information for screen buttons
RotaryParams        basicSynthRotaryParams[SCRN_BASICSYNTH_BTN_COUNT] = {
                    basicSynthStatRotaryParams,
                    basicSynthLvlRotaryParams,
                    basicSynthDlyRotaryParams,
                    basicSynthRvbRotaryParams,

                    basicSynthTypeRotaryParams,
                    basicSynthNoteShiftRotaryParams,
                    basicSynthNoteRangeMinRotaryParams,
                    basicSynthNoteRangeMaxRotaryParams,

                    basicSynthAttackRotaryParams,
                    basicSynthDecayRotaryParams,
                    basicSynthSustainRotaryParams,
                    basicSynthReleaseRotaryParams,

                    basicSynthModFreqRotaryParams,
                    basicSynthModAmtRotaryParams,
                    basicSynthPanRotaryParams,
                    basicSynthPanParamRotaryParams,

                    dummyRotaryParams,
                    dummyRotaryParams,
                    dummyRotaryParams,
                    dummyRotaryParams
};

// Basic Synthesizer settings screen instance
ParamButtonScreen   basicSynthScreen(basicSynthDisplayInfo, basicSynthPanelInfo, basicSynthBtnParams, basicSynthRotaryParams, InputDevices::encoders, deviceBasicSynth, Presets::presetsCallback);


#endif // BASICSYNTHSCREENDEFS_H_
