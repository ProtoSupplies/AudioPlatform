/************************************************************************************
*                                  ScreenButton.h                                   *
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

#ifndef SCREENBUTTON_H_
#define SCREENBUTTON_H_

// User Arduino Libraries
//
#include "DisplayScreenBase.h"
#include "Bitmap.h"

// Teensyduino
//
#include <elapsedMillis.h>
#include <IntervalTimer.h>

/************************************************************************************
*                         M A C R O S  /  C O N S T A N T S                         *
*************************************************************************************
*/

/************************
* ScreenButton Defaults *
*************************
*/

#define SCREENBUTTON_COLOR              COLOR_GRAY
#define SCREENBUTTON_OUTLINE_COLOR      COLOR_WHITE
#define SCREENBUTTON_HIGHLIGHT_COLOR    COLOR_RED
#define SCREENBUTTON_OUTLINE_RADIUS     4
#define SCREENBUTTON_TEXT_COLOR         COLOR_WHITE
#define SCREENBUTTON_TEXT_FONT          Arial_12_Bold
#define SCREENBUTTON_TEXT_ALIGN         TextAlignCenter
#define SCREENBUTTON_TEXT_POSITION      TextInFront

#define SCREENBUTTON_ON_TEXT_COLOR      COLOR_RED
#define SCREENBUTTON_OFF_TEXT_COLOR     COLOR_WHITE

/************************************************************************************
*                     D A T A / T Y P E   D E F I N I T I O N S                     *
*************************************************************************************
*/

// Screen Button Profile
//
typedef struct {
    int16_t             btnX;
    int16_t             btnY;
    uint16_t            btnWid;
    uint16_t            btnHgt;
    uint16_t            btnColor;
    uint16_t            btnOutlineColor;
    uint8_t             btnOutlineRadius;
    const char*         btnLabel;
    uint16_t            btnLabelColor;
    ILI9341_t3_font_t   btnLabelFont;
    TextAlign           btnLabelAlign;
    TextPosition        btnLabelPosition;
    BitMapDataBlock     bitMapData;
} ScreenButtonInfo;

typedef enum : uint8_t {

    ScrnBtnOff      = 0,
    ScrnBtnOn       = 1,
    ScrnBtnFlash    = 2

} ScreenButtonState;

/************************************************************************************
*                         C L A S S   D E F I N I T I O N S                         *
*************************************************************************************
*/

class ScreenButton {

// Constructor/Destructor
//
public:

    ScreenButton(ScreenButtonInfo& buttonInfo_);
    virtual ~ScreenButton();

// Class Variables
//
public:

    ScreenButtonState   btnState;

private:


protected:

    ScreenButtonInfo&   btnInfo;

    bool                btnActive;


// Class Methods
//
public:

    ScreenButtonInfo&   getBtnInfo();
    bool                buttonTouched(int16_t pointX, int16_t pointY);
    void                drawButton();
    void                drawLabel(uint16_t labelColor);
    void                removeButton();
    void                setState(ScreenButtonState btnState);
    ScreenButtonState   toggleState();

private:


protected:


};

#endif // SCREENBUTTON_H_
