/************************************************************************************
*                              StereoDelayScreenDefs.h                              *
*                                                                                   *
*                       Copyright (c) 2025, Donald Swearingen                       *
*                                All Rights Reserved                                *
*************************************************************************************
*
* Description
*
*   Screen layout and definitions for the StereoDelayScreen of the AudioPlatform application.
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
#ifndef STEREODELAYSCREENDEFS_H_
#define STEREODELAYSCREENDEFS_H_

/***********************
* MIDI Settings Screen *
************************
*
*/

DisplayScreenInfo sDlySettingsDisplayInfo = (DisplayScreenInfo) {
    DISPLAY_BGD_COLOR_DEFAULT,  // displayBgdColor
    PARAMBTN_SCRN_MARGIN_TOP,   // marginTop
    PARAMBTN_SCRN_MARGIN_BOTTOM,// marginBottom
    PARAMBTN_SCRN_MARGIN_LEFT,  // marginLeft
    PARAMBTN_SCRN_MARGIN_RIGHT, // marginRight
    HDR_FONT_DEFAULT,           // headerFont
    HDR_TEXT_COLOR_DEFAULT,     // headerFontColor
    SUBHDR_FONT_DEFAULT,        // subHeaderFont
    SUBHDR_TEXT_COLOR_DEFAULT,  // subHeaderFontColor
    "Stereo Delay",             // screenHeader
    "Dual Channel X-Feedback",  // screenSubHeader
    TextAlignLeft,              // textAlign
    true,                       // showPrevBtn
    "Home"                      // prevBtnText
};

#define SCRN_SDLY_BTN_COUNT     12

ButtonPanelInfo sDlySettingsPanelInfo = (ButtonPanelInfo) {
    SCRN_SDLY_BTN_COUNT,        // btnCount
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

// Stereo Delay Parameter information structures associated with the screen buttons
ParamButtonInfo     sDlyLDlyGainParams = (ParamButtonInfo) {StereoDelay_DlyL_Gain,        BtnParam_Type_Float, "Delay L Gain",      "dB", DELAY_LEVEL_MIN, DELAY_LEVEL_MAX};
ParamButtonInfo     sDlyLDlyTimeParams = (ParamButtonInfo) {StereoDelay_DlyL_DlyTime,     BtnParam_Type_Int,   "Delay L Time",      "ms", DELAY_TIME_MIN, DELAY_TIME_MAX};
ParamButtonInfo     sDlyLRvbSendParams = (ParamButtonInfo) {StereoDelay_DlyL_RvbSend,     BtnParam_Type_Int,   "Delay L > Reverb",  "%",  PCT_TO_FLOAT(DELAY_RVBSEND_MIN), PCT_TO_FLOAT(DELAY_RVBSEND_MAX)};
ParamButtonInfo     sDlyLFbAmtParams =   (ParamButtonInfo) {StereoDelay_DlyL_FB_DlyAmt,   BtnParam_Type_Int,   "Delay L FB %",      "%",  PCT_TO_FLOAT(DELAY_FEEDBACK_MIN), PCT_TO_FLOAT(DELAY_FEEDBACK_MAX)};
ParamButtonInfo     sDlyLXfbTimeParams = (ParamButtonInfo) {StereoDelay_DlyL_xFB_DlyTime, BtnParam_Type_Int,   "Delay L xFB Time",  "ms", DELAY_TIME_MIN, DELAY_TIME_MAX};
ParamButtonInfo     sDlyLXfbAmtParams =  (ParamButtonInfo) {StereoDelay_DlyL_xFB_DlyAmt,  BtnParam_Type_Int,   "Delay L xFB %",     "%",  PCT_TO_FLOAT(DELAY_XFEEDBACK_MIN), PCT_TO_FLOAT(DELAY_XFEEDBACK_MAX)};

ParamButtonInfo     sDlyRDlyGainParams = (ParamButtonInfo) {StereoDelay_DlyR_Gain,        BtnParam_Type_Float, "Delay R Gain",      "dB", DELAY_LEVEL_MIN, DELAY_LEVEL_MAX};
ParamButtonInfo     sDlyRDlyTimeParams = (ParamButtonInfo) {StereoDelay_DlyR_DlyTime,     BtnParam_Type_Int,   "Delay R Time",      "ms", DELAY_TIME_MIN, DELAY_TIME_MAX};
ParamButtonInfo     sDlyRRvbSendParams = (ParamButtonInfo) {StereoDelay_DlyR_RvbSend,     BtnParam_Type_Int,   "Delay R > Reverb",  "%",  PCT_TO_FLOAT(DELAY_RVBSEND_MIN), PCT_TO_FLOAT(DELAY_RVBSEND_MAX)};
ParamButtonInfo     sDlyRFbAmtParams =   (ParamButtonInfo) {StereoDelay_DlyR_FB_DlyAmt,   BtnParam_Type_Int,   "Delay R FB %",      "%",  PCT_TO_FLOAT(DELAY_FEEDBACK_MIN), PCT_TO_FLOAT(DELAY_FEEDBACK_MAX)};
ParamButtonInfo     sDlyRXfbTimeParams = (ParamButtonInfo) {StereoDelay_DlyR_xFB_DlyTime, BtnParam_Type_Int,   "Delay R xFB Time",  "ms", DELAY_TIME_MIN, DELAY_TIME_MAX};
ParamButtonInfo     sDlyRXfbAmtParams =  (ParamButtonInfo) {StereoDelay_DlyR_xFB_DlyAmt,  BtnParam_Type_Int,   "Delay R xFB %",     "%",  PCT_TO_FLOAT(DELAY_XFEEDBACK_MIN), PCT_TO_FLOAT(DELAY_XFEEDBACK_MAX)};


// Array of parameter information structures for screen buttons
ParamButtonInfo     sDlySettingsBtnParams[SCRN_SDLY_BTN_COUNT] = {
                    sDlyLDlyGainParams,
                    sDlyLRvbSendParams,
                    sDlyRDlyGainParams,
                    sDlyRRvbSendParams,
                    sDlyLDlyTimeParams,
                    sDlyLFbAmtParams,
                    sDlyRDlyTimeParams,
                    sDlyRFbAmtParams,
                    sDlyLXfbTimeParams,
                    sDlyLXfbAmtParams,
                    sDlyRXfbTimeParams,
                    sDlyRXfbAmtParams
};

ScreenID sid = ScreenMainMenu;

// Encoder parameters for Stereo Delay Effect
RotaryParams        sDlyLDlyTimeRotaryParams = {DELAY_TIME_DEFAULT, DELAY_TIME_MIN, DELAY_TIME_MAX, {1, 5, 10, 50, 100, 500}, 6, 0, Presets::setStereoDelayParam};
RotaryParams        sDlyLDlyGainRotaryParams = {DELAY_LEVEL_DEFAULT, DB_TO_ROTARY(DELAY_LEVEL_MIN), DB_TO_ROTARY(DELAY_LEVEL_MAX), {1, 2, 12, 24}, 4, 0, Presets::setStereoDelayParam};
RotaryParams        sDlyLRvbSendRotaryParams = {DELAY_RVBSEND_DEFAULT, DELAY_RVBSEND_MIN, DELAY_RVBSEND_MAX, {1, 5, 10}, 3, 0, Presets::setStereoDelayParam};
RotaryParams        sDlyLFbAmtRotaryParams   = {DELAY_FEEDBACK_DEFAULT, DELAY_FEEDBACK_MIN, DELAY_FEEDBACK_MAX, {1, 5, 10}, 3, 0, Presets::setStereoDelayParam};
RotaryParams        sDlyLXfbTimeRotaryParams = {DELAY_TIME_DEFAULT, DELAY_TIME_MIN, DELAY_TIME_MAX, {1, 5, 10, 50, 100, 500}, 6, 0, Presets::setStereoDelayParam};
RotaryParams        sDlyLXfbAmtRotaryParams  = {DELAY_XFEEDBACK_DEFAULT, DELAY_XFEEDBACK_MIN, DELAY_XFEEDBACK_MAX, {1, 5, 10}, 3, 0, Presets::setStereoDelayParam};

RotaryParams        sDlyRDlyTimeRotaryParams = {DELAY_TIME_DEFAULT, DELAY_TIME_MIN, DELAY_TIME_MAX, {1, 5, 10, 50, 100, 500}, 6, 0, Presets::setStereoDelayParam};
RotaryParams        sDlyRDlyGainRotaryParams = {DELAY_LEVEL_DEFAULT, DB_TO_ROTARY(DELAY_LEVEL_MIN), DB_TO_ROTARY(DELAY_LEVEL_MAX), {1, 2, 12, 24}, 4, 0, Presets::setStereoDelayParam};
RotaryParams        sDlyRRvbSendRotaryParams = {DELAY_RVBSEND_DEFAULT, DELAY_RVBSEND_MIN, DELAY_RVBSEND_MAX, {1, 5, 10}, 3, 0, Presets::setStereoDelayParam};
RotaryParams        sDlyRFbAmtRotaryParams   = {DELAY_FEEDBACK_DEFAULT, DELAY_FEEDBACK_MIN, DELAY_FEEDBACK_MAX, {1, 5, 10}, 3, 0, Presets::setStereoDelayParam};
RotaryParams        sDlyRXfbTimeRotaryParams = {DELAY_TIME_DEFAULT, DELAY_TIME_MIN, DELAY_TIME_MAX, {1, 5, 10, 50, 100, 500}, 6, 0, Presets::setStereoDelayParam};
RotaryParams        sDlyRXfbAmtRotaryParams  = {DELAY_XFEEDBACK_DEFAULT, DELAY_XFEEDBACK_MIN, DELAY_XFEEDBACK_MAX, {1, 5, 10}, 3, 0, Presets::setStereoDelayParam};

// Array of rotary encoder parameter information for screen buttons
RotaryParams        sDlySettingsRotaryParams[SCRN_SDLY_BTN_COUNT] = {
                    sDlyLDlyGainRotaryParams,
                    sDlyLRvbSendRotaryParams,
                    sDlyRDlyGainRotaryParams,
                    sDlyRRvbSendRotaryParams,
                    sDlyLDlyTimeRotaryParams,
                    sDlyLFbAmtRotaryParams,
                    sDlyRDlyTimeRotaryParams,
                    sDlyRFbAmtRotaryParams,
                    sDlyLXfbTimeRotaryParams,
                    sDlyLXfbAmtRotaryParams,
                    sDlyRXfbTimeRotaryParams,
                    sDlyRXfbAmtRotaryParams
};

// Stereo Delay settings screen instance
ParamButtonScreen   sDlySettingsScreen(sDlySettingsDisplayInfo, sDlySettingsPanelInfo, sDlySettingsBtnParams, sDlySettingsRotaryParams, InputDevices::encoders, deviceStereoDelay, Presets::presetsCallback);

#endif // STEREODELAYSCREENDEFS_H_
