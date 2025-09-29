/************************************************************************************
*                                    ListView.cpp                                   *
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
#include "ListView.h"
#include "BitMapData.h"

/************************************************************************************
*                    S T A T I C   C L A S S   V A R I A B L E S                    *
*************************************************************************************
*/

// Exit Button Panel (single button)
//
#define EXIT_PANEL_BTN_COUNT        1
#define EXIT_PANEL_BTNS_PER_ROW     1
#define EXIT_PANEL_BTN_WIDTH        75
#define EXIT_PANEL_BTN_HEIGHT       40
#define EXIT_PANEL_BTN_GAP_HORIZ    0
#define EXIT_PANEL_BTN_GAP_VERT     0

ButtonPanelInfo ListView::exitPanelInfo = (ButtonPanelInfo) {
    EXIT_PANEL_BTN_COUNT,           // btnCount
    EXIT_PANEL_BTNS_PER_ROW,        // btnsPerRow
    EXIT_PANEL_BTN_WIDTH,           // btnWidth
    EXIT_PANEL_BTN_HEIGHT,          // btnHeight
    EXIT_PANEL_BTN_GAP_HORIZ,       // btnGapHorizontal
    EXIT_PANEL_BTN_GAP_VERT,        // btnGapVertical
    BTN_PANEL_BGD_COLOR_DEFAULT,    // panelBgdColor
    COLOR_GRAY,                     // btnColor
    COLOR_GRAY,                     // btnActiveColor
    BTN_FONT_DEFAULT,               // btnFont
    COLOR_WHITE,                    // btnTextColor
    COLOR_WHITE,                    // btnActiveTextColor
    BTN_OUTLINE_COLOR_DEFAULT       // btnOutlineColor
};

// Exit Panel Button Info
//
LabelIdButtonInfo ListView::exitPanelBtnInfo[] = {
    {"Exit", BtnExit},
};

// Up/Down Buttons for scrolling list
//
ScreenButtonInfo ListView::upBtnInfo = (ScreenButtonInfo) {
     20,                            // btnX
     20,                            // btnY
     51,                            // btnWid
     51,                            // btnHgt
     COLOR_BLACK,                   // btnColor
     SCREENBUTTON_OUTLINE_COLOR,    // btnOutlineColor
     0,                             // btnOutlineRadius
     "",                            // btnLabel
     SCREENBUTTON_TEXT_COLOR,       // btnLabelColor
     SCREENBUTTON_TEXT_FONT,        // btnLabelFont (note: must set again in constructor B241001-1)
     SCREENBUTTON_TEXT_ALIGN,       // btnLabelAlign
     SCREENBUTTON_TEXT_POSITION,    // btnLabelPosition
     UpIcon_50x50,                  // bitMapInfo
};

ScreenButtonInfo ListView::downBtnInfo = (ScreenButtonInfo) {
     20,                            // btnX
     100,                           // btnY
     51,                            // btnWid
     51,                            // btnHgt
     COLOR_BLACK,                   // btnColor
     SCREENBUTTON_OUTLINE_COLOR,    // btnOutlineColor
     0,                             // btnOutlineRadius
     "",                            // btnLabel
     SCREENBUTTON_TEXT_COLOR,       // btnLabelColor
     SCREENBUTTON_TEXT_FONT,        // btnLabelFont (note: must set again in constructor B241001-1)
     SCREENBUTTON_TEXT_ALIGN,       // btnLabelAlign
     SCREENBUTTON_TEXT_POSITION,    // btnLabelPosition
     DownIcon_50x50,                // bitMapInfo
};

// Default settings for List Display
//
TextColumnInfo defaultTextColumnInfo = (TextColumnInfo)  {
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

uint16_t ListView::WaitTimeBeforeRepeat = 50;   // ms

/************************************************************************************
*                    P U B L I C   C L A S S   F U N C T I O N S                    *
*************************************************************************************
*/

// Constructor/Destructor
//
ListView::ListView() :
        exitPanel(exitPanelInfo),
        upBtn(upBtnInfo), downBtn(downBtnInfo),
        textColumn(&defaultTextColumnInfo), columnList(0),
        columnListLength(0), columnListDisplayBase(0) {

    pollEnabled     = false;

    exitPanelX      = (textColumn.columnInfo->xPos - exitPanel.panelWidth) / 2;
    exitPanelY      = (DISPLAY_HEIGHT - exitPanel.panelHeight) / 2;
    currentExitBtn  = 0;

    popupHeading    = "";
}

ListView::~ListView() {

}

// showPopup
//
void ListView::showPopup() {

//    Serial.printf("ListView::showPopup:\n");

    // Clear screen
    DisplayScreenBase::fillScreen(COLOR_BLACK);

    // Basic screen elements
    showBaseObjects();

    // Recalculate Exit panel X position
    exitPanelX = (textColumn.columnInfo->xPos - exitPanel.panelWidth) / 2;

    // Draw Exit Panel
    exitPanel.btnFont = exitPanelInfo.btnFont;
    exitPanel.setPanelPosition(exitPanelX, exitPanelY);
    exitPanel.drawPanel(exitPanelBtnInfo, currentExitBtn);

    // Polling enabled by owner/user
    pollEnabled = true;
}


// removePopup
//
void ListView::removePopup() {

    pollEnabled = false;
    upBtn.removeButton();
    downBtn.removeButton();
}

// showBaseObjects
//
// Display basic screen objects
//
void ListView::showBaseObjects() {

    // Heading
    showPopupHeading();

    // TextColumn and current list segmrnt
    textColumn.drawColumnCells();
    showListSegment(columnListDisplayBase);

    // Draw Up Down Buttons
    // Centered in area between right side of list and right side of sreen
    //
    uint16_t rightAreaX = textColumn.columnInfo->xPos + textColumn.columnInfo->width;
    uint16_t rightAreaWidth = DISPLAY_WIDTH - rightAreaX;

    upBtnInfo.btnX = rightAreaX + (rightAreaWidth - upBtnInfo.btnWid) / 2;
    upBtnInfo.btnY = textColumn.columnInfo->yPos + (textColumn.columnInfo->height - (2 * upBtnInfo.btnHgt + 10)) / 2;
    upBtn.drawButton();

    downBtnInfo.btnX = upBtnInfo.btnX;
    downBtnInfo.btnY = upBtnInfo.btnY + upBtnInfo.btnHgt + 10;
    downBtn.drawButton();

//    Serial.printf("ListView::showBaseObjects: textColumn yPos %d textColumn height %d\n", textColumn.columnInfo->yPos, textColumn.columnInfo->height);
//    Serial.printf("ListView::showBaseObjects: upBtn.btnY %d upBtn.btnHgt %d downBtn.btnY %d \n", upBtn.btnY, upBtn.btnHgt, downBtn.btnY);
}

// pollPopup
//
ButtonId ListView::pollPopup() {

    ButtonId touchedBtn = BtnNone;
    uint16_t pointX, pointY;

    // Check for touched buttons
    if (DisplayScreenBase::touched(WaitTimeBeforeRepeat, true)) {
        DisplayScreenBase::getTouchedPoint(pointX, pointY);

        if (exitPanel.isPanelButton(pointX, pointY, touchedBtn)) {
            touchedBtn = BtnExit;

        } else if (upBtn.buttonTouched(pointX, pointY)) {
            handleUpDownBtn(BtnUp);

        } else if (downBtn.buttonTouched(pointX, pointY)) {
            handleUpDownBtn(BtnDown);
        }
    }

    return touchedBtn;
}

// setPopupHeading
//
void ListView::setPopupHeading(const char* text) {

    if (strlen(text) > 0) {
        popupHeading = text;
    } else {
        popupHeading = "";
    }
}

// showPopupHeading
//
void ListView::showPopupHeading() {

    DisplayScreenBase::setFont(LISTVIEW_HEADING_FONT);

    uint16_t textWid, textHgt;
    DisplayScreenBase::measureText(popupHeading, textWid, textHgt);

//    Serial.printf("ListView::showPopupHeading: popupHeading %s\n", popupHeading);

    TextArea tArea;
    tArea.posX = 250;
    tArea.posY = textColumn.columnInfo->yPos - 25;
    tArea.wid = 300;
    tArea.hgt = textHgt;
    tArea.align = TextAlignCenter;

    DisplayScreenBase::setTextColor(LISTVIEW_HEADING_FONT_COLOR);
    DisplayScreenBase::printInArea(popupHeading, tArea);
}

// setList
//
void ListView::setList(char *list[], uint16_t listLength) {

    columnList = list;
    columnListLength = listLength;
    columnListDisplayBase = 0;

//    Serial.printf("ListView::setList: columnListLength %d columnList[0] %s\n", columnListLength, columnList[0]);
}

// showListSegment
//
void ListView::showListSegment(uint16_t segmentBase) {

//    Serial.printf("ListView::showListSegment: segmentBase %d columnListLength %d\n", segmentBase, columnListLength);

    if (columnListLength > 0) {
        textColumn.drawColumnBgnd();
        for (int lineNum = 1; (lineNum + segmentBase) <= columnListLength; lineNum++) {
            printTextLineInColumn(lineNum, columnList[segmentBase + lineNum-1]);
        }
    }
}

// setTextColumnInfo
//
void ListView::setTextColumnInfo(TextColumnInfo* columnInfo_) {

    textColumn.setColumnInfo(columnInfo_);
}

// showTextColumnInfo
//
void ListView::showTextColumnInfo() {

    textColumn.showColumnInfo();
}

// setListDisplayWidth
//
void ListView::setListDisplayX(uint16_t listX) {

    textColumn.columnInfo->xPos = listX;
}

// setListDisplayWidth
//
void ListView::setListDisplayY(uint16_t listY) {

    textColumn.columnInfo->yPos = listY;
}

// setListDisplayWidth
//
void ListView::setListDisplayWidth(uint16_t listWidth) {

    textColumn.columnInfo->width = listWidth;
}

// setListDisplayWidth
//
void ListView::setListDisplayHeight(uint16_t listHeight) {

    textColumn.columnInfo->height = listHeight;
}

// setTextAlign
//
void ListView::setTextAlign(TextAlign align) {

    textColumn.columnInfo->textAlign = align;
}

/************************************************************************************
*        P R I V A T E / P R O T E C T E D   C L A S S   F U N C T I O N S          *
*************************************************************************************
*/

// handleUpDownBtn
//
void ListView::handleUpDownBtn(ButtonId btnId) {

//    Serial.printf("ListView::handleUpDownBtn: %d\n", btnId);

    switch (btnId) {
        case BtnDown:
//            Serial.printf("ListView::handleUpDownBtn: BtnDown\n");
            if (columnListDisplayBase + textColumn.columnInfo->maxLines < columnListLength) {
                columnListDisplayBase += textColumn.columnInfo->maxLines;
                showListSegment(columnListDisplayBase);
            }
            break;

        case BtnUp:
//            Serial.printf("ListView::handleUpDownBtn: BtnUp\n");
            if (columnListDisplayBase - textColumn.columnInfo->maxLines >= 0) {
                columnListDisplayBase -= textColumn.columnInfo->maxLines;
                showListSegment(columnListDisplayBase);
            }
            break;
    }
}

// printTextLineInColumn
//
void ListView::printTextLineInColumn(uint16_t lineNum, const char* text) {

    if (lineNum > 0 && lineNum <= columnListLength) {
        textColumn.printTextLine(lineNum, text);
    }
}

