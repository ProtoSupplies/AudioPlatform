/************************************************************************************
*                            ActivityMonitorScreenDefs.h                            *
*                                                                                   *
*                       Copyright (c) 2025, Donald Swearingen                       *
*                                All Rights Reserved                                *
*************************************************************************************
*
* Description
*
*   Screen layout and definitions for the ActivityMonitorScreen of the AudioPlatform application.
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
#ifndef ACTIVITYSCREENDEFS_H_
#define ACTIVITYSCREENDEFS_H_

/**************************
* Activity Monitor Screen *
***************************
*
*/

DisplayScreenInfo monitorDisplayInfo = (DisplayScreenInfo) {
    DISPLAY_BGD_COLOR_DEFAULT,  // displayBgdColor
    DEFAULT_SCRN_MARGIN_TOP,    // marginTop
    DEFAULT_SCRN_MARGIN_BOTTOM, // marginBottom
    DEFAULT_SCRN_MARGIN_LEFT,   // marginLeft
    DEFAULT_SCRN_MARGIN_RIGHT,  // marginRight
    HDR_FONT_DEFAULT,           // headerFont
    HDR_TEXT_COLOR_DEFAULT,     // headerFontColor
    SUBHDR_FONT_DEFAULT,        // subHeaderFont
    SUBHDR_TEXT_COLOR_DEFAULT,  // subHeaderFontColor
    "Activity Monitor",         // screenHeader
    "Levels and Resource Usage", // screenSubHeader
    TextAlignLeft,              // textAlign
    true,                       // showPrevBtn
    "Home"                      // prevBtnText
};

// Monitor display screen
MonitorScreen  monitorScreen(monitorDisplayInfo, AudioDevices::OutMixerLeft, AudioDevices::OutMixerRight);

#endif // ACTIVITYSCREENDEFS_H_
