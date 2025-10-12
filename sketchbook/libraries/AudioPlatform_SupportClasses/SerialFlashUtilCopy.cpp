/************************************************************************************
*                              SerialFlashUtilCopy.cpp                              *
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
#include "SerialFlashUtilCopy.h"

/************************************************************************************
*                         M A C R O S  /  C O N S T A N T S                         *
*************************************************************************************
*/

#define CANCEL_BTN_INDEX            0
#define PROCEED_BTN_INDEX           1

#define POPUP_HEADING_FONT          Arial_20_Bold
#define POPUP_HEADING_FONT_COLOR    COLOR_WHITE
#define USER_MSG_FONT               Arial_12_Bold
#define USER_MSG_FONT_COLOR         COLOR_WHITE
#define STATUS_DISPLAY_FONT         Arial_16_Bold
#define STATUS_DISPLAY_FONT_COLOR   COLOR_WHITE
#define SF_SCREEN_BGND_COLOR        COLOR_BLACK

/************************************************************************************
*                    S T A T I C   C L A S S   V A R I A B L E S                    *
*************************************************************************************
*/

const uint16_t  SdToFlashCopyBufSize = 4096;
EXTMEM uint8_t  sdToFlashCopyBuf[SdToFlashCopyBufSize];

/************************************************************************************
*                    P U B L I C   C L A S S   F U N C T I O N S                    *
*************************************************************************************
*/

// Constructor/Destructor
//
SerialFlashUtilCopy::SerialFlashUtilCopy(SystemHardwareProfile& _sysProfile) :
    SerialFlashUtilBase(_sysProfile) {

    progressDisplayPanel.setButtonShape(false);      // Use square buttons
    progressDisplayPanel.setPanelAttributes(20, 20, progressDisplayPanel.btnGapHorizontal, progressDisplayPanel.btnGapVertical);
    progressDisplayPanelX = (DISPLAY_WIDTH - progressDisplayPanel.panelWidth) / 2;
    progressDisplayPanelY = 220;
    currentProgressDisplayBtn = 0;

    infoMsgAreaY        = 110;
    infoMsgAreaHgt      = 60;
    statusMsgAreaY      = 250;
    statusMsgAreaHgt    = 60;
}

SerialFlashUtilCopy::~SerialFlashUtilCopy() {

}

// showPopup
//
void SerialFlashUtilCopy::showPopup() {

//    Serial.printf("SerialFlashUtilCopy::showPopup:\n");

#if INIT_MEM_DURING_DEV
    memset((void*)sdToFlashCopyBuf, 0, sizeof(sdToFlashCopyBuf));
#else // NO_INIT
#endif // INIT_MEM_DURING_DEV / NO_INIT

    SerialFlashUtilBase::showPopup();

    // Check Serial Flash and SD Card Availability
    serialFlashPresent = sysProfile.serialFlashPresent;
    if (!serialFlashPresent) {
        showInfoMsg("Unable to locate Serial Flash device.");
    }
    sdCardPresent = sysProfile.sdCardPresent;
    if (!sdCardPresent) {
        showInfoMsg("Unable to locate SD Card.");
    }

    serialFlashBytes = sysProfile.serialFlashSizeMB;
    snprintf(messageBuf, TempBufLength-1, "SerialFlashUtilCopy: Serial Flash Size %ld MB", serialFlashBytes);
    messageBuf[TempBufLength-1] = '\0';
    showInfoMsg(messageBuf);
}

// showObjects
//
void SerialFlashUtilCopy::showObjects() {

//    Serial.printf("SerialFlashUtilCopy::showObjects:\n");

    SerialFlashUtilBase::showObjects();
}

/************************************************************************************
*        P R I V A T E / P R O T E C T E D   C L A S S   F U N C T I O N S          *
*************************************************************************************
*/

// showProgress
//
void SerialFlashUtilCopy::showProgress(int16_t progressParam) {

    if (progressParam == ProgressPanelReset) {
        progressDisplayPanel.drawPanelButtons();
    } else {
        progressDisplayPanel.drawButton(progressParam, true);
    }
}

#define TEST_REAL_COPY 1

// execute
//
void SerialFlashUtilCopy::execute() {

#if TEST_REAL_COPY
    if (serialFlashPresent && sdCardPresent) {

        const char* fileSuffix  = ".raw";    // file tag
        uint16_t    fileCount   = 0;
        char        numberStr[32];

        // Begin copying
        File rootdir = SD.open("/");
        while (true) {

            // Open next file from the SD
            File sdFile = rootdir.openNextFile();

            // Are we finished
            if (!sdFile) {
                break;  // finished
            }

            // Make sure this is a legitimate RAW file
            const char *filename = sdFile.name();
            if (filename[0] == '.' || !strstr(filename, fileSuffix)) {
                continue;
            }

            // Got a RAW file, proceed with copy
            fileCount++;
            uint32_t fileLength = sdFile.size();
            UtilFns::uint32ToString(fileLength, numberStr);

            // Display user status message
            snprintf(messageBuf, TempBufLength-1,"Copying %s, length %s", filename, numberStr);
//            Serial.printf("Copying %s, length %s\n", filename, numberStr);
            messageBuf[TempBufLength-1] = '\0';
            showStatusMsg(messageBuf);

#if 1 // FFS: Should this be a user option?
            // Delete and overwrite if already present
            if (SerialFlash.exists(filename)) {
                SerialFlash.remove(filename);
            }
#endif // FFS: Should this be a user option?

            // Create the file on the Flash chip and copy data
            if (SerialFlash.create(filename, fileLength)) {
                SerialFlashFile flashFile = SerialFlash.open(filename);
                if (flashFile) {

                    // Clear progress display
                    uint32_t bytesCopied = 0;
                    int16_t progressCounter = 0;       // 0-19, 5-100%
                    int16_t progressCounterPrev = -1;
                    showProgress(ProgressPanelReset);

                    // Copy next block
                    while (bytesCopied < fileLength) {
                        uint16_t nRead = sdFile.read(sdToFlashCopyBuf, SdToFlashCopyBufSize);
                        flashFile.write(sdToFlashCopyBuf, nRead);
                        bytesCopied += nRead;

                        // Update progress display
                        progressCounter = (uint16_t)(0.5 + 20.0 * ((float)bytesCopied / (float)fileLength));
                        if (progressCounter != progressCounterPrev) {
//                            Serial.printf("bytesCopied %d progressCounter %d\n", bytesCopied, progressCounter);
                            if (progressCounter < 20) {
                                showProgress(progressCounter);
                                progressCounterPrev = progressCounter;
                            }
                        }
                    }
                    flashFile.close();
                }
            }
            sdFile.close();
//            Serial.printf("Finished. %d files copied.\n", fileCount);
        }

        // Close SD card and "exit"
//        rootdir.close();  // close causes subsequent list of directory to show empty until reboot (B241220-1).

        // Update global file list and count, sort list
        if (sysProfile.sfRawFileUpdateFn) {
            (*sysProfile.sfRawFileUpdateFn)();
        }

        snprintf(messageBuf, TempBufLength-1,"Finished Copying. %d Files Copied to Serial Flash.", fileCount);
        messageBuf[TempBufLength-1] = '\0';
        showInfoMsg(messageBuf);

    } else {
        if (!serialFlashPresent) {
            showInfoMsg("Unable Copy Files. Serial Flash device not found.");
        } else if (!sdCardPresent) {
            showInfoMsg("Unable Copy Files. SD card not found.");
        }
    }
#endif // TEST_REAL_COPY
}

