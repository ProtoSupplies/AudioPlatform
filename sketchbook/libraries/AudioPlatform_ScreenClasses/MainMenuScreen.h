/************************************************************************************
*                                  MainMenuScreen.h                                 *
*                                                                                   *
*                       Copyright (c) 2025, Donald Swearingen                       *
*                                All Rights Reserved                                *
*************************************************************************************
*
* Description
*
*   See MainMenuScreen.cpp for additional details.
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

#ifndef MAINMENUSCREEN_H_
#define MAINMENUSCREEN_H_

// User Arduino Libraries
//
#include "DisplayScreenBase.h"
#include "UtilFns.h"
#include "Teensy41_Util.h"
#include "ButtonPanel.h"
#include "StatusDisplay.h"

/************************************************************************************
*                         M A C R O S  /  C O N S T A N T S                         *
*************************************************************************************
*/

#define DATETIME_DISPLAY_FONT   Arial_13

const uint16_t InfoDisplayHgt   = 35;

/************************************************************************************
*                     D A T A / T Y P E   D E F I N I T I O N S                     *
*************************************************************************************
*/

/************************************************************************************
*                         C L A S S   D E F I N I T I O N S                         *
*************************************************************************************
*/

class MainMenuScreen : public DisplayScreenBase {

// Constructor/Destructor
//
public:

    MainMenuScreen(DisplayScreenInfo& displayInfo, ButtonPanelInfo& screenPanelInfo, LabelIdButtonInfo *screenPanelBtnInfo);
    virtual ~MainMenuScreen();

// Class Variables
//
public:


private:

    // Panel for Display of command buttons
    ButtonPanel         screenPanel;
    LabelIdButtonInfo*  screenPanelLabelIdInfo;

protected:

    // Footer status display
    StatusDisplay       statusDisplay;

// Class Methods
//
public:

    void                activateScreen() override;      // clear and display screen
    void                deActivateScreen() override;    // save screen parameters before closing
    void                updateScreen(StatusUpdatePacketType pktType, void* updateInfoPkt) override;
    ButtonId            pollScreen() override;

private:

    void                updateDateAndTime(const char* timestr);

protected:


};

#endif // MAINMENUSCREEN_H_
