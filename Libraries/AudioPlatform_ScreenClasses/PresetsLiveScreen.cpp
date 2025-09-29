/************************************************************************************
*                               PresetsLiveScreen.cpp                               *
*                                                                                   *
*                     Copyright (c) 2025, Donald Swearingen                         *
*                                All Rights Reserved                                *
*************************************************************************************
*
* Description
*
*   PresetsLiveScreen is a custom AudioPlatform screen derived from the generic base class
*   DisplayScreenBase.
*
* Single Instance Class
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

// User Arduino Libraries
//
#include "PresetsLiveScreen.h"
#include "ArduinoEclipseDefs.h"
#include "BitMap.h"
#include "BitMapData.h"
#include "UtilFns.h"
#include "Teensy41_Util.h"

// Teensyduino Libraries
//
#include <Audio.h>

/************************************************************************************
*                         M A C R O S  /  C O N S T A N T S                         *
*************************************************************************************
*/

/************************************************************************************
*                    S T A T I C   C L A S S   V A R I A B L E S                    *
*************************************************************************************
*/
static ScreenButtonInfo _defaultBankButtonInfo = {
    400,                            // btnX
    80,                             // btnY
    295,                            // btnWid
    28,                             // btnHgt
    SCREENBUTTON_COLOR,             // btnColor
    SCREENBUTTON_COLOR,             // btnOutlineColor
    3,                              // btnOutlineRadius
    DefaultPresetsBankFilename,     // btnLabel
    SCREENBUTTON_OFF_TEXT_COLOR,    // btnLabelColor
    LiberationMono_12_Bold,         // btnLabelFont (note: must set again in constructor B241001-1)
    SCREENBUTTON_TEXT_ALIGN,        // btnLabelAlign
    SCREENBUTTON_TEXT_POSITION,     // btnLabelPosition
    0                               // bitMapInfo
    };

EXTMEM static char presetLabels[PresetDeviceCount][PresetNameSize];

ScreenButtonInfo PresetsLiveScreen::allNotesOffBtnInfo = (ScreenButtonInfo) {
     296,                           // btnX
     23,                            // btnY
     36,                            // btnWid
     36,                            // btnHgt
     COLOR_BLACK,                   // btnColor
     SCREENBUTTON_OUTLINE_COLOR,    // btnOutlineColor
     0,                             // btnOutlineRadius
     "All Notes Off",               // btnLabel (set at runtime)
     SCREENBUTTON_TEXT_COLOR,       // btnLabelColor
     SCREENBUTTON_TEXT_FONT,        // btnLabelFont (note: must set again in constructor B241001-1)
     TextAlignLeft,                 // btnLabelAlign
     TextRight,                     // btnLabelPosition
     AllNotesOffIcon_35x35          // bitMapInfo
};

ScreenButtonInfo PresetsLiveScreen::ignoreNoteOffBtnInfo = (ScreenButtonInfo) {
     470,                           // btnX
     23,                            // btnY
     36,                            // btnWid
     36,                            // btnHgt
     COLOR_BLACK,                   // btnColor
     SCREENBUTTON_OUTLINE_COLOR,    // btnOutlineColor
     0,                             // btnOutlineRadius
     "Ignore Note Off",             // btnLabel (set at runtime)
     SCREENBUTTON_TEXT_COLOR,       // btnLabelColor
     SCREENBUTTON_TEXT_FONT,        // btnLabelFont (note: must set again in constructor B241001-1)
     TextAlignLeft,                 // btnLabelAlign
     TextRight,                     // btnLabelPosition
     IgnoreNoteOffIcon_35x35        // bitMapInfo
};

TextColumnInfo PresetsLiveScreen::bankFileListDisplay = (TextColumnInfo)  {
    LISTVIEW_XPOS,                  // xPos
    90,                             // yPos
    295,                            // width
    280,                            // height
    27,                             // lineHeight
    10,                             // maxLines
    LiberationMono_12_Bold,         // font
    LISTVIEW_FONT_COLOR,            // textColor
    LISTVIEW_TEXT_MARGIN,           // textMargin
    LISTVIEW_TEXT_ALIGN,            // textAlign
    LISTVIEW_BGND_COLOR,            // columnColor
    LISTVIEW_OUTLINE_COLOR,         // outlineColor
    LISTVIEW_SEL_OUTLINE_COLOR      // selectedOutlineColor
    };

uint16_t PresetsLiveScreen::WaitTimeAfterKeyTouch = 100;   // ms

/************************************************************************************
*                    P U B L I C   C L A S S   F U N C T I O N S                    *
*************************************************************************************
*/
// Static Member Initializations
//

// Constructor/Destructor
//
PresetsLiveScreen::PresetsLiveScreen(DisplayScreenInfo& displayInfo, ButtonPanelInfo& presetsPanelInfo,
    const char* (*_presetsCallbackFn)(CommandButtonId cmdId, uint16_t& numberParam, void* voidParam),
    SystemHardwareProfile& _sysProfile) :

        DisplayScreenBase(displayInfo),
        presetsPanel(presetsPanelInfo),
        allNotesOffBtn(allNotesOffBtnInfo), ignoreNoteOffBtn(ignoreNoteOffBtnInfo),
        presetsCallbackFn(_presetsCallbackFn), ignoredCallbackArg(0),
        defaultBankButton(_defaultBankButtonInfo),
        bankFileSelect(),
        sysProfile(_sysProfile),
        statusDisplay() {

    lastPresetSelectMillis = 0;
    selectedPreset = 0;
    strcpy(bankFileName, DefaultPresetsBankFilename);
    selectedBank = BtnNone;
    sdBankCount = 0;

    // Place presets panel
    presetsPanel.setPanelPosition(marginLeft, marginTop);

    // Must set columninfo here since pointer argument is not yet
    // available/set if done so in constructor arguments above
    bankFileSelect.setTextColumnInfo(&bankFileListDisplay);

    // List select state flags
    //
    // On PresetsLiveScreen, we don't want to clear screen because list will be
    // displayed on main screen of PresetsLiveScreen alongside existing screen objects.
    //
    // Setting "reportOnTouch" true prevents normal Select/Cancel/etc. buttons
    // from being displayed in favor of immediate reporting of item touched in list.
    //
    bankFileSelect.clearScreen = false;
    bankFileSelect.reportOnTouch = true;

    // Workaround for B241001-1
    ignoreNoteOffBtnInfo.btnLabelFont = Arial_9_Bold;
    allNotesOffBtnInfo.btnLabelFont = Arial_9_Bold;
}

PresetsLiveScreen::~PresetsLiveScreen() {

}

// activateScreen
//
// Reset/display dynamic screen elements each time screen is displayed
//
void PresetsLiveScreen::activateScreen() {

//    Serial.printf("PresetsLiveScreen::activateScreen:\n");

    // Clear and display basic screen
    clearScreen();
    presetsPanel.clearPanel();
    showScreenHeading();        // Must come _after_ clear panels

    // Get current preset names from application and draw presets panel
    updatePresetNames();

    // (Re)Select current preset
    setSelectedPreset(selectedPreset);

    // Display status bar (preset, player states) at bottom of screen
    statusDisplay.initialize();

    // Position bank file list panel
    bankFileListDisplay.xPos = DISPLAY_WIDTH - (bankFileListDisplay.width + 82);
    bankFileListDisplay.yPos = marginTop + _defaultBankButtonInfo.btnHgt + 7;
    uint16_t panelHeight = presetsPanel.btnHeight * (presetsPanel.btnCount / presetsPanel.btnsPerRow) + presetsPanel.btnGapVertical * (presetsPanel.btnRowCount - 1);
    bankFileListDisplay.height = panelHeight - _defaultBankButtonInfo.btnHgt - 8;
    bankFileListDisplay.textAlign = TextAlignCenter;

    // Get current preset bank filename
    // (May have been changed in other screens)
    uint16_t activeBank = (uint16_t) BtnActiveBank;
    const char* fileName = (*presetsCallbackFn)(CmdGetBankFilename, activeBank, 0);
//    strcpy(bankFileName, fileName);
    strncpy(bankFileName, fileName, MaxPresetBankFileNameLen-1);
    showBankName();
    showSdBanksLabel();

    // Get a list of .pbnk files on SD card in Teensy41_Util array largeTempBufLines
    getSdBankList();

    // Display the SD BankList
//    Serial.printf("PresetsLiveScreen::activateScreen: selectedBank %d\n", selectedBank);
    bankFileSelect.showPopup();
    if (selectedBank != BtnNone) {
        bankFileSelect.setSelectedLine(selectedBank);
    }

    // Display Default Bank Button Above SD BankList
    // Highlight button if default bank is selected
    showDefaultBankButton(selectedBank == BtnNone);

    // Enable polling of list, and report selection when touched
    bankFileSelect.pollEnabled = true;
    bankFileSelect.reportOnTouch = true;

    // Display/Intialize Preset Save and Settings Import buttons
    allNotesOffBtn.drawButton();
    ignoreNoteOffBtn.drawButton();
    allOffFlashing = false;
}

// deActivateScreen
//
void PresetsLiveScreen::deActivateScreen() {

    ignoreNoteOffBtn.setState(ScrnBtnOff);
    uint16_t btnState = 0;
    (*presetsCallbackFn)(CmdIgnoreNoteOff, btnState, 0);
}

// pollScreen
//
ButtonId PresetsLiveScreen::pollScreen() {

    ButtonId touchedBtn = BtnNone;
    uint16_t pointX, pointY;

    // Reset allNotesOff button when "flash" time expires
    if (allOffFlashing && (allOffFlashTime > allOffFlashInterval)) {
        allOffFlashing = false;
        allNotesOffBtn.setState(ScrnBtnOff);
    }

    // Check for touched buttons
    if (DisplayScreenBase::touched(WaitTimeAfterKeyTouch)) {
        DisplayScreenBase::getTouchedPoint(pointX, pointY);

        // Select preset bank file
        if (bankFileSelect.pollEnabled) {
            ButtonId touchedBtn = bankFileSelect.pollPopup(pointX, pointY);
            if (touchedBtn != BtnNone) {
                selectedBank = bankFileSelect.getSelectedLine();
//                Serial.printf("PresetsLiveScreen::pollScreen: selectedBank %d\n", selectedBank);
                strncpy(bankFileName, sysProfile.largeTempBufLines[selectedBank - 1], PresetFileNameSize-1);
                bankFileName[PresetFileNameSize-1] ='\0';
                uint16_t selectActive = BtnSelectActive;
                (*presetsCallbackFn)(CmdLoadSdBank, selectActive, bankFileName);
                updatePresetNames();
                showBankName();

                showDefaultBankButton(false);
            }
        }

        // Exit Screen
        if (DisplayScreenBase::prevBtnTouched(pointX, pointY)) {
            touchedBtn = BtnPrev;

        // Select preset
        } else if (presetsPanel.isPanelButton(pointX, pointY, touchedBtn)) {
            handlePresetBtnPress(touchedBtn);

        // Select default preset bank
        } else if (defaultBankButton.buttonTouched(pointX, pointY)) {
            strncpy(bankFileName, DefaultPresetsBankFilename, PresetFileNameSize-1);
            bankFileName[PresetFileNameSize-1] ='\0';
            uint16_t dummyBtn = BtnNone;
            (*presetsCallbackFn)(CmdLoadDefaultBank, dummyBtn, 0);
            updatePresetNames();
            showBankName();

            showDefaultBankButton(true);
            bankFileSelect.unsetSelectedLine();

        // Handle all notes off command
        } else if (allNotesOffBtn.buttonTouched(pointX, pointY)) {
            if (!allOffFlashing) {
                // Start button "flash" sequence
                allOffFlashing = true;
                allOffFlashTime = 0;
                allNotesOffBtn.setState(ScrnBtnFlash);
            }
            uint16_t dummyBtn = BtnNone;
            (*presetsCallbackFn)(CmdAllNotesOff, dummyBtn, 0);

        // Handle (toggle) ignore note off setting
        } else if (ignoreNoteOffBtn.buttonTouched(pointX, pointY)) {
            uint16_t btnState = (uint16_t )ignoreNoteOffBtn.toggleState();
            (*presetsCallbackFn)(CmdIgnoreNoteOff, btnState, 0);
        }
    }

    return touchedBtn;
}

// setBtnLabel
//
// malloc()-free version fixes memory leak bug B241217-1
//
void PresetsLiveScreen::setBtnLabel(ButtonId btnNum, char* btnLabel, bool reDraw) {

//    Serial.printf("PresetsLiveScreen::setBtnLabel: %d, %s %d\n", btnNum, btnLabel, reDraw);
//    Serial.printf("PresetsLiveScreen::setBtnLabel: len %d\n", strlen(btnLabel));

    // Clear button label
    memset(presetLabels[btnNum], 0, PresetNameSize);

    // Check whether to abbreviate name
    if (strlen(btnLabel) <= MaxPresetBtnNameLen) {
        // Just copy label to labels array
        strncpy(presetLabels[btnNum], btnLabel, PresetNameSize-1);
        presetLabels[btnNum][PresetNameSize-1] = '\0';
        presetsPanelBtnLabels[btnNum] = presetLabels[btnNum];

    } else {
        // Abbreviate Name
        // Use first 8 letters + ellipsis as button name.
        strncpy(presetLabels[btnNum], btnLabel, 8);
        strcat(presetLabels[btnNum], "...");
        presetsPanelBtnLabels[btnNum] = presetLabels[btnNum];
    }

    // Redraw button if indicated
    if (reDraw) {
        presetsPanel.drawButton(btnNum, btnNum == selectedPreset);
        presetsPanel.showButtonLabel(btnNum, btnLabel, btnNum == selectedPreset);
    }
}

// getPresetNum
//
uint16_t PresetsLiveScreen::getPresetNum() {

    return selectedPreset;
}

// setSelectedPreset
//
// Set/change selected button
//
void PresetsLiveScreen::setSelectedPreset(ButtonId btnNum) {

//    Serial.printf("PresetsLiveScreen::setSelectedPreset: current selected %d, new selected %d\n", selectedPreset, btnNum);

    if (selectedPreset != BtnNone) {
        presetsPanel.drawButton(selectedPreset, false);
        presetsPanel.showButtonLabel(selectedPreset, getPresetBtnLabel(selectedPreset), false);
        showPresetBtnNumber(selectedPreset);
    }

    // Make touchedBtn active and redisplay
    selectedPreset = btnNum;
    presetsPanel.drawButton(selectedPreset, true);
    presetsPanel.showButtonLabel(selectedPreset, (*presetsCallbackFn)(CmdGetPresetName, selectedPreset, 0), true);
    showPresetBtnNumber(selectedPreset);

}

/************************************************************************************
*                   P R I V A T E   C L A S S   F U N C T I O N S                   *
*************************************************************************************
*/

// getSdBankList
//
// Print a list of .pbnk files on SD card
//
void PresetsLiveScreen::getSdBankList() {

    sdBankCount = 0;

    if (sysProfile.sdCardPresent) {
        sdBankCount = Teensy41_Util::printSdDirFileListToBigBuf(".pbnk");
    }

//    Serial.printf("PresetsLiveScreen::getSdFileList: %d SD Files\n", lineCount);

    bankFileSelect.setList(sysProfile.largeTempBufLines, sdBankCount);

    // Find bankFileName in bank file list
    // FFS: Introduced as a fix for M250203-1 (v0.5.1.1, 3/23/25).
    //      A bit kludgey. Review in future if a more comprehensive
    //      approach to bank name management and coordination between
    //      PresetsScreen and PresetsLiveScreen is implemented.
    //
    selectedBank = BtnNone;
    for (uint16_t i = 0; i < sdBankCount; i++) {
        if (strncmp(bankFileName, sysProfile.largeTempBufLines[i], MaxPresetBankFileNameLen-1) == 0) {
            selectedBank = (ButtonId) (i + 1);
            break;
        }
    }
//    Serial.printf("PresetsLiveScreen::getSdBankList: selectedBank %d\n", selectedBank);

}

// showPresetName
//
void PresetsLiveScreen::showPresetName(bool importName) {

    uint16_t presetNameDispHgt  = 35;
    uint16_t presetNameDispWid  = 150;
    uint16_t presetNameDispX    = 100;
    uint16_t presetNameDispY    = presetNameDispHgt - 30;

    // Clear message display area
    DisplayScreenBase::fillRect(presetNameDispX, presetNameDispY, presetNameDispWid, presetNameDispHgt, displayBgdColor);

    // Message position above presetsCmndPanel
    TextArea tArea;
    tArea.posX  = presetNameDispX;
    tArea.posY  = presetNameDispY;
    tArea.wid   = presetNameDispWid;
    tArea.hgt   = presetNameDispHgt;
    tArea.align = TextAlignCenter;

    const char* presetName;
    if (importName) {
        presetName = (*presetsCallbackFn)(CmdGetImportPresetName, selectedPreset, 0);
    } else {
        presetName = (*presetsCallbackFn)(CmdGetPresetName, selectedPreset, 0);
    }
    snprintf(stringDispBuf, sizeof(stringDispBuf)-1, "Preset: %s", presetName);
    DisplayScreenBase::setFont((ILI9341_t3_font_t)Arial_12_Bold);
    DisplayScreenBase::setTextColor(COLOR_WHITE);
    DisplayScreenBase::printInArea(stringDispBuf, tArea);
}

// showBankName
//
void PresetsLiveScreen::showBankName() {

    // Bank file name display coordinates
    bankFileNameWidth  = presetsPanel.panelWidth;
    bankFileNameHeight = 40;
    bankFileNameX      = presetsPanel.panelX;
    bankFileNameY      = DISPLAY_HEIGHT - 85;
//    Serial.printf("PresetsLiveScreen::showBankName: x %d y %d width %d height %d color %d\n", bankFileNameX, bankFileNameY, bankFileNameWidth, bankFileNameHeight, displayBgdColor);

    // Clear message display area
    DisplayScreenBase::fillRect(bankFileNameX, bankFileNameY, bankFileNameWidth, bankFileNameHeight, displayBgdColor);

    // Message position above bankCmndPanel
    TextArea tArea;
    tArea.posX  = bankFileNameX;
    tArea.posY  = bankFileNameY;
    tArea.wid   = bankFileNameWidth;
    tArea.hgt   = bankFileNameHeight;
    tArea.align = TextAlignCenter;

    // Remove file suffix if present, remove from displayed name
    char bankName[PresetFileNameSize];
    strcpy(bankName, bankFileName);

    snprintf(stringDispBuf, sizeof(stringDispBuf) - 1, "Active Bank: %s", bankName);

    // BUG/FFS: B241116-1. For some perplexing reason, applying a cast to the font
    // spec in the following:
    //
    //      DisplayScreenBase::setFont((ILI9341_t3_font_t)Arial_12_Bold);
    //
    // causes the application to crash in PresetsScreen when executing
    //
    //      -Load SD Card
    //      -Save SD Card
    //      -Save Default
    //
    // This even though the same cast applied elsewhere in this module and throughout
    // the application causes no such issue. A real mystery that took untold wasted
    // hours locating and correcting.
    //
    DisplayScreenBase::setFont(Arial_12_Bold);
    DisplayScreenBase::setTextColor(COLOR_WHITE);
    DisplayScreenBase::printInArea(stringDispBuf, tArea);
}

// showDefaultBankButton
//
void PresetsLiveScreen::showDefaultBankButton(bool selected) {

    // Draw default bank
    _defaultBankButtonInfo.btnX = bankFileListDisplay.xPos;
    _defaultBankButtonInfo.btnY = marginTop;
    _defaultBankButtonInfo.btnWid = bankFileListDisplay.width;
    _defaultBankButtonInfo.btnLabelFont = LiberationMono_12_Bold;
    _defaultBankButtonInfo.btnLabelColor = (selected ? COLOR_WHITE : COLOR_LIGHTGREY2);
//    _defaultBankButtonInfo.btnLabelColor = (selected ? SCREENBUTTON_ON_TEXT_COLOR : SCREENBUTTON_OFF_TEXT_COLOR);
    defaultBankButton.drawButton();

    // Draw outline around Bank Selection button
    // to match the appearance of SD bank file list
    int16_t btnX = _defaultBankButtonInfo.btnX;
    int16_t btnY = _defaultBankButtonInfo.btnY;
    int16_t btnWid = _defaultBankButtonInfo.btnWid;
    int16_t btnHgt = _defaultBankButtonInfo.btnHgt;
    DisplayScreenBase::drawRoundRect(btnX-2, btnY-2, btnWid+4, btnHgt+4, 3, COLOR_WHITE);
}

// showSdBanksLabel
//
void PresetsLiveScreen::showSdBanksLabel() {

    // Bank file name display coordinates
    uint16_t sdBanksLabelWidth  = bankFileListDisplay.width;
    uint16_t sdBanksLabelHeight = 40;
    uint16_t sdBanksLabelX      = DISPLAY_WIDTH - (bankFileListDisplay.width + 82);
    uint16_t sdBanksLabelY      = DISPLAY_HEIGHT - 87;
//    Serial.printf("PresetsLiveScreen::showBankName: x %d y %d width %d height %d color %d\n", bankFileNameX, bankFileNameY, bankFileNameWidth, bankFileNameHeight, displayBgdColor);

    // Clear message display area
    DisplayScreenBase::fillRect(sdBanksLabelX, sdBanksLabelY, sdBanksLabelWidth, sdBanksLabelHeight, displayBgdColor);

    // Message position above bankCmndPanel
    TextArea tArea;
    tArea.posX  = sdBanksLabelX;
    tArea.posY  = sdBanksLabelY;
    tArea.wid   = sdBanksLabelWidth;
    tArea.hgt   = sdBanksLabelHeight;
    tArea.align = TextAlignCenter;

    snprintf(stringDispBuf, sizeof(stringDispBuf) - 1, "%s", "Preset Banks on SD");

    DisplayScreenBase::setFont(Arial_12_Bold);
    DisplayScreenBase::setTextColor(COLOR_WHITE);
    DisplayScreenBase::printInArea(stringDispBuf, tArea);
}

// updatePresetNames
//
// Get current preset names from application and draw presets panel
//
void PresetsLiveScreen::updatePresetNames(bool importNames) {

//    Serial.printf("PresetsLiveScreen::updatePresetNames: updatePresetNames\n");

    const char *presetName;

    for (uint16_t presetNum = 0; presetNum < presetsPanel.btnCount; presetNum++) {
        if (importNames) {
            presetName = (*presetsCallbackFn)(CmdGetImportPresetName, presetNum, 0);
        } else {
            presetName = (*presetsCallbackFn)(CmdGetPresetName, presetNum, 0);
        }
        setBtnLabel(presetNum, (char *)presetName, false);
    }

    presetsPanel.drawPanel((const char**)presetsPanelBtnLabels);

    // Display preset numbers underneath buttons
    showPresetBtnNumbers();
}

// showPresetBtnNumber
//
// Display preset number on specified button
//
void PresetsLiveScreen::showPresetBtnNumber(ButtonId btnNum) {

    uint16_t btnX, btnY = 0;

    TextArea tArea;
    tArea.posX = 0;
    tArea.wid = presetsPanel.btnWidth;
    tArea.posY = 0;
    tArea.hgt = 10;
    tArea.align = TextAlignLeft;

    presetsPanel.getButtonXY(btnNum, btnX, btnY);
    tArea.posX = btnX + 2;
    tArea.posY = btnY + 2;

    DisplayScreenBase::setFont(Arial_8);
    DisplayScreenBase::setTextColor(COLOR_WHITE);

    snprintf(stringDispBuf, sizeof(stringDispBuf)-1, "%d", btnNum+1);
    DisplayScreenBase::printInArea(stringDispBuf, tArea);
}

// showPresetBtnNumbers
//
// Display preset numbers on preset buttons
//
void PresetsLiveScreen::showPresetBtnNumbers() {

    uint8_t btnCount = presetsPanel.btnCount;

    for (ButtonId btn = 0; btn < btnCount; btn++) {
        showPresetBtnNumber(btn);
    }
}

// getPresetBtnLabel
//
const char* PresetsLiveScreen::getPresetBtnLabel(ButtonId btnNum) {

    return presetsPanelBtnLabels[btnNum];
}

// handlePresetBtnPress
//
void PresetsLiveScreen::handlePresetBtnPress(ButtonId touchedBtn) {

//    Serial.printf("PresetsLiveScreen::handlePresetBtnPress: touchedBtn %d\n", touchedBtn);

    (*presetsCallbackFn)(CmdLoadPreset, touchedBtn, 0);
    setSelectedPreset(touchedBtn);
}

// updateScreen
//
void PresetsLiveScreen::updateScreen(StatusUpdatePacketType pktType, void* pkt) {

//    Serial.printf("PresetsLiveScreen::updateScreen:\n");

    if (pktType == StatusPacket && statusDisplay.displayInitialized) {
//        Serial.printf("ParamButtonScreen::updateScreen: Preset %02d: %s\n", fPkt->presetNumber, fPkt->presetName);
        statusDisplay.updateDisplay(pktType, pkt);

        StatusUpdatePacket* statusPkt = (StatusUpdatePacket*) pkt;
        ButtonId presetNum = statusPkt->presetNumber - 1;
        if (presetNum != selectedPreset) {
            setSelectedPreset(presetNum);
        }
    }
}


