/************************************************************************************
*                               NumberNameDisplay.cpp                               *
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
#include "NumberNameDisplay.h"

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
NumberNameDisplay::NumberNameDisplay(NumberNameDisplayInfo& dispInfo, uint16_t dispX, uint16_t dispY, uint16_t dispHgt) :
    displayInfo(dispInfo), displayX(dispX), displayY(dispY), displayWidth(10), displayHeight(dispHgt) {

    textArea.align   = TextAlignLeft;
    textArea.hgt     = displayHeight;
    textArea.wid     = displayWidth + 10;

    displayPrefix = "";
    displayNumber = 0;
    displayName = "None";
}

NumberNameDisplay::~NumberNameDisplay() {

}

// initialize
//
void NumberNameDisplay::initialize() {

}

// setPrefix
//
void NumberNameDisplay::setPrefix(const char* prefix) {

    displayPrefix = prefix;
}

// setDisplayParams
//
void NumberNameDisplay::setPosition(uint16_t xPos, uint16_t yPos) {

    displayX = xPos;
    displayY = yPos;

    textArea.posX = displayX + 7;
    textArea.posY = displayY;
}

// setNumberAndName
//
void NumberNameDisplay::setNumberAndName(uint16_t number, const char* name) {

    displayNumber = number;
    displayName = name;

    uint16_t labelWid, labelHgt;
    DisplayScreenBase::setFont(displayInfo.font);
    DisplayScreenBase::measureText(name, labelWid, labelHgt);
    textArea.wid = 5 + labelWid + 5;

    displayWidth = textArea.wid + 5;
}

// drawDisplay
//
void NumberNameDisplay::drawDisplay() {

    char displayBuf[64];    // FIXME: Magic number. Should this be on stack?
    snprintf(displayBuf, sizeof(displayBuf)-1, "%s %d: %s", displayPrefix, displayNumber, displayName);

    uint16_t labelWid, labelHgt;
    DisplayScreenBase::setFont(displayInfo.font);
    DisplayScreenBase::measureText(displayBuf, labelWid, labelHgt);
    textArea.wid = 5 + labelWid + 5;
    displayWidth = textArea.wid + 5;

    DisplayScreenBase::fillRect(displayX, displayY, displayWidth, displayHeight, displayInfo.bgndColor);
    DisplayScreenBase::drawRect(displayX-1, displayY-1, displayWidth+2, displayHeight+2, displayInfo.outlineColor);
    DisplayScreenBase::drawRect(displayX-2, displayY-2, displayWidth+4, displayHeight+4, displayInfo.outlineColor);

    DisplayScreenBase::setFont(displayInfo.font);
    DisplayScreenBase::setTextColor(displayInfo.textColor, displayInfo.bgndColor);
    DisplayScreenBase::printInArea(displayBuf, textArea);
}

// getWidth
//
uint16_t NumberNameDisplay::getWidth() {

    return displayWidth;
}

// clearToWidth
//
void NumberNameDisplay::clearToWidth() {

    if (displayWidth > 0) {
        DisplayScreenBase::fillRect(displayX-2, displayY-2, displayWidth+4, displayHeight+4, displayInfo.bgndColor);
    }
}

/************************************************************************************
*                   P R I V A T E   C L A S S   F U N C T I O N S                   *
*************************************************************************************
*/


