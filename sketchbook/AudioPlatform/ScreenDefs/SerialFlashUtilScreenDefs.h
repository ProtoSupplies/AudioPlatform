/************************************************************************************
*                            SerialFlashUtilScreenDefs.h                            *
*                                                                                   *
*                       Copyright (c) 2025, Donald Swearingen                       *
*                                All Rights Reserved                                *
*************************************************************************************
*
* Description
*
*   Screen layout and definitions for the Serial Flash Utilities screen of the AudioPlatform application.
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
#ifndef SERIALFLASHUTILSCREENDEFS_H_
#define SERIALFLASHUTILSCREENDEFS_H_

// Overall Screen Appearance
//
DisplayScreenInfo serialFlashUtilScreenDisplayInfo = (DisplayScreenInfo) {
    DISPLAY_BGD_COLOR_DEFAULT,  // displayBgdColor
    90,                         // marginTop
    TEXTCOL_SCRN_MARGIN_BOTTOM, // marginBottom
    TEXTCOL_SCRN_MARGIN_LEFT,   // marginLeft
    TEXTCOL_SCRN_MARGIN_RIGHT,  // marginRight
    HDR_FONT_DEFAULT,           // headerFont
    HDR_TEXT_COLOR_DEFAULT,     // headerFontColor
    SUBHDR_FONT_DEFAULT,        // subHeaderFont
    SUBHDR_TEXT_COLOR_DEFAULT,  // subHeaderFontColor
    "Serial Flash Utilities",   // screenHeader
    "Serial Flash Storage",     // screenSubHeader
    TextAlignLeft,              // textAlign
    true,                       // showPrevBtn
    "Back"                      // prevBtnText
};

// Command Button Panel
//
#define CMND_PANEL_BTN_COUNT        4
#define CMND_PANEL_BTNS_PER_ROW     1
#define CMND_PANEL_BTN_WIDTH        350
#define CMND_PANEL_BTN_HEIGHT       40
#define CMND_PANEL_BTN_GAP_HORIZ    1
#define CMND_PANEL_BTN_GAP_VERT     9

ButtonPanelInfo cmndPanelInfo = (ButtonPanelInfo) {
    CMND_PANEL_BTN_COUNT,           // btnCount
    CMND_PANEL_BTNS_PER_ROW,        // btnsPerRow
    CMND_PANEL_BTN_WIDTH,           // btnWidth
    CMND_PANEL_BTN_HEIGHT,          // btnHeight
    CMND_PANEL_BTN_GAP_HORIZ,       // btnGapHorizontal
    CMND_PANEL_BTN_GAP_VERT,        // btnGapVertical
    BTN_PANEL_BGD_COLOR_DEFAULT,    // panelBgdColor
    COLOR_WHEAT,                    // btnColor
    COLOR_GRAY,                     // btnActiveColor
    BTN_FONT_DEFAULT,               // btnFont
    BTN_TEXT_COLOR_DEFAULT,         // btnTextColor
    BTN_SEL_TEXT_COLOR_DEFAULT,     // btnActiveTextColor
    BTN_OUTLINE_COLOR_DEFAULT       // btnOutlineColor
};

const char* cmndPanelBtnLabels[4] = {
    "View Serial Flash Contents",
    "Erase Serial Flash",
    "View RAW Files on SD",
    "Copy RAW Files from SD to Serial Flash"
};

// Serial Flash Utilities screen instance
//
SerialFlashUtilScreen serialFlashUtilScreen(serialFlashUtilScreenDisplayInfo, cmndPanelInfo, cmndPanelBtnLabels, AudioPlatform::systemProfile);

#endif // SERIALFLASHUTILSCREENDEFS_H_
