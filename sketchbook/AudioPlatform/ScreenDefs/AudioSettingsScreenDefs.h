/************************************************************************************
*                             AudioSettingsScreenDefs.h                             *
*                                                                                   *
*                       Copyright (c) 2025, Donald Swearingen                       *
*                                All Rights Reserved                                *
*************************************************************************************
*
* Description
*
*   Screen layout and definitions for the AudioSettingsScreen of the AudioPlatform application.
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
#ifndef AUDIOSETTINGSSCREENDEFS_H_
#define AUDIOSETTINGSSCREENDEFS_H_

#define SCRN_INOUT_BTN_COUNT        8

/*****************
* AudioIO Screen *
******************
*
*/

DisplayScreenInfo audioIoDisplayInfo = (DisplayScreenInfo) {
    DISPLAY_BGD_COLOR_DEFAULT,  // displayBgdColor
    PARAMBTN_SCRN_MARGIN_TOP,   // marginTop
    PARAMBTN_SCRN_MARGIN_BOTTOM,// marginBottom
    PARAMBTN_SCRN_MARGIN_LEFT,  // marginLeft
    PARAMBTN_SCRN_MARGIN_RIGHT, // marginRight
    HDR_FONT_DEFAULT,           // headerFont
    HDR_TEXT_COLOR_DEFAULT,     // headerFontColor
    SUBHDR_FONT_DEFAULT,        // subHeaderFont
    SUBHDR_TEXT_COLOR_DEFAULT,  // subHeaderFontColor
    "Audio IO Settings",        // screenHeader
    "Audio In/Out/Codec",       // screenSubHeader
    TextAlignLeft,              // textAlign
    true,                       // showPrevBtn
    "Home"                      // prevBtnText
};

ButtonPanelInfo audioIoPanelInfo = (ButtonPanelInfo) {
    SCRN_INOUT_BTN_COUNT,       // btnCount
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
// TODO: How to handle variable text more efficiently
ParamButtonInfo     audioIoLineInParams         = (ParamButtonInfo) {AudioIo_LineIn_Status, BtnParam_Type_Text, "LineIn Status", "", Device_Disabled, Device_Enabled, AudioPlatform::DeviceStatusLabels};
ParamButtonInfo     audioIoLineInLevelParams    = (ParamButtonInfo) {AudioIo_LineIn_Level, BtnParam_Type_Text, "LineIn Level", "Volts p-p", (float)AUDIOIO_LINEIN_LVL_MIN, (float)AUDIOIO_LINEIN_LVL_MAX, AudioPlatform::AudioLineInPpLabels};
ParamButtonInfo     audioIoLineOutLevelParams   = (ParamButtonInfo) {AudioIo_LineOut_Level, BtnParam_Type_Text, "LineOut Level", "Volts p-p", (float)AUDIOIO_LINEOUT_LVL_MIN, (float)AUDIOIO_LINEOUT_LVL_MAX, AudioPlatform::AudioLineOutPpLabels};
ParamButtonInfo     audioIoPhoneVolParams       = (ParamButtonInfo) {AudioIo_Phone_Volume, BtnParam_Type_Int, "Phone Volume", "%", PCT_TO_FLOAT(AUDIOIO_PHONEVOLUME_MIN), PCT_TO_FLOAT(AUDIOIO_PHONEVOLUME_MAX)};
ParamButtonInfo     audioIoLineDlyParams        = (ParamButtonInfo) {AudioIo_LineIn_DlySend, BtnParam_Type_Int, "LineIn -> Delay", "%", PCT_TO_FLOAT(AUDIOIO_LINEIN_DLY_SEND_MIN), PCT_TO_FLOAT(AUDIOIO_LINEIN_DLY_SEND_MAX)};
ParamButtonInfo     audioIoLineInRvbParams      = (ParamButtonInfo) {AudioIo_LineIn_RvbSend, BtnParam_Type_Int, "LineIn -> Reverb", "%", PCT_TO_FLOAT(AUDIOIO_LINEIN_RVB_SEND_MIN), PCT_TO_FLOAT(AUDIOIO_LINEIN_RVB_SEND_MAX)};

// Array of audio device parameter information structures for screen buttons
ParamButtonInfo     audioIoBtnParams[SCRN_INOUT_BTN_COUNT] = {
                    audioIoLineInParams,
                    audioIoLineInLevelParams,
                    audioIoLineOutLevelParams,
                    audioIoPhoneVolParams,
                    audioIoLineDlyParams,
                    audioIoLineInRvbParams,
                    dummyBtnParams,
                    dummyBtnParams
};

// Encoder parameters for Audio Effects (Delay, Reverb, etc.)
RotaryParams        audioIoLineInRotaryParams       = {AUDIOIO_LINEIN_STATUS_DEFAULT, AUDIOIO_LINEIN_STATUS_MIN, AUDIOIO_LINEIN_STATUS_MAX, {1,1}, 2, 0, Presets::setInputOutputParam};
RotaryParams        audioIoLineInLevelRotaryParams  = {AUDIOIO_LINEIN_LVL_DEFAULT, AUDIOIO_LINEIN_LVL_MIN, AUDIOIO_LINEIN_LVL_MAX, {1, 1}, 2, 0, Presets::setInputOutputParam};
RotaryParams        audioIoLineOutLevelRotaryParams = {AUDIOIO_LINEOUT_LVL_DEFAULT, AUDIOIO_LINEOUT_LVL_MIN, AUDIOIO_LINEOUT_LVL_MAX, {1, 1}, 2, 0, Presets::setInputOutputParam};
RotaryParams        audioIoPhoneVolRotaryParams     = {AUDIOIO_PHONEVOLUME_DEFAULT, AUDIOIO_PHONEVOLUME_MIN, AUDIOIO_PHONEVOLUME_MAX, {1, 5, 10}, 3, 0, Presets::setInputOutputParam};
RotaryParams        audioIoLineInDlyRotaryParams    = {AUDIOIO_LINEIN_DLY_SEND_DEFAULT, AUDIOIO_LINEIN_DLY_SEND_MIN, AUDIOIO_LINEIN_DLY_SEND_MAX, {1, 5, 10}, 3, 0, Presets::setInputOutputParam};
RotaryParams        audioIoLineInRvbRotaryParams    = {AUDIOIO_LINEIN_RVB_SEND_DEFAULT, AUDIOIO_LINEIN_RVB_SEND_MIN, AUDIOIO_LINEIN_RVB_SEND_MAX, {1, 5, 10}, 3, 0, Presets::setInputOutputParam};

// Array of rotary encoder parameter information for screen buttons
RotaryParams        audioIoRotaryParams[SCRN_INOUT_BTN_COUNT] = {
                    audioIoLineInRotaryParams,
                    audioIoLineInLevelRotaryParams,
                    audioIoLineOutLevelRotaryParams,
                    audioIoPhoneVolRotaryParams,
                    audioIoLineInDlyRotaryParams,
                    audioIoLineInRvbRotaryParams,
                    dummyRotaryParams,
                    dummyRotaryParams
};

// Audio IO Settings screen instance
ParamButtonScreen   audioIoScreen(audioIoDisplayInfo, audioIoPanelInfo, audioIoBtnParams, audioIoRotaryParams, InputDevices::encoders, deviceAudioIo, Presets::presetsCallback);

#endif // AUDIOSETTINGSSCREENDEFS_H_
