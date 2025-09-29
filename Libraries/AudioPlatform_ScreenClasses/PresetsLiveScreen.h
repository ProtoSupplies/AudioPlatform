/************************************************************************************
*                                PresetsLiveScreen.h                                *
*                                                                                   *
*                       Copyright (c) 2025, Donald Swearingen                       *
*                                All Rights Reserved                                *
*************************************************************************************
*
* Description
*
*   See PresetsLiveScreen.cpp for additional details.
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

#ifndef PRESETSLIVE_SCREEN_H_
#define PRESETSLIVE_SCREEN_H_

// User Arduino Libraries
//
#include "DisplayScreenBase.h"
#include "AudioPlatformGlobal.h"
#include "TextEdit.h"
#include "ListSelect.h"
#include "StatusDisplay.h"
#include "SelectPresetUtil.h"

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

    stateCancel     = 0,
    stateLoad       = 1,

} PresetLiveCmndState;

/************************************************************************************
*                         C L A S S   D E F I N I T I O N S                         *
*************************************************************************************
*/

class PresetsLiveScreen : public DisplayScreenBase {

// Constructor/Destructor
//
public:

    PresetsLiveScreen(DisplayScreenInfo& displayInfo, ButtonPanelInfo& presetsPanelInfo,
        const char* (*presetsCallbackFn)(CommandButtonId cmdId, uint16_t& numberParam, void* voidParam),
        SystemHardwareProfile& sysProfile_);
    virtual ~PresetsLiveScreen();

// Class Variables
//
public:


private:

    // Panel for display of preset buttons
    ButtonPanel             presetsPanel;
    char*                   presetsPanelBtnLabels[PresetCount];

    // Global note control buttons
    static ScreenButtonInfo allNotesOffBtnInfo;
    ScreenButton            allNotesOffBtn;
    static ScreenButtonInfo ignoreNoteOffBtnInfo;
    ScreenButton            ignoreNoteOffBtn;

    // All Notes Off button momentary flash support
    bool                    allOffFlashing;             // allNotesOffBtn in flash state
    elapsedMillis           allOffFlashTime;            // time since flash began
    const uint16_t          allOffFlashInterval = 500;  // button flash length

    // Current preset
    uint32_t                lastPresetSelectMillis;     // ms time of last preset button touch
    uint16_t                selectedPreset;

    // Callback functions in Presets.ino
    const char*             (*presetsCallbackFn)(CommandButtonId cmdId, uint16_t& numberParam, void* voidParam);
    uint16_t                ignoredCallbackArg;

    // Up/Down Screen Buttons
//    ScreenButtonInfo        defaultBankButtonInfo;
    ScreenButton            defaultBankButton;

    static TextColumnInfo   bankFileListDisplay;
    ListSelect              bankFileSelect;
    char                    bankFileName[PresetFileNameSize];
    uint16_t                bankFileNameWidth;
    uint16_t                bankFileNameHeight;
    uint16_t                bankFileNameX;
    uint16_t                bankFileNameY;

    uint16_t                sdBankCount;
    ButtonId                selectedBank;

    // System Information
    SystemHardwareProfile&  sysProfile;

    // Footer status display
    StatusDisplay           statusDisplay;

    // ms to wait after key touch
    static uint16_t         WaitTimeAfterKeyTouch;

protected:


// Class Methods
//
public:

    void                activateScreen() override;      // clear and display screen
    void                deActivateScreen() override;    // save screen parameters before closing
    ButtonId            pollScreen() override;
    void                setBtnLabel(ButtonId btnNum, char* btnLabel, bool reDraw = false);
    uint16_t            getPresetNum();
    void                setSelectedPreset(ButtonId btn); // set and highlight selected preset

private:

    void                getSdBankList();

    void                showPresetName(bool importName = false);
    void                showBankName();
    void                showDefaultBankButton(bool selected);
    void                showSdBanksLabel();

    void                updatePresetNames(bool importNames = false);
    void                showPresetBtnNumber(ButtonId btnNum);
    void                showPresetBtnNumbers();
    const char*         getPresetBtnLabel(ButtonId btn);

    void                handlePresetBtnPress(ButtonId touchedBtn);

    void                updateScreen(StatusUpdatePacketType pktType, void* pkt) override;
    ButtonId            pollSelectPreset();

protected:


};

#endif // PRESETSLIVE_SCREEN_H_
