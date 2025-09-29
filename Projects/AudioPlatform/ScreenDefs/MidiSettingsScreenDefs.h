/************************************************************************************
*                             MidiSettingsScreenDefs.h                              *
*                                                                                   *
*                       Copyright (c) 2025, Donald Swearingen                       *
*                                All Rights Reserved                                *
*************************************************************************************
*
* Description
*
*   Screen layout and definitions for the MidiSettingsScreen of the AudioPlatform application.
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
#ifndef MIDISETTINGSSCREENDEFS_H_
#define MIDISETTINGSSCREENDEFS_H_

/***********************
* MIDI Settings Screen *
************************
*
*/

DisplayScreenInfo midiSettingsDisplayInfo = (DisplayScreenInfo) {
    DISPLAY_BGD_COLOR_DEFAULT,  // displayBgdColor
    PARAMBTN_SCRN_MARGIN_TOP,   // marginTop
    PARAMBTN_SCRN_MARGIN_BOTTOM,// marginBottom
    PARAMBTN_SCRN_MARGIN_LEFT,  // marginLeft
    PARAMBTN_SCRN_MARGIN_RIGHT, // marginRight
    HDR_FONT_DEFAULT,           // headerFont
    HDR_TEXT_COLOR_DEFAULT,     // headerFontColor
    SUBHDR_FONT_DEFAULT,        // subHeaderFont
    SUBHDR_TEXT_COLOR_DEFAULT,  // subHeaderFontColor
    "MIDI Settings",            // screenHeader
    "For Future Development",   // screenSubHeader
    TextAlignLeft,              // textAlign
    true,                       // showPrevBtn
    "Back"                      // prevBtnText
};

#define SCRN_MIDI_BTN_COUNT     16

ButtonPanelInfo midiSettingsPanelInfo = (ButtonPanelInfo) {
    SCRN_MIDI_BTN_COUNT,        // btnCount
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

ParamButtonInfo     MidiWavPlyrChanParams       = (ParamButtonInfo) {Midi_Chan_WavPlayer,   BtnParam_Type_Int,  "Wav Player Chan",  "", 0, 16};
ParamButtonInfo     MidiRawPlyrChanParams       = (ParamButtonInfo) {Midi_Chan_RawPlayer,   BtnParam_Type_Int,  "Raw Player Chan",  "", 0, 16};
ParamButtonInfo     MidiBasicSynthChanParams    = (ParamButtonInfo) {Midi_Chan_BasicSynth,  BtnParam_Type_Int,  "BasicSynth Chan",  "", 0, 16};
ParamButtonInfo     MidiFmSynthChanParams       = (ParamButtonInfo) {Midi_Chan_FmSynth,     BtnParam_Type_Int,  "FmSynth Chan",     "", 0, 16};
ParamButtonInfo     MidiDeviceParams            = (ParamButtonInfo) {Midi_Input_Device,     BtnParam_Type_Text, "MIDI In Device",   "", MidiInDevice_Generic, MidiInDevice_ML3, AudioPlatform::MidiInDeviceLabels};

// Array of parameter information structures for screen buttons
ParamButtonInfo     midiSettingsBtnParams[SCRN_MIDI_BTN_COUNT] = {
                    MidiWavPlyrChanParams,
                    MidiRawPlyrChanParams,
                    MidiBasicSynthChanParams,
                    MidiFmSynthChanParams,
                    MidiDeviceParams,
                    dummyBtnParams,
                    dummyBtnParams,
                    dummyBtnParams,
                    dummyBtnParams,
                    dummyBtnParams,
                    dummyBtnParams,
                    dummyBtnParams,
                    dummyBtnParams,
                    dummyBtnParams,
                    dummyBtnParams,
                    dummyBtnParams
};

RotaryParams        MidiWavPlyrRotaryParams     = {0, 0, 16, {1,1}, 2, 0, Presets::setMidiParam};
RotaryParams        MidiRawPlyrRotaryParams     = {0, 0, 16, {1,1}, 2, 0, Presets::setMidiParam};
RotaryParams        MidiBasicSynthRotaryParams  = {0, 0, 16, {1,1}, 2, 0, Presets::setMidiParam};
RotaryParams        MidiWavFmSynthRotaryParams  = {0, 0, 16, {1,1}, 2, 0, Presets::setMidiParam};
RotaryParams        MidiDeviceRotaryParams      = {0, MidiInDevice_Generic, MidiInDevice_ML3, {1,1}, 2, 0, Presets::setMidiParam};

// Array of rotary encoder parameter information for screen buttons
RotaryParams        midiSettingsRotaryParams[SCRN_MIDI_BTN_COUNT] = {
                    MidiWavPlyrRotaryParams,
                    MidiRawPlyrRotaryParams,
                    MidiBasicSynthRotaryParams,
                    MidiWavFmSynthRotaryParams,
                    MidiDeviceRotaryParams,
                    dummyRotaryParams,
                    dummyRotaryParams,
                    dummyRotaryParams,
                    dummyRotaryParams,
                    dummyRotaryParams,
                    dummyRotaryParams,
                    dummyRotaryParams,
                    dummyRotaryParams,
                    dummyRotaryParams,
                    dummyRotaryParams,
                    dummyRotaryParams
};

// MIDI Settings screen instance
ParamButtonScreen   midiSettingsScreen(midiSettingsDisplayInfo, midiSettingsPanelInfo, midiSettingsBtnParams, midiSettingsRotaryParams, InputDevices::encoders, deviceMidi, Presets::presetsCallback);

#endif // MIDISETTINGSSCREENDEFS_H_
