/************************************************************************************
*                                NumberNameDisplay.h                                *
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

#ifndef NUMBERNAMEDISPLAY_H_
#define NUMBERNAMEDISPLAY_H_

// User Arduino Libraries
//
#include "DisplayScreenBase.h"

/************************************************************************************
*                         M A C R O S  /  C O N S T A N T S                         *
*************************************************************************************
*/

/******************
* Column Defaults *
*******************
*/

#define NUMBERNAME_WIDTH_DEFAULT            150
#define NUMBERNAME_HEIGHT_DEFAULT           30
#define NUMBERNAME_FONT_DEFAULT             Arial_12
#define NUMBERNAME_COLOR_DEFAULT            COLOR_BLACK
#define NUMBERNAME_TEXT_COLOR_DEFAULT       COLOR_WHITE
#define NUMBERNAME_TEXT_MARGIN_DEFAULT      10
#define NUMBERNAME_OUTLINE_COLOR_DEFAULT    COLOR_WHITE

#define NUMBERNAME_INDICATOR_SIDE_LEN       10

/************************************************************************************
*                     D A T A / T Y P E   D E F I N I T I O N S                     *
*************************************************************************************
*/

typedef struct {

    uint16_t            bgndColor;
    ILI9341_t3_font_t   font;
    uint16_t            textColor;
    uint16_t            textMargin;
    uint16_t            outlineColor;

} NumberNameDisplayInfo;

/************************************************************************************
*                         C L A S S   D E F I N I T I O N S                         *
*************************************************************************************
*/

class NumberNameDisplay {

// Constructor/Destructor
//
public:

    NumberNameDisplay(NumberNameDisplayInfo& dispInfo, uint16_t dispX, uint16_t dispY, uint16_t dispHgt);
//    NumberNameDisplay() {};
    virtual ~NumberNameDisplay();

// Class Variables
//
public:


private:

    NumberNameDisplayInfo&  displayInfo;

    uint16_t                displayX;
    uint16_t                displayY;
    uint16_t                displayWidth;
    uint16_t                displayHeight;

    const char*             displayPrefix;
    uint16_t                displayNumber;
    const char*             displayName;

protected:


// Class Methods
//
public:

    void                    initialize();
    void                    setPosition(uint16_t xPos, uint16_t yPos);
    void                    setPrefix(const char* prefix);
    void                    setNumberAndName(uint16_t number, const char* name);
    void                    drawDisplay();
    uint16_t                getWidth();
    void                    clearToWidth();

private:

    TextArea                textArea;

protected:

};

#endif // NUMBERNAMEDISPLAY_H_
