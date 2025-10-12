/************************************************************************************
*                                TestTextColumns.cpp                                *
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

// Standard C++
//
#include <Math.h>
#include <string.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

// User Arduino Libraries
//
#include "TestTextColumns.h"
#include "ArduinoEclipseDefs.h"

/************************************************************************************
*                    S T A T I C   C L A S S   V A R I A B L E S                    *
*************************************************************************************
*/

// Initialization of static class variables
//

/************************************************************************************
*                    P U B L I C   C L A S S   F U N C T I O N S                    *
*************************************************************************************
*/

// Static Member Initializations
//

// Constructor/Destructor
//
TestTextColumns::TestTextColumns(DisplayScreenInfo& displayInfo, uint8_t columnCount_, TextColumnInfo* columnInfo_, SystemHardwareProfile& sysProfile_) :
    TextColumnScreen(displayInfo, columnCount_, columnInfo_), sysProfile(sysProfile_) {

}

TestTextColumns::~TestTextColumns() {

}

// activateScreen
//
void TestTextColumns::activateScreen() {

//    Serial.printf("TestTextColumns::activateScreen\n");
//    Serial.printf("   marginTop = %d\n", marginTop);
//    Serial.printf("   marginBottom = %d\n", marginBottom);
//    Serial.printf("   marginLeft = %d\n", marginLeft);
//    Serial.printf("   marginRight = %d\n", marginRight);
//    Serial.printf("   columnCount = %d\n", columnCount);
//    Serial.printf("   columnGap = %d\n", columnGap);

    // Clear and display screen
    TextColumnScreen::activateScreen();
    showScreen();
}

// deActivateScreen
//
void TestTextColumns::deActivateScreen() {

}

/************************************************************************************
*                   P R I V A T E   C L A S S   F U N C T I O N S                   *
*************************************************************************************
*/

// showScreen
//
void TestTextColumns::showScreen() {

    char** textSrc;
    uint16_t labelCount;

    for (uint8_t column = 1; column <= columnCount; column++) {

        textSrc = 0;
        labelCount = 0;

        switch (column) {
            case 1:
                textSrc = sysProfile.sdPresetBankFileNames;
                labelCount = sysProfile.sdPresetBankFileCount;
                break;

            case 2:
                textSrc = sysProfile.sfRawFileNames;
                labelCount = sysProfile.sfRawFileCount;
                break;
        }

        if (labelCount > 0) {
            for (int lineNum = 1; lineNum <= labelCount; lineNum++) {
                printTextLineInColumn(column, lineNum, textSrc[lineNum-1]);
            }
        }
    }
}

