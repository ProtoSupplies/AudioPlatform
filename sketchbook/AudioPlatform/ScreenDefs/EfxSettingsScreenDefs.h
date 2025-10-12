/************************************************************************************
*                              EfxSettingsScreenDefs.h                              *
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
#ifndef EFXSETTINGSSCREENDEFS_H_
#define EFXSETTINGSSCREENDEFS_H_

/**************************
* Effects Settings Screen *
***************************
*
* This section defines structures passed to and used on the Effects Settings screen, which
* manages an array of screen "buttons" representing various parameters of the Audio effects
* that can be set and managed using the rotary encoders.
*
*/

DisplayScreenInfo efxSettingsDisplayInfo = (DisplayScreenInfo) {
    DISPLAY_BGD_COLOR_DEFAULT,  // displayBgdColor
    PARAMBTN_SCRN_MARGIN_TOP,   // marginTop
    PARAMBTN_SCRN_MARGIN_BOTTOM, // marginBottom
    PARAMBTN_SCRN_MARGIN_LEFT,  // marginLeft
    PARAMBTN_SCRN_MARGIN_RIGHT, // marginRight
    HDR_FONT_DEFAULT,           // headerFont
    HDR_TEXT_COLOR_DEFAULT,     // headerFontColor
    SUBHDR_FONT_DEFAULT,        // subHeaderFont
    SUBHDR_TEXT_COLOR_DEFAULT,  // subHeaderFontColor
    "FutureEfx Settings",       // screenHeader
    "Audio Efx TBD",            // screenSubHeader
    TextAlignLeft,              // textAlign
    true,                       // showPrevBtn
    "Home"                      // prevBtnText
};

#define SCRN_EFX_BTN_COUNT      8

ButtonPanelInfo efxSettingsPanelInfo = (ButtonPanelInfo) {
    SCRN_EFX_BTN_COUNT,         // btnCount
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

// fUTUREeFX Parameter information structures associated with the screen buttons
ParamButtonInfo     efxLevelParams  = (ParamButtonInfo) {FutureEfx_Level, BtnParam_Type_Float, "Efx Level", "dB", EFX_LEVEL_MIN, EFX_LEVEL_MAX};
ParamButtonInfo     efxParam2Params = (ParamButtonInfo) {FutureEfx_Param2, BtnParam_Type_Int, "Param2 %", "",  PCT_TO_FLOAT(EFX_PARAM_MIN), PCT_TO_FLOAT(EFX_PARAM_MAX)};
ParamButtonInfo     efxParam3Params = (ParamButtonInfo) {FutureEfx_Param3, BtnParam_Type_Int, "Param3 %", "",  PCT_TO_FLOAT(EFX_PARAM_MIN), PCT_TO_FLOAT(EFX_PARAM_MAX)};
ParamButtonInfo     efxParam4Params = (ParamButtonInfo) {FutureEfx_Param4, BtnParam_Type_Int, "Param4 %", "",  PCT_TO_FLOAT(EFX_PARAM_MIN), PCT_TO_FLOAT(EFX_PARAM_MAX)};

// Array of parameter information structures for screen buttons
ParamButtonInfo     efxSettingsBtnParams[SCRN_EFX_BTN_COUNT] = {
                    efxLevelParams,
                    efxParam2Params,
                    efxParam3Params,
                    efxParam4Params,
                    dummyBtnParams,
                    dummyBtnParams,
                    dummyBtnParams,
                    dummyBtnParams
};

// Encoder parameters for Effect
RotaryParams        efxLevelRotaryParams    = {EFX_LEVEL_DEFAULT, DB_TO_ROTARY(EFX_LEVEL_MIN), DB_TO_ROTARY(EFX_LEVEL_MAX), {1, 2, 12, 24}, 4, 0, Presets::setEfxParam};
RotaryParams        efxParam2RotaryParams   = {EFX_PARAM_DEFAULT, EFX_PARAM_MIN, EFX_PARAM_MAX, {1, 5, 10}, 3, 0, Presets::setEfxParam};
RotaryParams        efxParam3RotaryParams   = {EFX_PARAM_DEFAULT, EFX_PARAM_MIN, EFX_PARAM_MAX, {1, 5, 10}, 3, 0, Presets::setEfxParam};
RotaryParams        efxParam4RotaryParams   = {EFX_PARAM_DEFAULT, EFX_PARAM_MIN, EFX_PARAM_MAX, {1, 5, 10}, 3, 0, Presets::setEfxParam};

// Array of rotary encoder parameter information for screen buttons
RotaryParams        efxSettingsRotaryParams[SCRN_EFX_BTN_COUNT] = {
                    efxLevelRotaryParams,
                    efxParam2RotaryParams,
                    efxParam3RotaryParams,
                    efxParam4RotaryParams,
                    dummyRotaryParams,
                    dummyRotaryParams,
                    dummyRotaryParams,
                    dummyRotaryParams
};

// Effects settings screen instance
ParamButtonScreen   efxSettingsScreen(efxSettingsDisplayInfo, efxSettingsPanelInfo, efxSettingsBtnParams, efxSettingsRotaryParams, InputDevices::encoders, deviceEfx, Presets::presetsCallback);

#endif // EFXSETTINGSSCREENDEFS_H_
