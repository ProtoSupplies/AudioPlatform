/************************************************************************************
*                                 TextEdit.cpp                                 *
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
* Modifications List
*
*   •
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

#include "ArduinoEclipseDefs.h"
#include "TextEdit.h"

/************************************************************************************
*                         M A C R O S  /  C O N S T A N T S                         *
*************************************************************************************
*/

/************************************************************************************
*                    S T A T I C   C L A S S   V A R I A B L E S                    *
*************************************************************************************
*
* Static Member Initializations
*
*/

// Keyboard Button Panel
//
#define KBDIN_SCRN_KBD_BTN_COUNT        40
#define KBDIN_SCRN_KBD_BTNS_PER_ROW     10
#define KBDIN_SCRN_KBD_BTN_WIDTH        50
#define KBDIN_SCRN_KBD_BTN_HEIGHT       40
#define KBDIN_SCRN_KBD_BTN_GAP_HORIZ    7
#define KBDIN_SCRN_KBD_BTN_GAP_VERT     7
#define KBDIN_SCRN_KBD_BTN_FONT         Arial_12_Bold

ButtonPanelInfo TextEdit::kbdInBtnPanelInfo = (ButtonPanelInfo) {
    KBDIN_SCRN_KBD_BTN_COUNT,           // btnCount
    KBDIN_SCRN_KBD_BTNS_PER_ROW,        // btnsPerRow
    KBDIN_SCRN_KBD_BTN_WIDTH,           // btnWidth
    KBDIN_SCRN_KBD_BTN_HEIGHT,          // btnHeight
    KBDIN_SCRN_KBD_BTN_GAP_HORIZ,       // btnGapHorizontal
    KBDIN_SCRN_KBD_BTN_GAP_VERT,        // btnGapVertical
    COLOR_BLACK,                        // panelBgdColor
    BTN_COLOR_DEFAULT,                  // btnColor
    BTN_SEL_COLOR_DEFAULT,              // btnActiveColor
    KBDIN_SCRN_KBD_BTN_FONT,            // btnFont
    COLOR_BLACK,                        // btnTextColor
    BTN_SEL_TEXT_COLOR_DEFAULT,         // btnActiveTextColor
    BTN_OUTLINE_COLOR_DEFAULT           // btnOutlineColor
};

// Case Select Button Panel
//
#define CASE_PANEL_BTN_COUNT        2
#define CASE_PANEL_BTNS_PER_ROW     2
#define CASE_PANEL_BTN_WIDTH        50
#define CASE_PANEL_BTN_HEIGHT       40
#define CASE_PANEL_BTN_GAP_HORIZ    4
#define CASE_PANEL_BTN_GAP_VERT     0

ButtonPanelInfo TextEdit::casePanelInfo = (ButtonPanelInfo) {
    CASE_PANEL_BTN_COUNT,           // btnCount
    CASE_PANEL_BTNS_PER_ROW,        // btnsPerRow
    CASE_PANEL_BTN_WIDTH,           // btnWidth
    CASE_PANEL_BTN_HEIGHT,          // btnHeight
    CASE_PANEL_BTN_GAP_HORIZ,       // btnGapHorizontal
    CASE_PANEL_BTN_GAP_VERT,        // btnGapVertical
    BTN_PANEL_BGD_COLOR_DEFAULT,    // panelBgdColor
    COLOR_WHEAT,                // btnColor
    COLOR_GRAY,                     // btnActiveColor
    BTN_FONT_DEFAULT,               // btnFont
    BTN_TEXT_COLOR_DEFAULT,         // btnTextColor
    BTN_SEL_TEXT_COLOR_DEFAULT,     // btnActiveTextColor
    BTN_OUTLINE_COLOR_DEFAULT       // btnOutlineColor
};

// Edit Button Panel
//
#define EDIT_PANEL_BTN_COUNT        2
#define EDIT_PANEL_BTNS_PER_ROW     2
#define EDIT_PANEL_BTN_WIDTH        65
#define EDIT_PANEL_BTN_HEIGHT       40
#define EDIT_PANEL_BTN_GAP_HORIZ    4
#define EDIT_PANEL_BTN_GAP_VERT     0

ButtonPanelInfo TextEdit::editPanelInfo = (ButtonPanelInfo) {
    EDIT_PANEL_BTN_COUNT,           // btnCount
    EDIT_PANEL_BTNS_PER_ROW,        // btnsPerRow
    EDIT_PANEL_BTN_WIDTH,           // btnWidth
    EDIT_PANEL_BTN_HEIGHT,          // btnHeight
    EDIT_PANEL_BTN_GAP_HORIZ,       // btnGapHorizontal
    EDIT_PANEL_BTN_GAP_VERT,        // btnGapVertical
    BTN_PANEL_BGD_COLOR_DEFAULT,    // panelBgdColor
    COLOR_WHEAT,                // btnColor
    COLOR_GRAY,                     // btnActiveColor
    BTN_FONT_DEFAULT,               // btnFont
    BTN_TEXT_COLOR_DEFAULT,         // btnTextColor
    BTN_SEL_TEXT_COLOR_DEFAULT,     // btnActiveTextColor
    BTN_OUTLINE_COLOR_DEFAULT       // btnOutlineColor
};

// Exit Button Panel
//
#define EXIT_PANEL_BTN_COUNT        2
#define EXIT_PANEL_BTNS_PER_ROW     2
#define EXIT_PANEL_BTN_WIDTH        75
#define EXIT_PANEL_BTN_HEIGHT       40
#define EXIT_PANEL_BTN_GAP_HORIZ    4
#define EXIT_PANEL_BTN_GAP_VERT     0

ButtonPanelInfo TextEdit::exitPanelInfo = (ButtonPanelInfo) {
    EXIT_PANEL_BTN_COUNT,           // btnCount
    EXIT_PANEL_BTNS_PER_ROW,        // btnsPerRow
    EXIT_PANEL_BTN_WIDTH,           // btnWidth
    EXIT_PANEL_BTN_HEIGHT,          // btnHeight
    EXIT_PANEL_BTN_GAP_HORIZ,       // btnGapHorizontal
    EXIT_PANEL_BTN_GAP_VERT,        // btnGapVertical
    BTN_PANEL_BGD_COLOR_DEFAULT,    // panelBgdColor
    COLOR_WHEAT,                // btnColor
    COLOR_GRAY,                     // btnActiveColor
    BTN_FONT_DEFAULT,               // btnFont
    BTN_TEXT_COLOR_DEFAULT,         // btnTextColor
    BTN_SEL_TEXT_COLOR_DEFAULT,     // btnActiveTextColor
    BTN_OUTLINE_COLOR_DEFAULT       // btnOutlineColor
};

// Upper/Lower Case Panel Button Info
//
const char* TextEdit::kbdUpperBtnInfo[] = {
    {"1"}, {"2"}, {"3"}, {"4"}, {"5"}, {"6"}, {"7"}, {"8"}, {"9"}, {"0"},
    {"Q"}, {"W"}, {"E"}, {"R"}, {"T"}, {"Y"}, {"U"}, {"I"}, {"O"}, {"P"},
    {"A"}, {"S"}, {"D"}, {"F"}, {"G"}, {"H"}, {"J"}, {"K"}, {"L"}, {"."},
    {"_"}, {"Z"}, {"X"}, {"C"}, {"V"}, {"B"}, {"N"}, {"M"}, {"-"}, {" "}
};

const char* TextEdit::kbdLowerBtnInfo[] = {
    {"1"}, {"2"}, {"3"}, {"4"}, {"5"}, {"6"}, {"7"}, {"8"}, {"9"}, {"0"},
    {"q"}, {"w"}, {"e"}, {"r"}, {"t"}, {"y"}, {"u"}, {"i"}, {"o"}, {"p"},
    {"a"}, {"s"}, {"d"}, {"f"}, {"g"}, {"h"}, {"j"}, {"k"}, {"l"}, {"."},
    {"_"}, {"z"}, {"x"}, {"c"}, {"v"}, {"b"}, {"n"}, {"m"}, {"-"}, {" "}
};

// Case Select Panel Button Info
//
LabelIdButtonInfo TextEdit::casePanelBtnInfo[] = {
    {"abc", BtnLower},
    {"ABC", BtnUpper}
};

// Edit Panel Button Info
//
LabelIdButtonInfo TextEdit::editPanelBtnInfo[] = {
    {"BkSpc", BtnBkSpc},
    {"Clear", BtnClear}
};

// Exit Panel Button Info
// The button IDs and labels are defined here and will remain unchanged at run-time.
//
LabelIdButtonInfo TextEdit::exitPanelBtnInfo[] = {
    {"Set",    BtnSave},
    {"Cancel", BtnCancel}
};

uint16_t TextEdit::textFieldWidth;
uint16_t TextEdit::textFieldHeight;
uint16_t TextEdit::kbdPanelX;
uint16_t TextEdit::kbdPanelY;
uint16_t TextEdit::casePanelX;
uint16_t TextEdit::casePanelY;
uint16_t TextEdit::editPanelX;
uint16_t TextEdit::editPanelY;
uint16_t TextEdit::exitPanelX;
uint16_t TextEdit::exitPanelY;

uint16_t TextEdit::WaitTimeAfterKeyTouch = 100;   // ms

/************************************************************************************
*                    P U B L I C   C L A S S   F U N C T I O N S                    *
*************************************************************************************
*/

// Constructor/Destructor
//
TextEdit::TextEdit(uint16_t popupX, uint16_t popupY, uint8_t _maxTextLength) :
    kbdPanel(kbdInBtnPanelInfo),
    casePanel(casePanelInfo),
    editPanel(editPanelInfo),
    exitPanel(exitPanelInfo),
    maxTextLength(_maxTextLength) {

    pollEnabled         = false;
    keyPadBtnLabels     = kbdLowerBtnInfo;
    currentKbdBtn       = BtnNone;

    maxTextLength       = min(maxTextLength, TextEditBufferSize - 1);
    textFieldWidth      = 400;
    textFieldHeight     = 40;
    textFieldX          = (DISPLAY_WIDTH - textFieldWidth) / 2;;
    textFieldY          = popupY;

    kbdPanelX           = (DISPLAY_WIDTH - kbdPanel.panelWidth) / 2;
    kbdPanelY           = textFieldY + textFieldHeight + 40;

    casePanelX          = kbdPanelX;
    casePanelY          = kbdPanelY + kbdPanel.panelHeight + 40;
    currentCaseBtn      = BtnLower;

    editPanelX          = (DISPLAY_WIDTH - editPanel.panelWidth) / 2;
    editPanelY          = casePanelY;
    currentEditBtn      = BtnBkSpc;

    exitPanelX          = kbdPanelX + kbdPanel.panelWidth - exitPanel.panelWidth;
    exitPanelY          = casePanelY;
    currentExitBtn      = BtnCancel;

    textFieldArea.posX  = textFieldX;
    textFieldArea.posY  = textFieldY;
    textFieldArea.wid   = textFieldWidth;
    textFieldArea.hgt   = textFieldHeight;
    textFieldArea.align = TextAlignCenter;
    textHeading         = TEXTFIELD_HEADING_DEFAULT;

    textLength          = 0;
}

TextEdit::~TextEdit() {

}

// showPopup
//
void TextEdit::showPopup() {

//    Serial.printf("TextEdit::showPopup: \n");

    // Set button font addresses (not yet defined in constructor above, B240924-1)
    // TextEdit sets the attributes for the button panels it manages in its constructor,
    // using the definitions defined in its body. This is a general standard. But, in this
    // case, since the ButtonPanels are objects within TextEdit, for some reason the
    // addresses of the fonts are not yet set, and are instead all still set to 0. This causes
    // crashes in ButtonPanel.showButtonLabel at runtime. The order of all this is not fully
    // understood at this time (9/24/24 rev 3.4.5, B240924-1). But what this means is that the
    // btnFont component must be set directly at runtime, before the panel is drawn.
    kbdPanel.btnFont  = kbdInBtnPanelInfo.btnFont;
    casePanel.btnFont = casePanelInfo.btnFont;
    editPanel.btnFont = editPanelInfo.btnFont;
    exitPanel.btnFont = exitPanelInfo.btnFont;

    // Clear screen
    // TODO: Define a background color field
    DisplayScreenBase::fillScreen(COLOR_BLACK);


    // (Re)Set action panel edit button states
    currentCaseBtn = BtnLower;
    currentEditBtn = BtnBkSpc;
    currentExitBtn = BtnCancel;

    // Draw screen panels
    showTextHeading();
    drawTextDisplayField();
    displayTextBuffer();
    keyPadBtnLabels = kbdLowerBtnInfo;
    drawKeyPad();
    drawActionPanels();

    pollEnabled = true;
}

// removePopup
//
void TextEdit::removePopup() {

    pollEnabled = false;
}

// pollPopup
//
ButtonId TextEdit::pollPopup() {

    ButtonId touchedBtn = BtnNone;
    uint16_t pointX, pointY;

    // Check for touched buttons
    if (DisplayScreenBase::touched(WaitTimeAfterKeyTouch)) {
        DisplayScreenBase::getTouchedPoint(pointX, pointY);

//        Serial.printf("TextEdit::pollPopup: pollPopup touched x %d y %d\n", pointX, pointY);

        if (kbdPanel.isPanelButton(pointX, pointY, touchedBtn)) {
            if (currentKbdBtn != BtnNone) {
                kbdPanel.drawButton(currentKbdBtn, false);
                kbdPanel.showButtonLabel(currentKbdBtn, keyPadBtnLabels[currentKbdBtn], false);
            }

            handleKbdBtn(keyPadBtnLabels[touchedBtn][0]);

            currentKbdBtn = touchedBtn;
            kbdPanel.drawButton(currentKbdBtn, true);
            kbdPanel.showButtonLabel(currentKbdBtn, keyPadBtnLabels[currentKbdBtn], true);
            touchedBtn = BtnNone;

        } else if (casePanel.isPanelButton(pointX, pointY, touchedBtn)) {
            handleCaseBtn(touchedBtn);
            touchedBtn = BtnNone;

        } else if (editPanel.isPanelButton(pointX, pointY, touchedBtn)) {
            handleEditBtn(touchedBtn);
            touchedBtn = BtnNone;

        } else if (exitPanel.isPanelButton(pointX, pointY, touchedBtn)) {
            touchedBtn = handleExitBtn(touchedBtn);
        }
    }

    return touchedBtn;
}

// setMaxTextLength
//
void TextEdit::setMaxTextLength(uint8_t length) {

    maxTextLength = length;
}

// drawKeyPad
//
void TextEdit::setTextValue(const char* text) {

    clearTextBuffer();
    strncpy(textBuffer, text, TextEditBufferSize-1);
    textLength = strlen(textBuffer);
}

// drawKeyPad
//
const char* TextEdit::getTextValue() {

    return &textBuffer[0];
}

// setTextHeading
//
void TextEdit::setTextHeading(const char* text) {

    if (strlen(text) > 0) {
        textHeading = text;
    } else {
        textHeading = TEXTFIELD_HEADING_DEFAULT;
    }
}

/************************************************************************************
*                   P R I V A T E   C L A S S   F U N C T I O N S                   *
*************************************************************************************
*/

// showTextHeading
//
void TextEdit::showTextHeading() {

//    Serial.printf("TextEdit::showTextHeading:\n");

    DisplayScreenBase::setFont(TEXTFIELD_HEADING_FONT);
    uint16_t textWid, textHgt;
    DisplayScreenBase::measureText(textHeading, textWid, textHgt);

    TextArea tArea;
    tArea.posX = (DISPLAY_WIDTH - textWid) / 2;
    tArea.posY = 50;
    tArea.wid = textWid;
    tArea.hgt = textHgt;
    tArea.align = TextAlignCenter;

    DisplayScreenBase::setTextColor(TEXTFIELD_HEADING_FONT_COLOR);
    DisplayScreenBase::printInArea(textHeading, tArea);

}

// drawKeyPad
//
void TextEdit::drawKeyPad() {

//    Serial.printf("TextEdit::drawKeyPad: \n");

    kbdPanel.setPanelPosition(kbdPanelX, kbdPanelY);
    kbdPanel.drawPanel(keyPadBtnLabels);
}

// drawActionPanels
//
void TextEdit::drawActionPanels() {

//    Serial.printf("TextEdit::drawActionPanels: \n");

    casePanel.setPanelPosition(casePanelX, casePanelY);
    casePanel.drawPanel(casePanelBtnInfo, currentCaseBtn);

    editPanel.setPanelPosition(editPanelX, editPanelY);
    editPanel.drawPanel(editPanelBtnInfo, currentEditBtn);

    exitPanel.setPanelPosition(exitPanelX, exitPanelY);
    exitPanel.drawPanel(exitPanelBtnInfo, currentExitBtn);
}

// clearTextDisplayArea
//
void TextEdit::clearTextDisplayArea() {

//    Serial.printf("TextEdit::clearTextDisplayArea: \n");

    DisplayScreenBase::fillRoundRect(textFieldX, textFieldY, textFieldWidth, textFieldHeight, 4, TEXTFIELD_COLOR);
}

// drawTextDisplayField
//
void TextEdit::drawTextDisplayField() {

//    Serial.printf("TextEdit::drawTextDisplayField: \n");

    clearTextDisplayArea();

    DisplayScreenBase::drawRoundRect(textFieldX-1, textFieldY-1, textFieldWidth+2, textFieldHeight+2, 4, TEXTFIELD_OUTLINE_COLOR);
    DisplayScreenBase::drawRoundRect(textFieldX-2, textFieldY-2, textFieldWidth+4, textFieldHeight+4, 4, TEXTFIELD_OUTLINE_COLOR);

    DisplayScreenBase::setFont(TEXTFIELD_FONT);
    DisplayScreenBase::setTextColor(TEXTFIELD_FONT_COLOR, TEXTFIELD_COLOR);
}

// displayTextBuffer
//
void TextEdit::displayTextBuffer() {

//    Serial.printf("TextEdit::displayTextBuffer: %s\n", textBuffer);

    DisplayScreenBase::setFont(TEXTFIELD_FONT);
    DisplayScreenBase::setTextColor(TEXTFIELD_FONT_COLOR, TEXTFIELD_COLOR);
    DisplayScreenBase::printInArea(textBuffer, textFieldArea);
}

// clearTextBuffer
//
void TextEdit::clearTextBuffer() {

//    Serial.printf("TextEdit::clearTextBuffer: \n");

    textBuffer[0] = '\0';
    textLength = 0;
    clearTextDisplayArea();
}

// handleKbdBtn
//
void TextEdit::handleKbdBtn(char ch) {

//    Serial.printf("TextEdit::handleKbdBtn: %c\n", ch);

    addToTextBuffer(ch);
}

// handleCaseBtn
//
void TextEdit::handleCaseBtn(ButtonId btnIndex) {

//    Serial.printf("TextEdit::handleCaseBtn: %d (current %d)\n", btnIndex, currentCaseBtn);

    ButtonId btnId = casePanelBtnInfo[btnIndex].buttonId;

    if (btnId != currentCaseBtn) {

        currentCaseBtn = btnId;
        casePanel.drawPanel(casePanelBtnInfo, currentCaseBtn);

        switch (currentCaseBtn) {

            case BtnLower:
                keyPadBtnLabels = kbdLowerBtnInfo;
                break;

            case BtnUpper:
                keyPadBtnLabels = kbdUpperBtnInfo;
              break;
        }

        drawKeyPad();
    }
}

// handleEditBtn
//
void TextEdit::handleEditBtn(ButtonId btnIndex) {

//    Serial.printf("TextEdit::handleEditBtn: %d (current %d)\n", btnIndex, currentEditBtn);

    ButtonId btnId = editPanelBtnInfo[btnIndex].buttonId;

    switch (btnId) {
        case BtnBkSpc:
//            Serial.printf("BtnBkSpc\n");
            if (textLength > 0) {
                textLength--;
                textBuffer[textLength] = '\0';
//                Serial.printf("TextEdit::handleEditBtn textBuffer %s\n", textBuffer);
                clearTextDisplayArea();
                displayTextBuffer();
            }
            break;

        case BtnClear:
//            Serial.printf("BtnClear\n");
            clearTextBuffer();
            break;
    }

    if (btnId != currentEditBtn) {
        currentEditBtn = btnId;
        editPanel.drawPanel(editPanelBtnInfo, currentEditBtn);
    }
}

// handleExitBtn
//
ButtonId TextEdit::handleExitBtn(ButtonId btnIndex) {

//    Serial.printf("TextEdit::handleExitBtn: %d (current %d)\n", btnIndex, currentExitBtn);

    ButtonId returnVal = BtnNone;

    ButtonId btnId = exitPanelBtnInfo[btnIndex].buttonId;

    if (btnId != currentExitBtn) {
        currentExitBtn = btnId;
        exitPanel.drawPanel(exitPanelBtnInfo, currentExitBtn);
    }

    switch (btnId) {
        case BtnSave:
//            Serial.printf("BtnSave\n");
            returnVal = BtnSave;
            break;

        case BtnCancel:
//            Serial.printf("BtnCancel\n");
            returnVal = BtnCancel;
            break;
    }

    return returnVal;
}

// addToTextBuffer
//
void TextEdit::addToTextBuffer(char ch) {

//    Serial.printf("TextEdit::addToTextBuffer: ch %c textLength %d maxTextLength %d\n", ch, textLength, maxTextLength);

    if (textLength < maxTextLength) {
        textBuffer[textLength] = ch;
        textLength++;
        textBuffer[textLength] = '\0';
    }
    clearTextDisplayArea();
    displayTextBuffer();
}


