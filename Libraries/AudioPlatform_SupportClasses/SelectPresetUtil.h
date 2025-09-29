/************************************************************************************
*                                SelectPresetUtil.h                                 *
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

#ifndef SELECTPRESETUTIL_H_
#define SELECTPRESETUTIL_H_

// User Arduino Libraries
//
#include "AudioPlatformGlobal.h"
#include "Teensy41_Util.h"
#include "DisplayScreenBase.h"
#include "ScreenButton.h"
#include "ButtonPanel.h"

/************************************************************************************
*                E X P O R T E D   M A C R O S  /  C O N S T A N T S                *
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

class SelectPresetUtil {

// Constructor/Destructor
//
public:
    SelectPresetUtil();
    virtual ~SelectPresetUtil();

// Class Variables
//
public:

    bool                        pollEnabled;
    uint16_t                    selectedPreset;

private:

    static ButtonPanelInfo      presetsPanelInfo;
    static ButtonPanel          presetsPanel;
//    ButtonPanel                 presetsPanel;
    const char**                presetsPanelBtnLabels;

    // Exit Panel Buttons
    static ButtonPanelInfo      exitPanelInfo;
    static LabelIdButtonInfo    exitPanelBtnInfo[];
    ButtonPanel                 exitPanel;
    ButtonId                    currentExitBtn;

    const char*                 popupHeading;

    uint32_t                    lastPresetSelectMillis;     // ms time of last preset button touch

protected:


// Class Methods
//
public:

    virtual void        showPopup();
    virtual void        removePopup();
    virtual ButtonId    pollPopup();
    void                setPopupHeading(const char* text);
    void                setBtnLabels(const char** btnLabels);

private:

    virtual void        showObjects();
    void                showPopupHeading();
    void                setSelectedPreset(ButtonId btnNum);

protected:

};

#endif // SELECTPRESETUTIL_H_
