/************************************************************************************
*                                  TestTextEdit.cpp                                 *
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

// Standard C++
//
#include <Math.h>
#include <string.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

// User Arduino Libraries
//
#include "TestTextEdit.h"
#include "ArduinoEclipseDefs.h"

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
TestTextEdit::TestTextEdit(DisplayScreenInfo& displayInfo) :
    DisplayScreenBase(displayInfo), textEd(100, 80, 32) {

    strncpy(textToEdit, "Here is the Text to Edit", TextEditBufferSize-1);

    textEd.pollEnabled = false;

    textFieldWidth  = 300;
    textFieldHeight = 40;
    textFieldX      = (DISPLAY_WIDTH - textFieldWidth) / 2;;
    textFieldY      = marginTop + 20;

    editBtnWid  = 100;
    editBtnHgt  = 40;
    editBtnX    = (DISPLAY_WIDTH - editBtnWid) / 2;
    editBtnY    = textFieldY + textFieldHeight + 40;
}

TestTextEdit::~TestTextEdit() {

}

// activateScreen
//
void TestTextEdit::activateScreen() {

//    Serial.printf("TestTextEdit::activateScreen:\n");

//    Serial.printf("TestTextEdit::activateScreen: textToEdit %s\n", textToEdit);
    textEd.pollEnabled = false;

    // Clear and display screen
    clearScreen();
    showScreenHeading();
    showScreen();
}

// deActivateScreen
//
void TestTextEdit::deActivateScreen() {

}

/************************************************************************************
*                   P R I V A T E   C L A S S   F U N C T I O N S                   *
*************************************************************************************
*/

// pollScreen
//
ButtonId TestTextEdit::pollScreen() {

    ButtonId touchedBtn = BtnNone;
    uint16_t pointX, pointY;

    if (textEd.pollEnabled) {
        touchedBtn = textEd.pollPopup();
        if (touchedBtn != BtnNone) {
//            Serial.printf("TestTextEdit::pollScreen: pollPopup returns %d\n", touchedBtn);
            textEd.pollEnabled = false;
            if (touchedBtn == BtnSave) {
                strncpy(textToEdit, textEd.getTextValue(), TextEditBufferSize-1);
            }
            this->activateScreen();
        }
    } else if (DisplayScreenBase::touched()) {
            DisplayScreenBase::getTouchedPoint(pointX, pointY);

            // First check for Home/Back button. If pressed, we'll return that value to main application.
            if (DisplayScreenBase::prevBtnTouched(pointX, pointY)) {
                touchedBtn = BtnPrev;

            } else if (DisplayScreenBase::rectTouched(pointX, pointY, editBtnX, editBtnY, editBtnWid, editBtnHgt)) {
                textEd.setTextHeading("Testing Text Editor");
                textEd.setTextValue(textToEdit);
                textEd.showPopup();
                textEd.pollEnabled = true;
            }
    }

    return touchedBtn;
}

// showScreen
//
void TestTextEdit::showScreen() {

    drawTextDisplayField();
    drawEditButton();
    showText();
}

// showText
//
void TestTextEdit::showText() {

    TextArea tArea;
    tArea.posX = textFieldX;
    tArea.posY = textFieldY;
    tArea.wid = textFieldWidth;
    tArea.hgt = textFieldHeight;
    tArea.align = TextAlignCenter;

    DisplayScreenBase::setFont(TEXTFIELD_FONT);
    DisplayScreenBase::setTextColor(TEXTFIELD_FONT_COLOR);
    DisplayScreenBase::printInArea(textToEdit, tArea);
}

// drawTextDisplayField
//
void TestTextEdit::drawTextDisplayField() {

//    Serial.printf("TestTextEdit::drawTextDisplayField: \n");

    DisplayScreenBase::fillRoundRect(textFieldX, textFieldY, textFieldWidth, textFieldHeight, 4, TEXTFIELD_COLOR);

    DisplayScreenBase::drawRoundRect(textFieldX-1, textFieldY-1, textFieldWidth+2, textFieldHeight+2, 4, TEXTFIELD_OUTLINE_COLOR);
    DisplayScreenBase::drawRoundRect(textFieldX-2, textFieldY-2, textFieldWidth+4, textFieldHeight+4, 4, TEXTFIELD_OUTLINE_COLOR);
}

// drawEditButton
//
void TestTextEdit::drawEditButton() {

    DisplayScreenBase::fillRoundRect(editBtnX, editBtnY, editBtnWid, editBtnHgt, 4, COLOR_GRAY);
    DisplayScreenBase::drawRoundRect(editBtnX-1, editBtnY-1, editBtnWid+2, editBtnHgt+2, 4, BTN_OUTLINE_COLOR_DEFAULT);

    TextArea tArea;
    tArea.posX = editBtnX;
    tArea.posY = editBtnY;
    tArea.wid = editBtnWid;
    tArea.hgt = editBtnHgt;
    tArea.align = TextAlignCenter;

    DisplayScreenBase::setFont(BTN_FONT_DEFAULT);
    DisplayScreenBase::setTextColor(BTN_SEL_TEXT_COLOR_DEFAULT);
    DisplayScreenBase::printInArea(editBtnText, tArea);
}

