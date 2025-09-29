/************************************************************************************
*                                MainMenuScreenDefs.h                               *
*                                                                                   *
*                       Copyright (c) 2025, Donald Swearingen                       *
*                                All Rights Reserved                                *
*************************************************************************************
*
* Description
*
*   Screen layout and definitions for the MainMenuScreen of the AudioPlatform application.
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
#ifndef MAINMENUSCREENDEFS_H_
#define MAINMENUSCREENDEFS_H_

/*******************
* Main Menu Screen *
********************
*
* This section defines structures passed to and used on the Main Menu screen, which
* presents a set of buttons which select the active screen to be displayed.
*
*/
#define SCRN_MAIN_MENU_BTN_COUNT    12

#define MAIN_MENU_MARGIN_TOP        125
#define MAIN_MENU_MARGIN_BOTTOM     125
#define MAIN_MENU_MARGIN_LEFT       70
#define MAIN_MENU_MARGIN_RIGHT      70
#define MAIN_MENU_BTN_WIDTH         150
#define MAIN_MENU_BTN_HEIGHT        60
#define MAIN_MENU_GAP_HORIZ         20
#define MAIN_MENU_GAP_VERT          20

#define MAIN_FOOTER_XPOS            MAIN_MENU_MARGIN_LEFT
#define MAIN_FOOTER_HEIGHT          40
#define MAIN_FOOTER_YPOS            (DISPLAY_HEIGHT - (MAIN_FOOTER_HEIGHT + 20))
#define MAIN_FOOTER_WIDTH           (DISPLAY_WIDTH - (MAIN_MENU_MARGIN_LEFT + MAIN_MENU_MARGIN_RIGHT))
#define MAIN_FOOTER_COLOR           COLOR_WHEAT
#define MAIN_FOOTER_OUTLINE_COLOR   COLOR_RED
#define MAIN_FOOTER_FONT            Arial_14
#define MAIN_FOOTER_FONT_COLOR      COLOR_BLACK

DisplayScreenInfo mainMenuDisplayInfo = (DisplayScreenInfo) {
    DISPLAY_BGD_COLOR_DEFAULT,  // displayBgdColor
    150,                        // marginTop
    MAIN_MENU_MARGIN_BOTTOM,    // marginBottom
    MAIN_MENU_MARGIN_LEFT,      // marginLeft
    MAIN_MENU_MARGIN_RIGHT,     // marginRight
    HDR_FONT_DEFAULT,           // headerFont
    HDR_TEXT_COLOR_DEFAULT,     // headerFontColor
    SUBHDR_FONT_DEFAULT,        // subHeaderFont
    SUBHDR_TEXT_COLOR_DEFAULT,  // subHeaderFontColor
    ApplicationName,            // screenHeader
    "for Teensy 4.1",           // screenSubHeader
    TextAlignCenter,            // textAlign
    false,                      // showPrevBtn
    ""                          // prevBtnText
};

ButtonPanelInfo mainMenuPanelInfo = (ButtonPanelInfo) {
    SCRN_MAIN_MENU_BTN_COUNT,   // btnCount
    4,                          // btnsPerRow
    MAIN_MENU_BTN_WIDTH,        // btnWidth
    MAIN_MENU_BTN_HEIGHT,       // btnHeight
    MAIN_MENU_GAP_HORIZ,        // btnGapHorizontal
    MAIN_MENU_GAP_VERT,         // btnGapVertical
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
LabelIdButtonInfo    mainMenuBtn01  = (LabelIdButtonInfo) {"SD Wav Player",     ScreenWavPlayer};
LabelIdButtonInfo    mainMenuBtn02  = (LabelIdButtonInfo) {"Flash Raw Player",  ScreenRawPlayer};
LabelIdButtonInfo    mainMenuBtn03  = (LabelIdButtonInfo) {"Basic Synth",       ScreenBasicSynth};
LabelIdButtonInfo    mainMenuBtn04  = (LabelIdButtonInfo) {"FM Synth",          ScreenFmSynth};
// Row 2
LabelIdButtonInfo    mainMenuBtn05  = (LabelIdButtonInfo) {"Stereo Delay",      ScreenStereoDelay};
LabelIdButtonInfo    mainMenuBtn06  = (LabelIdButtonInfo) {"Reverb",            ScreenReverb};
LabelIdButtonInfo    mainMenuBtn07  = (LabelIdButtonInfo) {"Future Efx",        ScreenEfx};
LabelIdButtonInfo    mainMenuBtn08  = (LabelIdButtonInfo) {"Audio IO",          ScreenInputOutput};

// Row 3
LabelIdButtonInfo    mainMenuBtn09  = (LabelIdButtonInfo) {"Activity Monitor",  ScreenMonitor};
LabelIdButtonInfo    mainMenuBtn10  = (LabelIdButtonInfo) {"Utility Menu",      ScreenUtilMenu};
LabelIdButtonInfo    mainMenuBtn11  = (LabelIdButtonInfo) {"Presets Manager",   ScreenPresets};
LabelIdButtonInfo    mainMenuBtn12  = (LabelIdButtonInfo) {"Presets Live",      ScreenLive};

LabelIdButtonInfo   mainMenuBtnInfo[SCRN_MAIN_MENU_BTN_COUNT] = {
                    // Row 1
                    mainMenuBtn01,
                    mainMenuBtn02,
                    mainMenuBtn03,
                    mainMenuBtn04,
                    // Row 2
                    mainMenuBtn05,
                    mainMenuBtn06,
                    mainMenuBtn07,
                    mainMenuBtn08,
                    // Row 3
                    mainMenuBtn09,
                    mainMenuBtn10,
                    mainMenuBtn11,
                    mainMenuBtn12
};

// Main menu screen instance
MainMenuScreen  mainMenuScreen(mainMenuDisplayInfo, mainMenuPanelInfo, mainMenuBtnInfo);

#endif // MAINMENUSCREENDEFS_H_
