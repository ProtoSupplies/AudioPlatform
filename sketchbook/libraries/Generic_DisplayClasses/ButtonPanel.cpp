/************************************************************************************
*                                  ButtonPanel.cpp                                  *
*                                                                                   *
*                      Copyright (c) 2025, Donald Swearingen                        *
*                               All Rights Reserved                                 *
*************************************************************************************
*
* Description
*
*   The ButtonPanel class creates and manages a panel (rectangular array) of individually
*   touchable screen "buttons".
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

#include "ButtonPanel.h"
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
ButtonPanel::ButtonPanel() {

    setPanelAttributes(BTN_COUNT_DEFAULT, BTNS_PER_ROW_DEFAULT, BTN_GAP_HORIZONTAL_DEFAULT, BTN_GAP_VERTICAL_DEFAULT);
    setButtonAttributes(BTN_COLOR_DEFAULT, BTN_SEL_COLOR_DEFAULT, BTN_FONT_DEFAULT, BTN_TEXT_COLOR_DEFAULT, BTN_SEL_TEXT_COLOR_DEFAULT, BTN_OUTLINE_COLOR_DEFAULT);
    setPanelPosition(DEFAULT_SCRN_MARGIN_LEFT, DEFAULT_SCRN_MARGIN_TOP);

    roundingValue = RoundedFillRadius;
    panelBgdColor = BTN_PANEL_BGD_COLOR_DEFAULT;
    selectedButton = 0;

    setDerivedAttributes();
}

ButtonPanel::ButtonPanel(ButtonPanelInfo& panelInfo) :
    btnCount(panelInfo.btnCount),
    btnsPerRow(panelInfo.btnsPerRow),
    btnWidth(panelInfo.btnWidth),
    btnHeight(panelInfo.btnHeight),
    btnGapHorizontal(panelInfo.btnGapHorizontal),
    btnGapVertical(panelInfo.btnGapVertical),
    panelBgdColor(panelInfo.panelBgdColor),
    btnColor(panelInfo.btnColor),
    btnActiveColor(panelInfo.btnActiveColor),
    btnFont(panelInfo.btnFont),
    btnTextColor(panelInfo.btnTextColor),
    btnActiveTextColor(panelInfo.btnActiveTextColor),
    btnOutlineColor(panelInfo.btnOutlineColor) {

    roundingValue = RoundedFillRadius;
    selectedButton = 0;

    setDerivedAttributes();

}

ButtonPanel::~ButtonPanel() {

}

// setBgColor()
//
void ButtonPanel::setBgdColor(int16_t color) {

    panelBgdColor = color;
}

// setPanelPosition()
//
void ButtonPanel::setPanelInfo(ButtonPanelInfo& panelInfo) {

    btnCount = panelInfo.btnCount;
    btnsPerRow = panelInfo.btnsPerRow;
    btnWidth = panelInfo.btnWidth;
    btnHeight = panelInfo.btnHeight;
    btnGapHorizontal = panelInfo.btnGapHorizontal;
    btnGapVertical = panelInfo.btnGapVertical;
    panelBgdColor = panelInfo.panelBgdColor;
    btnColor = panelInfo.btnColor;
    btnActiveColor = panelInfo.btnActiveColor;
    btnFont = panelInfo.btnFont;
    btnTextColor = panelInfo.btnTextColor;
    btnActiveTextColor = panelInfo.btnActiveTextColor;
    btnOutlineColor = panelInfo.btnOutlineColor;

    setDerivedAttributes();
}

// showPanelInfo()
//
void ButtonPanel::showPanelInfo() {
#if 0
    Serial.printf("ButtonPanel::showPanelInfo: btnCount %d\n", btnCount);
    Serial.printf("ButtonPanel::showPanelInfo: btnsPerRow %d\n", btnsPerRow);
    Serial.printf("ButtonPanel::showPanelInfo: btnWidth %d\n", btnWidth);
    Serial.printf("ButtonPanel::showPanelInfo: btnHeight %d\n", btnHeight);
    Serial.printf("ButtonPanel::showPanelInfo: btnGapHorizontal %d\n", btnGapHorizontal);
    Serial.printf("ButtonPanel::showPanelInfo: btnGapVertical %d\n", btnGapVertical);
    Serial.printf("ButtonPanel::showPanelInfo: panelBgdColor %d\n", panelBgdColor);
    Serial.printf("ButtonPanel::showPanelInfo: btnColor %d\n", btnColor);
    Serial.printf("ButtonPanel::showPanelInfo: btnActiveColor %d\n", btnActiveColor);
    Serial.printf("ButtonPanel::showPanelInfo: btnFont %x\n", btnFont);
    Serial.printf("ButtonPanel::showPanelInfo: btnTextColor %d\n", btnTextColor);
    Serial.printf("ButtonPanel::showPanelInfo: btnActiveTextColor %d\n", btnActiveTextColor);
    Serial.printf("ButtonPanel::showPanelInfo: btnOutlineColor %d\n", btnOutlineColor);

    Serial.println();
#endif
}

// setPanelPosition()
//
void ButtonPanel::setPanelPosition(int16_t x, int16_t y) {

//    Serial.printf("ButtonPanel::setPanelPosition: x %d y %d\n", x, y);

    panelX = x;
    panelY = y;
}

// setPanelAttributes()
//
void ButtonPanel::setPanelAttributes(uint8_t count, uint8_t perRow, uint8_t hGap, uint8_t vGap) {

    btnCount = count;
    btnsPerRow = perRow;
    btnGapHorizontal = hGap;
    btnGapVertical = vGap;

    setDerivedAttributes();
}

// setButtonAttributes()
//
void ButtonPanel::setButtonAttributes(uint16_t color, uint16_t activeColor, ILI9341_t3_font_t font, uint16_t textColor,  uint16_t textActiveColor, uint16_t outlineColor) {

    btnColor = color;
    btnActiveColor = activeColor;
    btnFont = font;
    btnTextColor = textColor;
    btnActiveTextColor = textActiveColor;
    btnOutlineColor = outlineColor;
}

// setButtonShape()
//
void ButtonPanel::setButtonShape(bool rounded) {

    if (rounded) {
        roundingValue = RoundedFillRadius;
    } else {
        roundingValue = 0;

    }
}

// setSelected()
//
void ButtonPanel::setSelected(LabelIdButtonInfo* btnInfo, ButtonId btnNum) {

//    Serial.printf("ButtonPanel::setSelected: btnInfo %x btnNum %d\n", btnInfo, btnNum);

    if (selectedButton != BtnNone) {
        drawButton(selectedButton, false);
        showButtonLabel(selectedButton, btnInfo[selectedButton].buttonLabel, false);
    }

    if (btnNum < btnCount) {
        // Make touchedBtn active and redisplay
        selectedButton = btnNum;
        drawButton(selectedButton, true);
        showButtonLabel(selectedButton, btnInfo[selectedButton].buttonLabel, true);
    }
}

void ButtonPanel::setSelected(const char* btnLabels[], ButtonId btnNum) {

//    Serial.printf("ButtonPanel::setSelected: btnLabels %x btnNum %d\n", btnLabels, btnNum);

    if (selectedButton != BtnNone) {
        drawButton(selectedButton, false);
        showButtonLabel(selectedButton, btnLabels[selectedButton], false);
    }

    if (btnNum < btnCount) {
        // Make touchedBtn active and redisplay
        selectedButton = btnNum;
        drawButton(selectedButton, true);
        showButtonLabel(selectedButton, btnLabels[selectedButton], true);
    }
}

// clearPanel()
//
void ButtonPanel::clearPanel() {

//    DisplayScreenBase::fillRoundRect(panelX, panelY, panelWidth, panelHeight, 0, panelBgdColor);
    DisplayScreenBase::fillRoundRect(panelX-1, panelY-1, panelWidth+2, panelHeight+2, 0, panelBgdColor);
}

// drawPanel()
//
void ButtonPanel::drawPanel(const char* btnLabels[]) {

//    Serial.printf("ButtonPanel::drawPanel: btnLabels %x\n", btnLabels);

    showPanelInfo();

    for (ButtonId btnNum = 0; btnNum < btnCount; btnNum++) {
//        Serial.printf("ButtonPanel::drawPanel: btnNum %d btnLabels[btnNum] %s\n", btnNum, btnLabels[btnNum]);
        drawButton(btnNum, false);
        showButtonLabel(btnNum, btnLabels[btnNum], false);
    }
}

void ButtonPanel::drawPanel(LabelIdButtonInfo* btnInfo, ButtonId selectedBtn) {

//    Serial.printf("ButtonPanel::drawPanel: selectedBtn Id %d\n", selectedBtn);

    for (ButtonId btnNum = 0; btnNum < btnCount; btnNum++) {
        drawButton(btnNum, (btnInfo[btnNum].buttonId == selectedBtn) ? true : false);

//        Serial.printf("ButtonPanel::drawPanel: btnNum %d btnId %d label %s\n", selectedBtn, btnInfo[btnNum].buttonId, btnInfo[btnNum].buttonLabel);

        snprintf(btnLabelBuf, ButtonLabelBufSize-1, "%s", btnInfo[btnNum].buttonLabel);
        showButtonLabel(btnNum, btnLabelBuf, (btnInfo[btnNum].buttonId == selectedBtn) ? true : false);
    }
}

// drawPanelButtons()
//
void ButtonPanel::drawPanelButtons() {

    for (ButtonId btnNum = 0; btnNum < btnCount; btnNum++) {
        drawButton(btnNum, false);
    }
}

// drawButton()
//
void ButtonPanel::drawButton(ButtonId btnNum, bool isActive) {

    uint16_t    btnX;
    uint16_t    btnY;

//    Serial.printf("ButtonPanel::drawButton: btnNum %d isActive %d\n", btnNum, isActive);

    getButtonXY(btnNum, btnX, btnY);

    DisplayScreenBase::fillRoundRect(btnX, btnY, btnWidth, btnHeight, roundingValue, isActive ? btnActiveColor : btnColor);
    DisplayScreenBase::drawRoundRect(btnX-1, btnY-1, btnWidth+2, btnHeight+2, roundingValue, btnOutlineColor);
}

// drawButtonOutline()
//
void ButtonPanel::drawButtonOutline(ButtonId btnNum, uint16_t outlineColor) {

    uint16_t    btnX;
    uint16_t    btnY;

//    Serial.printf("ButtonPanel::drawButtonOutline: btnNum %d outlineColor %d\n", btnNum, outlineColor);

    getButtonXY(btnNum, btnX, btnY);

    DisplayScreenBase::drawRoundRect(btnX-1, btnY-1, btnWidth+2, btnHeight+2, roundingValue, outlineColor);
}

// showButtonLabel()
//
// Display button label in horizontal center and vertical center of button
// (for buttons with single text entry)
//
void ButtonPanel::showButtonLabel(ButtonId btnNum, const char* btnLabel, bool isActive) {

//    Serial.printf("ButtonPanel::showButtonLabel: btnNum %d btnLabel %s isActive %d\n", btnNum, btnLabel, isActive);

    uint16_t    btnX;
    uint16_t    btnY;

    getButtonXY(btnNum, btnX, btnY);

    // Display area
    TextArea tArea;
    tArea.posX = btnX;
    tArea.posY = btnY;
    tArea.wid = btnWidth;
    tArea.hgt = btnHeight;
    tArea.align = TextAlignCenter;

//    Serial.printf("ButtonPanel::showButtonLabel: About to print tArea x %d y %d wid %d hgt %d\n", tArea.posX, tArea.posY, tArea.wid, tArea.hgt);

    DisplayScreenBase::setFont(btnFont);
    DisplayScreenBase::setTextColor(isActive ? btnActiveTextColor : btnTextColor);
    DisplayScreenBase::printInArea(btnLabel, tArea);
}

// showButtonName()
//
// Display button name in horizontal center and vertical center of top half of button
// (for buttons with name and value text entries)
//
void ButtonPanel::showButtonName(ButtonId btnNum, const char* paramName, bool isActive) {

    uint16_t    btnX;
    uint16_t    btnY;

    getButtonXY(btnNum, btnX, btnY);

    // Display area
    TextArea tArea;
    tArea.posX = btnX;
    tArea.posY = btnY + 2;
    tArea.wid = btnWidth;
    tArea.hgt = btnHeight / 2;
    tArea.align = TextAlignCenter;

    DisplayScreenBase::setFont(btnFont);
    DisplayScreenBase::setTextColor(isActive ? btnActiveTextColor : btnTextColor);
    DisplayScreenBase::printInArea(paramName, tArea);
}

// showButtonValue()
//
// Display button value in horizontal center and vertical center of lower half of button
// (for buttons with name and value text entries)
//
void ButtonPanel::showButtonValue(ButtonId btnNum, const char* valueStr, bool isActive) {

    uint16_t    btnX;
    uint16_t    btnY;

    getButtonXY(btnNum, btnX, btnY);

    // Display area
    TextArea tArea;
    tArea.posX = btnX;
    tArea.posY = btnY +  btnHeight / 2 - 2;
    tArea.wid = btnWidth;
    tArea.hgt = btnHeight / 2;
    tArea.align = TextAlignCenter;

    DisplayScreenBase::setFont(btnFont);
    DisplayScreenBase::setTextColor(isActive ? btnActiveTextColor : btnTextColor);
    DisplayScreenBase::fillRect(tArea.posX, tArea.posY, tArea.wid, tArea.hgt, isActive ? btnActiveColor : btnColor);  // Clear previous value
    DisplayScreenBase::printInArea(valueStr, tArea);
}

// getButtonXY()
//
void ButtonPanel::getButtonXY(ButtonId btnNum, uint16_t& objectX, uint16_t& objectY) {

    uint8_t     objectRow;
    uint8_t     objectColumn;

    objectRow = 1 +  btnNum / btnsPerRow;
    objectColumn = 1 + btnNum % btnsPerRow;

    objectX = panelX + btnGapHorizontal * (objectColumn - 1) + (btnNum % btnsPerRow) * btnWidth;
    objectY = panelY + btnGapVertical * (objectRow - 1) + btnHeight * (objectRow - 1);
}

// isPanelButton()
//
bool ButtonPanel::isPanelButton(int16_t pointX, int16_t pointY, ButtonId& btnNum) {

    uint8_t     numRows = btnCount / btnsPerRow;
    uint16_t    colX;
    uint16_t    rowY;
    bool        panelButton = false;

    btnNum = BtnNone;

    for (int row = 0; row < numRows; row++) {
        rowY = panelY + row * (btnHeight + btnGapVertical);

        for (int col = 0; col < btnsPerRow; col++) {
            colX = panelX + col * (btnGapHorizontal + btnWidth);

            if ((pointX > colX) && (pointX <= (colX + btnWidth))) {
                if ((pointY > rowY) && (pointY <= (rowY + btnHeight))) {
                    btnNum = row * btnsPerRow + col;
                    panelButton = true;
                    break;
                }
            }
        }
    }

    return panelButton;
}

/************************************************************************************
*                   P R I V A T E   C L A S S   F U N C T I O N S                   *
*************************************************************************************
*/

// setDerivedAttributes()
//
void ButtonPanel::setDerivedAttributes() {

    btnRowCount = btnCount / btnsPerRow;
    if (btnCount % btnsPerRow != 0) {
        btnRowCount += 1;
    }
    btnColumnCount = btnsPerRow;

    panelWidth = (btnsPerRow * btnWidth) + ((btnsPerRow - 1) * btnGapHorizontal);
    panelHeight = (btnRowCount * btnHeight) + ((btnRowCount - 1) * btnGapVertical);
}

