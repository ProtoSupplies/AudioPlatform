/************************************************************************************
*                               SelectPresetUtil.cpp                                *
*                                                                                   *
*                       Copyright (c) 2025, Donald Swearingen                       *
*                                All Rights Reserved                                *
*************************************************************************************
*
* Description
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

// Teensyduino
//
#include <SD.h>
#include <FS.h>
#include <SerialFlash.h>
#include <LittleFS.h>

// User Arduino Libraries
//
#include "SelectPresetUtil.h"
#include "ArduinoEclipseDefs.h"
#include "UtilFns.h"

/************************************************************************************
*                     D A T A / T Y P E   D E F I N I T I O N S                     *
*************************************************************************************
*/

/************************************************************************************
*                    S T A T I C   C L A S S   V A R I A B L E S                    *
*************************************************************************************
*/

// Exit Button Panel (single button)
//
#define EXIT_PANEL_BTN_COUNT        2
#define EXIT_PANEL_BTNS_PER_ROW     2
#define EXIT_PANEL_BTN_WIDTH        75
#define EXIT_PANEL_BTN_HEIGHT       40
#define EXIT_PANEL_BTN_GAP_HORIZ    5
#define EXIT_PANEL_BTN_GAP_VERT     0

ButtonPanelInfo SelectPresetUtil::exitPanelInfo = (ButtonPanelInfo) {
    EXIT_PANEL_BTN_COUNT,           // btnCount
    EXIT_PANEL_BTNS_PER_ROW,        // btnsPerRow
    EXIT_PANEL_BTN_WIDTH,           // btnWidth
    EXIT_PANEL_BTN_HEIGHT,          // btnHeight
    EXIT_PANEL_BTN_GAP_HORIZ,       // btnGapHorizontal
    EXIT_PANEL_BTN_GAP_VERT,        // btnGapVertical
    BTN_PANEL_BGD_COLOR_DEFAULT,    // panelBgdColor
    COLOR_GRAY,                     // btnColor
    COLOR_GRAY,                     // btnActiveColor
    BTN_FONT_DEFAULT,               // btnFont
    COLOR_WHITE,                    // btnTextColor
    COLOR_WHITE,                    // btnActiveTextColor
    BTN_OUTLINE_COLOR_DEFAULT       // btnOutlineColor
};

// Exit Panel Button Info
//
LabelIdButtonInfo SelectPresetUtil::exitPanelBtnInfo[] = {
    {"Select", BtnSelect},
    {"Cancel", BtnCancel},
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

ButtonPanelInfo SelectPresetUtil::presetsPanelInfo = (ButtonPanelInfo) {
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

ButtonPanel SelectPresetUtil::presetsPanel(presetsPanelInfo);

/************************************************************************************
*                    P U B L I C   C L A S S   F U N C T I O N S                    *
*************************************************************************************
*/

// Static Member Initializations
//

// Constructor/Destructor
//
SelectPresetUtil::SelectPresetUtil() :
//    presetsPanel(presetsPanelInfo),
    exitPanel(exitPanelInfo) {

    int16_t presetsPanelX   = (DISPLAY_WIDTH - presetsPanel.panelWidth) / 2;
    int16_t presetsPanelY   = 120;
    presetsPanel.setPanelPosition(presetsPanelX, presetsPanelY);

    pollEnabled       = false;

    popupHeading    = "";
    selectedPreset  = 0;

    // Workaround for B241001-1
    presetsPanelInfo.btnFont = PRESETS_PANEL_BTN_FONT;
}

SelectPresetUtil::~SelectPresetUtil() {

}

// showPopup
//
void SelectPresetUtil::showPopup() {

//    Serial.printf("SelectPresetUtil::showPopup:\n");

    DisplayScreenBase::fillScreen(COLOR_BLACK);

    // Clear and display screen
    showObjects();

    setSelectedPreset(selectedPreset);
    pollEnabled = true;
}

// removePopup
//
void SelectPresetUtil::removePopup() {

    pollEnabled = false;
}

// pollPopup
//
ButtonId SelectPresetUtil::pollPopup() {

    ButtonId pollResult = BtnNone;
    ButtonId touchedBtn = BtnNone;
    uint16_t pointX, pointY;

    // Check for touched buttons
    if (DisplayScreenBase::touched()) {
        DisplayScreenBase::getTouchedPoint(pointX, pointY);

        if (presetsPanel.isPanelButton(pointX, pointY, touchedBtn)) {
            setSelectedPreset(touchedBtn);
            pollResult = BtnNone;

        } else if (exitPanel.isPanelButton(pointX, pointY, touchedBtn)) {
            pollResult = exitPanelBtnInfo[touchedBtn].buttonId;
//            Serial.printf("SelectPresetUtil::pollPopup: touched exitBtn %d (%d) %s\n", touchedBtn, pollResult, exitPanelBtnInfo[touchedBtn].buttonLabel);
        }
    }

    return pollResult;
}

// setPopupHeading
//
void SelectPresetUtil::setPopupHeading(const char* text) {

    if (strlen(text) > 0) {
        popupHeading = text;
    } else {
        popupHeading = "";
    }
}

// setBtnLabels
//
void SelectPresetUtil::setBtnLabels(const char** btnLabels) {

//    Serial.printf("ParamButtonScreen::setBtnLabels: btnLabels %x\n", btnLabels);
    presetsPanelBtnLabels = btnLabels;

}

/************************************************************************************
*                   P R I V A T E   C L A S S   F U N C T I O N S                   *
*************************************************************************************
*/

// showObjects
//
void SelectPresetUtil::showObjects() {

//    Serial.printf("SelectPresetUtil::showObjects:\n");

    showPopupHeading();

    // Presets Panel position
    int16_t presetsPanelX = (DISPLAY_WIDTH - presetsPanel.panelWidth) / 2;
    int16_t presetsPanelY = 120;

    // Draw Presets Panel
    presetsPanel.setPanelPosition(presetsPanelX, presetsPanelY);
    presetsPanel.clearPanel();
    presetsPanel.drawPanel((const char**)presetsPanelBtnLabels);

    // Exit Panel position
    int16_t exitPanelX = (DISPLAY_WIDTH - exitPanel.panelWidth) / 2;
    int16_t exitPanelY = presetsPanelY + presetsPanel.panelHeight + 40;

    // Draw Exit Panel
    exitPanel.btnFont = exitPanelInfo.btnFont;
    exitPanel.setPanelPosition(exitPanelX, exitPanelY);
    exitPanel.drawPanel(exitPanelBtnInfo, currentExitBtn);
    currentExitBtn  = 0;

    setSelectedPreset(0);
}

// showPopupHeading
//
void SelectPresetUtil::showPopupHeading() {

    DisplayScreenBase::setFont(Arial_14_Bold);

    uint16_t textWid, textHgt;
    DisplayScreenBase::measureText(popupHeading, textWid, textHgt);

//    Serial.printf("SelectPresetUtil::showPopupHeading: popupHeading %s\n", popupHeading);

    TextArea tArea;
    tArea.posX = 250;
    tArea.posY = 50;
    tArea.wid = 300;
    tArea.hgt = textHgt;
    tArea.align = TextAlignCenter;

    DisplayScreenBase::setTextColor(COLOR_WHITE);
    DisplayScreenBase::printInArea(popupHeading, tArea);
}

// setSelectedPreset
//
// Set/change selected button
//
void SelectPresetUtil::setSelectedPreset(ButtonId btnNum) {

//    Serial.printf("SelectPresetUtil::setSelectedPreset: current selected %d, new selected %d\n", selectedPreset, btnNum);

    if (selectedPreset != BtnNone) {
        presetsPanel.drawButton(selectedPreset, false);
        presetsPanel.showButtonLabel(selectedPreset, presetsPanelBtnLabels[selectedPreset], false);
    }

    // Make touchedBtn active and redisplay
    selectedPreset = btnNum;
    presetsPanel.drawButton(selectedPreset, true);
    presetsPanel.showButtonLabel(selectedPreset, presetsPanelBtnLabels[selectedPreset], true);
}


/************************************************************************************
*                 P R O T E C T E D   C L A S S   F U N C T I O N S                 *
*************************************************************************************
*/



