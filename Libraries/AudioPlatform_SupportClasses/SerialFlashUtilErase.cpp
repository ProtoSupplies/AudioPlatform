/************************************************************************************
*                                SerialFlashUtilErase.cpp                               *
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

// User Arduino Libraries
//
#include "SerialFlashUtilErase.h"

/************************************************************************************
*                         M A C R O S  /  C O N S T A N T S                         *
*************************************************************************************
*/

#define CANCEL_BTN_INDEX            0
#define ERASE_BTN_INDEX             1

#define POPUP_HEADING_FONT          Arial_20_Bold
#define POPUP_HEADING_FONT_COLOR    COLOR_WHITE
#define USER_MSG_FONT               Arial_12_Bold
#define USER_MSG_FONT_COLOR         COLOR_WHITE
#define STATUS_DISPLAY_FONT         Arial_14_Bold
#define STATUS_DISPLAY_FONT_COLOR   COLOR_WHITE
#define SF_SCREEN_BGND_COLOR        COLOR_BLACK

const float EstBytesErasedPerSec    = 1100145;  // Empirically Determined: Winbond W25Q01JV*Q / Teensy 4.1 (600 MHz)

/************************************************************************************
*                    S T A T I C   C L A S S   V A R I A B L E S                    *
*************************************************************************************
*/

/************************************************************************************
*                    P U B L I C   C L A S S   F U N C T I O N S                    *
*************************************************************************************
*/

// Constructor/Destructor
//
SerialFlashUtilErase::SerialFlashUtilErase(SystemHardwareProfile& _sysProfile) :
    SerialFlashUtilBase(_sysProfile) {

    secondsCounter = 0;

    progressDisplayPanel.setButtonShape(false);      // Use square buttons
    progressDisplayPanel.setPanelAttributes(150, 30, progressDisplayPanel.btnGapHorizontal, progressDisplayPanel.btnGapVertical);
    progressDisplayPanelX = (DISPLAY_WIDTH - progressDisplayPanel.panelWidth) / 2;
    progressDisplayPanelY = 190;

    infoMsgAreaY        = 100;
    infoMsgAreaHgt      = 60;
    statusMsgAreaY      = 300;
    statusMsgAreaHgt    = 60;
}

SerialFlashUtilErase::~SerialFlashUtilErase() {

}

// showPopup
//
void SerialFlashUtilErase::showPopup() {

//    Serial.printf("SerialFlashUtilErase::showPopup:\n");

    SerialFlashUtilBase::showPopup();

    // Check Serial Flash Availability
    serialFlashPresent = sysProfile.serialFlashPresent;
    if (serialFlashPresent) {
        serialFlashBytes = sysProfile.serialFlashSizeMB * BytesPerMegaByte;
        uint16_t eraseTime = ((float)serialFlashBytes / EstBytesErasedPerSec);
        snprintf(messageBuf, TempBufLength-1, "Serial Flash Size: %ld MB. Estimated erase time: %d seconds.", sysProfile.serialFlashSizeMB, eraseTime);
        messageBuf[TempBufLength-1] = '\0';
        showInfoMsg(messageBuf);

    } else {
        showInfoMsg("Unable to locate Serial Flash device.");
    }

    pollEnabled = true;
}

// showObjects
//
void SerialFlashUtilErase::showObjects() {

//    Serial.printf("SerialFlashUtilErase::showObjects: app %s\n", sysProfile.appName);

    // Re-establish font settings (not set at compile time)
    execPanel.btnFont = execPanelInfo.btnFont;

    // Draw Execute Panel
    execPanel.setPanelPosition(execPanelX, execPanelY);
    execPanel.drawPanel(execPanelBtnInfo, currentExecBtn);
    execPanel.setSelected(execPanelBtnInfo, currentExecBtn);

    // Draw Seconds Display Panel
    progressDisplayPanel.setPanelPosition(progressDisplayPanelX, progressDisplayPanelY);
    progressDisplayPanel.drawPanelButtons();
}

/************************************************************************************
*        P R I V A T E / P R O T E C T E D   C L A S S   F U N C T I O N S          *
*************************************************************************************
*/

// showRemainingTime
//
void SerialFlashUtilErase::showRemainingTime(uint16_t seconds) {

    snprintf(messageBuf, TempBufLength-1, "%d Seconds Remaining", seconds);
    messageBuf[TempBufLength-1] = '\0';
    showStatusMsg(messageBuf);
}

// showProgress
//
void SerialFlashUtilErase::showProgress(int16_t progressParam) {

    if (progressParam == ProgressPanelReset) {
        progressDisplayPanel.drawPanelButtons();
    } else {
        progressDisplayPanel.drawButton(progressParam, true);
    }
}

#define TEST_REAL_ERASE 1

// execute
//
void SerialFlashUtilErase::execute() {

//    Serial.printf("SerialFlashUtilErase::execute:\n");

    if (serialFlashBytes > 0) {

        execPanel.btnTextColor = COLOR_RED;
        execPanel.setSelected(execPanelBtnInfo, 1);

        snprintf(messageBuf, TempBufLength-1, "Erasing Serial Flash contents. Erase cannot be interrupted.");
        messageBuf[TempBufLength-1] = '\0';
        showInfoMsg(messageBuf);

        uint16_t eraseTime = 3 + ((float)serialFlashBytes / EstBytesErasedPerSec);

        // Reset buttons from previous erase
        if (secondsCounter > 0) {
            showProgress(ProgressPanelReset);
            secondsCounter = 0;
        }

        // Reset timer and counter
        oneSecondTimer = 0;
        secondsCounter = 0;

#if TEST_REAL_ERASE // Testing Real Erase
        showRemainingTime(eraseTime);

        // Erase the flash
        SerialFlash.eraseAll();

        while (!SerialFlash.ready()) {  // Wait for completion
#else // Simulated Erase
        eraseTime = 10;
        showRemainingTime(eraseTime);
        while (secondsCounter < eraseTime) {  // Wait for completion
#endif // Testing Real Erase / Simulated Erase
            // TODO: Allow user exit

            if (oneSecondTimer >= MillisPerSecond) {
                // Highlight next button progressDisplayPanel
                showProgress(secondsCounter);

                // Reset timer and show remaining time
                oneSecondTimer = 0;
                secondsCounter++;
                if (secondsCounter <= eraseTime) {
                    showRemainingTime(eraseTime- secondsCounter);
                }
            }
        }
        //        Serial.printf("SerialFlashUtilErase::eraseSerialFlashFiles: Erase Completed. Elapsed time: %d seconds\n", secondsCounter);

#if TEST_REAL_ERASE // Testing Real Erase
        // Update global file list and count, sort list
        if (sysProfile.sfRawFileUpdateFn) {
            (*sysProfile.sfRawFileUpdateFn)();
        }
#endif // Testing Real Erase
    }

    execPanel.btnTextColor = COLOR_BLACK;
    execPanel.setSelected(execPanelBtnInfo, 0);

    snprintf(messageBuf, TempBufLength-1, "Serial Flash Erase Complete: Elapsed erase time: %d seconds.", secondsCounter);
    messageBuf[TempBufLength-1] = '\0';
    showInfoMsg(messageBuf);
}

