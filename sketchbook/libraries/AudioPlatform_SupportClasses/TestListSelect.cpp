/************************************************************************************
*                                 TestListSelect.cpp                                *
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

// User Arduino Libraries
//
#include "TestListSelect.h"
#include "ArduinoEclipseDefs.h"
#include "UtilFns.h"

/************************************************************************************
*                    S T A T I C   C L A S S   V A R I A B L E S                    *
*************************************************************************************
*/

TextColumnInfo TestListSelect::listDisplay = (TextColumnInfo)  {
    LISTVIEW_XPOS,                  // xPos
    LISTVIEW_YPOS,                  // yPos
    300,                            // width
    400,                            // height
    LISTVIEW_LINE_HEIGHT,           // lineHeight
    LISTVIEW_MAX_LINES,             // maxLines
    LISTVIEW_FONT,                  // font
    LISTVIEW_FONT_COLOR,            // textColor
    LISTVIEW_TEXT_MARGIN,           // textMargin
    TextAlignCenter,                // textAlign
    LISTVIEW_BGND_COLOR,            // columnColor
    LISTVIEW_OUTLINE_COLOR,         // outlineColor
    LISTVIEW_SEL_OUTLINE_COLOR      // selectedOutlineColor
    };

/************************************************************************************
*                    P U B L I C   C L A S S   F U N C T I O N S                    *
*************************************************************************************
*/

// Static Member Initializations
//

// Constructor/Destructor
//
TestListSelect::TestListSelect(DisplayScreenInfo& displayInfo, SystemHardwareProfile& _sysProfile) :
    DisplayScreenBase(displayInfo), sysProfile(_sysProfile), listSelect() {

    textFieldWidth  = 500;
    textFieldHeight = 40;
    textFieldX      = (DISPLAY_WIDTH - textFieldWidth) / 2;;
    textFieldY      = marginTop + 80;

    editBtnWid  = 100;
    editBtnHgt  = 40;
    editBtnX    = (DISPLAY_WIDTH - editBtnWid) / 2;
    editBtnY    = textFieldY + textFieldHeight + 60;

    listSelect.setTextColumnInfo(&listDisplay);

    selectedLine = BtnNone;
}

TestListSelect::~TestListSelect() {

}

// getSdFileList
//
// Print a list of files on SD card
//
void TestListSelect::getSdFileList() {

    uint16_t lineCount = 0;

    if (sysProfile.sdCardPresent) {
        lineCount = Teensy41_Util::printSdDirFileListToBigBuf("");
    }

//    Serial.printf("TestListSelect::getSdFileList: %d SD Files\n", lineCount);

    listSelect.setList(sysProfile.largeTempBufLines, lineCount);
    listSelect.setPopupHeading("List of Files on SD Card (unsorted)");

//    uint16_t dispWidth = 130;
//    listDisplay.width = dispWidth;
//    listDisplay.xPos = (DISPLAY_WIDTH - dispWidth) / 2;
//    listSelect.setListDisplayWidth(dispWidth);
//    listSelect.setListDisplayX((DISPLAY_WIDTH - dispWidth) / 2);
}

// activateScreen
//
void TestListSelect::activateScreen() {

//    Serial.printf("TestListSelect::activateScreen:\n");

    // Get a list of all SD files in
    getSdFileList();

    // Clear and display screen
    clearScreen();
    showScreenHeading();
    showScreen();

    listSelect.pollEnabled = false;
}

// deActivateScreen
//
void TestListSelect::deActivateScreen() {

}

/************************************************************************************
*                   P R I V A T E   C L A S S   F U N C T I O N S                   *
*************************************************************************************
*/

// pollScreen
//
ButtonId TestListSelect::pollScreen() {

    ButtonId touchedBtn = BtnNone;
    uint16_t pointX, pointY;

    if (listSelect.pollEnabled) {
        touchedBtn = listSelect.pollPopup();
        if (touchedBtn != BtnNone) {
//            Serial.printf("TestListSelect::pollScreen: pollPopup returns %d\n", touchedBtn);
            listSelect.pollEnabled = false;
            if (touchedBtn == BtnSelect) {
//                Serial.printf("TestListSelect::pollScreen: touchedBtn == BtnSelect\n");
                selectedLine = listSelect.getSelectedLine();
                if (selectedLine != BtnNone) {
                    strncpy(textToEdit, sysProfile.largeTempBufLines[selectedLine - 1], 64);
                    textToEdit[63] ='\0';
                }
            }
            this->activateScreen();
        }
    } else if (DisplayScreenBase::touched()) {
        DisplayScreenBase::getTouchedPoint(pointX, pointY);

        // First check for Home/Back button. If pressed, we'll return that value to main application.
        if (DisplayScreenBase::prevBtnTouched(pointX, pointY)) {
            touchedBtn = BtnPrev;

        } else if (DisplayScreenBase::rectTouched(pointX, pointY, editBtnX, editBtnY, editBtnWid, editBtnHgt)) {
            listSelect.showPopup();
            if (selectedLine != BtnNone) {
                listSelect.setSelectedLine(selectedLine);
            }
            listSelect.pollEnabled = true;
        }
    }

    return touchedBtn;
}

// showScreen
//
void TestListSelect::showScreen() {

//    Serial.printf("TestListSelect::showScreen: \n");

    listDisplay.xPos = (DISPLAY_WIDTH - listDisplay.width) / 2;
//    listDisplay.height = 400;
//    listSelect.setListDisplayWidth(listWidth);
//    uint16_t dispX = (DISPLAY_WIDTH - listWidth) / 2;
//    listSelect.setListDisplayX(dispX);
//    listSelect.setListDisplayHeight(400);
    drawTextDisplayField();
    drawEditButton();
    showText();
}

// showText
//
void TestListSelect::showText() {

//    Serial.printf("TestListSelect::showText: \n");

    TextArea tArea;
    tArea.posX = textFieldX;
    tArea.posY = textFieldY;
    tArea.wid = textFieldWidth;
    tArea.hgt = textFieldHeight;
    tArea.align = TextAlignCenter;

    DisplayScreenBase::setFont(COLUMN_FONT_DEFAULT);
    DisplayScreenBase::setTextColor(COLOR_BLACK);
    DisplayScreenBase::printInArea(textToEdit, tArea);
}

// drawTextDisplayField
//
void TestListSelect::drawTextDisplayField() {

//    Serial.printf("TestListSelect::drawTextDisplayField: \n");

    DisplayScreenBase::fillRoundRect(textFieldX, textFieldY, textFieldWidth, textFieldHeight, 4, COLOR_WHEAT);

    DisplayScreenBase::drawRoundRect(textFieldX-1, textFieldY-1, textFieldWidth+2, textFieldHeight+2, 4, COLOR_WHITE);
    DisplayScreenBase::drawRoundRect(textFieldX-2, textFieldY-2, textFieldWidth+4, textFieldHeight+4, 4, COLOR_WHITE);
}

// drawEditButton
//
void TestListSelect::drawEditButton() {

//    Serial.printf("TestListSelect::drawEditButton: \n");

    DisplayScreenBase::fillRoundRect(editBtnX, editBtnY, editBtnWid, editBtnHgt, 4, COLOR_GRAY);
    DisplayScreenBase::drawRoundRect(editBtnX-1, editBtnY-1, editBtnWid+2, editBtnHgt+2, 4, COLOR_WHITE);

    TextArea tArea;
    tArea.posX = editBtnX;
    tArea.posY = editBtnY;
    tArea.wid = editBtnWid;
    tArea.hgt = editBtnHgt;
    tArea.align = TextAlignCenter;

    DisplayScreenBase::setFont(COLUMN_FONT_DEFAULT);
    DisplayScreenBase::setTextColor(COLOR_WHITE);
    DisplayScreenBase::printInArea(editBtnText, tArea);
}

