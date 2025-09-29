/************************************************************************************
*                                  SetTimeScreen.h                                  *
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

#ifndef SETTIMESCREEN_H_
#define SETTIMESCREEN_H_

// User Arduino Libraries
//
#include "AudioPlatformGlobal.h"
#include "DisplayScreenBase.h"
#include "Teensy41_Util.h"
#include "ScreenButton.h"
#include "UpDownButton.h"

#include "DateTimeDisplay.h"

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

class SetTimeScreen : public DisplayScreenBase {

// Constructor/Destructor
//
public:

    SetTimeScreen(DisplayScreenInfo& displayInfo, DateTimeDisplayInfo& dtDisplayInfo, DateTimeDisplayInfo& dtEnterInfo);
//    SetTimeScreen(DisplayScreenInfo& displayInfo, ScreenButtonInfo& enterTimeBtnInfo, DateTimeDisplayInfo& dtDisplayInfo, DateTimeDisplayInfo& dtEnterInfo);
    virtual ~SetTimeScreen();

// Class Variables
//
public:


private:

    static ScreenButtonInfo setTimeBtnInfo;
    ScreenButton            setTimeBtn;

//    UpDownButton            upDownBtn;
//    bool                    upDownBtnActive;
//    uint16_t                upDownCount;
//    uint16_t                upDownCountPrev;
//    uint16_t                lastTouchedPointY;

    DateTimeDisplay         dtDisplay;
    bool                    forceUpdate;
    DateTimeDisplay         dtEnter;

//    char                    textToEdit[TextEditBufferSize];
//    uint16_t                textFieldWidth;
//    uint16_t                textFieldHeight;
//    uint16_t                textFieldX;
//    uint16_t                textFieldY;

//    ButtonId                selectedDtEnterCell;

protected:


// Class Methods
//
public:

    void        activateScreen() override;      // clear and display screen
    void        deActivateScreen() override;    // save screen parameters before closing
    void        updateScreen(StatusUpdatePacketType pktType, void* pkt) override;
    ButtonId    pollScreen() override;

private:

    void        showScreen();
    void        setDateAndTime();


protected:

};

#endif // SETTIMESCREEN_H_
