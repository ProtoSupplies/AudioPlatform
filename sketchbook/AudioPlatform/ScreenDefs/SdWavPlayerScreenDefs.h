/************************************************************************************
*                              SdWavPlayerScreenDefs.h                              *
*                                                                                   *
*                       Copyright (c) 2025, Donald Swearingen                       *
*                                All Rights Reserved                                *
*************************************************************************************
*
* Description
*
*   Screen layout and definitions for the SdWavPlayerScreen of the AudioPlatform application.
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
#ifndef SDWAVPLAYERSCREENDEFS_H_
#define SDWAVPLAYERSCREENDEFS_H_

/**********************
* Media Player Screen *
***********************
*
*/

DisplayScreenInfo mediaPlayerDisplayInfo = (DisplayScreenInfo) {
    DISPLAY_BGD_COLOR_DEFAULT,  // displayBgdColor
    PARAMBTN_SCRN_MARGIN_TOP,   // marginTop
    PARAMBTN_SCRN_MARGIN_BOTTOM,// marginBottom
    PARAMBTN_SCRN_MARGIN_LEFT,  // marginLeft
    PARAMBTN_SCRN_MARGIN_RIGHT, // marginRight
    HDR_FONT_DEFAULT,           // headerFont
    HDR_TEXT_COLOR_DEFAULT,     // headerFontColor
    SUBHDR_FONT_DEFAULT,        // subHeaderFont
    SUBHDR_TEXT_COLOR_DEFAULT,  // subHeaderFontColor
    "WAV File Player",          // screenHeader
    "Teensy 4.1 SD Wav Files",  // screenSubHeader
    TextAlignLeft,              // textAlign
    true,                       // showPrevBtn
    "Home"                      // prevBtnText
};

#define SCRN_WAVPLAYER_BTN_COUNT    12

ButtonPanelInfo mediaPlayerPanelInfo = (ButtonPanelInfo) {
    SCRN_WAVPLAYER_BTN_COUNT,   // btnCount
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
ParamButtonInfo     sdStatParams            = (ParamButtonInfo) {MediaPlayWav_Status,  BtnParam_Type_Text, "Player Status", "", Device_Disabled, Device_Enabled, AudioPlatform::DeviceStatusLabels};
ParamButtonInfo     sdLvlParams             = (ParamButtonInfo) {MediaPlayWav_Level,   BtnParam_Type_Float, "Player Level", "dB", SDWAVPLAY_LEVEL_MIN, SDWAVPLAY_LEVEL_MAX};
ParamButtonInfo     sdDlyParams             = (ParamButtonInfo) {MediaPlayWav_DlySend, BtnParam_Type_Int, "Player -> Delay", "%",  PCT_TO_FLOAT(SDWAVPLAY_DLY_SEND_MIN), PCT_TO_FLOAT(SDWAVPLAY_DLY_SEND_MAX)};
ParamButtonInfo     sdRvbParams             = (ParamButtonInfo) {MediaPlayWav_RvbSend, BtnParam_Type_Int, "Player -> Reverb", "%", PCT_TO_FLOAT(SDWAVPLAY_RVB_SEND_MIN), PCT_TO_FLOAT(SDWAVPLAY_RVB_SEND_MAX)};

ParamButtonInfo     sdSoundBankParams       = (ParamButtonInfo) {MediaPlayWav_SoundBank, BtnParam_Type_Int, "Sound Bank",  "", SOUND_BANK_MIN, SOUND_BANK_MAX};
ParamButtonInfo     sdNoteShiftParams       = (ParamButtonInfo) {MediaPlayWav_NoteShift, BtnParam_Type_Int, "Note Shift",  "", NOTE_SHIFT_MIN_PLYR, NOTE_SHIFT_MAX_PLYR};
ParamButtonInfo     sdNoteRangeMinParams    = (ParamButtonInfo) {MediaPlayWav_NoteRangeMin, BtnParam_Type_Int, "Note Min", "", NOTE_RANGE_MIN_PLYR, NOTE_RANGE_MAX_PLYR};
ParamButtonInfo     sdNoteRangeMaxParams    = (ParamButtonInfo) {MediaPlayWav_NoteRangeMax, BtnParam_Type_Int, "Note Max", "", NOTE_RANGE_MIN_PLYR, NOTE_RANGE_MAX_PLYR};

// NOTE: Pan-related constants defined in MidiDefines.h
ParamButtonInfo     sdPanType               = (ParamButtonInfo) {MediaPlayWav_PanType, BtnParam_Type_Text, "Pan Type", "", PanTypeMin, PanTypeMax, AudioPlatform::PanTypeLabels};
ParamButtonInfo     sdPanParameter          = (ParamButtonInfo) {MediaPlayWav_PanParameter, BtnParam_Type_Float, "Pan Parameter", "", PanParamMin, PanParamMax};

// Array of parameter information structures for screen buttons
ParamButtonInfo     mediaPlayerBtnParams[SCRN_WAVPLAYER_BTN_COUNT] = {
                    sdStatParams,
                    sdLvlParams,
                    sdDlyParams,
                    sdRvbParams,

                    sdSoundBankParams,
                    sdNoteShiftParams,
                    sdNoteRangeMinParams,
                    sdNoteRangeMaxParams,

                    sdPanType,
                    sdPanParameter,
                    dummyBtnParams,
                    dummyBtnParams
};

// Encoder parameters for SD WAV Player
RotaryParams        sdStatRotaryParams          = {SDWAVPLAY_STATUS_DEFAULT, SDWAVPLAY_STATUS_MIN, SDWAVPLAY_STATUS_MAX, {1,1}, 2, 0, Presets::setWavPlayerParam};
RotaryParams        sdLvlRotaryParams           = {SDWAVPLAY_LEVEL_DEFAULT, DB_TO_ROTARY(SDWAVPLAY_LEVEL_MIN), DB_TO_ROTARY(SDWAVPLAY_LEVEL_MAX), {1, 2, 12, 24}, 4, 0, Presets::setWavPlayerParam};
RotaryParams        sdDlyRotaryParams           = {SDWAVPLAY_DLY_SEND_DEFAULT, SDWAVPLAY_DLY_SEND_MIN, SDWAVPLAY_DLY_SEND_MAX, {1, 5, 10}, 3, 0, Presets::setWavPlayerParam};
RotaryParams        sdRvbRotaryParams           = {SDWAVPLAY_RVB_SEND_DEFAULT, SDWAVPLAY_RVB_SEND_MIN, SDWAVPLAY_RVB_SEND_MAX, {1, 5, 10}, 3, 0, Presets::setWavPlayerParam};

RotaryParams        sdSoundBankRotaryParams     = {SOUND_BANK_DEFAULT, SOUND_BANK_MIN, SOUND_BANK_MAX, {1,1}, 2, 0, Presets::setWavPlayerParam};
RotaryParams        sdNoteShiftRotaryParams     = {NOTE_SHIFT_DEFAULT, NOTE_SHIFT_MIN_PLYR, NOTE_SHIFT_MAX_PLYR, {1,5,10}, 3, 0, Presets::setWavPlayerParam};
RotaryParams        sdNoteRangeMinRotaryParams  = {NOTE_RANGE_MIN_PLYR, NOTE_RANGE_MIN_PLYR, NOTE_RANGE_MAX_PLYR, {1,5,10}, 3, 0, Presets::setWavPlayerParam};
RotaryParams        sdNoteRangeMaxRotaryParams  = {NOTE_RANGE_MAX_PLYR, NOTE_RANGE_MIN_PLYR, NOTE_RANGE_MAX_PLYR, {1,5,10}, 3, 0, Presets::setWavPlayerParam};

// NOTE: Pan-related constants defined in MidiDefines.h
RotaryParams        sdPanRotaryParams           = {PanTypeDefault, PanTypeMin, PanTypeMax, {1,1}, 2, 0, Presets::setWavPlayerParam};
RotaryParams        sdPanParamRotaryParams      = {PanParamDefault, PanParamMin, PanParamMax, {1,5,10}, 3, 0, Presets::setWavPlayerParam};

// Array of rotary encoder parameter information for screen buttons
RotaryParams        mediaPlayerRotaryParams[SCRN_WAVPLAYER_BTN_COUNT] = {
                    sdStatRotaryParams,
                    sdLvlRotaryParams,
                    sdDlyRotaryParams,
                    sdRvbRotaryParams,

                    sdSoundBankRotaryParams,
                    sdNoteShiftRotaryParams,
                    sdNoteRangeMinRotaryParams,
                    sdNoteRangeMaxRotaryParams,

                    sdPanRotaryParams,
                    sdPanParamRotaryParams,
                    dummyRotaryParams,
                    dummyRotaryParams
};

// SD WAV Player screen instance
ParamButtonScreen   sdWavPlayerScreen(mediaPlayerDisplayInfo, mediaPlayerPanelInfo, mediaPlayerBtnParams, mediaPlayerRotaryParams, InputDevices::encoders, deviceWavPlayer, Presets::presetsCallback);

#endif // SDWAVPLAYERSCREENDEFS_H_
