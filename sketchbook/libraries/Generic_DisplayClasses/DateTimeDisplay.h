/************************************************************************************
*                                 DateTimeDisplay.h                                 *
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

#ifndef DATETIMEDISPLAY_H_
#define DATETIMEDISPLAY_H_

// Teensyduino Libraries
//
#include <Time.h>

// User Arduino Libraries
//
#include "DisplayScreenBase.h"
//#include "font_LiberationMonoBold.h"
#include "font_CourierNewBold.h"

/************************************************************************************
*                         M A C R O S  /  C O N S T A N T S                         *
*************************************************************************************
*/

#define CELL_MONTH  0
#define CELL_DAY    1
#define CELL_YEAR   2
#define CELL_HOUR   3
#define CELL_MINUTE 4
#define CELL_SECOND 5

/******************
* Column Defaults *
*******************
*/

//extern const ILI9341_t3_font_t CourierNew_24_Bold;

#define DATETIME_CELL_WIDTH_DEFAULT     50
#define DATETIME_CELL_HEIGHT_DEFAULT    50
#define DATETIME_CELL_GAP_DEFAULT       5
#define DATETIME_YEAR_TIME_GAP_DEFAULT  30

#define DATETIME_BGND_COLOR_DEFAULT     BTN_COLOR_DEFAULT
#define DATETIME_OUTLINE_COLOR_DEFAULT  BTN_OUTLINE_COLOR_DEFAULT
#define DATETIME_TEXT_COLOR_DEFAULT     BTN_TEXT_COLOR_DEFAULT

#define DATETIME_NEW_BGND_COLOR_DEFAULT BTN_SEL_COLOR_DEFAULT
#define DATETIME_NEW_TEXT_COLOR_DEFAULT BTN_SEL_TEXT_COLOR_DEFAULT

#define DATETIME_FONT_DEFAULT           CourierNew_24_Bold

/************************************************************************************
*                     D A T A / T Y P E   D E F I N I T I O N S                     *
*************************************************************************************
*/

typedef struct {

    uint16_t            cellWidth;
    uint16_t            cellHeight;
    uint16_t            cellGap;
    uint16_t            yearTimeGap;
    uint16_t            bgndColor;
    uint16_t            outlineColor;
    uint16_t            fontColor;
    ILI9341_t3_font_t   cellValueFont;
    ILI9341_t3_font_t   cellLabelFont;
    bool                showCellLabels;

} DateTimeDisplayInfo;

const uint8_t DateTimeCellCount = 6;

typedef struct {

    uint16_t            valueMin;
    uint16_t            valueMax;

} DateTimeCellLimits;

/************************************************************************************
*                         C L A S S   D E F I N I T I O N S                         *
*************************************************************************************
*/

class DateTimeDisplay {

// Constructor/Destructor
//
public:

    DateTimeDisplay(DateTimeDisplayInfo& dispInfo, uint16_t dispX, uint16_t dispY);
    virtual ~DateTimeDisplay();

// Class Variables
//
public:

    bool                    touchActive;

    DateTimeDisplayInfo&    displayInfo;
    uint16_t                displayX;
    uint16_t                displayY;

    uint8_t                 month;
    uint8_t                 day;
    uint8_t                 year;
    uint8_t                 hour;
    uint8_t                 minute;
    uint8_t                 second;

private:

    uint8_t                 monthDays[12] = {31,28,31,30,31,30,31,31,30,31,30,31};

    DateTimeCellLimits      dtCellLimits[DateTimeCellCount] = {{1, 12}, {1, 31}, {0, 99}, {0, 23}, {0, 59}, {0, 59}};
    uint16_t                dtCellValues[DateTimeCellCount] = {1, 1, 0, 0, 0, 0};

    TextArea                titleTextArea;
    TextArea                cellValueTextArea;
    TextArea                cellLabelTextArea;
    bool                    showCellLabels;

    ButtonId                touchedCell;

    uint16_t                cellValue;
    uint16_t                cellValuePrev;
    uint16_t                lastTouchedPointY;

protected:


// Class Methods
//
public:

    void                    initialize();
    void                    drawDisplay();
    void                    pollDisplay(int16_t pointX, int16_t pointY);
    void                    pollEnd();
    bool                    displayCellTouched(int16_t pointX, int16_t pointY, ButtonId& cellId);
    void                    updateDisplay(DateTimeFields* dt, bool forceDisplay = false);
    void                    updateDisplayCell(uint8_t cellNum, uint8_t cellValue);
    uint16_t                getCellValue(uint8_t cellNum);

private:

    uint16_t                cellXpos(uint8_t cellNum);
    void                    handleTouchBegin();
    void                    handleTouchEnd();

protected:

};

#endif // DATETIMEDISPLAY_H_
