/************************************************************************************
*                              SerialFlashUtilBase.cpp                              *
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
#include "SerialFlashUtilBase.h"

/************************************************************************************
*                         M A C R O S  /  C O N S T A N T S                         *
*************************************************************************************
*/

#define CANCEL_BTN_INDEX            0
#define COPY_BTN_INDEX              1

#define POPUP_HEADING_FONT          Arial_20_Bold
#define POPUP_HEADING_FONT_COLOR    COLOR_WHITE
#define USER_MSG_FONT               Arial_12_Bold
#define USER_MSG_FONT_COLOR         COLOR_WHITE
#define STATUS_DISPLAY_FONT         Arial_14_Bold
#define STATUS_DISPLAY_FONT_COLOR   COLOR_WHITE
#define SF_SCREEN_BGND_COLOR        COLOR_BLACK

/************************************************************************************
*                    S T A T I C   C L A S S   V A R I A B L E S                    *
*************************************************************************************
*/

// Progress Display Button Panel
//
#define PROGRESS_PANEL_BTN_COUNT        150
#define PROGRESS_PANEL_BTNS_PER_ROW     30
#define PROGRESS_PANEL_BTN_WIDTH        15
#define PROGRESS_PANEL_BTN_HEIGHT       15
#define PROGRESS_PANEL_BTN_GAP_HORIZ    2
#define PROGRESS_PANEL_BTN_GAP_VERT     0

ButtonPanelInfo SerialFlashUtilBase::progressDisplayPanellInfo = (ButtonPanelInfo) {
    PROGRESS_PANEL_BTN_COUNT,       // btnCount
    PROGRESS_PANEL_BTNS_PER_ROW,    // btnsPerRow
    PROGRESS_PANEL_BTN_WIDTH,       // btnWidth
    PROGRESS_PANEL_BTN_HEIGHT,      // btnHeight
    PROGRESS_PANEL_BTN_GAP_HORIZ,   // btnGapHorizontal
    PROGRESS_PANEL_BTN_GAP_VERT,    // btnGapVertical
    COLOR_BLACK,                    // panelBgdColor
    COLOR_BLACK,                    // btnColor
    COLOR_LIMEGREEN,                // btnActiveColor
    BTN_FONT_DEFAULT,               // btnFont
    BTN_TEXT_COLOR_DEFAULT,         // btnTextColor
    BTN_SEL_TEXT_COLOR_DEFAULT,     // btnActiveTextColor
    COLOR_ORANGERED                 // btnOutlineColor
};

// Exit Button Panel (single button)
//
#define EXIT_PANEL_BTN_COUNT        2
#define EXIT_PANEL_BTNS_PER_ROW     2
#define EXIT_PANEL_BTN_WIDTH        70
#define EXIT_PANEL_BTN_HEIGHT       40
#define EXIT_PANEL_BTN_GAP_HORIZ    8
#define EXIT_PANEL_BTN_GAP_VERT     0

ButtonPanelInfo SerialFlashUtilBase::execPanelInfo = (ButtonPanelInfo) {
    EXIT_PANEL_BTN_COUNT,           // btnCount
    EXIT_PANEL_BTNS_PER_ROW,        // btnsPerRow
    EXIT_PANEL_BTN_WIDTH,           // btnWidth
    EXIT_PANEL_BTN_HEIGHT,          // btnHeight
    EXIT_PANEL_BTN_GAP_HORIZ,       // btnGapHorizontal
    EXIT_PANEL_BTN_GAP_VERT,        // btnGapVertical
    BTN_PANEL_BGD_COLOR_DEFAULT,    // panelBgdColor
    COLOR_WHEAT,                    // btnColor
    COLOR_GRAY,                     // btnActiveColor
    BTN_FONT_DEFAULT,               // btnFont
    BTN_TEXT_COLOR_DEFAULT,         // btnTextColor
    BTN_SEL_TEXT_COLOR_DEFAULT,     // btnActiveTextColor
    BTN_OUTLINE_COLOR_DEFAULT       // btnOutlineColor
};

// Exit Panel Button Info
//
LabelIdButtonInfo SerialFlashUtilBase::execPanelBtnInfo[] = {
    {"Cancel", BtnExit},
    {"Begin",  BtnProceed},
};

// Buffer for User Messages
//
char SerialFlashUtilBase::messageBuf[];

/************************************************************************************
*                    P U B L I C   C L A S S   F U N C T I O N S                    *
*************************************************************************************
*/

// Constructor/Destructor
//
SerialFlashUtilBase::SerialFlashUtilBase(SystemHardwareProfile& _sysProfile) :
        sysProfile(_sysProfile), execPanel(execPanelInfo),
        progressDisplayPanel(progressDisplayPanellInfo) {

    pollEnabled     = false;

    execPanelX      = (DISPLAY_WIDTH - execPanel.panelWidth) / 2;
    execPanelY      = DISPLAY_HEIGHT - (execPanel.panelHeight + 50);
    currentExecBtn  = CANCEL_BTN_INDEX;

    progressDisplayPanel.setButtonShape(false);      // Use square buttons
    progressDisplayPanelX = (DISPLAY_WIDTH - progressDisplayPanel.panelWidth) / 2;
    progressDisplayPanelY = 180;
    currentProgressDisplayBtn = 0;

    popupHeading = "";

    headingMsgAreaY         = 50;
    headingMsgAreaHgt       = 50;
    infoMsgAreaY            = 100;
    infoMsgAreaHgt          = 60;
    statusMsgAreaY          = 300;
    statusMsgAreaHgt        = 60;

    serialFlashPresent = false;
    serialFlashBytes = 0;
    sdCardPresent = false;
}

SerialFlashUtilBase::~SerialFlashUtilBase() {

}

// showPopup
//
void SerialFlashUtilBase::showPopup() {

//    Serial.printf("SerialFlashUtilBase::showPopup:\n");

    DisplayScreenBase::fillScreen(COLOR_BLACK);

    showPopupHeading();
    showObjects();

    pollEnabled = true;
}

// removePopup
//
void SerialFlashUtilBase::removePopup() {

    pollEnabled = false;
}

// showObjects
//
void SerialFlashUtilBase::showObjects() {

//    Serial.printf("SerialFlashUtilBase::showObjects:\n");

    // Re-establish font settings (not set at compile time)
    execPanel.btnFont = execPanelInfo.btnFont;

    // Draw Execute Panel
    execPanel.setPanelPosition(execPanelX, execPanelY);
    execPanel.drawPanel(execPanelBtnInfo, currentExecBtn);
    execPanel.setSelected(execPanelBtnInfo, currentExecBtn);

    // Draw Progress Display Panel
    progressDisplayPanel.setPanelPosition(progressDisplayPanelX, progressDisplayPanelY);
    progressDisplayPanel.drawPanelButtons();
}

// pollPopup
//
ButtonId SerialFlashUtilBase::pollPopup() {

    ButtonId touchedBtn = BtnNone;
    uint16_t pointX, pointY;

    // Check for touched buttons
    if (DisplayScreenBase::touched()) {
        DisplayScreenBase::getTouchedPoint(pointX, pointY);

        if (execPanel.isPanelButton(pointX, pointY, touchedBtn)) {
            touchedBtn = handleExecuteBtnPress(touchedBtn);
        }
    }

    return touchedBtn;
}

// setPopupHeading
//
void SerialFlashUtilBase::setPopupHeading(const char* text) {

//    Serial.printf("SerialFlashUtilBase::setPopupHeading: popupHeading %s\n", text);

    if (strlen(text) > 0) {
        popupHeading = text;
    } else {
        popupHeading = "";
    }
}

/************************************************************************************
*        P R I V A T E / P R O T E C T E D   C L A S S   F U N C T I O N S          *
*************************************************************************************
*/

// showPopupHeading
//
void SerialFlashUtilBase::showPopupHeading() {

    DisplayScreenBase::setFont(POPUP_HEADING_FONT);

    uint16_t textWid, textHgt;
    DisplayScreenBase::measureText(popupHeading, textWid, textHgt);

//    Serial.printf("SerialFlashUtilBase::showPopupHeading: popupHeading %s\n", popupHeading);

    TextArea tArea;
    tArea.posX = 0;
    tArea.posY = headingMsgAreaY;
    tArea.wid = DISPLAY_WIDTH;;
    tArea.hgt = 40;
    tArea.align = TextAlignCenter;

    DisplayScreenBase::setTextColor(POPUP_HEADING_FONT_COLOR);
    DisplayScreenBase::printInArea(popupHeading, tArea);
}

// showInfoMsg
//
void SerialFlashUtilBase::showInfoMsg(const char* text) {

//    Serial.printf("SerialFlashUtilBase::showInfoMsg: text %s (len %d)\n", text, strlen(text));

    TextArea tArea;
    tArea.posX = 0;
    tArea.posY = infoMsgAreaY;
    tArea.wid = DISPLAY_WIDTH;;
    tArea.hgt = infoMsgAreaHgt;
    tArea.align = TextAlignCenter;

    DisplayScreenBase::fillRect(tArea.posX, tArea.posY, tArea.wid, tArea.hgt, SF_SCREEN_BGND_COLOR);

    DisplayScreenBase::setFont(USER_MSG_FONT);
    DisplayScreenBase::setTextColor(USER_MSG_FONT_COLOR);
    DisplayScreenBase::printInArea(text, tArea);
}

// showStatusMsg
//
void SerialFlashUtilBase::showStatusMsg(const char* text) {

//    Serial.printf("SerialFlashUtilBase::showStatusMsg: text %s (len %d)\n", text, strlen(text));

        TextArea tArea;
        tArea.posX = 0;
        tArea.posY = statusMsgAreaY;
        tArea.wid = DISPLAY_WIDTH;;
        tArea.hgt = statusMsgAreaHgt;
        tArea.align = TextAlignCenter;

        DisplayScreenBase::fillRect(tArea.posX, tArea.posY, tArea.wid, tArea.hgt, SF_SCREEN_BGND_COLOR);

        DisplayScreenBase::setFont(STATUS_DISPLAY_FONT);
        DisplayScreenBase::setTextColor(STATUS_DISPLAY_FONT_COLOR);
        DisplayScreenBase::printInArea(text, tArea);
}

// handleExecuteBtnPress
//
ButtonId SerialFlashUtilBase::handleExecuteBtnPress(ButtonId btnId) {

    ButtonId returnVal = BtnNone;
    ButtonId execCmd = execPanelBtnInfo[btnId].buttonId;

//    Serial.printf("SerialFlashUtilScreen::handleExecuteBtnPress: btnId %d execCmd %d %s\n", btnId, execCmd, execPanelBtnInfo[btnId].buttonLabel);

    if (execCmd == BtnProceed) {
        execute();

    } else {

        currentExecBtn = CANCEL_BTN_INDEX;
        execPanel.setSelected(execPanelBtnInfo, currentExecBtn);

        returnVal = BtnCancel;
    }

    return returnVal;
}


