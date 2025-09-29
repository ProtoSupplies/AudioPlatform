/************************************************************************************
*                             ViewMediaListsScreenDefs.h                            *
*                                                                                   *
*                       Copyright (c) 2025, Donald Swearingen                       *
*                                All Rights Reserved                                *
*************************************************************************************
*
* Description
*
*   Screen layout and definitions for the ViewMediaListsScreen of the AudioPlatform application.
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
#ifndef TESTVIEWMEDIASCREENDEFS_H_
#define TESTVIEWMEDIASCREENDEFS_H_

// Overall Screen Appearance
//
DisplayScreenInfo listViewScreenDisplayInfo = (DisplayScreenInfo) {
    DISPLAY_BGD_COLOR_DEFAULT,  // displayBgdColor
    90,                         // marginTop
    TEXTCOL_SCRN_MARGIN_BOTTOM, // marginBottom
    TEXTCOL_SCRN_MARGIN_LEFT,   // marginLeft
    TEXTCOL_SCRN_MARGIN_RIGHT,  // marginRight
    HDR_FONT_DEFAULT,           // headerFont
    HDR_TEXT_COLOR_DEFAULT,     // headerFontColor
    SUBHDR_FONT_DEFAULT,        // subHeaderFont
    SUBHDR_TEXT_COLOR_DEFAULT,  // subHeaderFontColor
    "AudioPlatform Media",      // screenHeader
    "View Contents of Media Sources", // screenSubHeader
    TextAlignLeft,              // textAlign
    true,                       // showPrevBtn
    "Back"                      // prevBtnText
};

// View Selection Button Panel
//
#define VIEW_PANEL_BTN_COUNT        6
#define VIEW_PANEL_BTNS_PER_ROW     3
#define VIEW_PANEL_BTN_WIDTH        180
#define VIEW_PANEL_BTN_HEIGHT       40
#define VIEW_PANEL_BTN_GAP_HORIZ    8
#define VIEW_PANEL_BTN_GAP_VERT     9

ButtonPanelInfo viewPanelInfo = (ButtonPanelInfo) {
    VIEW_PANEL_BTN_COUNT,           // btnCount
    VIEW_PANEL_BTNS_PER_ROW,        // btnsPerRow
    VIEW_PANEL_BTN_WIDTH,           // btnWidth
    VIEW_PANEL_BTN_HEIGHT,          // btnHeight
    VIEW_PANEL_BTN_GAP_HORIZ,       // btnGapHorizontal
    VIEW_PANEL_BTN_GAP_VERT,        // btnGapVertical
    BTN_PANEL_BGD_COLOR_DEFAULT,    // panelBgdColor
    COLOR_WHEAT,                // btnColor
    COLOR_GRAY,                     // btnActiveColor
    BTN_FONT_DEFAULT,               // btnFont
    BTN_TEXT_COLOR_DEFAULT,         // btnTextColor
    BTN_SEL_TEXT_COLOR_DEFAULT,     // btnActiveTextColor
    BTN_OUTLINE_COLOR_DEFAULT       // btnOutlineColor
};

const char* viewPanelBtnLabels[VIEW_PANEL_BTN_COUNT] = {
    "Teensy 4.1 SD Card",
    "Serial Flash",
    "PROGMEM",
    "Preset Banks",
    "FM Synth Banks",
    "Media Player Files"
};

// ViewMediaListsScreen screen instance
ViewMediaLists  viewMediaListsScreen(listViewScreenDisplayInfo, viewPanelInfo, viewPanelBtnLabels, AudioPlatform::systemProfile);

#endif // TESTVIEWMEDIASCREENDEFS_H_
