/************************************************************************************
*                                  UpDownButton.cpp                                 *
*                                                                                   *
*                       Copyright (c) 2025, Donald Swearingen                       *
*                                All Rights Reserved                                *
*************************************************************************************
*
* Description
*
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

/****************
* "Pre-Defines" *
*****************
*
* Definitions that affect subsequent external references
*
*/

/************************************************************************************
*                       E X T E R N A L   R E F E R E N C E S                       *
*************************************************************************************
*/

// User Arduino Libraries
//
#include "UpDownButton.h"

/************************************************************************************
*                    S T A T I C   C L A S S   V A R I A B L E S                    *
*************************************************************************************
*/

// Up/Down Button Panel
//
#define UPDOWN_PANEL_BTN_COUNT        2
#define UPDOWN_PANEL_BTNS_PER_ROW     1
#define UPDOWN_PANEL_BTN_WIDTH        60
#define UPDOWN_PANEL_BTN_HEIGHT       40
#define UPDOWN_PANEL_BTN_GAP_HORIZ    0
#define UPDOWN_PANEL_BTN_GAP_VERT     10

ButtonPanelInfo UpDownButton::upDownPanelInfo = (ButtonPanelInfo) {
    UPDOWN_PANEL_BTN_COUNT,         // btnCount
    UPDOWN_PANEL_BTNS_PER_ROW,      // btnsPerRow
    UPDOWN_PANEL_BTN_WIDTH,         // btnWidth
    UPDOWN_PANEL_BTN_HEIGHT,        // btnHeight
    UPDOWN_PANEL_BTN_GAP_HORIZ,     // btnGapHorizontal
    UPDOWN_PANEL_BTN_GAP_VERT,      // btnGapVertical
    BTN_PANEL_BGD_COLOR_DEFAULT,    // panelBgdColor
    COLOR_WHEAT,                // btnColor
    COLOR_GRAY,                     // btnActiveColor
    BTN_FONT_DEFAULT,               // btnFont
    BTN_TEXT_COLOR_DEFAULT,         // btnTextColor
    BTN_SEL_TEXT_COLOR_DEFAULT,     // btnActiveTextColor
    BTN_OUTLINE_COLOR_DEFAULT       // btnOutlineColor
};

// UpDownPanel Button Info
//
LabelIdButtonInfo UpDownButton::upDownPanelBtnInfo[] = {
    {"^^^", BtnUp},
    {"vvv", BtnDown}
};

/************************************************************************************
*                    P U B L I C   C L A S S   F U N C T I O N S                    *
*************************************************************************************
*/

// Constructor/Destructor
//
UpDownButton::UpDownButton(uint16_t btnX, uint16_t btnY) :
    upDownPanel(upDownPanelInfo) {

    upDownPanelX    = btnX;
    upDownPanelY    = btnY;
    upDownPanelWidth = upDownPanelInfo.btnWidth;
    currentUpDownBtn = upDownPanelInfo.btnHeight;

    touchHeld = false;
    touchHeldTime = cellTouchReportTime;
}

UpDownButton::~UpDownButton() {

}

// showButton
//
void UpDownButton::showButton() {

//    Serial.printf("UpDownButton::showPopup:\n");

    // Draw UpDown Panel
    upDownPanel.btnFont = upDownPanelInfo.btnFont;
    upDownPanel.setPanelPosition(upDownPanelX, upDownPanelY);
    upDownPanel.drawPanel(upDownPanelBtnInfo, currentUpDownBtn);

    touchHeld = false;
    touchHeldTime = cellTouchReportTime;
}

// removeButton
//
void UpDownButton::removeButton() {

}

// buttonTouched
//
bool UpDownButton::buttonTouched(int16_t pointX, int16_t pointY, ButtonId& btnId) {

    bool returnVal = false;

    btnId = BtnNone;

    if (upDownPanel.isPanelButton(pointX, pointY, btnId)) {
//        Serial.printf("UpDownButton::buttonTouched: %d\n", btnId);
        returnVal = true;
    } else {
        touchHeld = false;
    }

    return returnVal;
}

/************************************************************************************
*        P R I V A T E / P R O T E C T E D   C L A S S   F U N C T I O N S          *
*************************************************************************************
*/
