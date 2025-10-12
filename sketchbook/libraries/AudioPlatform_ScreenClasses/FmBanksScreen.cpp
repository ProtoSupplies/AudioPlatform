/************************************************************************************
*                                 FmBanksScreen.cpp                                 *
*                                                                                   *
*                       Copyright (c) 2025, Donald Swearingen                       *
*                                All Rights Reserved                                *
*************************************************************************************
*
* Description
*
*   FmBanksScreen is a custom AudioPlatform screen derived from the generic base class
*   DisplayScreebBase.
*
* Single Instance Class
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
* Modifications List
*
*   •
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

#include "FmBanksScreen.h"
#include "ArduinoEclipseDefs.h"
#include "BitMapData.h"

/************************************************************************************
*                         M A C R O S  /  C O N S T A N T S                         *
*************************************************************************************
*/

/************************************************************************************
*                    S T A T I C   C L A S S   V A R I A B L E S                    *
*************************************************************************************
*/

ScreenButtonInfo FmBanksScreen::exitBtnInfo = (ScreenButtonInfo) {
     10,                            // btnX
     155,                           // btnY
     36,                            // btnWid
     36,                            // btnHgt
     COLOR_BLACK,                   // btnColor
     COLOR_WHITE,                   // btnOutlineColor
     0,                             // btnOutlineRadius
     "FM Synth",                    // btnLabel (set at runtime)
     COLOR_WHITE,                   // btnLabelColor
     Arial_9_Bold,                  // btnLabelFont (note: must set again in constructor B241001-1)
     SCREENBUTTON_TEXT_ALIGN,       // btnLabelAlign
     TextBelow,                     // btnLabelPosition
     ExitIcon_35x35,                // bitMapInfo
};

#if 1  // Test multiple instance of static with same name

static ScreenButtonInfo xxxBtnInfo = {
     10,                            // btnX
     155,                           // btnY
     36,                            // btnWid
     36,                            // btnHgt
     COLOR_BLACK,                   // btnColor
     COLOR_WHITE,                   // btnOutlineColor
     0,                             // btnOutlineRadius
     "FM Synth",                    // btnLabel (set at runtime)
     COLOR_WHITE,                   // btnLabelColor
     Arial_9_Bold,                  // btnLabelFont (note: must set again in constructor B241001-1)
     SCREENBUTTON_TEXT_ALIGN,       // btnLabelAlign
     TextBelow,                     // btnLabelPosition
     ExitIcon_35x35,                // bitMapInfo
};

#endif // Test multiple instance of static with same name

/************************************************************************************
*                    P U B L I C   C L A S S   F U N C T I O N S                    *
*************************************************************************************
*/

// Static Member Initializations
//

// Constructor/Destructor
//
FmBanksScreen::FmBanksScreen(DisplayScreenInfo& displayInfo, ButtonPanelInfo presetSelectPanelInfo, ButtonPanelInfo bankSelectPanelInfo,
    const char** _presetSelectLabels, const char** _bankSelectLabels, uint16_t paramId, void (*_setPresetFn)(uint16_t, float), float (*_getPresetFn)(uint16_t)) :

    DisplayScreenBase(displayInfo), presetSelectPanel(presetSelectPanelInfo), bankSelectPanel(bankSelectPanelInfo),
    presetSelectLabels(_presetSelectLabels), bankSelectLabels(_bankSelectLabels), presetNumId(paramId), setPresetFn(_setPresetFn), getPresetFn(_getPresetFn),
    exitBtn(exitBtnInfo) {

    // Place panels centered between left and right margins
    displayAreaX1           = marginLeft;
    displayAreaX2           = DISPLAY_WIDTH - marginRight;
    displayAreaWidth        = displayAreaX2 - displayAreaX1;

    bankSelectPanelWid      = displayAreaWidth;
    bankSelectPanelHgt      = 70;
    presetSelectPanelX      = displayAreaX1 + ((displayAreaWidth - presetSelectPanel.panelWidth) / 2);
    presetSelectPanelY       = marginTop + 10;
    presetSelectPanel.setPanelPosition(presetSelectPanelX, presetSelectPanelY);

    presetSelectPanelWid    = displayAreaWidth;
    presetSelectPanelHgt    = 150;
    bankSelectPanelX        = displayAreaX1 + ((displayAreaWidth - presetSelectPanel.panelWidth) / 2);
    bankSelectPanelY        = presetSelectPanelY + presetSelectPanelHgt + 60;
    bankSelectPanel.setPanelPosition(bankSelectPanelX, bankSelectPanelY);

    exitBtnWid              = 70;
    exitBtnHgt              = 40;
    exitBtnX                = (DISPLAY_WIDTH - exitBtnWid) / 2;
    exitBtnY                = DISPLAY_HEIGHT - marginBottom + exitBtnHgt / 2;

    // Workaround for B241001-1
    exitBtnInfo.btnLabelFont    = Arial_9_Bold;
    exitBtnInfo.btnX            = (DISPLAY_WIDTH - exitBtnInfo.btnWid) / 2;;
    exitBtnInfo.btnY            = DISPLAY_HEIGHT - (marginBottom + 6) + exitBtnInfo.btnHgt / 2;

    currentBank             = 0;
    currentPreset           = 0;
    currentPresetBase       = 0;
}

FmBanksScreen::~FmBanksScreen() {

}

// activateScreen
//
// Reset/display dynamic screen elements each time screen is displayed
//
void FmBanksScreen::activateScreen() {

    // Screen setup
    clearScreen();
    showScreenHeading();

    if (getPresetFn) {
        uint16_t screenPreset = getPresetFn(presetNumId) + 0.5;  // Round to nearest int

        currentBank = screenPreset / presetSelectPanel.btnCount;
        currentPreset = screenPreset % presetSelectPanel.btnCount;
        currentPresetBase = currentBank * presetSelectPanel.btnCount; // Fix bug B241229-1

        selectPreset();
    }

    // Display screen panels
    drawBanksPanel();
    drawPresetsPanel();
//    drawExitButton();
    exitBtn.drawButton();
}

// deActivateScreen
//
void FmBanksScreen::deActivateScreen() {

    exitBtn.removeButton();
}

// pollScreen
//
// In addition to monitoring touchable screen buttons, FmBanksScreen's real-time
// display functions are "piggy-backed" on the pollScreen() function call.
//
// Only the "Home/Back" button will return a value other than BtnNone. Detected
// buttons related to monitoring are processed within FmBanksScreen.
//
ButtonId FmBanksScreen::pollScreen() {

    ButtonId touchedBtn = BtnNone;
    uint16_t pointX, pointY;

    // Check for touched buttons
    if (DisplayScreenBase::touched()) {
        DisplayScreenBase::getTouchedPoint(pointX, pointY);

        // First check for Home/Back button. If pressed, we'll return that value to main application.
        if (DisplayScreenBase::prevBtnTouched(pointX, pointY)) {
            touchedBtn = BtnPrev;

//        } else if (DisplayScreenBase::rectTouched(pointX, pointY, exitBtnX, exitBtnY, exitBtnWid, exitBtnHgt)) {
//            touchedBtn = BtnPrev;

        } else if (exitBtn.buttonTouched(pointX, pointY)) {
                touchedBtn = BtnPrev;

        } else if (bankSelectPanel.isPanelButton(pointX, pointY, touchedBtn)) {
            if (touchedBtn != currentBank) {
                // Highlight newly selected bank button
                bankSelectPanel.drawButton(currentBank, false);
                bankSelectPanel.showButtonLabel(currentBank, bankSelectLabels[currentBank], false);
                currentBank = touchedBtn;
                bankSelectPanel.drawButton(currentBank, true);
                bankSelectPanel.showButtonLabel(currentBank, bankSelectLabels[currentBank], true);

                currentPresetBase = currentBank * presetSelectPanel.btnCount;
                currentPreset = 0;
                drawPresetsPanel();
                selectPreset();

                touchedBtn = BtnNone;   // handled locally
            }
        } else if (presetSelectPanel.isPanelButton(pointX, pointY, touchedBtn)) {
            // Highlight newly selected preset/voice button
            if (touchedBtn != currentPreset) {
                presetSelectPanel.drawButton(currentPreset, false);
                presetSelectPanel.showButtonLabel(currentPreset, presetSelectLabels[currentPresetBase + currentPreset], false);
                currentPreset = touchedBtn;
                presetSelectPanel.drawButton(currentPreset, true);
                presetSelectPanel.showButtonLabel(currentPreset, presetSelectLabels[currentPresetBase + currentPreset], true);
                selectPreset();

                touchedBtn = BtnNone;   // handled locally
            }
        }
    }

    return touchedBtn;
}

/************************************************************************************
*                   P R I V A T E   C L A S S   F U N C T I O N S                   *
*************************************************************************************
*/

// drawPresetsPanel
//
void FmBanksScreen::drawPresetsPanel() {

    // Display label underneath bars
    TextArea tArea;
    tArea.posX = displayAreaX1;
    tArea.posY = presetSelectPanelY - 30;
    tArea.wid = displayAreaWidth;
    tArea.hgt = 30;
    tArea.align = TextAlignLeft;
    DisplayScreenBase::setFont(Arial_10_Bold);
    DisplayScreenBase::setTextColor(SUBHDR_TEXT_COLOR_DEFAULT);
    DisplayScreenBase::printInArea("Voice", tArea);

    presetSelectPanel.clearPanel();

    for (ButtonId btnNum = 0; btnNum < presetSelectPanel.btnCount; btnNum++) {
        presetSelectPanel.drawButton(btnNum, false);
        presetSelectPanel.showButtonLabel(btnNum, presetSelectLabels[currentPresetBase + btnNum], false);
    }
    presetSelectPanel.drawButton(currentPreset, true);
    presetSelectPanel.showButtonLabel(currentPreset, presetSelectLabels[currentPresetBase + currentPreset], true);
}

// drawBanksPanel
//
void FmBanksScreen::drawBanksPanel() {

    TextArea tArea;
    tArea.posX = displayAreaX1;
    tArea.posY = bankSelectPanelY - 30;
    tArea.wid = displayAreaWidth;
    tArea.hgt = 30;
    tArea.align = TextAlignLeft;
    DisplayScreenBase::setFont(Arial_10_Bold);
    DisplayScreenBase::setTextColor(SUBHDR_TEXT_COLOR_DEFAULT);
    DisplayScreenBase::printInArea("Voice Bank", tArea);

    bankSelectPanel.clearPanel();

    for (ButtonId btnNum = 0; btnNum < bankSelectPanel.btnCount; btnNum++) {
        bankSelectPanel.drawButton(btnNum, false);

        bankSelectPanel.showButtonLabel(btnNum, bankSelectLabels[btnNum], false);
    }
    bankSelectPanel.drawButton(currentBank, true);
    bankSelectPanel.showButtonLabel(currentBank, bankSelectLabels[currentBank], true);
}

// drawExitButton
//
// Draw peak range button panel
//
void FmBanksScreen::drawExitButton() {

    DisplayScreenBase::fillRoundRect(exitBtnX, exitBtnY, exitBtnWid, exitBtnHgt, 4, COLOR_GRAY);
    DisplayScreenBase::drawRoundRect(exitBtnX-1, exitBtnY-1, exitBtnWid+2, exitBtnHgt+2, 4, BTN_OUTLINE_COLOR_DEFAULT);

    TextArea tArea;
    tArea.posX = exitBtnX;
    tArea.posY = exitBtnY;
    tArea.wid = exitBtnWid;
    tArea.hgt = exitBtnHgt;
    tArea.align = TextAlignCenter;
    DisplayScreenBase::setFont(BTN_FONT_DEFAULT);
    DisplayScreenBase::setTextColor(BTN_SEL_TEXT_COLOR_DEFAULT);
    DisplayScreenBase::printInArea("Exit", tArea);
}

// selectPreset
//
void FmBanksScreen::selectPreset() {

    uint16_t presetNum = currentBank * presetSelectPanel.btnCount + currentPreset;
//    Serial.printf("FmBanksScreen::selectPreset: %d\n", presetNum);
    if (setPresetFn) {
        setPresetFn(presetNumId, presetNum);
    }
}
