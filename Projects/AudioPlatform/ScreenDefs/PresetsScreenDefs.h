/************************************************************************************
*                                PresetsScreenDefs.h                                *
*                                                                                   *
*                       Copyright (c) 2025, Donald Swearingen                       *
*                                All Rights Reserved                                *
*************************************************************************************
*
* Description
*
*   Screen layout and definitions for the PresetsScreen of the AudioPlatform application.
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
#ifndef PRESETSCREENDEFS_H_
#define PRESETSCREENDEFS_H_

// Overall Screen Appearance
//
#define PRESETS_SCRN_MARGIN_TOP     80
#define PRESETS_SCRN_MARGIN_BOTTOM  50
#define PRESETS_SCRN_MARGIN_LEFT    DEFAULT_SCRN_MARGIN_LEFT
#define PRESETS_SCRN_MARGIN_RIGHT   DEFAULT_SCRN_MARGIN_RIGHT

DisplayScreenInfo presetsDisplayInfo = (DisplayScreenInfo) {
    DISPLAY_BGD_COLOR_DEFAULT,      // displayBgdColor
    PRESETS_SCRN_MARGIN_TOP,        // marginTop
    PRESETS_SCRN_MARGIN_BOTTOM,     // marginBottom
    PRESETS_SCRN_MARGIN_LEFT,       // marginLeft
    PRESETS_SCRN_MARGIN_RIGHT,      // marginRight
    HDR_FONT_DEFAULT,               // headerFont
    HDR_TEXT_COLOR_DEFAULT,         // headerFontColor
    SUBHDR_FONT_DEFAULT,            // subHeaderFont
    SUBHDR_TEXT_COLOR_DEFAULT,      // subHeaderFontColor
    "Presets Management",           // screenHeader
    "Load,Save,Edit Presets & Banks", // screenSubHeader
    TextAlignLeft,                  // textAlign
    true,                           // showPrevBtn
    "Home"                          // prevBtnText
};

// Presets Button Panel
//
#define PRESETS_PANEL_BTN_COUNT     32
#define PRESETS_PANEL_BTNS_PER_ROW  8
#define PRESETS_PANEL_BTN_WIDTH     92
#define PRESETS_PANEL_BTN_HEIGHT    36
#define PRESETS_PANEL_BTN_GAP_HORIZ 6
#define PRESETS_PANEL_BTN_GAP_VERT  22
#define PRESETS_PANEL_BTN_FONT      Arial_9_Bold

ButtonPanelInfo presetsPanelInfo = (ButtonPanelInfo) {
    PRESETS_PANEL_BTN_COUNT,        // btnCount
    PRESETS_PANEL_BTNS_PER_ROW,     // btnsPerRow
    PRESETS_PANEL_BTN_WIDTH,        // btnWidth
    PRESETS_PANEL_BTN_HEIGHT,       // btnHeight
    PRESETS_PANEL_BTN_GAP_HORIZ,    // btnGapHorizontal
    PRESETS_PANEL_BTN_GAP_VERT,     // btnGapVertical
    BTN_PANEL_BGD_COLOR_DEFAULT,    // panelBgdColor
    BTN_COLOR_DEFAULT,              // btnColor
    BTN_SEL_COLOR_DEFAULT,          // btnActiveColor
    PRESETS_PANEL_BTN_FONT,         // btnFont
    BTN_TEXT_COLOR_DEFAULT,         // btnTextColor
    BTN_SEL_TEXT_COLOR_DEFAULT,     // btnActiveTextColor
    BTN_OUTLINE_COLOR_DEFAULT       // btnOutlineColor
};

// Preset Command Button Panel
//
#define PRESET_CMND_PANELBTN_COUNT        6
#define PRESET_CMND_PANELBTNS_PER_ROW     6
#define PRESET_CMND_PANELBTN_WIDTH        60
#define PRESET_CMND_PANELBTN_HEIGHT       36
#define PRESET_CMND_PANELBTN_GAP_HORIZ    4
#define PRESET_CMND_PANELBTN_GAP_VERT     0
#define PRESET_CMND_PANELBTN_FONT         Arial_9_Bold

ButtonPanelInfo presetCmndPanelInfo = (ButtonPanelInfo) {
    PRESET_CMND_PANELBTN_COUNT,     // btnCount
    PRESET_CMND_PANELBTNS_PER_ROW,  // btnsPerRow
    PRESET_CMND_PANELBTN_WIDTH,     // btnWidth
    PRESET_CMND_PANELBTN_HEIGHT,    // btnHeight
    PRESET_CMND_PANELBTN_GAP_HORIZ, // btnGapHorizontal
    PRESET_CMND_PANELBTN_GAP_VERT,  // btnGapVertical
    BTN_PANEL_BGD_COLOR_DEFAULT,    // panelBgdColor
    COLOR_WHEAT,                // btnColor
    COLOR_GRAY,                     // btnActiveColor
    PRESET_CMND_PANELBTN_FONT,      // btnFont
    BTN_TEXT_COLOR_DEFAULT,         // btnTextColor
    BTN_SEL_TEXT_COLOR_DEFAULT,     // btnActiveTextColor
    BTN_OUTLINE_COLOR_DEFAULT       // btnOutlineColor
};

// Preset Command Panel Buttons
// The button IDs and labels are defined here and will remain unchanged at run-time.
//
LabelIdButtonInfo presetCmndPanelBtnInfo[] = {
    {"Load",    BtnLoad},
    {"Save",    BtnSave},
    {"Import",  BtnImport},
    {"Clear",   BtnClear},
    {"Rename",  BtnRename},
    {"Cancel",  BtnCancel}
};

// Bank Command Button Panel
//
#define BANK_CMND_PANELBTN_COUNT        5
#define BANK_CMND_PANELBTNS_PER_ROW     5
#define BANK_CMND_PANELBTN_WIDTH        60
#define BANK_CMND_PANELBTN_HEIGHT       36
#define BANK_CMND_PANELBTN_GAP_HORIZ    4
#define BANK_CMND_PANELBTN_GAP_VERT     0
#define BANK_CMND_PANELBTN_FONT         Arial_9_Bold

ButtonPanelInfo bankCmndPanelInfo = (ButtonPanelInfo) {
    BANK_CMND_PANELBTN_COUNT,       // btnCount
    BANK_CMND_PANELBTNS_PER_ROW,    // btnsPerRow
    BANK_CMND_PANELBTN_WIDTH,       // btnWidth
    BANK_CMND_PANELBTN_HEIGHT,      // btnHeight
    BANK_CMND_PANELBTN_GAP_HORIZ,   // btnGapHorizontal
    BANK_CMND_PANELBTN_GAP_VERT,    // btnGapVertical
    BTN_PANEL_BGD_COLOR_DEFAULT,    // panelBgdColor
    COLOR_WHEAT,                // btnColor
    COLOR_GRAY,                     // btnActiveColor
    BANK_CMND_PANELBTN_FONT,        // btnFont
    BTN_TEXT_COLOR_DEFAULT,         // btnTextColor
    BTN_SEL_TEXT_COLOR_DEFAULT,     // btnActiveTextColor
    BTN_OUTLINE_COLOR_DEFAULT       // btnOutlineColor
};

// Bank Command Panel Buttons
// The button IDs and labels are defined here and will remain unchanged at run-time.
//
LabelIdButtonInfo bankCmndPanelBtnInfo[] = {
    {"Load",    BtnLoad},
    {"Save",    BtnSave},
    {"Default", BtnDefaultBank},
    {"SD Card", BtnSdCardBank},
    {"Cancel",  BtnCancel}
};

// Presets Screen instance
//
PresetsScreen presetsScreen(presetsDisplayInfo, presetsPanelInfo, presetCmndPanelInfo, presetCmndPanelBtnInfo,
    bankCmndPanelInfo, bankCmndPanelBtnInfo, Presets::presetsCallback, AudioPlatform::systemProfile);

#endif // PRESETSCREENDEFS_H_
