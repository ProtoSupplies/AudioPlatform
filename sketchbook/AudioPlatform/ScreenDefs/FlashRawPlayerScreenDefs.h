/************************************************************************************
*                               FlashRawPlayerScreenDefs.h                          *
*                                                                                   *
*                       Copyright (c) 2025, Donald Swearingen                       *
*                                All Rights Reserved                                *
*************************************************************************************
*
* Description
*
*   Screen layout and definitions for the FlashRawPlayerScreen of the AudioPlatform application.
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
#ifndef FLASHRAWPLAYERSCREENDEFS_H_
#define FLASHRAWPLAYERSCREENDEFS_H_

/**********************
* Media Player Screen *
***********************
*
*/

DisplayScreenInfo flashRawPlayerDisplayInfo = (DisplayScreenInfo) {
    DISPLAY_BGD_COLOR_DEFAULT,  // displayBgdColor
    PARAMBTN_SCRN_MARGIN_TOP,   // marginTop
    PARAMBTN_SCRN_MARGIN_BOTTOM,// marginBottom
    PARAMBTN_SCRN_MARGIN_LEFT,  // marginLeft
    PARAMBTN_SCRN_MARGIN_RIGHT, // marginRight
    HDR_FONT_DEFAULT,           // headerFont
    HDR_TEXT_COLOR_DEFAULT,     // headerFontColor
    SUBHDR_FONT_DEFAULT,        // subHeaderFont
    SUBHDR_TEXT_COLOR_DEFAULT,  // subHeaderFontColor
    "Raw File Player",          // screenHeader
    "Serial Flash Raw Files",   // screenSubHeader
    TextAlignLeft,              // textAlign
    true,                       // showPrevBtn
    "Home"                      // prevBtnText
};

#define SCRN_RAWPLAYER_BTN_COUNT    12

ButtonPanelInfo flashRawPlayerPanelInfo = (ButtonPanelInfo) {
    SCRN_RAWPLAYER_BTN_COUNT,   // btnCount
    BTNS_PER_ROW_DEFAULT,       // btnsPerRow
    PARAMBTN_SCRN_BTN_WIDTH,    // btnWidth
    PARAMBTN_SCRN_BTN_HEIGHT,   // btnHeight
    PARAMBTN_SCRN_GAP_HORIZ,    // btnGapHorizontal
    PARAMBTN_SCRN_GAP_VERT,     // btnGapVertical
    COLOR_BLACK,                // panelBgdColor
    BTN_COLOR_DEFAULT,          // btnColor
    BTN_SEL_COLOR_DEFAULT,      // btnActiveColor
    BTN_FONT_DEFAULT,           // btnFont
    COLOR_BLACK,                // btnTextColor
    BTN_SEL_TEXT_COLOR_DEFAULT, // btnActiveTextColor
    BTN_OUTLINE_COLOR_DEFAULT   // btnOutlineColor
};

// Parameter information structures associated with the screen buttons
// TODO: How to handle variable text more efficiently
ParamButtonInfo     flashRawStatParams          = (ParamButtonInfo) {MediaPlayRaw_Status,  BtnParam_Type_Text, "Player Status", "", Device_Disabled, Device_Enabled, AudioPlatform::DeviceStatusLabels};
ParamButtonInfo     flashRawLvlParams           = (ParamButtonInfo) {MediaPlayRaw_Level,   BtnParam_Type_Float, "Player Level", "dB", FLASHRAWPLAY_LEVEL_MIN, FLASHRAWPLAY_LEVEL_MAX};
ParamButtonInfo     flashRawDlyParams           = (ParamButtonInfo) {MediaPlayRaw_DlySend, BtnParam_Type_Int, "Player -> Delay", "%",  PCT_TO_FLOAT(FLASHRAWPLAY_DLY_SEND_MIN), PCT_TO_FLOAT(FLASHRAWPLAY_DLY_SEND_MAX)};
ParamButtonInfo     flashRawRvbParams           = (ParamButtonInfo) {MediaPlayRaw_RvbSend, BtnParam_Type_Int, "Player -> Reverb", "%", PCT_TO_FLOAT(FLASHRAWPLAY_RVB_SEND_MIN), PCT_TO_FLOAT(FLASHRAWPLAY_RVB_SEND_MAX)};

ParamButtonInfo     flashRawSoundBankParams     = (ParamButtonInfo) {MediaPlayRaw_SoundBank, BtnParam_Type_Int, "Sound Bank",  "", SOUND_BANK_MIN, SOUND_BANK_MAX};
ParamButtonInfo     flashRawNoteShiftParams     = (ParamButtonInfo) {MediaPlayRaw_NoteShift, BtnParam_Type_Int, "Note Shift",  "", NOTE_SHIFT_MIN_PLYR, NOTE_SHIFT_MAX_PLYR};
ParamButtonInfo     flashRawNoteRangeMinParams  = (ParamButtonInfo) {MediaPlayRaw_NoteRangeMin, BtnParam_Type_Int, "Note Min", "", NOTE_RANGE_MIN_PLYR, NOTE_RANGE_MAX_PLYR};
ParamButtonInfo     flashRawNoteRangeMaxParams  = (ParamButtonInfo) {MediaPlayRaw_NoteRangeMax, BtnParam_Type_Int, "Note Max", "", NOTE_RANGE_MIN_PLYR, NOTE_RANGE_MAX_PLYR};

// NOTE: Pan-related constants defined in MidiDefines.h

ParamButtonInfo     flashRawPanType             = (ParamButtonInfo) {MediaPlayRaw_PanType, BtnParam_Type_Text, "Pan Type", "", PanTypeMin, PanTypeMax, AudioPlatform::PanTypeLabels};
ParamButtonInfo     flashRawPanParameter        = (ParamButtonInfo) {MediaPlayRaw_PanParameter, BtnParam_Type_Float, "Pan Parameter", "", PanParamMin, PanParamMax};

// Array of parameter information structures for screen buttons
ParamButtonInfo     flashRawPlayerBtnParams[SCRN_RAWPLAYER_BTN_COUNT] = {
                    flashRawStatParams,
                    flashRawLvlParams,
                    flashRawDlyParams,
                    flashRawRvbParams,

                    flashRawSoundBankParams,
                    flashRawNoteShiftParams,
                    flashRawNoteRangeMinParams,
                    flashRawNoteRangeMaxParams,

                    flashRawPanType,
                    flashRawPanParameter,
                    dummyBtnParams,
                    dummyBtnParams
};

// Encoder parameters for Flash Raw Player
RotaryParams        flashRawStatRotaryParams            = {FLASHRAWPLAY_STATUS_DEFAULT, FLASHRAWPLAY_STATUS_MIN, FLASHRAWPLAY_STATUS_MAX, {1,1}, 2, 0, Presets::setRawPlayerParam};
RotaryParams        flashRawLvlRotaryParams             = {FLASHRAWPLAY_LEVEL_DEFAULT, DB_TO_ROTARY(FLASHRAWPLAY_LEVEL_MIN), DB_TO_ROTARY(FLASHRAWPLAY_LEVEL_MAX), {1, 2, 12, 24}, 4, 0, Presets::setRawPlayerParam};
RotaryParams        flashRawDlyRotaryParams             = {FLASHRAWPLAY_DLY_SEND_DEFAULT, FLASHRAWPLAY_DLY_SEND_MIN, FLASHRAWPLAY_DLY_SEND_MAX, {1, 5, 10}, 3, 0, Presets::setRawPlayerParam};
RotaryParams        flashRawRvbRotaryParams             = {FLASHRAWPLAY_RVB_SEND_DEFAULT, FLASHRAWPLAY_RVB_SEND_MIN, FLASHRAWPLAY_RVB_SEND_MAX, {1, 5, 10}, 3, 0, Presets::setRawPlayerParam};

RotaryParams        flashRawSoundBankRotaryParams       = {SOUND_BANK_DEFAULT, SOUND_BANK_MIN, SOUND_BANK_MAX, {1,1}, 2, 0, Presets::setRawPlayerParam};
RotaryParams        flashRawNoteShiftRotaryParams       = {NOTE_SHIFT_DEFAULT, NOTE_SHIFT_MIN_PLYR, NOTE_SHIFT_MAX_PLYR, {1,5,10}, 3, 0, Presets::setRawPlayerParam};
RotaryParams        flashRawNoteRangeMinRotaryParams    = {NOTE_RANGE_MIN_PLYR, NOTE_RANGE_MIN_PLYR, NOTE_RANGE_MAX_PLYR, {1,5,10}, 3, 0, Presets::setRawPlayerParam};
RotaryParams        flashRawNoteRangeMaxRotaryParams    = {NOTE_RANGE_MAX_PLYR, NOTE_RANGE_MIN_PLYR, NOTE_RANGE_MAX_PLYR, {1,5,10}, 3, 0, Presets::setRawPlayerParam};

// NOTE: Pan-related constants defined in MidiDefines.h
RotaryParams        flashRawPanRotaryParams             = {PanTypeDefault, PanTypeMin, PanTypeMax, {1,1}, 2, 0, Presets::setRawPlayerParam};
RotaryParams        flashRawPanParamRotaryParams        = {PanParamDefault, PanParamMin, PanParamMax, {1,5,10}, 3, 0, Presets::setRawPlayerParam};

// Array of rotary encoder parameter information for screen buttons
RotaryParams        flashRawPlayerRotaryParams[SCRN_RAWPLAYER_BTN_COUNT] = {
                    flashRawStatRotaryParams,
                    flashRawLvlRotaryParams,
                    flashRawDlyRotaryParams,
                    flashRawRvbRotaryParams,

                    flashRawSoundBankRotaryParams,
                    flashRawNoteShiftRotaryParams,
                    flashRawNoteRangeMinRotaryParams,
                    flashRawNoteRangeMaxRotaryParams,

                    flashRawPanRotaryParams,
                    flashRawPanParamRotaryParams,
                    dummyRotaryParams,
                    dummyRotaryParams
};

// Flash RAW Player settings screen instance
ParamButtonScreen   flashRawPlayerScreen(flashRawPlayerDisplayInfo, flashRawPlayerPanelInfo, flashRawPlayerBtnParams, flashRawPlayerRotaryParams, InputDevices::encoders, deviceRawPlayer, Presets::presetsCallback);

#endif // FLASHRAWPLAYERSCREENDEFS_H_
