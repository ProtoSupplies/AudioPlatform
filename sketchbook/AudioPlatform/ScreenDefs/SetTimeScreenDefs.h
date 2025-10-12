/************************************************************************************
*                                SetTimeScreenDefs.h                                *
*                                                                                   *
*                       Copyright (c) 2025, Donald Swearingen                       *
*                                All Rights Reserved                                *
*************************************************************************************
*
* Description
*
*   Screen layout and definitions for SetTimeScreen of the AudioPlatform application.
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
#ifndef SETTIMESCREENDEFS_H_
#define SETTIMESCREENDEFS_H_

DisplayScreenInfo setTimeScreenDisplayInfo = (DisplayScreenInfo) {
    DISPLAY_BGD_COLOR_DEFAULT,  // displayBgdColor
    TEXTCOL_SCRN_MARGIN_TOP,    // marginTop
    TEXTCOL_SCRN_MARGIN_BOTTOM, // marginBottom
    TEXTCOL_SCRN_MARGIN_LEFT,   // marginLeft
    TEXTCOL_SCRN_MARGIN_RIGHT,  // marginRight
    HDR_FONT_DEFAULT,           // headerFont
    HDR_TEXT_COLOR_DEFAULT,     // headerFontColor
    SUBHDR_FONT_DEFAULT,        // subHeaderFont
    SUBHDR_TEXT_COLOR_DEFAULT,  // subHeaderFontColor
    "Set Time Screen",          // screenHeader
    "Manually Set RTC Time",    // screenSubHeader
    TextAlignLeft,              // textAlign
    true,                       // showPrevBtn
    "Back"                      // prevBtnText
};

static DateTimeDisplayInfo dtDisplayInfo = (DateTimeDisplayInfo) {

    50,                             // cellWidth
    50,                             // cellHeight
    5,                              // cellGap
    20,                             // yearTimeGap
    DATETIME_BGND_COLOR_DEFAULT,    // bgndColor
    DATETIME_OUTLINE_COLOR_DEFAULT, // outlineColor
    DATETIME_TEXT_COLOR_DEFAULT,    // fontColor
    CourierNew_20_Bold,             // cellValueFont
    Arial_9_Bold,                   // cellLabelFont
    true                            // showCellLabels
};

static DateTimeDisplayInfo dtEnterInfo = (DateTimeDisplayInfo) {

    60,                             // cellWidth
    60,                             // cellHeight
    7,                              // cellGap
    DATETIME_YEAR_TIME_GAP_DEFAULT, // yearTimeGap
    DATETIME_NEW_BGND_COLOR_DEFAULT, // bgndColor
    DATETIME_OUTLINE_COLOR_DEFAULT, // outlineColor
    DATETIME_NEW_TEXT_COLOR_DEFAULT, // fontColor
    CourierNew_28_Bold,             // cellValueFont
    Arial_8_Bold,                   // cellLabelFont
    false                           // showCellLabels
};

// Set Time screen instance
SetTimeScreen  setTimeScreen(setTimeScreenDisplayInfo, dtDisplayInfo, dtEnterInfo);

#endif // SETTIMESCREENDEFS_H_
