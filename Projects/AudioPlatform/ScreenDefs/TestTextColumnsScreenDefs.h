/************************************************************************************
*                            TestTextColumnsScreenDefs.h                            *
*                                                                                   *
*                       Copyright (c) 2025, Donald Swearingen                       *
*                                All Rights Reserved                                *
*************************************************************************************
*
* Description
*
*   Screen layout and definitions for TestTextColumnsScreen of the AudioPlatform application.
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
#ifndef TESTTEXTCOLUMNSSCREENDEFS_H_
#define TESTTEXTCOLUMNSSCREENDEFS_H_

/************************
* Test3 Settings Screen *
*************************
*
*/

DisplayScreenInfo testTextColumnsScreenDisplayInfo = (DisplayScreenInfo) {
    DISPLAY_BGD_COLOR_DEFAULT,  // displayBgdColor
    TEXTCOL_SCRN_MARGIN_TOP,    // marginTop
    TEXTCOL_SCRN_MARGIN_BOTTOM, // marginBottom
    TEXTCOL_SCRN_MARGIN_LEFT,   // marginLeft
    TEXTCOL_SCRN_MARGIN_RIGHT,  // marginRight
    HDR_FONT_DEFAULT,           // headerFont
    HDR_TEXT_COLOR_DEFAULT,     // headerFontColor
    SUBHDR_FONT_DEFAULT,        // subHeaderFont
    SUBHDR_TEXT_COLOR_DEFAULT,  // subHeaderFontColor
    "Test Screen",              // screenHeader
    "Text Text Columns",        // screenSubHeader
    TextAlignLeft,              // textAlign
    true,                       // showPrevBtn
    "Back"                      // prevBtnText
};

#if 0 // One column test

TextColumnInfo testTextColumnsScreenTextColumnInfo = {
    TEXTCOL_SCRN_MARGIN_LEFT,           // xPos
    80,                                 // yPos
    190,                                // width
    400,                                // height
    25,                                 // lineHeight
    60,                                 // maxLines
    Arial_9_Bold,                       // font
    COLUMN_TEXT_COLOR_DEFAULT,          // textColor
    8,                                  // textMargin
    TextAlignLeft,                      // textAlign
    COLOR_BLACK,                        // columnColor
    COLOR_BLACK,                        // outlineColor
    COLUMN_SEL_OUTLINE_COLOR_DEFAULT    // selectedOutlineColor
    };

// System Profile Display Screen
TestTextColumns  testTextColumnsScreen(testTextColumnsScreenDisplayInfo, 1, &testTextColumnsScreenTextColumnInfo, AudioPlatform::systemProfile);

#else // Two column test

TextColumnInfo testTextColumnsScreenTextColumnInfo[2] = {
    {
    TEXTCOL_SCRN_MARGIN_LEFT,           // xPos
    80,                                 // yPos
    190,                                // width
    400,                                // height
    23,                                 // lineHeight
    8,                                  // maxLines
    Arial_9_Bold,                       // font
    COLUMN_TEXT_COLOR_DEFAULT,          // textColor
    8,                                  // textMargin
    TextAlignLeft,                      // textAlign
    COLUMN_COLOR_DEFAULT,               // columnColor
    COLOR_GRAY,                         // outlineColor
    COLUMN_SEL_OUTLINE_COLOR_DEFAULT    // selectedOutlineColor
    },

    {
    210,                                // xPos
    80,                                 // yPos
    190,                                // width
    400,                                // height
    23,                                 // lineHeight
    COLUMN_MAX_LINES_DEFAULT,           // maxLines
    Arial_9_Bold,                       // font
    COLUMN_TEXT_COLOR_DEFAULT,          // textColor
    8,                                  // textMargin
    TextAlignLeft,                      // textAlign
    COLUMN_COLOR_DEFAULT,               // columnColor
    COLOR_GRAY,                         // outlineColor
    COLUMN_SEL_OUTLINE_COLOR_DEFAULT    // selectedOutlineColor
    }
};

// TestTextColumns screen instance
TestTextColumns  testTextColumnsScreen(testTextColumnsScreenDisplayInfo, 2, testTextColumnsScreenTextColumnInfo, AudioPlatform::systemProfile);

#endif // One column test / Two column test


#endif // TESTTEXTCOLUMNSSCREENDEFS_H_
