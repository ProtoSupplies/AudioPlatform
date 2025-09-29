/************************************************************************************
*                             SerialFlashUtilScreen.cpp                             *
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
#include <FS.h>
#include <SerialFlash.h>
#include <LittleFS.h>

// User Arduino Libraries
//
#include "SerialFlashUtilScreen.h"
#include "ArduinoEclipseDefs.h"
#include "UtilFns.h"

/************************************************************************************
*                    S T A T I C   C L A S S   V A R I A B L E S                    *
*************************************************************************************
*/

PROGMEM const char sfCopyPopupHeading[]     = "Copy RAW Files from SD to Serial Flash";
PROGMEM const char sfErasePopupHeading[]    = "Erase Serial Flash Contents";
PROGMEM const char showSdPopupHeading[]     = "List of RAW Audio Files on SD Card";
PROGMEM const char showSfPopupHeading[]     = "List of All Files in Serial Flash";
PROGMEM const char executeBtnViewText[]     = "View Contents";
PROGMEM const char executeBtnProceedText[]  = "Proceed";

ScreenButtonInfo SerialFlashUtilScreen::executeBtnInfo = (ScreenButtonInfo) {
     0,                             // btnX
     0,                             // btnY
     150,                           // btnWid
     40,                            // btnHgt
     SCREENBUTTON_COLOR,            // btnColor
     SCREENBUTTON_OUTLINE_COLOR,    // btnOutlineColor
     SCREENBUTTON_OUTLINE_RADIUS,   // btnOutlineRadius
     "",                            // btnLabel (set at runtime)
     SCREENBUTTON_TEXT_COLOR,       // btnLabelColor
     SCREENBUTTON_TEXT_FONT,        // btnLabelFont (note: must set again in constructor B241001-1)
     SCREENBUTTON_TEXT_ALIGN,       // btnLabelAlign
     SCREENBUTTON_TEXT_POSITION,    // btnLabelPosition
     0,                             // bitMapInfo
};

/************************************************************************************
*                    P U B L I C   C L A S S   F U N C T I O N S                    *
*************************************************************************************
*/

// Static Member Initializations
//

// Constructor/Destructor
//
SerialFlashUtilScreen::SerialFlashUtilScreen(DisplayScreenInfo& _displayInfo, ButtonPanelInfo& _cmndPanelInfo, const char** _cmndPanelBtnLabels, SystemHardwareProfile& _sysProfile) :
    DisplayScreenBase(_displayInfo),
    sysProfile(_sysProfile),
    executeBtn(executeBtnInfo),
    copyRawFiles(_sysProfile), eraseSerialFlash(_sysProfile),
    cmndPanel(_cmndPanelInfo), cmndPanelBtnLabels(_cmndPanelBtnLabels),
    selectedCmnd(SerialUtilViewFlash) {

    int16_t displayAreaX1       = marginLeft;
    int16_t displayAreaX2       = DISPLAY_WIDTH - marginRight;
    int16_t displayAreaWidth    = displayAreaX2 - displayAreaX1;
    int16_t cmndPanelX          = displayAreaX1 + ((displayAreaWidth - cmndPanel.panelWidth) / 2);
    int16_t cmndPanelY          = marginTop + 50;
    cmndPanel.setPanelPosition(cmndPanelX, cmndPanelY);
    cmndPanelActive             = true;

    executeBtnInfo.btnX         = (DISPLAY_WIDTH - executeBtnInfo.btnWid) / 2;
    executeBtnInfo.btnY         = cmndPanel.panelY + cmndPanel.panelHeight + 40;
    executeBtnActive            = false;

    // Workaround for B241001-1
    executeBtnInfo.btnLabelFont = SCREENBUTTON_TEXT_FONT;
}

SerialFlashUtilScreen::~SerialFlashUtilScreen() {

}

// activateScreen
//
void SerialFlashUtilScreen::activateScreen() {

//    Serial.printf("SerialFlashUtilScreen::activateScreen:\n");

    // Clear and display screen
    clearScreen();
    showScreenHeading();
    selectedCmnd = SerialUtilViewFlash;
    executeBtnInfo.btnLabel = executeBtnViewText;
    executeBtnActive = true;
    showScreen();

    listView.pollEnabled = false;
    copyRawFiles.pollEnabled = false;
    eraseSerialFlash.pollEnabled = false;
}

// deActivateScreen
//
void SerialFlashUtilScreen::deActivateScreen() {

}

// pollScreen
//
ButtonId SerialFlashUtilScreen::pollScreen() {

    ButtonId touchedBtn = BtnNone;
    uint16_t pointX, pointY;

    if (listView.pollEnabled) {
        touchedBtn = listView.pollPopup();
        if (touchedBtn != BtnNone) {
            listView.pollEnabled = false;
            executeBtn.removeButton();
            if (touchedBtn == BtnExit) {
                this->activateScreen();
            }
        }

    } else if (copyRawFiles.pollEnabled) {
        touchedBtn = copyRawFiles.pollPopup();
        if (touchedBtn != BtnNone) {
            copyRawFiles.pollEnabled = false;
            if (touchedBtn == BtnExit || touchedBtn == BtnCancel) {
                this->activateScreen();
            }
        }

    } else if (eraseSerialFlash.pollEnabled) {
        touchedBtn = eraseSerialFlash.pollPopup();
        if (touchedBtn != BtnNone) {
            eraseSerialFlash.pollEnabled = false;
            if (touchedBtn == BtnExit || touchedBtn == BtnCancel) {
                this->activateScreen();
            }
        }

    } else if (DisplayScreenBase::touched()) {
        DisplayScreenBase::getTouchedPoint(pointX, pointY);

        // First check for Home/Back button. If pressed, we'll return that value to main application.
        if (DisplayScreenBase::prevBtnTouched(pointX, pointY)) {
            touchedBtn = BtnPrev;

        } else if (executeBtnActive && executeBtn.buttonTouched(pointX, pointY)) {
            handleExecuteBtnPress();

//        } else if (execPanelActive && execPanel.isPanelButton(pointX, pointY, touchedBtn)) {
//            execPanel.setSelected(cmndPanelBtnLabels, touchedBtn);
//            handleExecuteBtnPress(touchedBtn);
//            touchedBtn = BtnNone;
//
        } else if (cmndPanelActive && cmndPanel.isPanelButton(pointX, pointY, touchedBtn)) {
            selectedCmnd = (SerialUtilCmnd)touchedBtn;
            cmndPanel.setSelected(cmndPanelBtnLabels, selectedCmnd);
            handleCmndBtnPress(selectedCmnd);
            touchedBtn = BtnNone;
        }
    }

    return touchedBtn;
}

/************************************************************************************
*                   P R I V A T E   C L A S S   F U N C T I O N S                   *
*************************************************************************************
*/

// printSdFileList
//
// Print a list of raw files on SD card
//
void SerialFlashUtilScreen::printSdFileList() {

    uint16_t lineCount = 0;

    if (sysProfile.sdCardPresent) {
        lineCount = Teensy41_Util::printSdDirFileInfoToBigBuf(".raw");
    }

//    Serial.printf("SerialFlashUtilScreen::printSdFileList: lineCount %d\n", lineCount);
    largeTempBufLineCount = lineCount;
}

// printSerialFlashFileList
//
// Print a list of files in the Serial Flash chip
//
void SerialFlashUtilScreen::printSerialFlashFileList() {

    uint16_t lineCount = 0;

    if (sysProfile.serialFlashPresent) {
        lineCount = Teensy41_Util::printFlashDirFileInfoToBigBuf("");
    }

    largeTempBufLineCount = lineCount;
}

// handleCmndBtnPress
//
void SerialFlashUtilScreen::handleCmndBtnPress(SerialUtilCmnd cmnd) {

//    Serial.printf("SerialFlashUtilScreen::handleCmndBtnPress: btn %d\n", cmnd);

    switch (cmnd) {

        case SerialUtilViewFlash:
        case SerialUtilViewSd:
            executeBtnInfo.btnLabel = executeBtnViewText;
            executeBtn.drawButton();
            executeBtnActive = true;
            break;

        case SerialUtilEraseSerialFlash:
        case SerialUtilCopyFromSd:
            executeBtnInfo.btnLabel = executeBtnProceedText;
            executeBtn.drawButton();
            executeBtnActive = true;
            break;

        default:
            break;
    }

}

// handleExecuteBtnPress
//
void SerialFlashUtilScreen::handleExecuteBtnPress() {

//    Serial.printf("SerialFlashUtilScreen::handleViewBtnPress:\n");

    switch (selectedCmnd) {

        case SerialUtilViewFlash:
            showFlash();
            listView.setTextAlign(TextAlignLeft);
            listView.showPopup();
            listView.pollEnabled = true;
            break;

        case SerialUtilViewSd:
            showSd();
            listView.setTextAlign(TextAlignLeft);
            listView.showPopup();
            listView.pollEnabled = true;
            break;

        case SerialUtilEraseSerialFlash:
            showEraseFlash();
            eraseSerialFlash.pollEnabled = true;
            break;

        case SerialUtilCopyFromSd:
            showCopyFromSd();
            copyRawFiles.pollEnabled = true;
            break;

        default:
            break;
    }

    executeBtnActive = false;
}

// showScreen
//
void SerialFlashUtilScreen::showScreen() {

//    Serial.printf("SerialFlashUtilScreen::showScreen: \n");

    cmndPanel.drawPanel(cmndPanelBtnLabels);
    cmndPanel.setSelected(cmndPanelBtnLabels, selectedCmnd);
    executeBtn.drawButton();
}

// showSd
//
void SerialFlashUtilScreen::showSd() {

//    Serial.printf("SerialFlashUtilScreen::showSd: \n");

    printSdFileList();

    listView.setList(sysProfile.largeTempBufLines, largeTempBufLineCount);
    listView.setPopupHeading(showSdPopupHeading);

//    Serial.printf("SerialFlashUtilScreen::showSd: largeTempBufLineCount %d\n", largeTempBufLineCount);

    uint16_t dispWidth = DirFullInfoWidth;
    listView.setListDisplayWidth(dispWidth);
    listView.setListDisplayX((DISPLAY_WIDTH - dispWidth) / 2);
}

// showFlash
//
void SerialFlashUtilScreen::showFlash() {

    printSerialFlashFileList();

    listView.setList(sysProfile.largeTempBufLines, largeTempBufLineCount);
    listView.setPopupHeading(showSfPopupHeading);

    uint16_t dispWidth = DirPartiaInfoWidth;
    listView.setListDisplayWidth(dispWidth);
    listView.setListDisplayX((DISPLAY_WIDTH - dispWidth) / 2);
}

// showCopyFromSd
//
void SerialFlashUtilScreen::showCopyFromSd() {

//    Serial.printf("SerialFlashUtilScreen::showCopyFromSd:\n");

    copyRawFiles.setPopupHeading(sfCopyPopupHeading);
    copyRawFiles.showPopup();

}

// showEraseFlash
//
void SerialFlashUtilScreen::showEraseFlash() {

//    Serial.printf("SerialFlashUtilScreen::showEraseFlash:\n");

    eraseSerialFlash.setPopupHeading(sfErasePopupHeading);
    eraseSerialFlash.showPopup();
}


/************************************************************************************
*                 P R O T E C T E D   C L A S S   F U N C T I O N S                 *
*************************************************************************************
*/


