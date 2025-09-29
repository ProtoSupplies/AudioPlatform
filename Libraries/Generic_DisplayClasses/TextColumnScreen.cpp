/************************************************************************************
*                               TextColumnScreen.cpp                                *
*                                                                                   *
*                      Copyright (c) 2025, Donald Swearingen                        *
*                               All Rights Reserved                                 *
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
#include "TextColumnScreen.h"

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

// Constructor/Destructor
//
TextColumnScreen::TextColumnScreen(DisplayScreenInfo& displayInfo, uint8_t columnCount_, TextColumnInfo columnInfo_[]) :
    DisplayScreenBase(displayInfo), columnCount(columnCount_) {

    for (int i = 0; i < columnCount; i++) {
        columns[i].setColumnInfo(&columnInfo_[i]);
        columns[i].selectedLine = BtnNone;
    }
}

TextColumnScreen::~TextColumnScreen() {

}

// activateScreen
//
void TextColumnScreen::activateScreen() {

    // Clear and display screen
    clearScreen();
    showScreenHeading();

    for (int i = 0; i < columnCount; i++) {
        columns[i].drawColumnCells();
    }
}

// deActivateScreen
//
void TextColumnScreen::deActivateScreen() {

}

// pollScreen
//
// FFS/TODO: How to handle return value(s) when multiple columns are being polled?
// Maybe a new function like getTouchedColumn()?
//
ButtonId TextColumnScreen::pollScreen() {

    ButtonId touchedBtn = BtnNone;
    uint16_t pointX, pointY;
    ButtonId touchedLine;

    // Check for touched buttons
    if (DisplayScreenBase::touched()) {
        DisplayScreenBase::getTouchedPoint(pointX, pointY);

        // First check for Home/Back button. If pressed, we'll return that value to main application.
        if (DisplayScreenBase::prevBtnTouched(pointX, pointY)) {
            touchedBtn = BtnPrev;
        }

        for (uint8_t columnNum = 0; columnNum < columnCount; columnNum++) {
            if (columns[columnNum].isColumnLine(pointX, pointY, touchedLine)) {
                if (touchedLine != columns[columnNum].selectedLine) {
//                    Serial.printf("TextColumnScreen::pollScreen: Column %d Line %d Touched\n", columnNum + 1, touchedLine);
                    columns[columnNum].setSelectedLine(touchedLine);
                }
            }
        }
    }

    return touchedBtn;
}

#if SHOW_TEXTCOLUMN_SETTINGS
// deActivateScreen
//
void TextColumnScreen::showSettings() {

    Serial.printf("TextColumnScreen::showSettings:\n");

    for (int i = 0; i < columnCount; i++) {
        Serial.printf("  %15s %d\n", "Column:", i);
        Serial.printf("    %20s %d\n", "xPos:", columns[i].columnInfo->xPos);
        Serial.printf("    %20s %d\n", "yPos:", columns[i].columnInfo->yPos);
        Serial.printf("    %20s %d\n", "width:", columns[i].columnInfo->width);
        Serial.printf("    %20s %d\n", "height:", columns[i].columnInfo->height);
        Serial.printf("    %20s %d\n", "maxLines:", columns[i].columnInfo->maxLines);
        Serial.printf("    %20s %d\n", "linesPerSubColumn:", columns[i].linesPerSubColumn);
        Serial.printf("    %20s %d\n", "subColumnCount:", columns[i].subColumnCount);
    }
}
#endif // SHOW_TEXTCOLUMN_SETTINGS

// getLineCount
//
uint16_t TextColumnScreen::getLineCount(uint16_t columnNum) {

    return columns[columnNum - 1].columnInfo->maxLines;
}

// getLinesPerSubColumn
//
uint16_t TextColumnScreen::getLinesPerSubColumn(uint16_t columnNum) {

    return columns[columnNum - 1].linesPerSubColumn;
}

// printTextLineInColumn
//
void TextColumnScreen::printTextLineInColumn(uint8_t columnNum, uint16_t lineNum, const char* text) {

    if (columnNum > 0 && columnNum <= columnCount) {
        columns[columnNum - 1].printTextLine(lineNum, text);
    }
}

/************************************************************************************
*                   P R I V A T E   C L A S S   F U N C T I O N S                   *
*************************************************************************************
*/

