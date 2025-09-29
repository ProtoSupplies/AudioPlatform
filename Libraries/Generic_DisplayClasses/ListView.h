/************************************************************************************
*                                     ListView.h                                    *
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

#ifndef LISTVIEWPOPUP_H_
#define LISTVIEWPOPUP_H_

// User Arduino Libraries
//
#include "DisplayScreenBase.h"
#include "TextColumn.h"
#include "ButtonPanel.h"
#include "ScreenButton.h"
#include "font_LiberationMonoBold.h"
#include "font_CourierNewBold.h"

/************************************************************************************
*                         M A C R O S  /  C O N S T A N T S                         *
*************************************************************************************
*/

#define LISTVIEW_MARGIN_TOP     50
#define LISTVIEW_MARGIN_BOTTOM  30
#define LISTVIEW_MARGIN_LEFT    125
#define LISTVIEW_MARGIN_RIGHT   125
#define LISTVIEW_COLUMN_WIDTH   ((DISPLAY_WIDTH - (LISTVIEW_MARGIN_LEFT + LISTVIEW_MARGIN_RIGHT)) - 40)
#define LISTVIEW_COLUMN_HEIGHT  (DISPLAY_HEIGHT - (LISTVIEW_MARGIN_TOP + LISTVIEW_MARGIN_BOTTOM))
#define LISTVIEW_COLUMN_COLOR   COLOR_WHEAT
#define LISTVIEW_LINE_HEIGHT    25
#define LISTVIEW_MAX_LINES      16
#define LISTVIEW_TEXT_MARGIN    10
#define LISTVIEW_TEXT_ALIGN     TextAlignLeft
#define LISTVIEW_XPOS           ((DISPLAY_WIDTH - LISTVIEW_COLUMN_WIDTH) / 2)
#define LISTVIEW_YPOS           LISTVIEW_MARGIN_TOP

// Text Display Field Constants
//
#define LISTVIEW_BGND_COLOR        COLOR_WHEAT
#define LISTVIEW_OUTLINE_COLOR     COLOR_WHEAT
#define LISTVIEW_SEL_OUTLINE_COLOR COLOR_GRAY
#define LISTVIEW_FONT              LiberationMono_11_Bold
#define LISTVIEW_FONT_COLOR        COLOR_BLACK
#define LISTVIEW_HEADING_FONT      Arial_12_Bold
#define LISTVIEW_HEADING_FONT_COLOR COLOR_WHITE
#define LISTVIEW_HEADING_DEFAULT   "ListView"

/************************************************************************************
*                     D A T A / T Y P E   D E F I N I T I O N S                     *
*************************************************************************************
*/

/************************************************************************************
*                         C L A S S   D E F I N I T I O N S                         *
*************************************************************************************
*/

class ListView {

// Constructor/Destructor
//
public:

    ListView(TextColumnInfo* columnInfo);
    ListView();

    virtual ~ListView();

// Class Variables
//
public:

    bool                        pollEnabled;

private:

    // Text Display Column
    TextColumnInfo              textColumnInfo;

    // ms to wait after key touch before repeat
    static uint16_t             WaitTimeBeforeRepeat;

protected:

    // Exit Panel Buttons
    static ButtonPanelInfo      exitPanelInfo;
    static LabelIdButtonInfo    exitPanelBtnInfo[];
    ButtonPanel                 exitPanel;
    uint16_t                    exitPanelX;
    uint16_t                    exitPanelY;
    ButtonId                    currentExitBtn;

    // Up/Down Screen Buttons
    static ScreenButtonInfo     upBtnInfo;
    ScreenButton                upBtn;
    static ScreenButtonInfo     downBtnInfo;
    ScreenButton                downBtn;

    // Text Display Column
    TextColumn                  textColumn;
    char**                      columnList;
    uint16_t                    columnListLength;
    uint16_t                    columnListDisplayBase;

    const char*                 popupHeading;

// Class Methods
//
public:

    virtual void        showPopup();
    virtual void        removePopup();
    void                showBaseObjects();
    virtual ButtonId    pollPopup();

    void                setPopupHeading(const char* text);
    void                showPopupHeading();
    void                setList(char *list[], uint16_t listLength);
    void                showListSegment(uint16_t segmentBase);

    void                setTextColumnInfo(TextColumnInfo* columnInfo_);
    void                showTextColumnInfo();
    // Note: These functions provide indirect access to local TextColumn,
    void                setListDisplayX(uint16_t listX);
    void                setListDisplayY(uint16_t listY);
    void                setListDisplayWidth(uint16_t listWidth);
    void                setListDisplayHeight(uint16_t listHeight);
    void                setTextAlign(TextAlign align);

private:


protected:

    void                handleUpDownBtn(ButtonId btnId);
    void                printTextLineInColumn(uint16_t lineNum, const char* text);
};

#endif // LISTVIEWPOPUP_H_
