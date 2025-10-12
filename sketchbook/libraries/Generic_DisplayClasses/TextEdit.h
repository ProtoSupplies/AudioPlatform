/************************************************************************************
*                                  TextEdit.h                                  *
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

#define TESTING_TEXTEDITPOPUP   1

#if TESTING_TEXTEDITPOPUP
#else // TEXTEDITPOPUP
#endif // TESTING_TEXTEDITPOPUP / TEXTEDITPOPUP

/************************************************************************************
*                       E X T E R N A L   R E F E R E N C E S                       *
*************************************************************************************
*/

#ifndef TEXTEDITPOPUP_H_
#define TEXTEDITPOPUP_H_

// User Arduino Libraries
//
#include "DisplayScreenBase.h"
#include "ButtonPanel.h"

/************************************************************************************
*                         M A C R O S  /  C O N S T A N T S                         *
*************************************************************************************
*/

// Text Display Field Constants
//
#define TEXTFIELD_COLOR             COLOR_WHEAT
#define TEXTFIELD_OUTLINE_COLOR     COLOR_WHITE
#define TEXTFIELD_FONT              Arial_14
#define TEXTFIELD_FONT_COLOR        COLOR_BLACK
#define TEXTFIELD_HEADING_FONT      Arial_14
#define TEXTFIELD_HEADING_FONT_COLOR COLOR_WHITE
#define TEXTFIELD_HEADING_DEFAULT   "Text Edit"

const uint8_t TextEditBufferSize    = 64;

/************************************************************************************
*                     D A T A / T Y P E   D E F I N I T I O N S                     *
*************************************************************************************
*/

/************************************************************************************
*                         C L A S S   D E F I N I T I O N S                         *
*************************************************************************************
*/

class TextEdit {

// Constructor/Destructor
//
public:

    TextEdit(uint16_t popupX, uint16_t popupY, uint8_t _maxTextLength);
    virtual ~TextEdit();

// Class Variables
//
public:

    bool                pollEnabled;

private:

    ButtonPanel         kbdPanel;
    ButtonPanel         casePanel;
    ButtonPanel         editPanel;
    ButtonPanel         exitPanel;

    static ButtonPanelInfo kbdInBtnPanelInfo;
    static ButtonPanelInfo casePanelInfo;
    static ButtonPanelInfo editPanelInfo;
    static ButtonPanelInfo exitPanelInfo;

    static const char*  kbdUpperBtnInfo[];
    static const char*  kbdLowerBtnInfo[];

    static LabelIdButtonInfo  casePanelBtnInfo[];
    static LabelIdButtonInfo  editPanelBtnInfo[];
    static LabelIdButtonInfo  exitPanelBtnInfo[];

    const char*         (*getTextFn)();
    void                (*setTextFn)(const char* presetName);

    const char**        keyPadBtnLabels;
    ButtonId            currentKbdBtn;

    uint8_t             maxTextLength;
    static uint16_t     textFieldWidth;
    static uint16_t     textFieldHeight;
    uint16_t            textFieldX;
    uint16_t            textFieldY;

    static uint16_t     kbdPanelX;
    static uint16_t     kbdPanelY;

    static uint16_t     casePanelX;
    static uint16_t     casePanelY;
    ButtonId            currentCaseBtn;

    static uint16_t     editPanelX;
    static uint16_t     editPanelY;
    ButtonId            currentEditBtn;

    static uint16_t     exitPanelX;
    static uint16_t     exitPanelY;
    ButtonId            currentExitBtn;

    TextArea            textFieldArea;
    char                textBuffer[TextEditBufferSize];
    uint8_t             textLength;                     // current length of text string in textBuffer
    const char*         textHeading;

    // ms to wait after key touch
    static uint16_t     WaitTimeAfterKeyTouch;

protected:


// Class Methods
//
public:

#if TESTING_TEXTEDITPOPUP
    void                showPopup();
    void                removePopup();
    ButtonId            pollPopup();
#else // TEXTEDITPOPUP
    void                showPopup() override;
    void                removePopup() override;
    ButtonId            pollPopup() override;
#endif // TESTING_TEXTEDITPOPUP / TEXTEDITPOPUP

    void                setMaxTextLength(uint8_t length);
    void                setTextValue(const char* text);
    const char*         getTextValue();
    void                setTextHeading(const char* text);

private:

    void                showTextHeading();
    void                drawKeyPad();
    void                drawActionPanels();
    void                clearTextDisplayArea();
    void                drawTextDisplayField();
    void                displayTextBuffer();
    void                clearTextBuffer();
    void                handleKbdBtn(char ch);
    void                handleCaseBtn(ButtonId btnIndex);
    void                handleEditBtn(ButtonId btnIndex);
    ButtonId            handleExitBtn(ButtonId btnIndex);
    void                addToTextBuffer(char ch);

protected:


};

#endif // TEXTEDITPOPUP_H_
