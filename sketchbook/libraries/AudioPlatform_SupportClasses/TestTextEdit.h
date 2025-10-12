/************************************************************************************
*                                   TestTextEdit.h                                  *
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

#ifndef TESTTEXTEDIT_H_
#define TESTTEXTEDIT_H_

// Teensyduino
//
//#include <Wire.h>
//#include <SPI.h>
//#include <SD.h>
//#include <FS.h>
//#include <SerialFlash.h>
//#include <LittleFS.h>

// User Arduino Libraries
//
#include "AudioPlatformGlobal.h"
#include "DisplayScreenBase.h"
#include "TextEdit.h"

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

class TestTextEdit : public DisplayScreenBase {

// Constructor/Destructor
//
public:

    TestTextEdit(DisplayScreenInfo& displayInfo);
    virtual ~TestTextEdit();

// Class Variables
//
public:


private:

    TextEdit        textEd;

    // Edit button (calls up AlphaNum edit screen)
    uint16_t        editBtnWid;
    uint16_t        editBtnHgt;
    int16_t         editBtnX;
    int16_t         editBtnY;
    const char*     editBtnText = "Edit Text";

    char            textToEdit[TextEditBufferSize];
    uint16_t        textFieldWidth;
    uint16_t        textFieldHeight;
    uint16_t        textFieldX;
    uint16_t        textFieldY;

protected:


// Class Methods
//
public:

    void        activateScreen() override;      // clear and display screen
    void        deActivateScreen() override;    // save screen parameters before closing
    ButtonId    pollScreen() override;

private:

    void        showScreen();
    void        showText();
    void        drawEditButton();
    void        drawTextDisplayField();

protected:

};

#endif // TESTTEXTEDIT_H_
