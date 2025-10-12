/************************************************************************************
*                                  FmBanksScreen.h                                  *
*                                                                                   *
*                       Copyright (c) 2025, Donald Swearingen                       *
*                                All Rights Reserved                                *
*************************************************************************************
*
* Description
*
*   See FmBanksScreen.cpp for additional details.
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

#ifndef FMBANKSCREEN_H_
#define FMBANKSCREEN_H_

// Teensyduino
//
#include <Audio.h>
#include <ElapsedMillis.h>

// User Arduino Libraries
//
#include "DisplayScreenBase.h"
#include "ParamButtonScreen.h"
#include "ButtonPanel.h"
#include "ScreenButton.h"

/************************************************************************************
*                         M A C R O S  /  C O N S T A N T S                         *
*************************************************************************************
*/

/************************************************************************************
*                     D A T A / T Y P E   D E F I N I T I O N S                     *
*************************************************************************************
*/

/************************************************************************************
*                         C L A S S   D E F I N I T I O N S                         *
*************************************************************************************
*/

class FmBanksScreen : public DisplayScreenBase {

// Constructor/Destructor
//
public:
    FmBanksScreen(DisplayScreenInfo& displayInfo, ButtonPanelInfo presetSelectPanelInfo, ButtonPanelInfo bankSelectPanelInfo,
        const char** _presetSelectLabels, const char** _bankSelectLabels, uint16_t paramId, void (*_setPresetFn)(uint16_t, float) = 0, float (*_getPresetFn)(uint16_t) = 0);
    virtual ~FmBanksScreen();

// Class Variables
//
public:


private:

    ButtonPanel             presetSelectPanel;
    ButtonPanel             bankSelectPanel;

    int16_t                 displayAreaX1;
    int16_t                 displayAreaX2;
    int16_t                 displayAreaWidth;

    uint16_t                currentPresetBase;
    uint16_t                currentPreset;
    int16_t                 presetSelectPanelX;
    int16_t                 presetSelectPanelY;
    uint16_t                presetSelectPanelWid;
    uint16_t                presetSelectPanelHgt;
    const char**            presetSelectLabels;

    uint16_t                currentBank;
    int16_t                 bankSelectPanelX;
    int16_t                 bankSelectPanelY;
    uint16_t                bankSelectPanelWid;
    uint16_t                bankSelectPanelHgt;
    const char**            bankSelectLabels;

    uint16_t                exitBtnWid;
    uint16_t                exitBtnHgt;
    int16_t                 exitBtnX;
    int16_t                 exitBtnY;

    uint16_t                presetNumId;
    void                    (*setPresetFn)(uint16_t, float);  // Function to call to select preset
    float                   (*getPresetFn)(uint16_t);

    static ScreenButtonInfo exitBtnInfo;
    ScreenButton            exitBtn;

protected:


// Class Methods
//
public:

    // Public functions visible to main application
    void                    activateScreen() override;      // clear and display screen
    void                    deActivateScreen() override;    // save screen parameters before closing
    ButtonId                pollScreen();

private:

    // Class internal functions
    void                    drawPresetsPanel();
    void                    drawBanksPanel();
    void                    drawExitButton();
    void                    selectPreset();

protected:

};

#endif // FMBANKSCREEN_H_
