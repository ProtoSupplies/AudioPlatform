/************************************************************************************
*                              PresetsLiveScreenDefs.h                              *
*                                                                                   *
*                       Copyright (c) 2025, Donald Swearingen                       *
*                                All Rights Reserved                                *
*************************************************************************************
*
* Description
*
*   Screen layout and definitions for the PresetsLiveScreen of the AudioPlatform application.
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
#ifndef PRESETSLIVECREENDEFS_H_
#define PRESETSLIVECREENDEFS_H_

// Overall Screen Appearance
//
#define PRESETSLIVE_SCRN_MARGIN_TOP     80
#define PRESETSLIVE_SCRN_MARGIN_BOTTOM  50
#define PRESETSLIVE_SCRN_MARGIN_LEFT    DEFAULT_SCRN_MARGIN_LEFT
#define PRESETSLIVE_SCRN_MARGIN_RIGHT   DEFAULT_SCRN_MARGIN_RIGHT

DisplayScreenInfo presetsLiveDisplayInfo = (DisplayScreenInfo) {
    DISPLAY_BGD_COLOR_DEFAULT,      // displayBgdColor
    PRESETSLIVE_SCRN_MARGIN_TOP,    // marginTop
    PRESETSLIVE_SCRN_MARGIN_BOTTOM, // marginBottom
    PRESETSLIVE_SCRN_MARGIN_LEFT,   // marginLeft
    PRESETSLIVE_SCRN_MARGIN_RIGHT,  // marginRight
    HDR_FONT_DEFAULT,               // headerFont
    HDR_TEXT_COLOR_DEFAULT,         // headerFontColor
    SUBHDR_FONT_DEFAULT,            // subHeaderFont
    SUBHDR_TEXT_COLOR_DEFAULT,      // subHeaderFontColor
    "Presets Live",                 // screenHeader
    "Direct Preset & Bank Access",  // screenSubHeader
    TextAlignLeft,                  // textAlign
    true,                           // showPrevBtn
    "Home"                          // prevBtnText
};

// Presets Button Panel
//
#define PRESETSLIVE_PANEL_BTN_COUNT     32
#define PRESETSLIVE_PANEL_BTNS_PER_ROW  4
#define PRESETSLIVE_PANEL_BTN_WIDTH     94
#define PRESETSLIVE_PANEL_BTN_HEIGHT    32
#define PRESETSLIVE_PANEL_BTN_GAP_HORIZ 7
#define PRESETSLIVE_PANEL_BTN_GAP_VERT  7
#define PRESETSLIVE_PANEL_BTN_FONT      Arial_9_Bold

ButtonPanelInfo presetsLivePanelInfo = (ButtonPanelInfo) {
    PRESETSLIVE_PANEL_BTN_COUNT,        // btnCount
    PRESETSLIVE_PANEL_BTNS_PER_ROW,     // btnsPerRow
    PRESETSLIVE_PANEL_BTN_WIDTH,        // btnWidth
    PRESETSLIVE_PANEL_BTN_HEIGHT,       // btnHeight
    PRESETSLIVE_PANEL_BTN_GAP_HORIZ,    // btnGapHorizontal
    PRESETSLIVE_PANEL_BTN_GAP_VERT,     // btnGapVertical
    BTN_PANEL_BGD_COLOR_DEFAULT,    // panelBgdColor
    BTN_COLOR_DEFAULT,              // btnColor
    BTN_SEL_COLOR_DEFAULT,          // btnActiveColor
    PRESETSLIVE_PANEL_BTN_FONT,         // btnFont
    BTN_TEXT_COLOR_DEFAULT,         // btnTextColor
    BTN_SEL_TEXT_COLOR_DEFAULT,     // btnActiveTextColor
    BTN_OUTLINE_COLOR_DEFAULT       // btnOutlineColor
};

// Presets Live screen instance
//
PresetsLiveScreen presetsLiveScreen(presetsLiveDisplayInfo, presetsLivePanelInfo, Presets::presetsCallback, AudioPlatform::systemProfile);

#endif // PRESETSLIVECREENDEFS_H_
