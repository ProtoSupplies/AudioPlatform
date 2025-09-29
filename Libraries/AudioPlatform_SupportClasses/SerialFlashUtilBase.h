/************************************************************************************
*                                 SerialFlashUtilBase.h                                 *
*                                                                                   *
*                       Copyright (c) 2025, Donald Swearingen                       *
*                                All Rights Reserved                                *
*************************************************************************************
*
* Description
*
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

#ifndef SERIALFLASFUTILBASE_H_
#define SERIALFLASFUTILBASE_H_

// Teensyduino Libraries
//
#include <SerialFlash.h>
#include <SD.h>

// User Arduino Libraries
//
#include "AudioPlatformGlobal.h"
#include "DisplayScreenBase.h"
#include "UtilFns.h"
#include "ButtonPanel.h"

/************************************************************************************
*                         M A C R O S  /  C O N S T A N T S                         *
*************************************************************************************
*/

const int16_t  ProgressPanelReset = -1;

/************************************************************************************
*                     D A T A / T Y P E   D E F I N I T I O N S                     *
*************************************************************************************
*/

/************************************************************************************
*                         C L A S S   D E F I N I T I O N S                         *
*************************************************************************************
*/

class SerialFlashUtilBase {

// Constructor/Destructor
//
public:

    SerialFlashUtilBase(SystemHardwareProfile& _sysProfile);
    virtual ~SerialFlashUtilBase();

// Class Variables
//
public:

    bool                        pollEnabled;

private:


protected:

    SystemHardwareProfile&      sysProfile;

    // Execute Panel Buttons
    static ButtonPanelInfo      execPanelInfo;
    static LabelIdButtonInfo    execPanelBtnInfo[];
    ButtonPanel                 execPanel;
    uint16_t                    execPanelX;
    uint16_t                    execPanelY;
    ButtonId                    currentExecBtn;

    static ButtonPanelInfo      progressDisplayPanellInfo;
    ButtonPanel                 progressDisplayPanel;
    uint16_t                    progressDisplayPanelX;
    uint16_t                    progressDisplayPanelY;
    ButtonId                    currentProgressDisplayBtn;

    const char*                 popupHeading;

    uint16_t                    headingMsgAreaY;
    uint16_t                    headingMsgAreaHgt;
    uint16_t                    infoMsgAreaY;
    uint16_t                    infoMsgAreaHgt;
    uint16_t                    statusMsgAreaY;
    uint16_t                    statusMsgAreaHgt;

    // NOTE: Compiler requires that buffer must be static if in EXTMEM.
    // This means that all instances share the buffer, and should use
    // it only for transient values (that will be displayed or copied).
    static EXTMEM char          messageBuf[TempBufLength];

    bool                        serialFlashPresent;
    uint32_t                    serialFlashBytes;
    bool                        sdCardPresent;

// Class Methods
//
public:

    virtual void        showPopup();
    virtual void        removePopup();
    virtual void        showObjects();
    virtual ButtonId    pollPopup();
    void                setPopupHeading(const char* text);

private:


protected:

    void                showPopupHeading();
    void                showInfoMsg(const char* text);
    void                showStatusMsg(const char* text);
    virtual void        showProgress(int16_t progressParam) {};

    ButtonId            handleExecuteBtnPress(ButtonId btnId);
    virtual void        execute() {};

};

#endif // SERIALFLASFUTILBASE_H_
