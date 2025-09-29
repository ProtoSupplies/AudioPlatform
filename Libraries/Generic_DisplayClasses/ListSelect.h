/************************************************************************************
*                                 ListSelect.h                                 *
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

#ifndef LISTSELECT_H_
#define LISTSELECT_H_

// User Arduino Libraries
//
#include "ListView.h"
#include "TextColumn.h"
#include "ButtonPanel.h"
#include "Teensy41_Util.h"

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

class ListSelect : public ListView {

// Constructor/Destructor
//
public:

    ListSelect(TextColumnInfo* columnInfo);
    ListSelect();
    virtual ~ListSelect();

// Class Variables
//
public:

    bool                        pollEnabled;
    bool                        reportOnTouch;
    bool                        clearScreen;

private:

    // Select Panel Buttons
    static ButtonPanelInfo      selectPanelDefaultInfo;
    LabelIdButtonInfo*          selectPanelBtnInfo;
    static LabelIdButtonInfo    selectPanelDefaultBtnInfo[];
    ButtonPanel                 selectPanel;
    uint16_t                    selectPanelX;
    uint16_t                    selectPanelY;
    ButtonId                    currentSelectBtn;

    // List Entries
    uint16_t                    selectedItemIndex;

    // ms to wait after key touch before repeat
    static uint16_t             WaitTimeBeforeRepeat;

protected:


// Class Methods
//
public:

    void            showPopup() override;
    ButtonId        pollPopup() override;
    ButtonId        pollPopup(uint16_t pointX, uint16_t pointY);
    void            removePopup() override;

    void            setSelectedLine(ButtonId touchedLine);
    void            unsetSelectedLine();
    ButtonId        getSelectedLine();

    void            setPanelInfo(ButtonPanelInfo& panelInfo);
    void            setPanelBtnInfo(LabelIdButtonInfo* panelBtnInfo);

private:

    void            handleUpDownTouch(ButtonId btnId);
    bool            handleUpDownTouch(uint16_t pointX, uint16_t pointY);
    ButtonId        handleSelectBtnTouch(ButtonId btnIndex);
    ButtonId        handleListTouch(ButtonId btnIndex);

protected:

};

#endif // LISTSELECT_H_
