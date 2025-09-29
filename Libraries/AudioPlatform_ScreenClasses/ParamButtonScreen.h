/************************************************************************************
*                               ParamButtonScreen.h                                 *
*                                                                                   *
*                      Copyright (c) 2025, Donald Swearingen                        *
*                               All Rights Reserved                                 *
*************************************************************************************
*
* Description
*
*   Create an array (BtnPanel) of screen "buttons" used to display and edit various
*   application parameter values.
*
*   See ParamButtonScreen.cpp for further details.
*
* Modifications List
*
*   •
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

#ifndef PARAMBUTTONSCREEN_H_
#define PARAMBUTTONSCREEN_H_

// User Arduino Libraries
//
#include "DisplayScreenBase.h"
#include "ButtonPanel.h"
#include "RotaryEncoderPB.h"
#include "StatusDisplay.h"
#include "UtilFns.h"
#include "ScreenButton.h"
#include "SelectPresetUtil.h"

/************************************************************************************
*                         M A C R O S  /  C O N S T A N T S                         *
*************************************************************************************
*/

#define PARAMBTN_SCRN_MARGIN_TOP    DEFAULT_SCRN_MARGIN_TOP
#define PARAMBTN_SCRN_MARGIN_BOTTOM DEFAULT_SCRN_MARGIN_BOTTOM
#define PARAMBTN_SCRN_MARGIN_LEFT   DEFAULT_SCRN_MARGIN_LEFT
#define PARAMBTN_SCRN_MARGIN_RIGHT  DEFAULT_SCRN_MARGIN_RIGHT
#define PARAMBTN_SCRN_BTN_WIDTH     156
#define PARAMBTN_SCRN_BTN_HEIGHT    50
#define PARAMBTN_SCRN_GAP_HORIZ     18
#define PARAMBTN_SCRN_GAP_VERT      10
#define ROTARY_CIRCLE_RADIUS        22
#define ROTARY_CIRCLE_DEF_GAP_ABOVE 15      // Default gap above circle

/************************************************************************************
*                     D A T A / T Y P E   D E F I N I T I O N S                     *
*************************************************************************************
*/

/********
* Types *
*********
*/

// Parameter Types for Individual Buttons
//
typedef enum {
    BtnParam_Type_Int = 0,
    BtnParam_Type_Float,
    BtnParam_Type_Text
} ButtonParamType;

// Parameter Information for Individual Buttons
//
// FFS: Note that paramId assignments generally are made from entries in the AudioPlatform.h
// DeviceParamId enum. But we can't use that type here to define paramId because the library
// modules are not supposed to have knowledge of the applications that use them.
//
typedef struct {
    uint16_t        paramId;        // Unique parameter identifier
    ButtonParamType paramType;      // Type: int, float, text
    const char*     paramName;      // Parameter name for display on btn/button (must fit)
    const char*     paramUnits;     // Parameter units for display on btn/button (2-3 chars)
    float           paramMin;       // Minimum applied value in parameter range
    float           paramMax;       // Maximum applied value in parameter range
    const char**    paramLabels;
} ParamButtonInfo;

/************************************************************************************
*                         C L A S S   D E F I N I T I O N S                         *
*************************************************************************************
*/

class ParamButtonScreen : public DisplayScreenBase {

// Constructor/Destructor
//
public:

    ParamButtonScreen(DisplayScreenInfo& displayInfo, ButtonPanelInfo& panelInfo, ParamButtonInfo *btnParams,
            RotaryParams *paramBtnRotaryParams, RotaryEncoderPB **encoders, uint16_t presetDeviceId,
            const char* (*presetsCallbackFn)(CommandButtonId cmdId, uint16_t& numberParam, void* voidParam));
    virtual ~ParamButtonScreen();

// Class Variables
//
public:


private:


protected:

    // Callback function to Presets.ino
    const char*             (*presetsCallbackFn)(CommandButtonId cmdId, uint16_t& numberParam, void* textParam);

    // Screen buttons
    ButtonPanel             btnPanel;

    static ScreenButtonInfo saveChangesBtnInfo;
    ScreenButton            saveChangesBtn;
    static ScreenButtonInfo copySettingsBtnInfo;
    ScreenButton            copySettingsBtn;

    static char presetDeviceNames[PresetDeviceCount][PresetNameSize];

    // Screen parameters
    ParamButtonInfo*        btnParams;             // array of button parameters, one per display button
    RotaryParams*           btnRotaryParams;       // array rotary parameters, one per display button
    RotaryEncoderPB**       rotaries;             // array of rotary encoders, one per display column

    // Currently active button in each column
    uint8_t                 numColumns;
    ButtonId                activeParamBtnInCol[ROTARY_COUNT_MAX] = {0, 1, 2, 3};

    // Rotary Increment Circle Data
    CircleParams            incrCircleData[ROTARY_COUNT_MAX];

    // Footer status display
    StatusDisplay           statusDisplay;

    // Select preset from which to copy device settings
    SelectPresetUtil        selectPresetToCopy;
    uint16_t                presetDevice;

// Class Methods
//
public:

    void        activateScreen() override;      // clear and display screen
    void        deActivateScreen() override;    // save screen parameters before closing
    ButtonId    pollScreen() override;
    void        updateScreen(StatusUpdatePacketType pktType, void* pkt) override;
    void        setBtnProperties(uint16_t paramId, float btnValue, bool screenActive = false) override;
    void        rotaryValueChange(uint8_t rotaryId, int16_t rotaryVal);
    void        rotaryIncrChange(uint8_t rotaryId, uint16_t incrVal);

private:


protected:

    void        handleParamBtnTouch(ButtonId touchedBtn);
    void        restoreBtnParamValues();
    bool        btnIsActive(ButtonId btnNum);
    void        initRotaryIncrDisplay();
    void        showRotaryIncr(uint8_t column, char* valueStr);
    bool        incrButtonTouched(uint16_t pointX, uint16_t pointY, ButtonId& touchedBtn);
    void        showBtnName(ButtonId btnNum);
    void        showBtnValue(ButtonId btnNum);
    float       btnRotaryValToParamVal(ButtonId btnNum);
    int16_t     paramValToBtnRotaryVal(ButtonId btnNum, float paramValue);
    uint16_t    getBtnParamId(ButtonId btnNum);

    ButtonId    pollSelectPreset();
    void        showSelectPresetPopup();
    void        updatePresetNames();
    void        setPresetBtnLabel(ButtonId btnNum, char* btnLabel, bool reDraw = false);
};

#endif // PARAMBUTTONSCREEN_H_
