/************************************************************************************
*                                ScreenFmBanksDefs.h                                *
*                                                                                   *
*                       Copyright (c) 2025, Donald Swearingen                       *
*                                All Rights Reserved                                *
*************************************************************************************
*
* Description
*
*   Screen layout and definitions for FM Banks screen of the AudioPlatform application.
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
#ifndef SCREENFMBANKSDEFS_H_
#define SCREENFMBANKSDEFS_H_

/***************************
* FM Banks Settings Screen *
****************************
*
*/

#define FMBANKS_MENU_MARGIN_TOP        100
#define FMBANKS_MENU_MARGIN_BOTTOM     80
#define FMBANKS_MENU_MARGIN_LEFT       DEFAULT_SCRN_MARGIN_LEFT
#define FMBANKS_MENU_MARGIN_RIGHT      DEFAULT_SCRN_MARGIN_RIGHT

DisplayScreenInfo fmBanksScreenDisplayInfo = (DisplayScreenInfo) {
    DISPLAY_BGD_COLOR_DEFAULT,  // displayBgdColor
    FMBANKS_MENU_MARGIN_TOP,    // marginTop
    FMBANKS_MENU_MARGIN_BOTTOM, // marginBottom
    FMBANKS_MENU_MARGIN_LEFT,   // marginLeft
    FMBANKS_MENU_MARGIN_RIGHT,  // marginRight
    HDR_FONT_DEFAULT,           // headerFont
    HDR_TEXT_COLOR_DEFAULT,     // headerFontColor
    SUBHDR_FONT_DEFAULT,        // subHeaderFont
    SUBHDR_TEXT_COLOR_DEFAULT,  // subHeaderFontColor
    "FM Synth Voice Banks",     // screenHeader
    "Select Bank and Voice",    // screenSubHeader
    TextAlignCenter,            // textAlign
    false,                      // showPrevBtn
    ""                          // prevBtnText
};

ButtonPanelInfo fmPresetsPanelInfo = (ButtonPanelInfo) {
    FmSynthPresetsPerBank,      // btnCount
    8,                          // btnsPerRow
    92,                         // btnWidth
    36,                         // btnHeight
    6,                          // btnGapHorizontal
    6,                          // btnGapVertical
    COLOR_BLACK,                // panelBgdColor
    BTN_COLOR_DEFAULT,          // btnColor
    BTN_SEL_COLOR_DEFAULT,      // btnActiveColor
    Arial_9_Bold,               // btnFont
    COLOR_BLACK,                // btnTextColor
    BTN_SEL_TEXT_COLOR_DEFAULT, // btnActiveTextColor
    BTN_OUTLINE_COLOR_DEFAULT   // btnOutlineColor
};

ButtonPanelInfo fmBanksPanelInfo = (ButtonPanelInfo) {
    FmSynthPresetsBankCount,    // btnCount
    8,                          // btnsPerRow
    92,                         // btnWidth
    36,                         // btnHeight
    6,                          // btnGapHorizontal
    6,                          // btnGapVertical
    BTN_PANEL_BGD_COLOR_DEFAULT,// panelBgdColor
    BTN_COLOR_DEFAULT,          // btnColor
    BTN_SEL_COLOR_DEFAULT,      // btnActiveColor
    Arial_9_Bold,               // btnFont
    BTN_TEXT_COLOR_DEFAULT,     // btnTextColor
    BTN_SEL_TEXT_COLOR_DEFAULT, // btnActiveTextColor
    BTN_OUTLINE_COLOR_DEFAULT   // btnOutlineColor
};

// FM Banks Screen instance
FmBanksScreen  fmBanksScreen(fmBanksScreenDisplayInfo, fmPresetsPanelInfo, fmBanksPanelInfo, Presets::fmSynthPresetNames, Presets::fmSynthBankNames, FmSynth_PresetNum, Presets::setFmSynthParam, Presets::getFmSynthParam);

#endif // SCREENFMBANKSDEFS_H_
