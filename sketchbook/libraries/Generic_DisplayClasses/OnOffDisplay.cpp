/************************************************************************************
*                                  OnOffDisplay.cpp                                 *
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
#include "OnOffDisplay.h"

/************************************************************************************
*                    S T A T I C   C L A S S   V A R I A B L E S                    *
*************************************************************************************
*/

// Initialization of static class variables
//

/************************************************************************************
*                    P U B L I C   C L A S S   F U N C T I O N S                    *
*************************************************************************************
*/

// Static Member Initializations
//

// Constructor/Destructor
//
OnOffDisplay::OnOffDisplay(OnOffDisplayInfo& dispInfo, uint16_t dispX, uint16_t dispY, uint16_t dispHgt, const char* dispLabel) :
    displayInfo(dispInfo), displayX(dispX), displayY(dispY), displayWidth(10), displayHeight(dispHgt), displayLabel(dispLabel) {

    displayState = false;

    labelDisplayArea.posX   = displayX + 7;
    labelDisplayArea.posY   = displayY;
    labelDisplayArea.align  = TextAlignLeft;
}

OnOffDisplay::~OnOffDisplay() {

}

// initialize
//
// NOTE: A separate initialize() is required to set label and display widths because
// this cannot be done in the constructor since setFont() and measureText() do not
// yet return accurate results at that time.
//
void OnOffDisplay::initialize() {

    uint16_t labelWid, labelHgt;
    DisplayScreenBase::setFont(displayInfo.font);
    DisplayScreenBase::measureText(displayLabel, labelWid, labelHgt);
    labelDisplayArea.wid = 5 + labelWid + 5;
    labelDisplayArea.hgt = displayHeight;

    displayWidth = labelDisplayArea.wid + 5 + IndicatorSideLen + 5;
}

// setPosition
//
void OnOffDisplay::setPosition(uint16_t xPos, uint16_t yPos) {

    displayX = xPos;
    displayY = yPos;

    labelDisplayArea.posX = displayX + 7;
    labelDisplayArea.posY = displayY;

//    Serial.printf("OnOffDisplay::setPosition: labelDisplayArea.wid %d displayWidth %d \n", labelDisplayArea.wid, displayWidth);
}

// drawDisplay
//
void OnOffDisplay::drawDisplay() {

//    Serial.printf("OnOffDisplay::drawDisplay: displayX %d displayY %d displayWidth %d displayHeight %d\n", displayX, displayY, displayWidth, displayHeight);
//    Serial.printf("OnOffDisplay::drawDisplay: labelDisplayArea.wid %d labelDisplayArea.hgt %d \n", labelDisplayArea.wid, labelDisplayArea.hgt);

    DisplayScreenBase::fillRect(displayX, displayY, displayWidth, displayHeight, displayInfo.bgndColor);
    DisplayScreenBase::drawRect(displayX-1, displayY-1, displayWidth+2, displayHeight+2, displayInfo.outlineColor);
    DisplayScreenBase::drawRect(displayX-2, displayY-2, displayWidth+4, displayHeight+4, displayInfo.outlineColor);

    DisplayScreenBase::setFont(displayInfo.font);
    DisplayScreenBase::setTextColor(displayInfo.textColor, displayInfo.bgndColor);
    DisplayScreenBase::printInArea(displayLabel, labelDisplayArea);
}

// getWidth
//
uint16_t OnOffDisplay::getWidth() {

    return displayWidth;
}

// clearToWidth
//
void OnOffDisplay::clearToWidth() {

    if (displayWidth > 0) {
        DisplayScreenBase::fillRect(displayX-2, displayY-2, displayWidth+4, displayHeight+4, displayInfo.bgndColor);
    }
}

// setState
//
void OnOffDisplay::setState(bool state) {

    displayState = state;
    uint16_t boxColor = displayState ? COLOR_GREEN : COLOR_RED;
    uint16_t boxOffsetY = (displayHeight - IndicatorSideLen) / 2;

    DisplayScreenBase::fillRect(displayX + labelDisplayArea.wid + 2, displayY + boxOffsetY, IndicatorSideLen, IndicatorSideLen, boxColor);
}


/************************************************************************************
*                   P R I V A T E   C L A S S   F U N C T I O N S                   *
*************************************************************************************
*/


