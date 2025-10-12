/************************************************************************************
*                            ReverbSettingsScreenDefs.h                             *
*                                                                                   *
*                       Copyright (c) 2025, Donald Swearingen                       *
*                                All Rights Reserved                                *
*************************************************************************************
*
* Description
*
*   Screen layout and definitions for the EfxSettingsScreen of the AudioPlatform application.
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
#ifndef REVERBSETTINGSSCREENDEFS_H_
#define REVERBSETTINGSSCREENDEFS_H_

/*************************
* Reverb Settings Screen *
**************************
*
* This section defines structures passed to and used on the Effects Settings screen, which
* manages an array of screen "buttons" representing various parameters of the Audio effects
* that can be set and managed using the rotary encoders.
*
*/

DisplayScreenInfo rvbSettingsDisplayInfo = (DisplayScreenInfo) {
    DISPLAY_BGD_COLOR_DEFAULT,  // displayBgdColor
    PARAMBTN_SCRN_MARGIN_TOP,   // marginTop
    PARAMBTN_SCRN_MARGIN_BOTTOM, // marginBottom
    PARAMBTN_SCRN_MARGIN_LEFT,  // marginLeft
    PARAMBTN_SCRN_MARGIN_RIGHT, // marginRight
    HDR_FONT_DEFAULT,           // headerFont
    HDR_TEXT_COLOR_DEFAULT,     // headerFontColor
    SUBHDR_FONT_DEFAULT,        // subHeaderFont
    SUBHDR_TEXT_COLOR_DEFAULT,  // subHeaderFontColor
    "Reverb Settings",          // screenHeader
    "Future Additions TBD",     // screenSubHeader
    TextAlignLeft,              // textAlign
    true,                       // showPrevBtn
    "Home"                      // prevBtnText
};

#define SCRN_RVB_BTN_COUNT      8

ButtonPanelInfo rvbSettingsPanelInfo = (ButtonPanelInfo) {
    SCRN_RVB_BTN_COUNT,         // btnCount
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

// Reverb Parameter information structures associated with the screen buttons
ParamButtonInfo     rvbLvlParams        = (ParamButtonInfo) {Reverb_Level, BtnParam_Type_Float, "Rvb Level", "dB", REVERB_LEVEL_MIN, REVERB_LEVEL_MAX};
ParamButtonInfo     rvbSizeParams       = (ParamButtonInfo) {Reverb_RoomSize, BtnParam_Type_Int,"Rvb Size", "",  PCT_TO_FLOAT(REVERB_ROOMSIZE_MIN), PCT_TO_FLOAT(REVERB_ROOMSIZE_MAX)};
ParamButtonInfo     rvbDfusParams       = (ParamButtonInfo) {Reverb_Diffusion, BtnParam_Type_Int, "Rvb Diffusion", "%", PCT_TO_FLOAT(REVERB_DIFFUSION_MIN), PCT_TO_FLOAT(REVERB_DIFFUSION_MAX)};
ParamButtonInfo     rvbPsParams         = (ParamButtonInfo) {Reverb_PchShft, BtnParam_Type_Int, "Rvb PchShft", "Semitones", REVERB_PCHSHFT_MIN, REVERB_PCHSHFT_MAX};
ParamButtonInfo     rvbPsAmtParams      = (ParamButtonInfo) {Reverb_PchShftAmt, BtnParam_Type_Int, "Rvb PchShftAmt", "%", PCT_TO_FLOAT(REVERB_PCHSHFTAMT_MIN), PCT_TO_FLOAT(REVERB_PCHSHFTAMT_MAX)};
ParamButtonInfo     rvbShimPsParams     = (ParamButtonInfo) {Reverb_ShimPchShft, BtnParam_Type_Int, "Rvb ShmrPchShft", "Semitones", REVERB_SHIMPCHSHFT_MIN, REVERB_SHIMPCHSHFT_MAX};
ParamButtonInfo     rvbShimRatioParams  = (ParamButtonInfo) {Reverb_ShimShftRatio, BtnParam_Type_Int, "Rvb ShmrRatio", "%", PCT_TO_FLOAT(REVERB_SHIMSHFTRATIO_MIN), PCT_TO_FLOAT(REVERB_SHIMSHFTRATIO_MAX)};
ParamButtonInfo     rvbShimAmtParams    = (ParamButtonInfo) {Reverb_ShimAmt, BtnParam_Type_Int, "Rvb ShmrAmt", "%", PCT_TO_FLOAT(REVERB_SHIMSHFTAMT_MIN), PCT_TO_FLOAT(REVERB_SHIMSHFTAMT_MAX)};

//ParamButtonInfo     fmSynthBendRange            = (ParamButtonInfo) {FmSynth_PitchBendRange, BtnParam_Type_Int, "PitchBend Range", "Semitones", 1.0, 12.0};

// Array of parameter information structures for screen buttons
ParamButtonInfo     rvbSettingsBtnParams[SCRN_RVB_BTN_COUNT] = {
                    rvbLvlParams,
                    rvbSizeParams,
                    rvbDfusParams,
                    rvbPsParams,
                    rvbPsAmtParams,
                    rvbShimPsParams,
                    rvbShimRatioParams,
                    rvbShimAmtParams
};

// Encoder parameters for Reverb Effect
RotaryParams        rvbLvlRotaryParams       = {REVERB_LEVEL_DEFAULT, DB_TO_ROTARY(REVERB_LEVEL_MIN), DB_TO_ROTARY(REVERB_LEVEL_MAX), {1, 2, 12, 24}, 4, 0, Presets::setRvbParam};
RotaryParams        rvbSizeRotaryParams      = {REVERB_ROOMSIZE_DEFAULT, REVERB_ROOMSIZE_MIN, REVERB_ROOMSIZE_MAX, {1, 5, 10}, 3, 0, Presets::setRvbParam};
RotaryParams        rvbDfusRotaryParams      = {REVERB_DIFFUSION_DEFAULT, REVERB_DIFFUSION_MIN, REVERB_DIFFUSION_MAX, {1, 5, 10}, 3, 0, Presets::setRvbParam};
RotaryParams        rvbPsRotaryParams        = {REVERB_PCHSHFT_DEFAULT, REVERB_PCHSHFT_MIN, REVERB_PCHSHFT_MAX, {1, 2, 4}, 3, 0, Presets::setRvbParam};
RotaryParams        rvbPsAmtRotaryParams     = {REVERB_PCHSHFTAMT_DEFAULT, REVERB_PCHSHFTAMT_MIN, REVERB_PCHSHFTAMT_MAX, {1, 5, 10}, 3, 0, Presets::setRvbParam};
RotaryParams        rvbDShimPsRotaryParams   = {REVERB_SHIMPCHSHFT_DEFAULT, REVERB_SHIMPCHSHFT_MIN, REVERB_SHIMPCHSHFT_MAX, {1, 5, 10}, 3, 0, Presets::setRvbParam};
RotaryParams        rvbShimRatioRotaryParams = {REVERB_SHIMSHFTRATIO_DEFAULT, REVERB_SHIMSHFTRATIO_MIN, REVERB_SHIMSHFTRATIO_MAX, {1, 5, 10}, 3, 0, Presets::setRvbParam};
RotaryParams        rvbShimAmtRotaryParams   = {REVERB_SHIMSHFTAMT_DEFAULT, REVERB_SHIMSHFTAMT_MIN, REVERB_SHIMSHFTAMT_MAX, {1, 5, 10}, 3, 0, Presets::setRvbParam};

// Array of rotary encoder parameter information for screen buttons
RotaryParams        rvbSettingsRotaryParams[SCRN_RVB_BTN_COUNT] = {
                    rvbLvlRotaryParams,
                    rvbSizeRotaryParams,
                    rvbDfusRotaryParams,
                    rvbPsRotaryParams,
                    rvbPsAmtRotaryParams,
                    rvbDShimPsRotaryParams,
                    rvbShimRatioRotaryParams,
                    rvbShimAmtRotaryParams
};

// Effects settings screen instance
ParamButtonScreen   rvbSettingsScreen(rvbSettingsDisplayInfo, rvbSettingsPanelInfo, rvbSettingsBtnParams, rvbSettingsRotaryParams, InputDevices::encoders, deviceRvb, Presets::presetsCallback);

#endif // REVERBSETTINGSSCREENDEFS_H_
