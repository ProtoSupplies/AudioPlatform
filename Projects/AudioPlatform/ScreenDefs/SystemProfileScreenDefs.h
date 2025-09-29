/************************************************************************************
*                             SystemProfileScreenDefs.h                             *
*                                                                                   *
*                       Copyright (c) 2025, Donald Swearingen                       *
*                                All Rights Reserved                                *
*************************************************************************************
*
* Description
*
*   Screen layout and definitions for the SystemProfileScreen of the AudioPlatform application.
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
#ifndef SYSPROFILESCREENDEFS_H_
#define SYSPROFILESCREENDEFS_H_

/************************
* System Profile Screen *
*************************
*
*/

DisplayScreenInfo profileDisplayInfo = (DisplayScreenInfo) {
    DISPLAY_BGD_COLOR_DEFAULT,  // displayBgdColor
    TEXTCOL_SCRN_MARGIN_TOP,    // marginTop
    TEXTCOL_SCRN_MARGIN_BOTTOM, // marginBottom
    TEXTCOL_SCRN_MARGIN_LEFT,   // marginLeft
    TEXTCOL_SCRN_MARGIN_RIGHT,  // marginRight
    HDR_FONT_DEFAULT,           // headerFont
    HDR_TEXT_COLOR_DEFAULT,     // headerFontColor
    SUBHDR_FONT_DEFAULT,        // subHeaderFont
    SUBHDR_TEXT_COLOR_DEFAULT,  // subHeaderFontColor
    "System Profile",           // screenHeader
    "Application and Hardware", // screenSubHeader
    TextAlignLeft,              // textAlign
    true,                       // showPrevBtn
    "Back"                      // prevBtnText
};

TextColumnInfo screenTextColumnInfo[2] = {
    {
    60,                                 // xPos
    120,                                // yPos
    250,                                // width
    COLUMN_HEIGHT_DEFAULT,              // height
    COLUMN_LINE_HEIGHT_DEFAULT,         // lineHeight
    10,                                 // maxLines
    Arial_13,                           // font
    COLUMN_TEXT_COLOR_DEFAULT,          // textColor
    5,                                  // textMargin
    TextAlignRight,                     // textAlign
    COLUMN_COLOR_DEFAULT,               // columnColor
    COLUMN_OUTLINE_COLOR_DEFAULT,       // outlineColor
    COLUMN_OUTLINE_COLOR_DEFAULT        // selectedOutlineColor
    },

    {
    330,                                // xPos
    120,                                // yPos
    300,                                // width
    COLUMN_HEIGHT_DEFAULT,              // height
    COLUMN_LINE_HEIGHT_DEFAULT,         // lineHeight
    10,                                 // maxLines
    Arial_13,                           // font
    COLUMN_TEXT_COLOR_DEFAULT,          // textColor
    5,                                  // textMargin
    TextAlignLeft,                      // textAlign
    COLUMN_COLOR_DEFAULT,               // columnColor
    COLUMN_OUTLINE_COLOR_DEFAULT,       // outlineColor
    COLUMN_OUTLINE_COLOR_DEFAULT        // selectedOutlineColor
    }
};

// System Profile Display screen instance
SystemProfileScreen  profileScreen(profileDisplayInfo, 2, screenTextColumnInfo, AudioPlatform::systemProfile);


#endif // SYSPROFILESCREENDEFS_H_
