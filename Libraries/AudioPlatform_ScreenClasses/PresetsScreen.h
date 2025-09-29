/************************************************************************************
*                                  PresetsScreen.h                                  *
*                                                                                   *
*                       Copyright (c) 2025, Donald Swearingen                       *
*                                All Rights Reserved                                *
*************************************************************************************
*
* Description
*
*   See PresetsScreen.cpp for additional details.
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

#ifndef PRESETS_SCREEN_H_
#define PRESETS_SCREEN_H_

// User Arduino Libraries
//
#include "DisplayScreenBase.h"
#include "AudioPlatformGlobal.h"
#include "TextEdit.h"
#include "ListSelect.h"

/************************************************************************************
*                         M A C R O S  /  C O N S T A N T S                         *
*************************************************************************************
*/

/************************************************************************************
*                     D A T A / T Y P E   D E F I N I T I O N S                     *
*************************************************************************************
*/

// Preset and Bank Panel Command States
//
typedef enum : ObjectId {

    cmndStateCancel     = 0,
    cmndStateLoad       = 1,
    cmndStateSave       = 2,
    cmndStateImport     = 3,
    cmndStateClear      = 4,
    cmndStateRename     = 5,
    cmndStateUpdating   = 6,

} PresetCmndState;

/************************************************************************************
*                         C L A S S   D E F I N I T I O N S                         *
*************************************************************************************
*/

class PresetsScreen : public DisplayScreenBase {

// Constructor/Destructor
//
public:

    PresetsScreen(DisplayScreenInfo& displayInfo, ButtonPanelInfo& presetsPanelInfo, ButtonPanelInfo& presetCmndPanelInfo,
        LabelIdButtonInfo* presetCmndPanelBtnInfo, ButtonPanelInfo& bankCmndPanelInfo, LabelIdButtonInfo* bankCmndPanelBtnInfo,
        const char* (*presetsCallbackFn)(CommandButtonId cmdId, uint16_t& numberParam, void* voidParam),
        SystemHardwareProfile& sysProfile_);
    virtual ~PresetsScreen();

// Class Variables
//
public:


private:

    // Panel for display of preset buttons
    ButtonPanel         presetsPanel;
    char*               presetsPanelBtnLabels[PresetCount];

    // Panel for Display of preset command buttons
    ButtonPanel         presetsCmndPanel;
    LabelIdButtonInfo*  presetsCmndPanelLabelIdInfo;

    // Panel for Display of bank command buttons
    ButtonPanel         bankCmndPanel;
    LabelIdButtonInfo*  bankCmndPanelLabelIdInfo;

    // Callback functions Presets.ino
    const char*         (*presetsCallbackFn)(CommandButtonId cmdId, uint16_t& numberParam, void* voidParam);
    uint16_t            ignoredCallbackArg;

    // Popup window to edit preset name
    TextEdit            presetNameEdit;
    char                presetName[PresetNameSize];

    // Popup window to edit SD bank filename
    TextEdit            bankFileNameEdit;
    char                bankFileName[PresetFileNameSize];
    char                importBankFileName[PresetFileNameSize];

    // Popup window to select SD bank file
    static TextColumnInfo sdBankFileListDisplay;
    ListSelect          sdBankFileSelect;

    // Current preset
    uint32_t            lastPresetSelectMillis;     // ms time of last preset button touch
    uint16_t            selectedPreset;

    // Current preset command state and button
    PresetCmndState     presetsCmndState;
    ButtonId            presetsCmndBtn;             // Currently selected button

    // Current bank command state and button
    PresetCmndState     bankCmndState;
    ButtonId            bankCmndBtn;             // Currently selected button
    uint16_t            bankModified;

    // System Information
    SystemHardwareProfile&  sysProfile;

    // ms to wait after key touch
    static uint16_t     WaitTimeAfterKeyTouch;

protected:


// Class Methods
//
public:

    void                activateScreen() override;      // clear and display screen
    void                deActivateScreen() override;    // save screen parameters before closing
    ButtonId            pollScreen() override;
    void                setBtnProperties(ButtonId btnId, bool screenActive) override;
    void                setBtnLabel(ButtonId btnNum, char* btnLabel, bool reDraw = false);
    uint16_t            getPresetNum();
    void                setSelectedPreset(ButtonId btn); // set and highlight selected preset

private:

    ButtonId            pollPresetNameEdit();
    void                editPresetName();
    ButtonId            pollBankFileNameEdit();
    void                editBankFileName();
    ButtonId            pollSdBankSelect();
    void                selectSdBank();

    void                showPresetName();
    void                showBankName(uint16_t currentBank);
    void                addPresetSuffix(char *fileName);
    void                removePresetSuffix(char *fileName);

    void                updatePresetNames(bool importNames = false);
    void                outlineModifiedPresetButton(ButtonId presetBtn);
    void                showPresetBtnNumbers();
    const char*         getPresetBtnLabel(ButtonId btn);

    void                handlePresetBtnPress(ButtonId touchedBtn);
    void                handlePresetCmndBtnPress(ButtonId touchedBtn);
    void                drawPresetCmndPanel(ButtonId activeBtn);
    void                showPresetCmndBtnMsg();

    void                handleBankCmndBtnPress(ButtonId touchedBtn);
    void                drawBankCmndPanel(ButtonId activeBtn);
    void                showBankCmndBtnMsg();

    void                updateScreen(StatusUpdatePacketType pktType, void* pkt) override;

protected:


};

#endif // PRESETS_SCREEN_H_
