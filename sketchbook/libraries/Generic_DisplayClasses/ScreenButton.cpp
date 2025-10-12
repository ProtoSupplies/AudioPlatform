/************************************************************************************
*                                 ScreenButton.cpp                                  *
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

// User Arduino Libraries
//
#include "ScreenButton.h"

/************************************************************************************
*                    S T A T I C   C L A S S   V A R I A B L E S                    *
*************************************************************************************
*/

// Static Member Initializations
//

/************************************************************************************
*                    P U B L I C   C L A S S   F U N C T I O N S                    *
*************************************************************************************
*/

// Constructor/Destructor
//
ScreenButton::ScreenButton(ScreenButtonInfo& buttonInfo_) :
    btnInfo(buttonInfo_) {

    btnState = ScrnBtnOff;
    btnActive = false;
}

ScreenButton::~ScreenButton() {

}

// getBtnInfo
//
ScreenButtonInfo& ScreenButton::getBtnInfo() {

    return btnInfo;
}

// buttonTouched()
//
// Called periodically by owner class
//
bool ScreenButton::buttonTouched(int16_t pointX, int16_t pointY) {

    //    Serial.printf("ScreenButton::buttonTouched: x %d, y %d btnInfo.btnActive %d\n", pointX, pointY, btnInfo.btnActive);

    bool touched = false;

    if (btnActive) {

        if ((pointX >= btnInfo.btnX) && (pointX <= (btnInfo.btnX + btnInfo.btnWid))) {
            if ((pointY >= btnInfo.btnY) && (pointY <= (btnInfo.btnY + btnInfo.btnHgt))) {
                touched = true;
            }
        }
    }

    return touched;
}

// drawButton
//
void ScreenButton::drawButton() {

//    Serial.printf("ScreenButton::drawButton:\n");

    DisplayScreenBase::fillRoundRect(btnInfo.btnX, btnInfo.btnY, btnInfo.btnWid, btnInfo.btnHgt, btnInfo.btnOutlineRadius, btnInfo.btnColor);
    DisplayScreenBase::drawRoundRect(btnInfo.btnX-1, btnInfo.btnY-1, btnInfo.btnWid+2, btnInfo.btnHgt+2, btnInfo.btnOutlineRadius, btnInfo.btnOutlineColor);

    if (btnInfo.bitMapData) {
        BitMap::drawBitMap(btnInfo.bitMapData, btnInfo.btnX + 1, btnInfo.btnY + 1);
    }

    // Draw label last so as to appear in front of bitmap (if present)
    if (strlen(btnInfo.btnLabel)) {
        drawLabel(btnInfo.btnLabelColor);
    }

    btnActive = true;
}

// drawLabel
//
void ScreenButton::drawLabel(uint16_t labelColor) {

    char         labelBuf[32];
    TextArea     labelText;
    uint16_t     textWid, textHgt;
    TextPosition labelPos = btnInfo.btnLabelPosition;

    DisplayScreenBase::measureText(btnInfo.btnLabel, textWid, textHgt);

    switch (labelPos) {

        case TextLeft:
            labelText.posX  = btnInfo.btnX - textWid - 20;
            labelText.posY  = btnInfo.btnY + (btnInfo.btnHgt / 2) - (textHgt / 2) - 5;
            labelText.wid   = textWid + 10;
            labelText.hgt   = textHgt + 10;
            labelText.align = btnInfo.btnLabelAlign;
            break;

        case TextRight:
            labelText.posX  = btnInfo.btnX + btnInfo.btnWid + 10;
            labelText.posY  = btnInfo.btnY + (btnInfo.btnHgt / 2) - (textHgt / 2) - 5;
            labelText.wid   = textWid + 10;
            labelText.hgt   = textHgt + 10;
            labelText.align = btnInfo.btnLabelAlign;
            break;

        case TextAbove:
            labelText.hgt   = textHgt;
            labelText.posY  = btnInfo.btnY - (textHgt + 10);
            labelText.align = btnInfo.btnLabelAlign;
            if (textWid <= btnInfo.btnWid) {
                labelText.posX  = btnInfo.btnX;
                labelText.wid   = btnInfo.btnWid;
            } else {
                labelText.posX  = btnInfo.btnX + (btnInfo.btnWid / 2) - (textWid / 2);
                labelText.wid   = textWid;
            }
            break;

        case TextBelow:
            labelText.hgt   = textHgt;
            labelText.posY  = btnInfo.btnY + btnInfo.btnHgt + 10;
            labelText.align = btnInfo.btnLabelAlign;
            if (textWid <= btnInfo.btnWid) {
                labelText.posX  = btnInfo.btnX;
                labelText.wid   = btnInfo.btnWid;
            } else {
                labelText.posX  = btnInfo.btnX + (btnInfo.btnWid / 2) - (textWid / 2);
                labelText.wid   = textWid;
            }
            break;

        case TextInFront:
        default:
            labelText.hgt   = btnInfo.btnHgt;
            labelText.posY  = btnInfo.btnY;
            labelText.align = btnInfo.btnLabelAlign;
            if (textWid <= btnInfo.btnWid) {
                labelText.posX  = btnInfo.btnX;
                labelText.wid   = btnInfo.btnWid;
            } else {
                labelText.posX  = btnInfo.btnX + (btnInfo.btnWid / 2) - (textWid / 2);
                labelText.wid   = textWid;
            }
            break;
    };


    // BUG: setFont() Crashes, btnInfo.btnLabelFont = 0 unless workaround applied (see B241001-1)
    DisplayScreenBase::setFont(btnInfo.btnLabelFont);
    DisplayScreenBase::setTextColor(labelColor);

    snprintf(labelBuf, sizeof(labelBuf)-1, "%s", btnInfo.btnLabel);
    DisplayScreenBase::printInArea(labelBuf, labelText);
}

// removeButton
//
void ScreenButton::removeButton() {

    if (btnActive) {

        DisplayScreenBase::fillRoundRect(btnInfo.btnX, btnInfo.btnY, btnInfo.btnWid, btnInfo.btnHgt, 4, COLOR_BLACK);
        DisplayScreenBase::drawRoundRect(btnInfo.btnX-1, btnInfo.btnY-1, btnInfo.btnWid+2, btnInfo.btnHgt+2, 4, COLOR_BLACK);

        btnActive = false;
    }
}

// setState
//
void ScreenButton::setState(ScreenButtonState state) {

    switch (state) {

        case ScrnBtnOff:
//            Serial.printf("ScreenButton::setState: ScrnBtnOff\n");
            drawLabel(SCREENBUTTON_OFF_TEXT_COLOR);
            btnState = ScrnBtnOff;
            break;

        case ScrnBtnOn:
//            Serial.printf("ScreenButton::setState: ScrnBtnOn\n");
            drawLabel(SCREENBUTTON_ON_TEXT_COLOR);
            btnState = ScrnBtnOn;
            break;

        case ScrnBtnFlash:
//            Serial.printf("ScreenButton::setState: ScrnBtnFlash\n");
            drawLabel(SCREENBUTTON_ON_TEXT_COLOR);
            btnState = ScrnBtnFlash;
            break;

        default:
            break;
    }

}

// toggleState
//
ScreenButtonState ScreenButton::toggleState() {

//    Serial.printf("ScreenButton::toggleState:\n");

    switch (btnState) {

        case ScrnBtnOff:
            setState(ScrnBtnOn);
            break;

        case ScrnBtnOn:
            setState(ScrnBtnOff);
            break;

        default:
            break;
    }

    return btnState;    // new state
}

/************************************************************************************
*        P R I V A T E / P R O T E C T E D   C L A S S   F U N C T I O N S          *
*************************************************************************************
*/
