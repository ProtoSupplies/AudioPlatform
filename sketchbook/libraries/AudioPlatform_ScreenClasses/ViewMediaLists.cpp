/************************************************************************************
*                                 ViewMediaLists.cpp                                *
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
#include "ViewMediaLists.h"
#include "ArduinoEclipseDefs.h"
#include "UtilFns.h"
#include "BitMapData.h"

/************************************************************************************
*                         M A C R O S  /  C O N S T A N T S                         *
*************************************************************************************
*/

/************************************************************************************
*                    S T A T I C   C L A S S   V A R I A B L E S                    *
*************************************************************************************
*/

// View List Button
//
ScreenButtonInfo ViewMediaLists::viewListBtnInfo = (ScreenButtonInfo) {
     0,                             // btnX
     0,                             // btnY
     150,                           // btnWid
     40,                            // btnHgt
     SCREENBUTTON_COLOR,            // btnColor
     SCREENBUTTON_OUTLINE_COLOR,    // btnOutlineColor
     SCREENBUTTON_OUTLINE_RADIUS,   // btnOutlineRadius
     "View Contents",               // btnLabel
     SCREENBUTTON_TEXT_COLOR,       // btnLabelColor
     SCREENBUTTON_TEXT_FONT,        // btnLabelFont (note: must set again in constructor B241001-1)
     SCREENBUTTON_TEXT_ALIGN,       // btnLabelAlign
     SCREENBUTTON_TEXT_POSITION,    // btnLabelPosition
     0,                             // bitMapInfo
};

// TextColumn for List Display
//
TextColumnInfo ViewMediaLists::listViewColumnInfo = (TextColumnInfo)  {
    LISTVIEW_XPOS,                  // xPos
    LISTVIEW_YPOS,                  // yPos
    LISTVIEW_COLUMN_WIDTH,          // width
    380,                            // height
    LISTVIEW_LINE_HEIGHT,           // lineHeight
    15,                             // maxLines
    LISTVIEW_FONT,                  // font
    LISTVIEW_FONT_COLOR,            // textColor
    LISTVIEW_TEXT_MARGIN,           // textMargin
    LISTVIEW_TEXT_ALIGN,            // textAlign
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
ViewMediaLists::ViewMediaLists(DisplayScreenInfo& _displayInfo, ButtonPanelInfo& _viewPanelInfo, const char** _viewPanelBtnLabels, SystemHardwareProfile& _sysProfile) :
    DisplayScreenBase(_displayInfo),
    largeTempBufLineCount(0),
    sysProfile(_sysProfile),
    listView(),
    viewListBtn(viewListBtnInfo),
    viewPanel(_viewPanelInfo), viewPanelBtnLabels(_viewPanelBtnLabels),
    selectedView(ViewSd) {

    // Place presets panel below top margin, centered between left and right margins
    int16_t displayAreaX1       = marginLeft;
    int16_t displayAreaX2       = DISPLAY_WIDTH - marginRight;
    int16_t displayAreaWidth    = displayAreaX2 - displayAreaX1;
    int16_t viewPanelX          = displayAreaX1 + ((displayAreaWidth - viewPanel.panelWidth) / 2);
    int16_t viewPanelY          = marginTop + 50;
    viewPanel.setPanelPosition(viewPanelX, viewPanelY);

    listView.setTextColumnInfo(&listViewColumnInfo);
    viewListBtnInfo.btnX        = (DISPLAY_WIDTH - viewListBtnInfo.btnWid) / 2;
    viewListBtnInfo.btnY        = viewPanel.panelY + viewPanel.panelHeight + 40;

    // Workaround for B241001-1
    viewListBtnInfo.btnLabelFont = SCREENBUTTON_TEXT_FONT;
}

ViewMediaLists::~ViewMediaLists() {

}


// activateScreen
//
void ViewMediaLists::activateScreen() {

//    Serial.printf("ViewMediaLists::activateScreen:\n");

    // Clear and display screen
    clearScreen();
    showScreenHeading();
    showScreen();

//    listView.setTextColumnInfo(&listViewColumnInfo);
    listView.pollEnabled = false;
}

// deActivateScreen
//
void ViewMediaLists::deActivateScreen() {

}

// pollScreen
//
ButtonId ViewMediaLists::pollScreen() {

    ButtonId touchedBtn = BtnNone;
    uint16_t pointX, pointY;

    if (listView.pollEnabled) {
        touchedBtn = listView.pollPopup();
        if (touchedBtn != BtnNone) {
//            Serial.printf("ViewMediaLists::pollScreen: pollPopup returns %d\n", touchedBtn);
            listView.pollEnabled = false;
            if (touchedBtn == BtnExit) {
                this->activateScreen();
            }
        }

    } else if (DisplayScreenBase::touched()) {
            DisplayScreenBase::getTouchedPoint(pointX, pointY);

            if (DisplayScreenBase::prevBtnTouched(pointX, pointY)) {
                touchedBtn = BtnPrev;

            } else if (viewListBtn.buttonTouched(pointX, pointY)) {
                handleViewBtnPress();
                listView.showPopup();
                listView.pollEnabled = true;

            } else if (viewPanel.isPanelButton(pointX, pointY, touchedBtn)) {
//                Serial.printf("ViewMediaLists::pollScreen: viewPanel BtnTouched btnId %d\n", touchedBtn);
                selectedView = (ViewListId)touchedBtn;
                viewPanel.setSelected(viewPanelBtnLabels, selectedView);
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
// Print a list of files on SD card
//
void ViewMediaLists::printSdFileList() {

    uint16_t lineCount = 0;

    if (sysProfile.sdCardPresent) {
        lineCount = Teensy41_Util::printSdDirFileInfoToBigBuf("");
    }

    largeTempBufLineCount = lineCount;
}

// printSerialFlashFileList
//
// Print a list of files in the Serial Flash chip
//
void ViewMediaLists::printSerialFlashFileList() {

    uint16_t lineCount = 0;

    if (sysProfile.serialFlashPresent) {
        lineCount = Teensy41_Util::printFlashDirFileInfoToBigBuf("");
    }

    largeTempBufLineCount = lineCount;
}

// printProgMemFileList
//
// Print a list of files in PROGMEM reserved storage
//
void ViewMediaLists::printProgMemFileList() {

    uint16_t lineCount = 0;

    if (sysProfile.progmemFsPresent) {
        lineCount = Teensy41_Util::printDirFileInfoToBigBuf(sysProfile.progmemFs.open("/"), "");
    }

    largeTempBufLineCount = lineCount;
}

// printPresetBanksFileList
//
// Print a list of preset bank files on the SD
//
void ViewMediaLists::printPresetBanksFileList() {

    uint16_t lineCount = 0;

//    Serial.printf("ViewMediaLists::printPresetBanksFileList: compare sysProfile string with local %d\n", strcmp(".data", PresetBankFileSuffix));

    if (sysProfile.sdCardPresent) {
        lineCount = Teensy41_Util::printSdDirFileInfoToBigBuf(PresetBankFileSuffix);
    }

    largeTempBufLineCount = lineCount;
}

// printFmSynthBanksFileList
//
// Print a list of Fm Synth bank files on the SD
//
void ViewMediaLists::printFmSynthBanksFileList() {

    uint16_t lineCount = 0;

    if (sysProfile.sdCardPresent) {
        lineCount = Teensy41_Util::printSdDirFileInfoToBigBuf(FmSynthBankFileSuffix);
    }

    largeTempBufLineCount = lineCount;
}

// printMediaPlayerFileList
//
// Print a list of WAV (on SD) and RAW (in SerialFlash chip) files
//
// TODO: These lists are generated and stored in SystemHardwareProfile at startup, and need to be
// updated as files are added and/or removed by various application utilities (see M241001-1).
//
void ViewMediaLists::printMediaPlayerFileList() {

    char lineBuf[MaxSoundFileNameLen+1];
    uint16_t lineCount = 0;

    Teensy41_Util::printlnToBigBuf("Serial Flash Raw Files", lineCount);
    if (sysProfile.sfRawFileCount > 0) {
        for (int i = 0; i < MaxSoundFileNames; i++) {
//        for (int i = 0; i < sysProfile.sfRawFileCount; i++) {
            if (sysProfile.sfRawFileNames[i]) {
                snprintf(lineBuf, MaxSoundFileNameLen, "%s", sysProfile.sfRawFileNames[i]);
                Teensy41_Util::printlnToBigBuf(lineBuf, lineCount);
            }
        }
        snprintf(lineBuf, MaxSoundFileNameLen, "%3d RAW Files Found", sysProfile.sfRawFileCount);
        Teensy41_Util::printlnToBigBuf(lineBuf, lineCount);

    } else {
        Teensy41_Util::printlnToBigBuf("No Raw Files found", lineCount);
    }

    Teensy41_Util::printlnToBigBuf("", lineCount);
    Teensy41_Util::printlnToBigBuf("SD Card Wav Files", lineCount);
    if (sysProfile.sdWavFileCount > 0) {
        for (int i = 0; i < MaxSoundFileNames; i++) {
//        for (int i = 0; i < sysProfile.sdWavFileCount; i++) {
//            Serial.printf("sysProfile.sdWavFileNames[%d] %s\n", i, sysProfile.sdWavFileNames[i]);
            if (sysProfile.sdWavFileNames[i]) {
                snprintf(lineBuf, MaxSoundFileNameLen, "%s", sysProfile.sdWavFileNames[i]);
                Teensy41_Util::printlnToBigBuf(lineBuf, lineCount);
            }
        }
        snprintf(lineBuf, MaxSoundFileNameLen, "%3d WAV Files Found", sysProfile.sdWavFileCount);
        Teensy41_Util::printlnToBigBuf(lineBuf, lineCount);

    } else {
        Teensy41_Util::printlnToBigBuf("No WAV Files found", lineCount);
    }

    largeTempBufLineCount = lineCount;
}

// handleViewBtnPress
//
void ViewMediaLists::handleViewBtnPress() {

    // Set Defaults
    listViewColumnInfo.textAlign = TextAlignLeft;
//    listView.setTextAlign(TextAlignLeft);

    switch (selectedView) {

        case ViewSd:
            showSd();
            break;

        case ViewFlash:
            showFlash();
            break;

        case ViewProgMem:
            showProgMem();
            break;

        case ViewPresetBanks:
            showPresetBanks();
            break;

        case ViewFmSynthBanks:
            showFmSynthBanks();
            break;

        case ViewMediaPlayerFiles:
        default:
            showMediaPlayerFiles();
            break;
    }
}

// showScreen
//
void ViewMediaLists::showScreen() {

//    Serial.printf("ViewMediaLists::showScreen: \n");

    viewPanel.drawPanel(viewPanelBtnLabels);
    viewPanel.setSelected(viewPanelBtnLabels, selectedView);
    viewListBtn.drawButton();
}

// showSd
//
void ViewMediaLists::showSd() {

//    Serial.printf("ViewMediaLists::showSd: \n");

    printSdFileList();

//    listView.showTextColumnInfo();

    listView.setList(sysProfile.largeTempBufLines, largeTempBufLineCount);
    listView.setPopupHeading("List of All Files on SD Card (unsorted)");

//    Serial.printf("ViewMediaLists::showSd: largeTempBufLineCount %d\n", largeTempBufLineCount);

    listViewColumnInfo.width = DirFullInfoWidth;
    listViewColumnInfo.xPos = (DISPLAY_WIDTH - listViewColumnInfo.width) / 2;
}

// showFlash
//
void ViewMediaLists::showFlash() {

    printSerialFlashFileList();

    listView.setList(sysProfile.largeTempBufLines, largeTempBufLineCount);
    listView.setPopupHeading("List of All Files in Serial Flash (unsorted)");

    listViewColumnInfo.width = DirPartiaInfoWidth;
    listViewColumnInfo.xPos = (DISPLAY_WIDTH - listViewColumnInfo.width) / 2;
}

// showProgMem
//
void ViewMediaLists::showProgMem() {

    printProgMemFileList();

    listView.setList(sysProfile.largeTempBufLines, largeTempBufLineCount);
    listView.setPopupHeading("List of Files in PROGMEM Reserved Storage (unsorted)");

    listViewColumnInfo.width = DirFullInfoWidth;
    listViewColumnInfo.xPos = (DISPLAY_WIDTH - listViewColumnInfo.width) / 2;
}

// showPresetBanks
//
void ViewMediaLists::showPresetBanks() {

    printPresetBanksFileList();

    listView.setList(sysProfile.largeTempBufLines, largeTempBufLineCount);
    listView.setPopupHeading("List of Preset Banks Files on SD Card (unsorted)");

    listViewColumnInfo.width = DirFullInfoWidth;
    listViewColumnInfo.xPos = (DISPLAY_WIDTH - listViewColumnInfo.width) / 2;
}

// showFmSynthBanks
//
void ViewMediaLists::showFmSynthBanks() {

    printFmSynthBanksFileList();

    listView.setList(sysProfile.largeTempBufLines, largeTempBufLineCount);
    listView.setPopupHeading("List of FM Synth Banks Files on SD Card (unsorted)");

    listViewColumnInfo.width = DirFullInfoWidth;
    listViewColumnInfo.xPos = (DISPLAY_WIDTH - listViewColumnInfo.width) / 2;
}

// showMediaPlayerFiles
//
void ViewMediaLists::showMediaPlayerFiles() {

    printMediaPlayerFileList();

    listView.setList(sysProfile.largeTempBufLines, largeTempBufLineCount);
    listView.setPopupHeading("List of Media Player Files (Sorted)");

    listViewColumnInfo.width = DirShortInfoWidth;
    listViewColumnInfo.xPos = (DISPLAY_WIDTH - listViewColumnInfo.width) / 2;
}

/************************************************************************************
*                 P R O T E C T E D   C L A S S   F U N C T I O N S                 *
*************************************************************************************
*/

