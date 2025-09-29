/************************************************************************************
*                                UtilMenuScreenDefs.h                               *
*                                                                                   *
*                       Copyright (c) 2025, Donald Swearingen                       *
*                                All Rights Reserved                                *
*************************************************************************************
*
* Description
*
*   Screen layout and definitions for the UtilMenuScreen of the AudioPlatform application.
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
#ifndef UTILMENUSCREENDEFS_H_
#define UTILMENUSCREENDEFS_H_

/*******************
* Main Menu Screen *
********************
*
* This section defines structures passed to and used on the Main Menu screen, which
* presents a set of buttons which select the active screen to be displayed.
*
*/
#define SCRN_UTIL_MENU_BTN_COUNT    8

#define UTIL_MENU_MARGIN_TOP        90
#define UTIL_MENU_MARGIN_BOTTOM     90
#define UTIL_MENU_MARGIN_LEFT       10
#define UTIL_MENU_MARGIN_RIGHT      10
#define UTIL_MENU_BTN_WIDTH         150
#define UTIL_MENU_BTN_HEIGHT        60
#define UTIL_MENU_GAP_HORIZ         20
#define UTIL_MENU_GAP_VERT          20

DisplayScreenInfo utilMenuDisplayInfo = (DisplayScreenInfo) {
    DISPLAY_BGD_COLOR_DEFAULT,  // displayBgdColor
    UTIL_MENU_MARGIN_TOP,       // marginTop
    UTIL_MENU_MARGIN_BOTTOM,    // marginBottom
    UTIL_MENU_MARGIN_LEFT,      // marginLeft
    UTIL_MENU_MARGIN_RIGHT,     // marginRight
    HDR_FONT_DEFAULT,           // headerFont
    HDR_TEXT_COLOR_DEFAULT,     // headerFontColor
    SUBHDR_FONT_DEFAULT,        // subHeaderFont
    SUBHDR_TEXT_COLOR_DEFAULT,  // subHeaderFontColor
    "Utility Menu",             // screenHeader
    "System Management",        // screenSubHeader
    TextAlignLeft,              // textAlign
    true,                       // showPrevBtn
    "Home"                      // prevBtnText
};

ButtonPanelInfo utilMenuPanelInfo = (ButtonPanelInfo) {
    SCRN_UTIL_MENU_BTN_COUNT,   // btnCount
    4,                          // btnsPerRow
    UTIL_MENU_BTN_WIDTH,        // btnWidth
    UTIL_MENU_BTN_HEIGHT,       // btnHeight
    UTIL_MENU_GAP_HORIZ,        // btnGapHorizontal
    UTIL_MENU_GAP_VERT,         // btnGapVertical
    COLOR_BLACK,                // panelBgdColor
    BTN_SEL_COLOR_DEFAULT,      // btnColor
    BTN_SEL_COLOR_DEFAULT,      // btnActiveColor
    BTN_FONT_DEFAULT,           // btnFont
    BTN_SEL_TEXT_COLOR_DEFAULT, // btnTextColor
    BTN_SEL_TEXT_COLOR_DEFAULT, // btnActiveTextColor
    BTN_OUTLINE_COLOR_DEFAULT   // btnOutlineColor
};

// Screen Button Profiles
// Row 1
LabelIdButtonInfo    utilMenuBtn01  = (LabelIdButtonInfo) {"System Profile",    ScreenSystemInfo};
LabelIdButtonInfo    utilMenuBtn02  = (LabelIdButtonInfo) {"Media Viewer",      ScreenViewMedia};
LabelIdButtonInfo    utilMenuBtn03  = (LabelIdButtonInfo) {"Serial Flash",      ScreenSerialFlashUtil};
LabelIdButtonInfo    utilMenuBtn04  = (LabelIdButtonInfo) {"Set Time",          ScreenSetTime};

// Row 2
LabelIdButtonInfo    utilMenuBtn05  = (LabelIdButtonInfo) {"Color Viewer",      ScreenColors};
LabelIdButtonInfo    utilMenuBtn06  = (LabelIdButtonInfo) {"MIDI Settings",     ScreenMidi};
LabelIdButtonInfo    utilMenuBtn07  = (LabelIdButtonInfo) {"Test TextEdit",     ScreenTestTextEdit};
LabelIdButtonInfo    utilMenuBtn08  = (LabelIdButtonInfo) {"Test ListSelect",   ScreenTestListSelect};

LabelIdButtonInfo   utilMenuTestBtnInfo[SCRN_UTIL_MENU_BTN_COUNT] = {
        utilMenuBtn01,
        utilMenuBtn02,
        utilMenuBtn03,
        utilMenuBtn04,
        utilMenuBtn05,
        utilMenuBtn06,
        utilMenuBtn07,
        utilMenuBtn08
};

// UtilMenu screen instance
UtilMenuScreen  utilMenuScreen(utilMenuDisplayInfo, utilMenuPanelInfo, utilMenuTestBtnInfo);

#endif // UTILMENUSCREENDEFS_H_
