/************************************************************************************
*                              SystemProfileScreen.cpp                              *
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
#include "SystemProfileScreen.h"
#include "ArduinoEclipseDefs.h"
#include "UtilFns.h"
#include "Teensy41_Util.h"

// Teensyduino
//
extern "C" uint8_t external_psram_size;

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
SystemProfileScreen::SystemProfileScreen(DisplayScreenInfo& displayInfo, uint8_t columnCount_, TextColumnInfo* columnInfo_, SystemHardwareProfile& sysProfile_) :
    TextColumnScreen(displayInfo, columnCount_, columnInfo_), sysProfile(sysProfile_) {

//    getSystemInfo();
}

SystemProfileScreen::~SystemProfileScreen() {

}

// activateScreen
//
void SystemProfileScreen::activateScreen() {

//    Serial.printf("SystemProfileScreen::activateScreen\n");
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
void SystemProfileScreen::deActivateScreen() {

}

/************************************************************************************
*                   P R I V A T E   C L A S S   F U N C T I O N S                   *
*************************************************************************************
*/

// showScreen
//
void SystemProfileScreen::showScreen() {

    char dispBuf[64];

    uint16_t lineNum = 1;
    uint16_t labelColumn = 1;
    uint16_t valueColumn = 2;

    snprintf(dispBuf, sizeof(dispBuf), "%s", sysProfile.appName);
    printTextLineInColumn(labelColumn, lineNum, "Application:");
    printTextLineInColumn(valueColumn, lineNum++, dispBuf);

    snprintf(dispBuf, sizeof(dispBuf), "%s %s", sysProfile.buildDate, sysProfile.buildTime);
    printTextLineInColumn(labelColumn, lineNum, "Build Date:");
    printTextLineInColumn(valueColumn, lineNum++, dispBuf);

    snprintf(dispBuf, sizeof(dispBuf), "%s", sysProfile.appVersion);
    printTextLineInColumn(labelColumn, lineNum, "Version:");
    printTextLineInColumn(valueColumn, lineNum++, dispBuf);

    snprintf(dispBuf, sizeof(dispBuf), "%s", sysProfile.teensyMAC);
    printTextLineInColumn(labelColumn, lineNum, "Teensy 4.1 MAC:");
    printTextLineInColumn(valueColumn, lineNum++, dispBuf);

    snprintf(dispBuf, sizeof(dispBuf), "%lu MB", sysProfile.psRamSize);
    printTextLineInColumn(labelColumn, lineNum, "PSRAM/EXTMEM Size:");
    printTextLineInColumn(valueColumn, lineNum++, dispBuf);

#if 0    
    if (sysProfile.qspiNorFlashPresent) {
        snprintf(dispBuf, sizeof(dispBuf), "%lu MB (NOR)", sysProfile.qspiNorFlashSize);
    }
    else if (sysProfile.qspiNandFlashPresent) {
        snprintf(dispBuf, sizeof(dispBuf), "%lu MB (NAND)", sysProfile.qspiNandFlashSize);
    }
    else {
        snprintf(dispBuf, sizeof(dispBuf), "0 MB (Not Found)");
    }
    printTextLineInColumn(labelColumn, lineNum, "QSPI Flash Size:");
    printTextLineInColumn(valueColumn, lineNum++, dispBuf);
#endif
    
    if (sysProfile.serialFlashPresent) {
        snprintf(dispBuf, sizeof(dispBuf), "%lu MB", sysProfile.serialFlashSizeMB);
    } else {
        snprintf(dispBuf, sizeof(dispBuf), "0 MB (Not Found)");
    }
    printTextLineInColumn(labelColumn, lineNum, "Serial Flash Size:");
    printTextLineInColumn(valueColumn, lineNum++, dispBuf);

    if (sysProfile.serialFlashPresent) {
        snprintf(dispBuf, sizeof(dispBuf), "%d", sysProfile.sfRawFileCount);
    } else {
        snprintf(dispBuf, sizeof(dispBuf), "Serial Flash Not Found");
    }
    printTextLineInColumn(labelColumn, lineNum, "Serial Flash Raw File Count:");
    printTextLineInColumn(valueColumn, lineNum++, dispBuf);

    if (sysProfile.sdCardPresent) {
        snprintf(dispBuf, sizeof(dispBuf), "Total %lu MB, Used %lu MB", sysProfile.sdCardSizeMB, sysProfile.sdUsedSizeMB);
    } else {
        snprintf(dispBuf, sizeof(dispBuf), "0 MB (Not Found)");
    }
    printTextLineInColumn(labelColumn, lineNum, "SD Card Size:");
    printTextLineInColumn(valueColumn, lineNum++, dispBuf);

    if (sysProfile.sdCardPresent) {
        snprintf(dispBuf, sizeof(dispBuf), "%d", sysProfile.sdWavFileCount);
    } else {
        snprintf(dispBuf, sizeof(dispBuf), "No WAV files present");
    }
    printTextLineInColumn(labelColumn, lineNum, "WAV File Count:");
    printTextLineInColumn(valueColumn, lineNum++, dispBuf);
}
