/************************************************************************************
*                                   TextColumn.h                                    *
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


/************************************************************************************
*                       E X T E R N A L   R E F E R E N C E S                       *
*************************************************************************************
*/

#ifndef TEXTCOLUMN_H_
#define TEXTCOLUMN_H_

// User Arduino Libraries
//
#include "DisplayScreenBase.h"
#include "Teensy41_Util.h"

/************************************************************************************
*                         M A C R O S  /  C O N S T A N T S                         *
*************************************************************************************
*/

/******************
* Column Defaults *
*******************
*/

#define COLUMN_WIDTH_DEFAULT                150
#define COLUMN_HEIGHT_DEFAULT               360
#define COLUMN_LINE_HEIGHT_DEFAULT          28
#define COLUMN_MAX_LINES_DEFAULT            16
#define COLUMN_FONT_DEFAULT                 Arial_12_Bold
#define COLUMN_TEXT_COLOR_DEFAULT           COLOR_WHITE
#define COLUMN_TEXT_MARGIN_DEFAULT          10
#define COLUMN_TEXT_ALIGN_DEFAULT           TextAlignCenter
#define COLUMN_COLOR_DEFAULT                COLOR_BLACK
#define COLUMN_OUTLINE_COLOR_DEFAULT        COLOR_BLACK
#define COLUMN_SEL_OUTLINE_COLOR_DEFAULT    COLOR_MAROON

/************************************************************************************
*                     D A T A / T Y P E   D E F I N I T I O N S                     *
*************************************************************************************
*/

typedef struct {

    uint16_t            xPos;
    uint16_t            yPos;
    uint16_t            width;
    uint16_t            height;
    uint16_t            lineHeight;
    uint16_t            maxLines;
    ILI9341_t3_font_t   font;
    uint16_t            textColor;
    uint16_t            textMargin;
    TextAlign           textAlign;
    uint16_t            columnColor;
    uint16_t            outlineColor;
    uint16_t            selectedOutlineColor;

} TextColumnInfo;

/************************************************************************************
*                         C L A S S   D E F I N I T I O N S                         *
*************************************************************************************
*/

class TextColumn {

// Constructor/Destructor
//
public:

    TextColumn(TextColumnInfo* columnInfo_);
    TextColumn();
    virtual ~TextColumn();

// Class Variables
//
public:

    TextColumnInfo* columnInfo;
    uint16_t        linesPerSubColumn;
    uint8_t         subColumnCount;

    // NOTE: In TextColumn, "Lines" in the column (generally filled with text) are considered
    // to be "Buttons" for the purpose of identification in polling functions, which operate
    // in terms of buttons rather than lines.
    ButtonId        selectedLine;

private:


protected:


// Class Methods
//
public:

    void            setColumnInfo(TextColumnInfo* columnInfo_);
    void            showColumnInfo();
    void            drawColumnCells();
    void            drawColumnBgnd();
    void            printTextLine(ButtonId lineNum, const char* text);
    bool            isColumnLine(int16_t pointX, int16_t pointY, ButtonId& btnNum);
    void            setSelectedLine(ButtonId newSelectedLine);
    void            unsetSelectedLine();
    ButtonId        getSelectedLine();

private:


protected:

};

#endif // TEXTCOLUMN_H_
