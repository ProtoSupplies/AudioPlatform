/************************************************************************************
*                                 FmSynthScreen.cpp                                 *
*                                                                                   *
*                       Copyright (c) 2025, Donald Swearingen                       *
*                                All Rights Reserved                                *
*************************************************************************************
*
* Description
*
*   FmSynthScreen is a custom AudioPlatform screen derived from the generic base class
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

#include "FmSynthScreen.h"
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

ScreenButtonInfo FmSynthScreen::banksBtnInfo = (ScreenButtonInfo) {
     11,                             // btnX
     153,                           // btnY
     36,                            // btnWid
     36,                            // btnHgt
     COLOR_BLACK,                   // btnColor
     SCREENBUTTON_OUTLINE_COLOR,    // btnOutlineColor
     0,                             // btnOutlineRadius
     "Voices",                      // btnLabel (set at runtime)
     SCREENBUTTON_TEXT_COLOR,       // btnLabelColor
     Arial_9_Bold,                  // btnLabelFont (note: must set again in constructor B241001-1)
     SCREENBUTTON_TEXT_ALIGN,       // btnLabelAlign
     TextBelow,                     // btnLabelPosition
     EnterIcon_35x35,               // bitMapInfo
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
FmSynthScreen::FmSynthScreen(DisplayScreenInfo& displayInfo, ButtonPanelInfo& panelInfo, ParamButtonInfo *btnParams,
        RotaryParams *paramBtnRotaryParams, RotaryEncoderPB **encoders, uint16_t presetDeviceId,
        const char* (*presetsCallbackFn)(CommandButtonId cmdId, uint16_t& numberParam, void* voidParam)) :

    ParamButtonScreen(displayInfo, panelInfo, btnParams, paramBtnRotaryParams, encoders, presetDeviceId, presetsCallbackFn),
    banksBtn(banksBtnInfo) {

    presetBanksBtnWid   = 130;
    presetBanksBtnHgt   = 40;
    presetBanksBtnX     = (DISPLAY_WIDTH - presetBanksBtnWid) / 2;
    presetBanksBtnY     = DISPLAY_HEIGHT - marginBottom + presetBanksBtnHgt / 2;

    // Workaround for B241001-1
    banksBtnInfo.btnLabelFont = Arial_9_Bold;
}

FmSynthScreen::~FmSynthScreen() {

}

// activateScreen
//
// Reset/display dynamic screen elements each time screen is displayed
//
void FmSynthScreen::activateScreen() {

    ParamButtonScreen::activateScreen();

//    drawPresetBanksButton();

//    BitMap::showHdrInfo((BitmapHeader*) banksBtn.bitMapData);
    banksBtn.drawButton();
}

// deActivateScreen
//
void FmSynthScreen::deActivateScreen() {

    ParamButtonScreen::deActivateScreen();
    banksBtn.removeButton();
}

// pollScreen
//
ButtonId FmSynthScreen::pollScreen() {

    ButtonId touchedBtn = BtnNone;
    uint16_t pointX, pointY;

    // Execute parent class poll first
    touchedBtn = ParamButtonScreen::pollScreen();

    // Now check items unique to FmSynthScreen
    DisplayScreenBase::getTouchedPoint(pointX, pointY);
    if (banksBtn.buttonTouched(pointX, pointY)) {
        touchedBtn = BtnNext;
    }

    return touchedBtn;
}

/************************************************************************************
*                   P R I V A T E   C L A S S   F U N C T I O N S                   *
*************************************************************************************
*/

// drawPresetBanksButton
//
void FmSynthScreen::drawPresetBanksButton() {

    DisplayScreenBase::fillRoundRect(presetBanksBtnX, presetBanksBtnY, presetBanksBtnWid, presetBanksBtnHgt, 4, COLOR_GRAY);
    DisplayScreenBase::drawRoundRect(presetBanksBtnX-1, presetBanksBtnY-1, presetBanksBtnWid+2, presetBanksBtnHgt+2, 4, BTN_OUTLINE_COLOR_DEFAULT);

    TextArea tArea;
    tArea.posX = presetBanksBtnX;
    tArea.posY = presetBanksBtnY;
    tArea.wid = presetBanksBtnWid;
    tArea.hgt = presetBanksBtnHgt;
    tArea.align = TextAlignCenter;
    DisplayScreenBase::setFont(BTN_FONT_DEFAULT);
    DisplayScreenBase::setTextColor(BTN_SEL_TEXT_COLOR_DEFAULT);
    DisplayScreenBase::printInArea("Preset Banks", tArea);
}

