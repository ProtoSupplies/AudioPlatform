/************************************************************************************
*                                  TextColumn.cpp                                   *
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
#include "TextColumn.h"

/************************************************************************************
*                    S T A T I C   C L A S S   V A R I A B L E S                    *
*************************************************************************************
*/

// Default TextColumnInfo
//
static TextColumnInfo xxx = (TextColumnInfo)  {
    0,                                  // xPos
    0,                                  // yPos
    COLUMN_WIDTH_DEFAULT,               // width
    COLUMN_HEIGHT_DEFAULT,              // height
    COLUMN_LINE_HEIGHT_DEFAULT,         // lineHeight
    COLUMN_MAX_LINES_DEFAULT,           // maxLines
    COLUMN_FONT_DEFAULT,                // font
    COLUMN_TEXT_COLOR_DEFAULT,          // textColor
    COLUMN_TEXT_MARGIN_DEFAULT,         // textMargin
    COLUMN_TEXT_ALIGN_DEFAULT,          // textAlign
    COLUMN_COLOR_DEFAULT,               // columnColor
    COLUMN_OUTLINE_COLOR_DEFAULT,       // outlineColor
    COLUMN_SEL_OUTLINE_COLOR_DEFAULT    // selectedOutlineColor
    };

/************************************************************************************
*                    P U B L I C   C L A S S   F U N C T I O N S                    *
*************************************************************************************
*/

// Static Member Initializations
//

// Constructor/Destructor
//
TextColumn::TextColumn(TextColumnInfo* columnInfo_) :
    columnInfo(columnInfo_), linesPerSubColumn(1), subColumnCount(1), selectedLine(BtnNone) {

    setColumnInfo(columnInfo);
    selectedLine = BtnNone;
}

TextColumn::TextColumn() : TextColumn(&xxx) {

}

TextColumn::~TextColumn() {

}

// setColumnInfo
//
void TextColumn::setColumnInfo(TextColumnInfo* columnInfo_) {

    columnInfo = columnInfo_;
    if ((columnInfo->yPos + columnInfo->height) > DISPLAY_HEIGHT) {
        columnInfo->height = DISPLAY_HEIGHT - columnInfo->yPos;
    }
    linesPerSubColumn = columnInfo->height / columnInfo->lineHeight;
    if (linesPerSubColumn > columnInfo->maxLines) {
        linesPerSubColumn = columnInfo->maxLines;
    }
//    columnInfo->height = columnInfo->lineHeight * linesPerSubColumn;

    subColumnCount = (columnInfo->maxLines / linesPerSubColumn);
    if ((columnInfo->maxLines % linesPerSubColumn) != 0) {
        subColumnCount++;
    }

    selectedLine = BtnNone;

//    Serial.printf("TextColumn::setColumnInfo column height = %d linesPerSubColumn = %d\n", columnInfo->height, columnInfo->lineHeight, linesPerSubColumn);
}

// showColumnInfo
//
void TextColumn::showColumnInfo() {

    Serial.printf("TextColumn::showColumnInfo()\n");

    Serial.printf("    %-32s %d\n", "xPos", columnInfo->xPos);
    Serial.printf("    %-32s %d\n", "yPos", columnInfo->yPos);
    Serial.printf("    %-32s %d\n", "width", columnInfo->width);
    Serial.printf("    %-32s %d\n", "height", columnInfo->height);
    Serial.printf("    %-32s %d\n", "lineHeight", columnInfo->lineHeight);
    Serial.printf("    %-32s %d\n", "maxLines", columnInfo->maxLines);
    Serial.printf("    %-32s %d\n", "font", columnInfo->font);
    Serial.printf("    %-32s %d\n", "textColor", columnInfo->textColor);
    Serial.printf("    %-32s %d\n", "textMargin", columnInfo->textMargin);
    Serial.printf("    %-32s %d\n", "textAlign", columnInfo->textAlign);
    Serial.printf("    %-32s %d\n", "columnColor", columnInfo->columnColor);
    Serial.printf("    %-32s %d\n", "outlineColor", columnInfo->outlineColor);
    Serial.printf("    %-32s %d\n", "selectedOutlineColor:", columnInfo->selectedOutlineColor);
}

// drawColumnCells
//
void TextColumn::drawColumnCells() {

//    Serial.printf("TextColumn::drawColumnCells x = %d y = %d width = %d height = %d\n", columnInfo->xPos, columnInfo->yPos, columnInfo->width, columnInfo->height);

    uint16_t subColumnX = columnInfo->xPos;
    uint16_t subColumnHeight = columnInfo->height;
    uint16_t linesRemaining = columnInfo->maxLines;
    for (int i = 0; i < subColumnCount; i++) {
        if (linesRemaining < linesPerSubColumn) {
            subColumnHeight = linesRemaining * columnInfo->lineHeight;
        }
        DisplayScreenBase::fillRect(subColumnX, columnInfo->yPos, columnInfo->width, subColumnHeight, columnInfo->columnColor);
        subColumnX += (columnInfo->width + columnInfo->textMargin);
        linesRemaining -= linesPerSubColumn;
    }

//    showColumnInfo();
}

// drawColumnBgnd
//
void TextColumn::drawColumnBgnd() {

//    Serial.printf("TextColumn::drawColumnBgnd x = %d y = %d width = %d height = %d\n", columnInfo->xPos, columnInfo->yPos, columnInfo->width, columnInfo->height);
    DisplayScreenBase::fillRoundRect(columnInfo->xPos, columnInfo->yPos, columnInfo->width, columnInfo->height, 4, columnInfo->columnColor);
    DisplayScreenBase::drawRoundRect(columnInfo->xPos-1, columnInfo->yPos-1, columnInfo->width+2, columnInfo->height+2, 4, COLOR_WHITE);
    DisplayScreenBase::drawRoundRect(columnInfo->xPos-2, columnInfo->yPos-2, columnInfo->width+4, columnInfo->height+4, 4, COLOR_WHITE);
}

// printTextLine
//
void TextColumn::printTextLine(ButtonId lineNum, const char* text) {

//    Serial.printf("TextColumn::printTextLine %d maxLines %d linesPerSubColumn %d %s (%x)\n", lineNum, columnInfo->maxLines, linesPerSubColumn, text, text);

    uint16_t subColumnX = columnInfo->xPos + ((lineNum - 1) / linesPerSubColumn) * (columnInfo->width + columnInfo->textMargin);
    if (lineNum <= columnInfo->maxLines) {

        // Display area
        TextArea tArea;
        tArea.posX = subColumnX + columnInfo->textMargin;
        tArea.posY = columnInfo->yPos + ((lineNum - 1) % linesPerSubColumn) * columnInfo->lineHeight;
        tArea.wid = columnInfo->width - (2 * columnInfo->textMargin);
        tArea.hgt = columnInfo->lineHeight;
        tArea.align = columnInfo->textAlign;

        DisplayScreenBase::drawRect(subColumnX, tArea.posY, columnInfo->width, columnInfo->lineHeight, columnInfo->outlineColor);
//        Serial.printf("TextColumn::printTextLine x = %d y = %d width = %d height = %d\n", subColumnX, tArea.posY, tArea.wid, tArea.hgt);

        DisplayScreenBase::setFont(columnInfo->font);
        DisplayScreenBase::setTextColor(columnInfo->textColor);
        DisplayScreenBase::printInArea(text, tArea);
    }
}

// isColumnLine()
//
bool TextColumn::isColumnLine(int16_t pointX, int16_t pointY, ButtonId& lineNum) {

    bool isColumnLine = false;

    lineNum = BtnNone;

    uint16_t subColumnX = columnInfo->xPos;
    uint16_t subColumnHeight = columnInfo->height;
    uint16_t linesRemaining = columnInfo->maxLines;
    for (int i = 0; i < subColumnCount; i++) {
        uint16_t subColumnFirstLine = i * linesPerSubColumn;
        if (linesRemaining < linesPerSubColumn) {
            subColumnHeight = linesRemaining * columnInfo->lineHeight;
        }
        if (pointX >= subColumnX && pointX <= (subColumnX + columnInfo->width)) {
            if (pointY >= columnInfo->yPos && pointY <= (columnInfo->yPos + subColumnHeight)) {
                isColumnLine = true;
                lineNum = 1 + subColumnFirstLine + (pointY - columnInfo->yPos) / columnInfo->lineHeight;
                break;
            }
        }
        linesRemaining -= linesPerSubColumn;
        subColumnX += (columnInfo->width + columnInfo->textMargin);
    }


    return isColumnLine;
}

// setSelectedLine()
//
void TextColumn::setSelectedLine(ButtonId newSelectedLine) {

//    Serial.printf("TextColumn::setSelectedLine newSelectedLine %d\n", newSelectedLine);

    uint16_t posX, posY;

    unsetSelectedLine();

    if (newSelectedLine != BtnNone &&  newSelectedLine != 0) {
        selectedLine = newSelectedLine;
        posX = columnInfo->xPos + ((selectedLine - 1) / linesPerSubColumn) * (columnInfo->width + columnInfo->textMargin);
        posY = columnInfo->yPos + ((selectedLine - 1) % linesPerSubColumn) * columnInfo->lineHeight;
        DisplayScreenBase::drawRect(posX, posY, columnInfo->width, columnInfo->lineHeight, columnInfo->selectedOutlineColor);
        DisplayScreenBase::drawRect(posX+1, posY+1, columnInfo->width-2, columnInfo->lineHeight-2, columnInfo->selectedOutlineColor);
        DisplayScreenBase::drawRect(posX+2, posY+2, columnInfo->width-4, columnInfo->lineHeight-4, columnInfo->selectedOutlineColor);
    }
}

// unsetSelectedLine()
//
void TextColumn::unsetSelectedLine() {

    uint16_t posX, posY;

    if (selectedLine != BtnNone) {
        posX = columnInfo->xPos + ((selectedLine - 1) / linesPerSubColumn) * (columnInfo->width + columnInfo->textMargin);
        posY = columnInfo->yPos + ((selectedLine - 1) % linesPerSubColumn) * columnInfo->lineHeight;
        DisplayScreenBase::drawRect(posX, posY, columnInfo->width, columnInfo->lineHeight, columnInfo->outlineColor);
        DisplayScreenBase::drawRect(posX+1, posY+1, columnInfo->width-2, columnInfo->lineHeight-2, columnInfo->columnColor);
        DisplayScreenBase::drawRect(posX+2, posY+2, columnInfo->width-4, columnInfo->lineHeight-4, columnInfo->columnColor);
    }

    selectedLine = BtnNone;
}

// getSelectedLine()
//
ButtonId TextColumn::getSelectedLine() {

    return selectedLine;
}

/************************************************************************************
*                   P R I V A T E   C L A S S   F U N C T I O N S                   *
*************************************************************************************
*/


