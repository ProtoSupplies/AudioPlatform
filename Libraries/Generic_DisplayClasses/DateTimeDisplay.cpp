/************************************************************************************
*                                DateTimeDisplay.cpp                                *
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
#include "DateTimeDisplay.h"

/************************************************************************************
*                         M A C R O S  /  C O N S T A N T S                         *
*************************************************************************************
*/

// leap year calculator expects year argument as years offset from 1970
#define LEAP_YEAR(Y)    ( ((2000+(Y))>0) && !((2000+(Y))%4) && ( ((2000+(Y))%100) || !((2000+(Y))%400) ) )

#define MONTH_JAN       0
#define MONTH_FEB       1

PROGMEM const char cellLabels[6][3] = {{"MM"}, {"DD"}, {"YY"}, {"HH"}, {"MM"}, {"SS"}};

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
DateTimeDisplay::DateTimeDisplay(DateTimeDisplayInfo& dispInfo, uint16_t dispX, uint16_t dispY) :
    touchActive(false), displayInfo(dispInfo), displayX(dispX), displayY(dispY) {

    month                   = 1;
    day                     = 1;
    year                    = 0;
    hour                    = 0;
    minute                  = 0;
    second                  = 0;

    titleTextArea.align     = TextAlignCenter;
    titleTextArea.hgt       = displayInfo.cellHeight;
    titleTextArea.wid       = displayInfo.cellWidth;

    cellValueTextArea.align = TextAlignCenter;
    cellValueTextArea.hgt   = displayInfo.cellHeight;
    cellValueTextArea.wid   = displayInfo.cellWidth;

    cellLabelTextArea.align = TextAlignCenter;
    cellLabelTextArea.hgt   = displayInfo.cellHeight;
    cellLabelTextArea.wid   = displayInfo.cellWidth;
    showCellLabels          = false;

    touchedCell             = BtnNone;

    cellValue               = 0;
    cellValuePrev           = 0;
    lastTouchedPointY       = 0;

    // Override class arg (TODO: eliminate from class args)
    displayX = (DISPLAY_WIDTH - (6 * displayInfo.cellWidth + 4 * displayInfo.cellGap + displayInfo.yearTimeGap)) / 2;
}

DateTimeDisplay::~DateTimeDisplay() {

}

// initialize
//
void DateTimeDisplay::initialize() {

}

// drawDisplay
//
void DateTimeDisplay::drawDisplay() {

    uint16_t xPos = displayX;
    uint16_t yPos = displayY;

    TextArea labelText;

    labelText.posY = yPos + displayInfo.cellHeight + 5;
    labelText.wid = displayInfo.cellWidth;
    labelText.hgt = 15;
    labelText.align = TextAlignCenter;

    DisplayScreenBase::setFont(displayInfo.cellLabelFont);
    DisplayScreenBase::setTextColor(displayInfo.fontColor);

    for (uint8_t cell = 0; cell < DateTimeCellCount; cell++) {

        xPos = cellXpos(cell);
        labelText.posX = xPos;

        DisplayScreenBase::fillRoundRect(xPos, yPos, displayInfo.cellWidth, displayInfo.cellHeight, 4, displayInfo.bgndColor);
        DisplayScreenBase::drawRoundRect(xPos-1, yPos-1, displayInfo.cellWidth+2, displayInfo.cellHeight+2, 4, displayInfo.outlineColor);

        if (displayInfo.showCellLabels) {
//            snprintf(dispBuf, sizeof(dispBuf)-1, "%02d", cellValue);
            DisplayScreenBase::printInArea(cellLabels[cell], labelText);
        }
    }

}

// pollDisplay
//
void DateTimeDisplay::pollDisplay(int16_t pointX, int16_t pointY) {

    if (touchActive) {

        uint16_t cellX = cellXpos(touchedCell);
        uint16_t cellWidth = displayInfo.cellWidth;

        if ((pointX < cellX) || (pointX > (cellX + cellWidth))) {
            touchActive = false;
            handleTouchEnd();

        } else {
            if (pointY < lastTouchedPointY) {
                if (cellValue < dtCellLimits[touchedCell].valueMax) {
                    cellValue++;
                }
                lastTouchedPointY = pointY;

            } else if (pointY > lastTouchedPointY) {
                if (cellValue > dtCellLimits[touchedCell].valueMin) {
                    cellValue--;
                }
                lastTouchedPointY = pointY;
            }

            if (cellValue != cellValuePrev) {
//                Serial.printf("DateTimeDisplay::pollDisplay: cellValue %d\n", cellValue);
                cellValuePrev = cellValue;
                if (touchedCell != BtnNone) {
                    updateDisplayCell(touchedCell, cellValue);
                }
            }
        }

    } else {

        // Touch is inactive. See if a new cell is touched.
        touchActive = displayCellTouched(pointX, pointY, touchedCell);
//        Serial.printf("DateTimeDisplay::pollDisplay: touchActive %d\n", touchActive);

        // Touch inactive to active transition
        if (touchActive) {
            handleTouchBegin();
        }
    }
}

// pollEnd
//
void DateTimeDisplay::pollEnd() {

    if (touchActive) {
        touchActive = false;
        handleTouchEnd();
    }
}

// displayCellTouched
//
bool DateTimeDisplay::displayCellTouched(int16_t pointX, int16_t pointY, ButtonId& cellId) {

    bool returnVal = false;

    cellId = BtnNone;

    uint16_t xPos = displayX;
    uint16_t yPos = displayY;

    for (uint8_t cell = 0; cell < DateTimeCellCount; cell++) {

        xPos = cellXpos(cell);

        if (pointX >= xPos && pointX <= (xPos + displayInfo.cellWidth)) {
            if (pointY >= yPos && pointY <= (yPos + displayInfo.cellHeight)) {
                if (cell != touchedCell) {
//                    Serial.printf("DateTimeDisplay::displayCellTouched %d\n", cell);
                    touchedCell = cell;
                    cellId = cell;
                    returnVal = true;
                }
            }
        }
    }

    return returnVal;
}

// updateDisplay
//
void DateTimeDisplay::updateDisplay(DateTimeFields* dt, bool forceDisplay) {

    if (dt->mon != month || forceDisplay) {
        month = dt->mon;
        dtCellValues[CELL_MONTH] = dt->mon + 1;
        updateDisplayCell(CELL_MONTH, month);
    }
    if (dt->mday != day || forceDisplay) {
        day = dt->mday;
        dtCellValues[CELL_DAY] = dt->mday;
        updateDisplayCell(CELL_DAY, day);
    }
    if ((dt->year % 100) != year || forceDisplay) {
        year = dt->year % 100;
        dtCellValues[CELL_YEAR] = dt->year % 100;
        updateDisplayCell(CELL_YEAR, year);
        if (LEAP_YEAR(year)) {
            monthDays[MONTH_FEB] = 29;
        } else {
            monthDays[MONTH_FEB] = 28;
        }
    }
    if (dt->hour != hour || forceDisplay) {
        hour = dt->hour;
        dtCellValues[CELL_HOUR] = dt->hour;
        updateDisplayCell(CELL_HOUR, hour);
    }
    if (dt->min != minute || forceDisplay) {
        minute = dt->min;
        dtCellValues[CELL_MINUTE] = dt->min;
        updateDisplayCell(CELL_MINUTE, minute);
    }
    if (dt->sec != second || forceDisplay) {
        second = dt->sec;
        dtCellValues[CELL_SECOND] = dt->sec;
        updateDisplayCell(CELL_SECOND, second);
    }
}

// updateDisplayCell
//
void DateTimeDisplay::updateDisplayCell(uint8_t cellNum, uint8_t cellValue) {

//    Serial.printf("DateTimeDisplay::updateDisplayCell %d value %d\n", cellNum, cellValue);

    uint16_t xPos = cellXpos(cellNum);
    uint16_t yPos = displayY;

    dtCellValues[cellNum] = cellValue;

    uint16_t outlineColor = displayInfo.outlineColor;
    if (cellNum == touchedCell) {
        outlineColor = displayInfo.outlineColor;
    }

    DisplayScreenBase::fillRoundRect(xPos, yPos, displayInfo.cellWidth, displayInfo.cellHeight, 4, displayInfo.bgndColor);
    DisplayScreenBase::drawRoundRect(xPos-1, yPos-1, displayInfo.cellWidth+2, displayInfo.cellHeight+2, 4, outlineColor);

    cellValueTextArea.posX = xPos;
    cellValueTextArea.posY = yPos;

    char dispBuf[32];
    DisplayScreenBase::setFont(displayInfo.cellValueFont);
    DisplayScreenBase::setTextColor(displayInfo.fontColor);
    snprintf(dispBuf, sizeof(dispBuf)-1, "%02d", cellValue);
    DisplayScreenBase::printInArea(dispBuf, cellValueTextArea);
}

// getCellValue
//
uint16_t DateTimeDisplay::getCellValue(uint8_t cellNum) {

    return dtCellValues[cellNum];
}

/************************************************************************************
*                   P R I V A T E   C L A S S   F U N C T I O N S                   *
*************************************************************************************
*/

// cellXpos
//
uint16_t DateTimeDisplay::cellXpos(uint8_t cellNum) {

    uint16_t cellPos = displayX;

    cellPos = displayX + (cellNum * (displayInfo.cellWidth + displayInfo.cellGap));
    if (cellNum >= 3) {
        cellPos += (displayInfo.yearTimeGap - displayInfo.cellGap);
    }

    return cellPos;
}

// handleTouchBegin
//
void DateTimeDisplay::handleTouchBegin() {

    cellValue = dtCellValues[touchedCell];
    cellValuePrev = cellValue;

//    Serial.printf("DateTimeDisplay::handleTouchBegin: touchedCell %d\n", touchedCell);

    switch (touchedCell) {

        case CELL_MONTH:
//            dtCellLimits[CELL_DAY].valueMax = monthDays[cellValue];
//            Serial.printf("DateTimeDisplay::handleTouchBegin: dtCellLimits[CELL_DAY].valueMax %d\n", dtCellLimits[CELL_DAY].valueMax);
            break;

        default:
            break;
    }
}

// handleTouchEnd
//
void DateTimeDisplay::handleTouchEnd() {

    cellValue = dtCellValues[touchedCell];
    cellValuePrev = cellValue;

//    Serial.printf("DateTimeDisplay::handleTouchEnd: touchedCell %d cell value %d\n", touchedCell, cellValue);

    if (LEAP_YEAR(dtCellValues[CELL_YEAR])) {
//        Serial.printf("DateTimeDisplay::handleTouchEnd: year %d is leap year\n", dtCellValues[CELL_YEAR]);
        monthDays[MONTH_FEB] = 29;
    } else {
//        Serial.printf("DateTimeDisplay::handleTouchEnd: year %d is not leap year\n", dtCellValues[CELL_YEAR]);
        monthDays[MONTH_FEB] = 28;
    }

    dtCellLimits[CELL_DAY].valueMax = monthDays[dtCellValues[CELL_MONTH]-1];
//    Serial.printf("DateTimeDisplay::handleTouchEnd: cell day max is %d\n", dtCellLimits[CELL_DAY].valueMax);
    if (dtCellValues[CELL_DAY] > dtCellLimits[CELL_DAY].valueMax) {
        dtCellValues[CELL_DAY] = dtCellLimits[CELL_DAY].valueMax;
        updateDisplayCell(CELL_DAY, dtCellValues[CELL_DAY]);
    }

//    touchActive = false;
}

