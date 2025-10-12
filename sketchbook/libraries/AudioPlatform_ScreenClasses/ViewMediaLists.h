/************************************************************************************
*                                  ViewMediaLists.h                                 *
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

#ifndef VIEWMEDIALISTS_H_
#define VIEWMEDIALISTS_H_

// User Arduino Libraries
//
#include "AudioPlatformGlobal.h"
#include "Teensy41_Util.h"
#include "DisplayScreenBase.h"
#include "ScreenButton.h"
#include "ListView.h"

/************************************************************************************
*                E X P O R T E D   M A C R O S  /  C O N S T A N T S                *
*************************************************************************************
*/

/************************************************************************************
*                     D A T A / T Y P E   D E F I N I T I O N S                     *
*************************************************************************************
*/

typedef enum : ButtonId {

    ViewSd = 0,
    ViewFlash,
    ViewProgMem,
    ViewPresetBanks,
    ViewFmSynthBanks,
    ViewMediaPlayerFiles,
    ViewListIdCount

} ViewListId;

/************************************************************************************
*                         C L A S S   D E F I N I T I O N S                         *
*************************************************************************************
*/

class ViewMediaLists : public DisplayScreenBase {

// Constructor/Destructor
//
public:

    ViewMediaLists(DisplayScreenInfo& _displayInfo, ButtonPanelInfo& _viewPanelInfo, const char** _viewPanelBtnLabels, SystemHardwareProfile& _sysProfile);
    virtual ~ViewMediaLists();

// Class Variables
//
public:

    uint16_t                largeTempBufLineCount;

private:

    SystemHardwareProfile&  sysProfile;

    static TextColumnInfo   listViewColumnInfo;
    ListView                listView;

    static ScreenButtonInfo viewListBtnInfo;
    ScreenButton            viewListBtn;

    ButtonPanel             viewPanel;
    const char**            viewPanelBtnLabels;
    ViewListId              selectedView;

protected:


// Class Methods
//
public:

    void            activateScreen() override;      // clear and display screen
    void            deActivateScreen() override;    // save screen parameters before closing
    ButtonId        pollScreen() override;

private:

    void            printSdFileList();
    void            printSerialFlashFileList();
    void            printProgMemFileList();
    void            printPresetBanksFileList();
    void            printFmSynthBanksFileList();
    void            printMediaPlayerFileList();

    void            handleViewBtnPress();

    void            showScreen();
    void            showSd();
    void            showFlash();
    void            showProgMem();
    void            showPresetBanks();
    void            showFmSynthBanks();
    void            showMediaPlayerFiles();

protected:

};

#endif // VIEWMEDIALISTS_H_
