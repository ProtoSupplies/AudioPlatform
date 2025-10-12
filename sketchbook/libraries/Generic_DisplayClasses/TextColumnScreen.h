/************************************************************************************
*                                TextColumnScreen.h                                 *
*                                                                                   *
*                      Copyright (c) 2025, Donald Swearingen                        *
*                               All Rights Reserved                                 *
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

#define SHOW_TEXTCOLUMN_SETTINGS    0

#if SHOW_TEXTCOLUMN_SETTINGS
#endif // SHOW_TEXTCOLUMN_SETTINGS

/************************************************************************************
*                       E X T E R N A L   R E F E R E N C E S                       *
*************************************************************************************
*/

#ifndef TEXTCOLUMNSCREEN_H_
#define TEXTCOLUMNSCREEN_H_

// User Arduino Libraries
//
#include "TextColumn.h"

/************************************************************************************
*                         M A C R O S  /  C O N S T A N T S                         *
*************************************************************************************
*/

#define COLUMN_COUNT_MAX            6
#define COLUMN_COUNT_DEFAULT        4

#define TEXTCOL_SCRN_MARGIN_TOP     DEFAULT_SCRN_MARGIN_TOP
#define TEXTCOL_SCRN_MARGIN_BOTTOM  20
#define TEXTCOL_SCRN_MARGIN_LEFT    DEFAULT_SCRN_MARGIN_LEFT
#define TEXTCOL_SCRN_MARGIN_RIGHT   DEFAULT_SCRN_MARGIN_RIGHT

/************************************************************************************
*                     D A T A / T Y P E   D E F I N I T I O N S                     *
*************************************************************************************
*/

/************************************************************************************
*                         C L A S S   D E F I N I T I O N S                         *
*************************************************************************************
*/

class TextColumnScreen : public DisplayScreenBase {

// Constructor/Destructor
//
public:

    TextColumnScreen(DisplayScreenInfo& displayInfo, uint8_t columnCount_, TextColumnInfo columnInfo_[]);
    virtual ~TextColumnScreen();

// Class Variables
//
public:


private:


protected:

    TextColumn      columns[COLUMN_COUNT_MAX];
    uint8_t         columnCount;
    uint16_t        columnGap;

// Class Methods
//
public:

    void            activateScreen() override;      // clear and display screen
    void            deActivateScreen() override;    // save screen parameters before closing
    ButtonId        pollScreen() override;
#if SHOW_TEXTCOLUMN_SETTINGS
    void            showSettings(); // FIXME: Temp for initial testing
#endif // SHOW_TEXTCOLUMN_SETTINGS

    uint16_t        getLineCount(uint16_t columnNum);
    uint16_t        getLinesPerSubColumn(uint16_t columnNum);
    void            printTextLineInColumn(uint8_t columnNum, uint16_t lineNum, const char* text);

private:


protected:

};

#endif // TEXTCOLUMNSCREEN_H_
