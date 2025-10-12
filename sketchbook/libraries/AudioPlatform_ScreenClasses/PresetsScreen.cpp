/************************************************************************************
*                                 PresetsScreen.cpp                                 *
*                                                                                   *
*                     Copyright (c) 2025, Donald Swearingen                         *
*                                All Rights Reserved                                *
*************************************************************************************
*
* Description
*
*   PresetsScreen is a custom AudioPlatform screen derived from the generic base class
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

#include "PresetsScreen.h"
#include "ArduinoEclipseDefs.h"
#include "UtilFns.h"
#include "Teensy41_Util.h"

/************************************************************************************
*                         M A C R O S  /  C O N S T A N T S                         *
*************************************************************************************
*/

/************************************************************************************
*                    S T A T I C   C L A S S   V A R I A B L E S                    *
*************************************************************************************
*/

EXTMEM static char presetLabels[PresetDeviceCount][PresetNameSize];

// Place constant string prompts in Program Flash
//
const uint8_t PresetCmndPromptCount     = 6;
const uint8_t PresetCmndPromptLength    = 64;

// Indexed by PresetCmndState
PROGMEM char presetCmndPrompts[PresetCmndPromptCount][PresetCmndPromptLength] = {
    "Press Load, Save, Import, Clear, or Rename",
    "Press Preset Button to Load",
    "Press Preset Button to Save",
    "Press Preset Button to Import",
    "Press Preset Button to Clear",
    "Press Preset Button to Rename"
};

const uint8_t BankCmndPromptCount       = 6;
const uint8_t BankCmndPromptLength      = 64;

PROGMEM char bankCmndPrompts[BankCmndPromptCount][BankCmndPromptLength] = {
    "Press Load or Save",
    "Press Load Bank Source: Default or SD Card",
    "Press Save Bank Destination: Default or SD Card",
    "Loading Default Preset Bank",
    "Loading SD Card Preset Bank",
    "Updating Default Bank",
};

// Select Button Panel
//
#define SELECT_PANEL_BTN_COUNT      3
#define SELECT_PANEL_BTNS_PER_ROW   1
#define SELECT_PANEL_BTN_WIDTH      75
#define SELECT_PANEL_BTN_HEIGHT     40
#define SELECT_PANEL_BTN_GAP_HORIZ  0
#define SELECT_PANEL_BTN_GAP_VERT   10

static ButtonPanelInfo selectSdLoadPanelDefaultInfo = (ButtonPanelInfo) {
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
static LabelIdButtonInfo selectSdLoadPanelDefaultBtnInfo[] = {
    {"Active", BtnSelectActive},
    {"Import", BtnSelectImport},
    {"Cancel", BtnCancel}
};

TextColumnInfo PresetsScreen::sdBankFileListDisplay = (TextColumnInfo)  {
    LISTVIEW_XPOS,                  // xPos
    LISTVIEW_YPOS,                  // yPos
    LISTVIEW_COLUMN_WIDTH,          // width
    305,                            // height
    LISTVIEW_LINE_HEIGHT,           // lineHeight
    12,                             // maxLines
    LISTVIEW_FONT,                  // font
    LISTVIEW_FONT_COLOR,            // textColor
    LISTVIEW_TEXT_MARGIN,           // textMargin
    LISTVIEW_TEXT_ALIGN,            // textAlign
    LISTVIEW_BGND_COLOR,            // columnColor
    LISTVIEW_OUTLINE_COLOR,         // outlineColor
    LISTVIEW_SEL_OUTLINE_COLOR      // selectedOutlineColor
    };

uint16_t PresetsScreen::WaitTimeAfterKeyTouch = 100;   // ms

/************************************************************************************
*                    P U B L I C   C L A S S   F U N C T I O N S                    *
*************************************************************************************
*/
// Static Member Initializations
//

// Constructor/Destructor
//
PresetsScreen::PresetsScreen(DisplayScreenInfo& displayInfo, ButtonPanelInfo& presetsPanelInfo, ButtonPanelInfo& presetCmndPanelInfo,
    LabelIdButtonInfo* presetCmndPanelBtnInfo, ButtonPanelInfo& bankCmndPanelInfo, LabelIdButtonInfo* bankCmndPanelBtnInfo,
    const char* (*_presetsCallbackFn)(CommandButtonId cmdId, uint16_t& numberParam, void* voidParam),
    SystemHardwareProfile& sysProfile_) :

    DisplayScreenBase(displayInfo),
    presetsPanel(presetsPanelInfo),
    presetsCmndPanel(presetCmndPanelInfo), presetsCmndPanelLabelIdInfo(presetCmndPanelBtnInfo),
    bankCmndPanel(bankCmndPanelInfo), bankCmndPanelLabelIdInfo(bankCmndPanelBtnInfo),
    presetsCallbackFn(_presetsCallbackFn), ignoredCallbackArg(0),
    presetNameEdit(100, 80, PresetNameSize-1),
    bankFileNameEdit(100, 80, PresetNameSize-1),
    sdBankFileSelect(),
    sysProfile(sysProfile_) {

    lastPresetSelectMillis = 0;
    selectedPreset = 0;
    strcpy(bankFileName, DefaultPresetsBankFilename);

    presetsCmndState = cmndStateCancel;
    presetsCmndBtn = BtnCancel;

    bankCmndState = cmndStateCancel;
    bankCmndBtn = BtnCancel;
    bankModified = 0;

    // Place presets panel below top margin, centered between left and right margins
    int16_t displayAreaX1       = marginLeft;
    int16_t displayAreaX2       = DISPLAY_WIDTH - marginRight;
    int16_t displayAreaWidth    = displayAreaX2 - displayAreaX1;
    int16_t presetsPanelX       = displayAreaX1 + ((displayAreaWidth - presetsPanel.panelWidth) / 2);
    int16_t presetsPanelY       = marginTop + 20;
    presetsPanel.setPanelPosition(presetsPanelX, presetsPanelY);

    // Setup Presets Command Panel at bottom left of screen
    int16_t presetsCmndPanelX  = marginLeft;
//    int16_t presetsCmndPanelX  = marginLeft + ((DISPLAY_WIDTH / 2) - (marginLeft + marginRight) - presetsCmndPanel.panelWidth) / 2;
    int16_t presetsCmndPanelY  = DISPLAY_HEIGHT - marginBottom - (presetsCmndPanel.panelHeight) + 10;
    presetsCmndPanel.setPanelPosition(presetsCmndPanelX, presetsCmndPanelY);

    // Setup Presets Command Panel at bottom left of screen
    int16_t bankCmndPanelX  = DISPLAY_WIDTH - marginRight - bankCmndPanel.panelWidth;
//    int16_t bankCmndPanelX  = marginLeft + ((DISPLAY_WIDTH / 2) - (marginLeft + marginRight) - bankCmndPanel.panelWidth) / 2;
    int16_t bankCmndPanelY  = DISPLAY_HEIGHT - marginBottom - (bankCmndPanel.panelHeight) + 10;
    bankCmndPanel.setPanelPosition(bankCmndPanelX, bankCmndPanelY);

    // Set Bank File select properties
    sdBankFileSelect.setTextColumnInfo(&sdBankFileListDisplay);
}

PresetsScreen::~PresetsScreen() {

}

// activateScreen
//
// Reset/display dynamic screen elements each time screen is displayed
//
void PresetsScreen::activateScreen() {

//    Serial.printf("PresetsScreen::activateScreen:\n");

    // Clear and display basic screen
    clearScreen();
    presetsPanel.clearPanel();
    presetsCmndPanel.clearPanel();
    bankCmndPanel.clearPanel();
    showScreenHeading();        // Must come _after_ clear panels

    // Get current preset bank filename
    // (May have been changed in other screens)
    uint16_t activeBank = (uint16_t) BtnActiveBank;
    const char* fileName = (*presetsCallbackFn)(CmdGetBankFilename, activeBank, 0);
    strcpy(bankFileName, fileName);

    // Get current preset names from application and draw presets panel
    updatePresetNames();

    // Display preset numbers underneath buttons
    showPresetBtnNumbers();

    presetsCmndState = cmndStateCancel;
    presetsCmndBtn = BtnCancel;

    bankCmndState = cmndStateCancel;
    bankCmndBtn = BtnCancel;

    drawPresetCmndPanel(presetsCmndBtn);
    drawBankCmndPanel(bankCmndBtn);

    // (Re)Select current preset
    setSelectedPreset(selectedPreset);

    // Override ListSelect Command Panel with Local Defines
    sdBankFileSelect.setPanelInfo(selectSdLoadPanelDefaultInfo);
    sdBankFileSelect.setPanelBtnInfo(selectSdLoadPanelDefaultBtnInfo);
}

// deActivateScreen
//
void PresetsScreen::deActivateScreen() {

}

// pollScreen
//
ButtonId PresetsScreen::pollScreen() {

    ButtonId touchedBtn = BtnNone;
    uint16_t pointX, pointY;

    if (presetNameEdit.pollEnabled) {
        pollPresetNameEdit();

    } else if (bankFileNameEdit.pollEnabled) {
        pollBankFileNameEdit();

    } else if (sdBankFileSelect.pollEnabled) {
        pollSdBankSelect();

    } else if (DisplayScreenBase::touched(WaitTimeAfterKeyTouch)) {
        DisplayScreenBase::getTouchedPoint(pointX, pointY);

        // First check for Home/Back button. If pressed, we'll return that value to main application.
        if (DisplayScreenBase::prevBtnTouched(pointX, pointY)) {
            touchedBtn = BtnPrev;

        // Check for touch on preset command selection button
        } else if (presetsCmndPanel.isPanelButton(pointX, pointY, touchedBtn)) {
            handlePresetCmndBtnPress(touchedBtn);

        // Check for touch on bank command selection button
        } else if (bankCmndPanel.isPanelButton(pointX, pointY, touchedBtn)) {
            handleBankCmndBtnPress(touchedBtn);

        // Check for touch on preset selection button.
        // NOTE: This must occur AFTER handling of command selection button.
        } else if (presetsPanel.isPanelButton(pointX, pointY, touchedBtn)) {
            // No new preset selection for a short time
                handlePresetBtnPress(touchedBtn);
//            if ((millis() - lastPresetSelectMillis) > WaitTimeAfterSelect) {
//                lastPresetSelectMillis = millis();
//                handlePresetBtnPress(touchedBtn);
//            }
        }
    }

    return touchedBtn;
}

// setBtnProperties
//
// TODO/FFS: Originally created to allow ParamButtonScreen buttons to be updated in real time by external
// controls, for example to assign MIDI controls to effects parameters when a MIDI controller is moved.
// Used here simply as a way to set the current preset, called from Screens.ino. As of v0.3.4.4, only
// being implemented here and in ParamButtonScreen. Need to find a more direct and less confusing means
// of implementing these two uses. (T240910-1)
//
void PresetsScreen::setBtnProperties(ButtonId btnId, bool screenActive) {

//    Serial.printf("PresetsScreen::setBtnProperties: btnId %d, screenActive %d\n", btnId, screenActive);

    if (screenActive) {
//        Serial.printf("PresetsScreen::setBtnProperties: calling setSelectedPreset(%d)\n", btnId);
        setSelectedPreset(btnId);
        showPresetName();

    } else {
        selectedPreset = btnId; // Screen will be updated when it is loaded
    }
}

// setBtnLabel
//
// malloc()-free version fixes memory leak bug B241217-1
//
void PresetsScreen::setBtnLabel(ButtonId btnNum, char* btnLabel, bool reDraw) {

//    Serial.printf("PresetsScreen::setBtnLabel: %d, %s %d\n", btnNum, btnLabel, reDraw);
//    Serial.printf("PresetsScreen::setBtnLabel: len %d\n", strlen(btnLabel));

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
        outlineModifiedPresetButton(btnNum);
    }
}

// getPresetNum
//
uint16_t PresetsScreen::getPresetNum() {

    return selectedPreset;
}

// setSelectedPreset
//
// Set/change selected button
//
void PresetsScreen::setSelectedPreset(ButtonId btnNum) {

//    Serial.printf("PresetsScreen::setSelectedPreset: current selected %d, new selected %d\n", selectedPreset, btnNum);

    if (selectedPreset != BtnNone) {
        presetsPanel.drawButton(selectedPreset, false);
        presetsPanel.showButtonLabel(selectedPreset, getPresetBtnLabel(selectedPreset), false);
    }

    // Make touchedBtn active and redisplay
    selectedPreset = btnNum;
    presetsPanel.drawButton(selectedPreset, true);
    presetsPanel.showButtonLabel(selectedPreset, (*presetsCallbackFn)(CmdGetPresetName, selectedPreset, 0), true);
    outlineModifiedPresetButton(selectedPreset);
//    presetsPanel.showButtonLabel(selectedPreset, getPresetBtnLabel(selectedPreset), true);
}

/************************************************************************************
*                   P R I V A T E   C L A S S   F U N C T I O N S                   *
*************************************************************************************
*/

// pollPresetNameEdit
//
// Polling activity when text editor for preset name is active
//
ButtonId PresetsScreen::pollPresetNameEdit() {

    ButtonId touchedBtn = presetNameEdit.pollPopup();

    if (touchedBtn != BtnNone) {
        presetNameEdit.pollEnabled = false;
        if (touchedBtn == BtnSave) {
            strncpy(presetName, presetNameEdit.getTextValue(), PresetNameSize-1);
            presetName[PresetNameSize-1] = '\0';
            (*presetsCallbackFn)(CmdSetPresetName, selectedPreset, presetName);
            setBtnLabel(selectedPreset, presetName, true);
        }

        touchedBtn = BtnNone;
        this->activateScreen();
    }

    return touchedBtn;
}

// editPresetName
//
// Setup and invoke text editor for preset name
//
void PresetsScreen::editPresetName() {

    presetNameEdit.setMaxTextLength(PresetNameSize - 1);
    presetNameEdit.setTextHeading("Enter Preset Name");
    presetNameEdit.setTextValue((*presetsCallbackFn)(CmdGetPresetName, selectedPreset, 0));
    presetNameEdit.showPopup();
    presetNameEdit.pollEnabled = true;
}

// pollBankFileNameEdit
//
// Polling activity when text editor for preset bank file name is active
//
ButtonId PresetsScreen::pollBankFileNameEdit() {

    ButtonId touchedBtn = bankFileNameEdit.pollPopup();

    if (touchedBtn != BtnNone) {
        bankFileNameEdit.pollEnabled = false;
        if (touchedBtn == BtnSave) {
            strncpy(bankFileName, bankFileNameEdit.getTextValue(), PresetFileNameSize-1);
            bankFileName[PresetFileNameSize - 1] = '\0';

            // If preset bank file suffix not present, add to filename
            addPresetSuffix(bankFileName);

            (*presetsCallbackFn)(CmdSaveSdBank, ignoredCallbackArg, bankFileName);
            // FFS: What if call fails? (shouldn't affect state, which should remain unchanged)
        }

        touchedBtn = BtnNone;
        this->activateScreen();
    }

    return touchedBtn;
}

// editBankFileName
//
// Setup and invoke text editor for preset bank file name
//
void PresetsScreen::editBankFileName() {

    bankFileNameEdit.setMaxTextLength(PresetFileNameSize - 1); // Leave room for ".pbnk" suffix
    bankFileNameEdit.setTextHeading("Enter Preset Bank Filename");
    removePresetSuffix(bankFileName);
    bankFileNameEdit.setTextValue(bankFileName);
    bankFileNameEdit.showPopup();
    bankFileNameEdit.pollEnabled = true;
}

// pollSdBankSelect
//
// Polling activity when list selection of preset bank file is active
//
ButtonId PresetsScreen::pollSdBankSelect() {

    ButtonId touchedBtn = sdBankFileSelect.pollPopup();

    if (touchedBtn != BtnNone) {
        sdBankFileSelect.pollEnabled = false;
        ButtonId selectedLine = BtnNone;
        selectedLine = sdBankFileSelect.getSelectedLine();
        if (selectedLine != BtnNone) {
            if (touchedBtn == BtnSelectActive) {
                strncpy(bankFileName, sysProfile.largeTempBufLines[selectedLine - 1], PresetFileNameSize-1);
                bankFileName[PresetFileNameSize-1] ='\0';
                (*presetsCallbackFn)(CmdLoadSdBank, touchedBtn, bankFileName);
            } else if (touchedBtn == BtnSelectImport) {
                strncpy(importBankFileName, sysProfile.largeTempBufLines[selectedLine - 1], PresetFileNameSize-1);
                importBankFileName[PresetFileNameSize-1] ='\0';
                (*presetsCallbackFn)(CmdLoadSdBank, touchedBtn, importBankFileName);
//                Serial.printf("PresetsScreen::pollSdBankSelect: importBankFileName %s\n", importBankFileName);
            }
        }

        touchedBtn = BtnNone;
        this->activateScreen();
    }

    return touchedBtn;
}

// selectSdBank
//
// Setup and invoke list selection of preset bank file
//
void PresetsScreen::selectSdBank() {

    uint16_t lineCount = 0;

    if (sysProfile.sdCardPresent) {
        lineCount = Teensy41_Util::printSdDirFileListToBigBuf(".pbnk");
    }

    sdBankFileSelect.setList(sysProfile.largeTempBufLines, lineCount);
    sdBankFileSelect.setPopupHeading("Select Preset Bank on SD Card");

    uint16_t dispWidth = 300;
    sdBankFileSelect.setListDisplayWidth(dispWidth);
    sdBankFileSelect.setListDisplayX((DISPLAY_WIDTH - dispWidth) / 2);
    sdBankFileSelect.setTextAlign(TextAlignCenter);

    sdBankFileSelect.showPopup();
    sdBankFileSelect.pollEnabled = true;
}

// showPresetName
//
void PresetsScreen::showPresetName() {

    uint16_t presetNameDispHgt  = 35;
    uint16_t presetNameDispWid  = presetsCmndPanel.panelWidth;
    uint16_t presetNameDispX    = presetsCmndPanel.panelX;
    uint16_t presetNameDispY    = presetsCmndPanel.panelY - presetNameDispHgt - 30;

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
    presetName = (*presetsCallbackFn)(CmdGetPresetName, selectedPreset, 0);
    snprintf(stringDispBuf, sizeof(stringDispBuf)-1, "Preset: %s", presetName);
    DisplayScreenBase::setFont((ILI9341_t3_font_t)Arial_12_Bold);
    DisplayScreenBase::setTextColor(COLOR_WHITE);
    DisplayScreenBase::printInArea(stringDispBuf, tArea);
}

// showBankName
//
void PresetsScreen::showBankName(uint16_t currentBank) {

//    Serial.printf("PresetsScreen::showBankName: currentBank %d\n", currentBank);

    uint16_t bankNameDispHgt  = 35;
    uint16_t bankNameDispWid  = bankCmndPanel.panelWidth;
    uint16_t bankNameDispX    = bankCmndPanel.panelX;
    uint16_t bankNameDispY    = bankCmndPanel.panelY - bankNameDispHgt - 30;

    // Clear message display area
    DisplayScreenBase::fillRect(bankNameDispX, bankNameDispY, bankNameDispWid, bankNameDispHgt, displayBgdColor);

    // Message position above bankCmndPanel
    TextArea tArea;
    tArea.posX  = bankNameDispX;
    tArea.posY  = bankNameDispY;
    tArea.wid   = bankNameDispWid;
    tArea.hgt   = bankNameDispHgt;
    tArea.align = TextAlignCenter;

    // Remove file suffix if present, remove from displayed name
    char bankName[PresetFileNameSize];
    if (currentBank == BtnActiveBank) {
        strcpy(bankName, bankFileName);
    } else {
        strcpy(bankName, importBankFileName);
    }
    removePresetSuffix(bankName);

    snprintf(stringDispBuf, sizeof(stringDispBuf) - 1, "%s Bank: %s", (currentBank == BtnActiveBank) ? "Active" : "Import", bankName);

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
    if (currentBank == BtnActiveBank && (bool)bankModified) {
        DisplayScreenBase::setTextColor(COLOR_RED);
    }
    DisplayScreenBase::printInArea(stringDispBuf, tArea);
}
// addPresetSuffix
//
// If not present, add PresetBankFileSuffix to filename
//
void PresetsScreen::addPresetSuffix(char *fileName) {

    char *suffixPos = strstr(fileName, PresetBankFileSuffix);
    if (!suffixPos) {
        strcat(fileName, PresetBankFileSuffix);
    }
}

// removePresetSuffix
//
// If present, remove PresetBankFileSuffix from filename
//
void PresetsScreen::removePresetSuffix(char *fileName) {

    char localName[PresetFileNameSize];
    strcpy(localName, fileName);

    char *suffixPos = strstr(localName, PresetBankFileSuffix);
    if (suffixPos) {
        *suffixPos = '\0';
    }

    strcpy(fileName, localName);
}

// updatePresetNames
//
// Get current preset names from application and draw presets panel
//
void PresetsScreen::updatePresetNames(bool importNames) {

//    Serial.printf("PresetsScreen::updatePresetNames: updatePresetNames\n");

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

    if (!importNames) {
        for (uint16_t presetNum = 0; presetNum < presetsPanel.btnCount; presetNum++) {
            outlineModifiedPresetButton(presetNum);
        }
    }
}

// outlineModifiedPresetButton
//
// Draw an variant outline around button if modified preset
//
void PresetsScreen::outlineModifiedPresetButton(ButtonId presetBtn) {

//    Serial.printf("PresetsScreen::outlineModifiedPresetButton:\n");

        bool presetModified = false;
        (*presetsCallbackFn)(CmdGetPresetModified, presetBtn, (void*)&presetModified);
        if (presetModified) {
//            Serial.printf("PresetsScreen::outlineModifiedPresets: redrawing button %d\n", presetBtn);
            presetsPanel.drawButtonOutline(presetBtn, BTN_OUTLINE_COLOR_MODIFIED);
        }
}

// showPresetBtnNumbers
//
// Display preset numbers underneath preset buttons
//
void PresetsScreen::showPresetBtnNumbers() {

    uint8_t btnCount = presetsPanel.btnCount;
    uint16_t btnX, btnY = 0;

    TextArea tArea;
    tArea.posX = 0;
    tArea.wid = presetsPanel.btnWidth;
    tArea.posY = 0;
    tArea.hgt = 10;
    tArea.align = TextAlignLeft;

    for (ButtonId btn = 0; btn < btnCount; btn++) {
        presetsPanel.getButtonXY(btn, btnX, btnY);
        tArea.posX = btnX;
        tArea.posY = btnY + presetsPanel.btnHeight + 5;

//        Serial.printf("PresetsScreen::activateScreen: Drawing btnNum %d at %d,%d\n", btn, tArea.posX, tArea.posY);

        snprintf(stringDispBuf, sizeof(stringDispBuf)-1, "%d", btn+1);
        DisplayScreenBase::setFont(Arial_8);
        DisplayScreenBase::setTextColor(COLOR_WHITE);
        DisplayScreenBase::printInArea(stringDispBuf, tArea);
    }
}

// getPresetBtnLabel
//
const char* PresetsScreen::getPresetBtnLabel(ButtonId btnNum) {

    return presetsPanelBtnLabels[btnNum];
}

// handlePresetBtnPress
//
// Select preset source for current preset command state
//
void PresetsScreen::handlePresetBtnPress(ButtonId touchedBtn) {

//    Serial.printf("PresetsScreen::handlePresetBtnPress: touchedBtn %d\n", touchedBtn);

    bool redrawPanel = true;

    switch (presetsCmndState) {

        case cmndStateLoad:
            (*presetsCallbackFn)(CmdLoadPreset, touchedBtn, 0);
            setSelectedPreset(touchedBtn);
            break;

        case cmndStateSave:
            (*presetsCallbackFn)(CmdSavePreset, touchedBtn, 0);
            setSelectedPreset(touchedBtn);
            break;

        case cmndStateImport:
            // Copy selected preset from import bank to current preset devices
            // (device settings now active, but not yet saved to preset bank)
            (*presetsCallbackFn)(CmdImportPreset, touchedBtn, 0);
            // Redisplay current active preset bank
            updatePresetNames();
            showBankName(BtnActiveBank);
            break;

        case cmndStateClear:
            // TODO: Code to clear selected preset
            setSelectedPreset(touchedBtn);
            break;

        case cmndStateRename:
            setSelectedPreset(touchedBtn);
            editPresetName();
            redrawPanel = false;
            break;

        case cmndStateCancel:
        default:
            redrawPanel = false;
            break;
    }

    presetsCmndState = cmndStateCancel;
    presetsCmndBtn = BtnCancel;
    if (redrawPanel) {
        drawPresetCmndPanel(presetsCmndBtn);
    }
}

// handlePresetCmndBtnPress
//
void PresetsScreen::handlePresetCmndBtnPress(ButtonId touchedBtn) {

//    Serial.printf("PresetsScreen::handlePresetCmndBtnPress: touchedBtn %d\n", touchedBtn);

    // Deactivate when Bank command is active
    if (bankCmndState != cmndStateCancel) {
        return;
    }

    ButtonId returnBtn = touchedBtn;
    ButtonId cmndBtnId = presetsCmndPanelLabelIdInfo[touchedBtn].buttonId;

//    Serial.printf("PresetsScreen::handlePresetCmndBtnPress: cmndBtnId %d\n", cmndBtnId);

    if (cmndBtnId == BtnCancel && presetsCmndBtn != BtnCancel) {
        presetsCmndState = cmndStateCancel;
        presetsCmndBtn = BtnCancel;
        returnBtn = BtnCancel;
        updatePresetNames();
        setSelectedPreset(selectedPreset);

    } else if (presetsCmndState == cmndStateCancel) {

        if (cmndBtnId == BtnLoad && presetsCmndBtn != BtnLoad) {
            presetsCmndState = cmndStateLoad;
            presetsCmndBtn = BtnLoad;
            returnBtn = BtnLoad;

        } else if (cmndBtnId == BtnSave && presetsCmndBtn != BtnSave) {
            presetsCmndState = cmndStateSave;
            presetsCmndBtn = BtnSave;
            returnBtn = BtnSave;

        } else if (cmndBtnId == BtnImport && presetsCmndBtn != BtnImport) {
            presetsCmndState = cmndStateImport;
            updatePresetNames(true);
            presetsCmndBtn = BtnImport;
            returnBtn = BtnImport;

        } else if (cmndBtnId == BtnClear && presetsCmndBtn != BtnClear) {
            presetsCmndState = cmndStateClear;
            presetsCmndBtn = BtnClear;

        } else if (cmndBtnId == BtnRename && presetsCmndBtn != BtnRename) {
            presetsCmndState = cmndStateRename;
            presetsCmndBtn = BtnRename;
        }

    } else {
        returnBtn = BtnNone;
    }

    if (presetsCmndState == cmndStateImport) {
        uint16_t bankBtn = BtnImportBank;
        const char* fileName = (*presetsCallbackFn)(CmdGetBankFilename, bankBtn, 0);
        strcpy(importBankFileName, fileName);
        showBankName(BtnImportBank);
    } else {
        showBankName(BtnActiveBank);
    }

    if (returnBtn != BtnNone) {
        drawPresetCmndPanel(presetsCmndBtn);
    }
}

// drawPresetCmndPanel
//
// Draw preset command button panel
//
void PresetsScreen::drawPresetCmndPanel(ButtonId activeBtn) {

    presetsCmndPanel.drawPanel(presetsCmndPanelLabelIdInfo, activeBtn);

    showPresetCmndBtnMsg();
    showPresetName();
}

// showPresetCmndBtnMsg
//
// Display a prompt message above the preset command panel. The message content is
// based on the current state of the preset command handler.
//
void PresetsScreen::showPresetCmndBtnMsg() {

//    Serial.printf("PresetsScreen::showPresetCmndBtnMsg: presetsCmndState = %d\n", presetsCmndState);

    uint16_t cmndMsgHgt  = 35;
    uint16_t cmndMsgWid  = presetsCmndPanel.panelWidth;
    uint16_t cmndMsgX    = presetsCmndPanel.panelX;
    uint16_t cmndMsgY    = presetsCmndPanel.panelY - cmndMsgHgt - 2;

    // Clear message display area
    DisplayScreenBase::fillRect(cmndMsgX, cmndMsgY, cmndMsgWid, cmndMsgHgt, displayBgdColor);

    // Message position above presetsCmndPanel
    TextArea tArea;
    tArea.posX  = cmndMsgX;
    tArea.posY  = cmndMsgY;
    tArea.wid   = cmndMsgWid;
    tArea.hgt   = cmndMsgHgt;
    tArea.align = TextAlignCenter;

    snprintf(stringDispBuf, sizeof(stringDispBuf)-1, presetCmndPrompts[presetsCmndState]);
    DisplayScreenBase::setTextColor(COLOR_WHITE);
    DisplayScreenBase::printInArea(stringDispBuf, tArea);
}

// handleBankCmndBtnPress
//
void PresetsScreen::handleBankCmndBtnPress(ButtonId touchedBtn) {

//    Serial.printf("PresetsScreen::handleBankCmndBtnPress: touchedBtn %d\n", touchedBtn);

    // Deactivate when Presets command is active
    if (presetsCmndState != cmndStateCancel) {
        return;
    }

    ButtonId returnBtn = touchedBtn;

    ButtonId cmndBtnId = bankCmndPanelLabelIdInfo[touchedBtn].buttonId;

    if (cmndBtnId == BtnCancel && bankCmndBtn != BtnCancel) {
        bankCmndState = cmndStateCancel;
        bankCmndBtn = BtnCancel;
        returnBtn = BtnCancel;

    } else if (cmndBtnId == BtnLoad && bankCmndBtn != BtnLoad) {
        if (bankCmndState == cmndStateCancel) {
            bankCmndState = cmndStateLoad;
            bankCmndBtn = BtnLoad;
            returnBtn = BtnLoad;
        }

    } else if (cmndBtnId == BtnSave && bankCmndBtn != BtnSave) {
        if (bankCmndState == cmndStateCancel) {
            bankCmndState = cmndStateSave;
            bankCmndBtn = BtnSave;
            returnBtn = BtnSave;
        }

    } else if (cmndBtnId == BtnDefaultBank && bankCmndBtn != BtnDefaultBank) {
        if (bankCmndState == cmndStateLoad) {
            (*presetsCallbackFn)(CmdLoadDefaultBank, ignoredCallbackArg, 0);
            updatePresetNames();
            setSelectedPreset(0);

        } else if (bankCmndState == cmndStateSave) {
            bankCmndState = cmndStateUpdating;
            showBankCmndBtnMsg();
            (*presetsCallbackFn)(CmdSaveDefaultBank, ignoredCallbackArg, 0);
            updatePresetNames();
        }

        strcpy(bankFileName, DefaultPresetsBankFilename);
        bankCmndState = cmndStateCancel;
        bankCmndBtn = BtnCancel;
        drawBankCmndPanel(presetsCmndBtn);
        returnBtn = BtnNone;

    } else if (cmndBtnId == BtnSdCardBank && bankCmndBtn != BtnSdCardBank) {
        if (bankCmndState == cmndStateLoad) {
            selectSdBank();

        } else if (bankCmndState == cmndStateSave) {
            editBankFileName();
        }
        returnBtn = BtnNone;

    } else {
        returnBtn = BtnNone;
    }

    if (returnBtn != BtnNone) {
        drawBankCmndPanel(bankCmndBtn);
    }
}

// drawBankCmndPanel
//
// Draw bank command button panel
//
void PresetsScreen::drawBankCmndPanel(ButtonId activeBtn) {

    bankCmndPanel.drawPanel(bankCmndPanelLabelIdInfo, activeBtn);

    showBankCmndBtnMsg();
    (*presetsCallbackFn)(CmdGetBankModified, bankModified, 0);
    showBankName(BtnActiveBank);
}

// showBankCmndBtnMsg
//
void PresetsScreen::showBankCmndBtnMsg() {

//    Serial.printf("PresetsScreen::showBankCmndBtnMsg: presetsCmndState = %d\n", presetsCmndState);

    uint16_t cmndMsgHgt  = 35;
    uint16_t cmndMsgWid  = bankCmndPanel.panelWidth;
    uint16_t cmndMsgX    = bankCmndPanel.panelX;
    uint16_t cmndMsgY    = bankCmndPanel.panelY - cmndMsgHgt - 2;

    // Clear message display area
    DisplayScreenBase::fillRect(cmndMsgX, cmndMsgY, cmndMsgWid, cmndMsgHgt, displayBgdColor);

    // Message position above bankCmndPanel
    TextArea tArea;
    tArea.posX  = cmndMsgX;
    tArea.posY  = cmndMsgY;
    tArea.wid   = cmndMsgWid;
    tArea.hgt   = cmndMsgHgt;
    tArea.align = TextAlignCenter;

    snprintf(stringDispBuf, sizeof(stringDispBuf)-1, bankCmndPrompts[bankCmndState]);
    DisplayScreenBase::setTextColor(COLOR_WHITE);
    DisplayScreenBase::printInArea(stringDispBuf, tArea);
}

// updateScreen
//
void PresetsScreen::updateScreen(StatusUpdatePacketType pktType, void* pkt) {

//        Serial.printf("PresetsScreen::updateScreen:\n");
}

