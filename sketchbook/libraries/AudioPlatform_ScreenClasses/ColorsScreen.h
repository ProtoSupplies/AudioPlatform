/************************************************************************************
*                                   ColorsScreen.h                                  *
*                                                                                   *
*                       Copyright (c) 2025, Donald Swearingen                       *
*                                All Rights Reserved                                *
*************************************************************************************
*
* Description
*
*   See ColorsScreen.cpp for additional details.
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

#ifndef COLORSSCREEN_H_
#define COLORSSCREEN_H_

// User Arduino Libraries
//
#include "DisplayScreenBase.h"
#include "ButtonPanel.h"

/************************************************************************************
*                         M A C R O S  /  C O N S T A N T S                         *
*************************************************************************************
*/

/************************************************************************************
*                     D A T A / T Y P E   D E F I N I T I O N S                     *
*************************************************************************************
*/

const uint8_t   BoxWidthDefault     = 35;
const uint8_t   BoxHeightDefault    = 35;
const uint8_t   BoxGapHorizDefault  = 5;
const uint8_t   BoxGapVertDefault   = 10;

/************************************************************************************
*                         C L A S S   D E F I N I T I O N S                         *
*************************************************************************************
*/

class ColorsScreen : public DisplayScreenBase {

// Constructor/Destructor
//
public:

    ColorsScreen(DisplayScreenInfo& displayInfo);
    virtual ~ColorsScreen();

// Class Variables
//
public:

    static const uint16_t*  colorDefs;
    static const char**     colorLabels;

private:

    uint8_t     boxCount;
    uint8_t     boxWid;
    uint8_t     boxHgt;
    uint8_t     boxGapHoriz;
    uint8_t     boxGapVert;

    uint8_t     boxesPerRow;
    uint8_t     rowCount;
    uint16_t    boxGridX;
    uint16_t    boxGridY;
    uint16_t    boxGridWid;
    uint16_t    boxGridHgt;

    ButtonId    selectedBtn;

protected:


// Class Methods
//
public:

    void        activateScreen() override;      // clear and display screen
    void        deActivateScreen() override;    // save screen parameters before closing
    ButtonId    pollScreen();
    void        setSelected(ButtonId selectedBtn = BtnNone);
    void        drawBtnOutline(ButtonId selectedBtn, bool btnActive);
    void        showSelectedBtnLabel(ButtonId selectedBtn);

private:

    bool        isColorButton(int16_t pointX, int16_t pointY, ButtonId& btnNum);

protected:


};

#endif // COLORSSCREEN_H_
