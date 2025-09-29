/************************************************************************************
*                                  StatusDisplay.h                                  *
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

#ifndef STATUSDISPLAY_H_
#define STATUSDISPLAY_H_

// User Arduino Libraries
//
#include "DisplayScreenBase.h"
#include "UtilFns.h"
#include "OnOffDisplay.h"
#include "NumberNameDisplay.h"

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

class StatusDisplay {

// Constructor/Destructor
//
public:

    StatusDisplay();
    virtual ~StatusDisplay();

// Class Variables
//
public:

    bool                displayInitialized;

private:

    // Current preset
    uint16_t            currentPresetNumber;
    // FFS: How to access Presets::PresetNameSize for buffer length
    char                currentPresetName[32];

    // Current preset devices status
    bool                wavPlayerEnabled;
    bool                rawPlayerEnabled;
    bool                basicSynthEnabled;
    bool                fmSynthEnabled;
    bool                lineInEnabled;

protected:

    OnOffDisplay        wavPlyrStatusDisplay;
    OnOffDisplay        rawPlyrStatusDisplay;
    OnOffDisplay        synthStatusDisplay;
    OnOffDisplay        fmSynthStatusDisplay;
    OnOffDisplay        lineInStatusDisplay;

    NumberNameDisplay   presetDisplay;

// Class Methods
//
public:

    void                initialize();
    void                updateDisplay(StatusUpdatePacketType pktType, void* updateInfoPkt);

private:

    void                drawDisplay();

protected:

};

#endif // STATUSDISPLAY_H_
