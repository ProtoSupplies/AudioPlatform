/************************************************************************************
*                                   ListSelect.cpp                                  *
*                                                                                   *
*                       Copyright (c) 2025, Donald Swearingen                       *
*                                All Rights Reserved                                *
*************************************************************************************
*
* Description
*
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
#include "ListSelect.h"
#include "ArduinoEclipseDefs.h"
#include "font_CourierNewBold.h"
#include "font_LiberationMonoBold.h"

/************************************************************************************
*                    S T A T I C   C L A S S   V A R I A B L E S                    *
*************************************************************************************
*/

// Select Button Panel
//
#define SELECT_PANEL_BTN_COUNT      2
#define SELECT_PANEL_BTNS_PER_ROW   1
#define SELECT_PANEL_BTN_WIDTH      75
#define SELECT_PANEL_BTN_HEIGHT     40
#define SELECT_PANEL_BTN_GAP_HORIZ  0
#define SELECT_PANEL_BTN_GAP_VERT   20

ButtonPanelInfo ListSelect::selectPanelDefaultInfo = (ButtonPanelInfo) {
    SELECT_PANEL_BTN_COUNT,         // btnCount
    SELECT_PANEL_BTNS_PER_ROW,      // btnsPerRow
    SELECT_PANEL_BTN_WIDTH,         // btnWidth
    SELECT_PANEL_BTN_HEIGHT,        // btnHeight
    SELECT_PANEL_BTN_GAP_HORIZ,     // btnGapHorizontal
    SELECT_PANEL_BTN_GAP_VERT,      // btnGapVertical
    BTN_PANEL_BGD_COLOR_DEFAULT,    // panelBgdColor
    COLOR_GRAY,                     // btnColor
    COLOR_GRAY,                     // btnActiveColor
    BTN_FONT_DEFAULT,               // btnFont
    BTN_SEL_TEXT_COLOR_DEFAULT,     // btnTextColor
    BTN_SEL_TEXT_COLOR_DEFAULT,     // btnActiveTextColor
    BTN_OUTLINE_COLOR_DEFAULT       // btnOutlineColor
};

// Exit Panel Button Info
// The button IDs and labels are defined here and will remain unchanged at run-time.
//
LabelIdButtonInfo ListSelect::selectPanelDefaultBtnInfo[] = {
    {"Select", BtnSelect},
    {"Cancel", BtnCancel}
};

// Default settings for List Display
//
TextColumnInfo listSelDefaultTextColumnInfo = (TextColumnInfo)  {
    LISTVIEW_XPOS,                  // xPos
    LISTVIEW_YPOS,                  // yPos
    LISTVIEW_COLUMN_WIDTH,          // width
    LISTVIEW_COLUMN_HEIGHT,         // height
    LISTVIEW_LINE_HEIGHT,           // lineHeight
    LISTVIEW_MAX_LINES,             // maxLines
    LISTVIEW_FONT,                  // font
    LISTVIEW_FONT_COLOR,            // textColor
    LISTVIEW_TEXT_MARGIN,           // textMargin
    LISTVIEW_TEXT_ALIGN,            // textAlign
    LISTVIEW_BGND_COLOR,            // columnColor
    LISTVIEW_OUTLINE_COLOR,         // outlineColor
    LISTVIEW_SEL_OUTLINE_COLOR      // selectedOutlineColor
    };

uint16_t ListSelect::WaitTimeBeforeRepeat = 50;   // ms

/************************************************************************************
*                    P U B L I C   C L A S S   F U N C T I O N S                    *
*************************************************************************************
*/

// Constructor/Destructor
//
ListSelect::ListSelect(TextColumnInfo* columnInfo) : selectPanelBtnInfo(selectPanelDefaultBtnInfo), selectPanel(selectPanelDefaultInfo) {

    pollEnabled         = false;
    reportOnTouch       = false;
    clearScreen         = true;

    selectPanelX        = (textColumn.columnInfo->xPos - selectPanel.panelWidth) / 2;
    selectPanelY        = (DISPLAY_HEIGHT - selectPanel.panelHeight) / 2;
    currentSelectBtn    = 0;

    selectedItemIndex   = 1;

    popupHeading        = "";
}

ListSelect::ListSelect() : ListSelect(&listSelDefaultTextColumnInfo) {

}

ListSelect::~ListSelect() {

}

// showPopup
//
void ListSelect::showPopup() {

//    Serial.printf("ListSelect::showPopup: columnListLength %d\n", columnListLength);

    // Clear screen
    if (clearScreen) {
        DisplayScreenBase::fillScreen(COLOR_BLACK);
    }

    showBaseObjects();

    // Draw Select Panel
    if (!reportOnTouch) {
        selectPanelX = (textColumn.columnInfo->xPos - selectPanel.panelWidth) / 2;
        selectPanelY = textColumn.columnInfo->yPos + (textColumn.columnInfo->height - selectPanel.panelHeight) / 2;
//        selectPanelY = (DISPLAY_HEIGHT - selectPanel.panelHeight) / 2;
        selectPanel.btnFont = selectPanelDefaultInfo.btnFont;
        selectPanel.setPanelPosition(selectPanelX, selectPanelY);
        selectPanel.drawPanel(selectPanelBtnInfo, currentSelectBtn);
    }

    pollEnabled = true;
}

// removePopup
//
void ListSelect::removePopup() {

    pollEnabled = false;
}

// pollPopup
//
ButtonId ListSelect::pollPopup() {

    ButtonId touchedBtn = BtnNone;
    uint16_t pointX, pointY;

    // Check for touched buttons
    if (DisplayScreenBase::touched(WaitTimeBeforeRepeat)) {
        DisplayScreenBase::getTouchedPoint(pointX, pointY);
        touchedBtn = pollPopup(pointX, pointY);
    }

    return touchedBtn;
}

ButtonId ListSelect::pollPopup(uint16_t pointX, uint16_t pointY) {

    ButtonId touchedBtn = BtnNone;
    ButtonId touchedLine;

    if (upBtn.buttonTouched(pointX, pointY)) {
        handleUpDownTouch(BtnUp);
//        ListView::handleUpDownBtn(BtnUp);
//        if (selectedItemIndex > columnListDisplayBase && selectedItemIndex < (columnListDisplayBase + textColumn.columnInfo->maxLines)) {
//            textColumn.setSelectedLine(selectedItemIndex - columnListDisplayBase);
//        }

    } else if (downBtn.buttonTouched(pointX, pointY)) {
        handleUpDownTouch(BtnDown);
//        ListView::handleUpDownBtn(BtnDown);
//        if (selectedItemIndex > columnListDisplayBase && selectedItemIndex < (columnListDisplayBase + textColumn.columnInfo->maxLines)) {
//            textColumn.setSelectedLine(selectedItemIndex - columnListDisplayBase);
//        }

    } else if (textColumn.isColumnLine(pointX, pointY, touchedLine)) {
        if ((touchedLine + columnListDisplayBase) != getSelectedLine()) {
            touchedBtn = handleListTouch(touchedLine);
        }
    } else if (!reportOnTouch) {
        if (selectPanel.isPanelButton(pointX, pointY, touchedBtn)) {
            touchedBtn = handleSelectBtnTouch(touchedBtn);
        }
    }

    return touchedBtn;
}

// setSelectedLine
//
void ListSelect::setSelectedLine(ButtonId touchedLine) {

//    Serial.printf("ListSelect::setSelectedLine: touchedLine %d\n", touchedLine);

    selectedItemIndex = touchedLine;

    uint16_t lineListDisplayBase = textColumn.columnInfo->maxLines * ((touchedLine - 1) / textColumn.columnInfo->maxLines);

//    Serial.printf("ListSelect::setSelectedLine: lineListDisplayBase %d\n", lineListDisplayBase);

    if (lineListDisplayBase != columnListDisplayBase) {
        columnListDisplayBase = lineListDisplayBase;
        showListSegment(columnListDisplayBase);
    }
    textColumn.setSelectedLine(touchedLine - columnListDisplayBase);
}

// unsetSelectedLine
//
void ListSelect::unsetSelectedLine() {

//    uint16_t lineListDisplayBase = textColumn.columnInfo->maxLines * ((selectedItemIndex - 1) / textColumn.columnInfo->maxLines);

//    Serial.printf("ListSelect::unsetSelectedLine: lineListDisplayBase %d\n", lineListDisplayBase);

//    if (lineListDisplayBase != columnListDisplayBase) {
//        columnListDisplayBase = lineListDisplayBase;
//        showListSegment(columnListDisplayBase);
//    }
    textColumn.unsetSelectedLine();
}

// getSelectedLine
//
ButtonId ListSelect::getSelectedLine() {

    ButtonId selected = BtnNone;

//    Serial.printf("ListSelect::getSelectedLine: textColumn.selectedLine %d\n", textColumn.selectedLine);

    if (textColumn.selectedLine != BtnNone) {
        selected = columnListDisplayBase + textColumn.selectedLine;
    }

    return selected;
}

// setPanelInfo
//
void ListSelect::setPanelInfo(ButtonPanelInfo& panelInfo) {
    selectPanel.setPanelInfo(panelInfo);
}

// setPanelBtnInfo
//
void ListSelect::setPanelBtnInfo(LabelIdButtonInfo* panelBtnInfo) {
    selectPanelBtnInfo = panelBtnInfo;
}

/************************************************************************************
*                   P R I V A T E   C L A S S   F U N C T I O N S                   *
*************************************************************************************
*/

// handleUpDownTouch
//
void ListSelect::handleUpDownTouch(ButtonId btnId) {

    switch (btnId) {
        case BtnDown:
//            Serial.printf("ListView::handleUpDownBtn: BtnDown\n");
            if (columnListDisplayBase + textColumn.columnInfo->maxLines < columnListLength) {
                columnListDisplayBase += textColumn.columnInfo->maxLines;
                showListSegment(columnListDisplayBase);
            }
            if (selectedItemIndex > columnListDisplayBase && selectedItemIndex < (columnListDisplayBase + textColumn.columnInfo->maxLines)) {
                textColumn.setSelectedLine(selectedItemIndex - columnListDisplayBase);
            }
            break;

        case BtnUp:
//            Serial.printf("ListView::handleUpDownBtn: BtnUp\n");
            if (columnListDisplayBase - textColumn.columnInfo->maxLines >= 0) {
                columnListDisplayBase -= textColumn.columnInfo->maxLines;
                showListSegment(columnListDisplayBase);
            }
            if (selectedItemIndex > columnListDisplayBase && selectedItemIndex < (columnListDisplayBase + textColumn.columnInfo->maxLines)) {
                textColumn.setSelectedLine(selectedItemIndex - columnListDisplayBase);
            }
            break;
    }
}

// handleUpDownTouch
//
bool ListSelect::handleUpDownTouch(uint16_t pointX, uint16_t pointY) {

    bool returnVal = false;

    if (upBtn.buttonTouched(pointX, pointY)) {
        // "ListView::" prefix unnecessary but added
        //  to emphasize use of base class function
        ListView::handleUpDownBtn(BtnUp);
        if (selectedItemIndex > columnListDisplayBase && selectedItemIndex < (columnListDisplayBase + textColumn.columnInfo->maxLines)) {
            textColumn.setSelectedLine(selectedItemIndex - columnListDisplayBase);
        }
//        Serial.printf("ListSelect::pollPopup(x,y): upBtn touched\n");
        returnVal = true;

    } else if (downBtn.buttonTouched(pointX, pointY)) {
        // "ListView::" prefix unnecessary but added
        //  to emphasize use of base class function
        ListView::handleUpDownBtn(BtnDown);
        if (selectedItemIndex > columnListDisplayBase && selectedItemIndex < (columnListDisplayBase + textColumn.columnInfo->maxLines)) {
            textColumn.setSelectedLine(selectedItemIndex - columnListDisplayBase);
        }
//        Serial.printf("ListSelect::pollPopup(x,y): downBtn touched\n");
        returnVal = true;
    }

    return returnVal;
}

// handleSelectBtnTouch
//
ButtonId ListSelect::handleSelectBtnTouch(ButtonId btnIndex) {

//    Serial.printf("ListSelect::handleSelectBtnTouch: %d\n", btnIndex);

    ButtonId returnVal = BtnNone;

    if (btnIndex != currentExitBtn) {
        currentExitBtn = btnIndex;
        selectPanel.drawPanel(selectPanelBtnInfo, currentExitBtn);
    }

    ButtonId btnId = selectPanelBtnInfo[btnIndex].buttonId;

    for (ButtonId btn = 0; btn < selectPanel.btnCount; btn++) {
        if (selectPanelBtnInfo[btn].buttonId == btnId) {
            returnVal = selectPanelBtnInfo[btn].buttonId;
            break;
        }
    }

    return returnVal;
}

// handleListTouch
//
ButtonId ListSelect::handleListTouch(ButtonId touchedLine) {

//    Serial.printf("ListSelect::handleListTouch: Line %d\n", touchedLine);

    ButtonId returnVal = BtnNone;

    touchedLine += columnListDisplayBase;
    if ((touchedLine) <= columnListLength) {
        setSelectedLine(touchedLine);
        if (reportOnTouch) {
            returnVal = touchedLine;
        }
    }

    return returnVal;
}

