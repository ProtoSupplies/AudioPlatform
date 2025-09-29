/************************************************************************************
*                                 Teensy41_Util.cpp                                 *
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

/************************************************************************************
*                       E X T E R N A L   R E F E R E N C E S                       *
*************************************************************************************
*/

// System includes
//
#include "Teensy41_Util.h"
#include "ArduinoEclipseDefs.h"

/************************************************************************************
*                         M A C R O S  /  C O N S T A N T S                         *
*************************************************************************************
*/

const uint16_t  FileNameFieldWidth = 28;

/************************************************************************************
*                     D A T A / T Y P E   D E F I N I T I O N S                     *
*************************************************************************************
*/

char    Teensy41_Util::bigBuffer[BigBufferSize];
char    Teensy41_Util::dateTimeStr[DateTimeStringLength];
bool    Teensy41_Util::sdAudioActive = false;

/************************************************************************************
*                          P U B L I C   F U N C T I O N S                          *
*************************************************************************************
*/

// Constructor/Destructor
//
Teensy41_Util::Teensy41_Util() {

}

Teensy41_Util::~Teensy41_Util() {}

// getTeensyMac()
//
// Note: uint8_t buf[6] assumed
//
void Teensy41_Util::getTeensyMac(uint8_t *buf) {

    // Using HW_OCOTP_MAC* - see https://forum.pjrc.com/threads/57595-Serial-amp-MAC-Address-Teensy-4-0
    for (uint8_t by=0; by<2; by++) buf[by]=(HW_OCOTP_MAC1 >> ((1-by)*8)) & 0xFF;
    for (uint8_t by=0; by<4; by++) buf[by+2]=(HW_OCOTP_MAC0 >> ((3-by)*8)) & 0xFF;
}

// setTeensySdAudioActive()
//
// Following code added solely in support of workaround for B250308-1
// so that audio interrupts can be disabled when accessing SD card file
// system when SD WAV Player is active.
//
void Teensy41_Util::setTeensySdAudioActive(bool active) {

    sdAudioActive = active;
}

// parseRtcDateTime()
//
void Teensy41_Util::parseRtcDateTime(DateTimeFields& dt) {

//    Serial.printf("Teensy41_Util::parseRtcDateTime:\n");

    dt.mon = month();
    dt.mday = day();
    dt.year = year() - 1900;

    dt.hour = hour();
    dt.min = minute();
    dt.sec = second();
}

#if 1 // Test Bug B250216-1 Fix

char* Teensy41_Util::parseRtcDateTime() {

    char strAm[3] = "AM";
    char strPm[3] = "PM";

    char* strAmPm;

    int hourAmPm = hour();

    if (hourAmPm == 0) {
        strAmPm = strAm;            // 12 AM
        hourAmPm = 12;              // 12 AM
    } else if (hourAmPm == 12) {
        strAmPm = strPm;            // 12 PM
    } else if (hourAmPm > 12) {
        hourAmPm -=12;              // Afternoon PM
        strAmPm = strPm;
    } else {
        strAmPm = strAm;            // Morning AM
    }

    snprintf(dateTimeStr, DateTimeStringLength, "%d/%d/%2d, %d:%02d:%02d %s", month(), day(), year() % 2000, hourAmPm, minute(), second(), strAmPm);

//    Serial.printf("Teensy41_Util::parseRtcDateTime: dateTimeStr %s\n", dateTimeStr);

    return dateTimeStr;
}

#else // Original parseRtcDateTime()

char* Teensy41_Util::parseRtcDateTime() {

    char strAm[3] = "AM";
    char strPm[3] = "PM";

    char* strAmPm;

    int hourAmPm = hour();
    Serial.printf("Teensy41_Util::parseRtcDateTime: hourAmPm %d\n", hourAmPm);

    if (hourAmPm == 0) {
        strAmPm = strAm;            // 12 AM
        hourAmPm = 12;              // 12 AM
    } else if (hourAmPm == 12) {
        strAmPm = strPm;            // 12 PM
    }
    if (hourAmPm > 12) {
        hourAmPm -=12;
        strAmPm = strPm;
    }

    snprintf(dateTimeStr, DateTimeStringLength, "%d/%d/%2d, %d:%02d:%02d %s", month(), day(), year() % 2000, hourAmPm, minute(), second(), strAmPm);

    return dateTimeStr;
}

#endif // Test Bug B250216-1 Fix / Original parseRtcDateTime()

// printlnToBuf()
//
void Teensy41_Util::printlnToBuf(const char* textLine, char **buf, uint16_t maxLineLen, uint16_t& lineCount) {

    snprintf(buf[lineCount++], maxLineLen-1, "%s", textLine);
}

// printlnToBigBuf()
//
void Teensy41_Util::printlnToBigBuf(const char* textLine, uint16_t& lineCount) {

    if (lineCount < BigBufferLineCount) {
        uint32_t offset = lineCount++ * BigBufferLineLength;
        snprintf(&bigBuffer[offset], BigBufferLineLength-1, "%s", textLine);
    }
}

// compareNames()
//
// Compare 2 strings via string pointers. Called below by qsort().
//
// NOTE: compareNames() must be defined as a C-space function in order
// for qsort() to function properly.
//
static int compareNames(const void* s1, const void* s2) {

    const char** a = (const char**) s1;
    const char** b = (const char**) s2;
    return strcmp(*a, *b);
}

// getSdWavFileInfo()
//
// Scan SD root directory for WAV filenames with a 3-digit prefix followed by dash:
//
//      "000-" to "399" (at present, representing 4 banks of 100 files)
//
// The prefixes are used as indexes into an array (fileNamePtrs) of pointers to strings representing
// the names of the indexed WAV files, which are themselves stored as character arrays (fileNameText)
// in external memory.
//
// For example, the SD file "012-SoundFileX.wav", would be processed as follows:
//
//      fileNameText[fileCount++] = "012-SoundFileX.wav" (string copied to external memory array)
//      fileSizes[012] = sizeof("012-SoundFileX.wav");
//      fileNamePtrs[012] = &fileNameText[fileCount];
//
// where the incrementing variable fileCount tracks the total number of WAV files stored, and us reported in
// the return value of the function.
//
// At runtime, MIDI notes 0-99 are played as WAV files with the MIDI note number representing the index of
// the WAV file in the fileNamePtrs[] array, mapped to a 100 sample range 0-3 as 0-99, 100-199, 200-299,
// and 300-399 respectively.
//
// FFS/TODO: Note that this function shares a great number of features with getSerialFlashRawFileInfo() below.
// The common code needs to be isolated and factored to eliminate and reduce duplication.
//
uint16_t Teensy41_Util::getSdWavFileInfo(const char* suffix, uint64_t filesizes[], char (*fileNameText) [32], char** fileNamePtrs) {

    uint16_t fileCount = 0;

    // B250308-1 Work Around
    if (sdAudioActive) {
        AudioNoInterrupts();
    }

    File dir = SD.open("/");

    while (true) {
        File entry = dir.openNextFile();
        if (!entry) {
            //Serial.println("** no more files **");
            break;
        }

        if (!entry.isDirectory()) {
            char *nn = (char *)entry.name();
            if (nn[0] != '.') {
                if (strstr(entry.name(), suffix)) {

                    int prefix;
                    char dash;
                    dash = 'x';
                    if (sscanf(entry.name(), "%d%c", &prefix, &dash) == 2) {
                        if (dash == '-' ) {
                            if (prefix >= 0 && prefix < MaxSdWavFileNames) {
                                // Copy filename to external PSRAM
                                strncpy(fileNameText[fileCount], entry.name(), MaxFileNameLen-1);
                                fileNameText[fileCount][MaxFileNameLen-1] = '\0';
//                                Serial.printf("Teensy41_Util::getSdDirFileInfo: File %d: %s has prefix %03d followed by %c\n", fileCount, fileNameText[fileCount], prefix, dash);

                                filesizes[prefix] = entry.size();
                                fileNamePtrs[prefix] = &fileNameText[fileCount][0];

                                fileCount++;
                            }
                        }
                    }

                    if (fileCount >= MaxSdWavFileNames) {
                        break;
                    }
                }
            }
        }
        entry.close();
    }

    // B250308-1 Work Around
    if (sdAudioActive) {
        AudioInterrupts();
    }

//    Serial.printf("Teensy41_Util::getSdDirFileInfo: Returning fileCount %d\n", fileCount);
    return fileCount;
}

// getSerialFlashRawFileInfo()
//
// Scan Serial Flash file directory for RAW filenames with a 3-digit prefix followed by dash:
//
// See the header notes for getSdWavFileInfo() above regarding similarity and duplication of functionality.
//
uint16_t Teensy41_Util::getSerialFlashRawFileInfo(const char* suffix, uint64_t filesizes[], char (*fileNameText)[32], char** fileNamePtrs) {

    char        filename[MaxFileNameLen];
    uint32_t    filesize;
    uint16_t    fileCount = 0;

    SerialFlash.opendir();

    while (true) {
        if (SerialFlash.readdir(filename, sizeof(filename), filesize)) {
            if (filename[0] != '.') {
                if (strstr(filename, suffix)) {

                    int prefix;
                    char dash = 'x';
                    if (sscanf(filename, "%d%c", &prefix, &dash) == 2) {
                        if (dash == '-' ) {
                            if (prefix >= 0 && prefix < MaxSfRawFileNames) {
                                // Copy filename to external PSRAM
                                strncpy(fileNameText[fileCount], filename, MaxFileNameLen);
                                fileNameText[fileCount][MaxFileNameLen-1] = '\0';
//                                Serial.printf("Teensy41_Util::getSerialFlashFileInfo: File %d: %s has prefix %03d followed by %c\n", fileCount, fileNameText[fileCount], prefix, dash);

                                filesizes[prefix] = filesize;
                                fileNamePtrs[prefix] = &fileNameText[fileCount][0];

                                fileCount++;
                            }
                        }
                    }

                    if (fileCount >= MaxSfRawFileNames) {
                        break;
                    }
                }
            }

        } else {
            break; // no more files
        }
    }

//    Serial.printf("Teensy41_Util::getSerialFlashFileInfo: Returning fileCount %d\n", fileCount);
    return fileCount;
}

uint16_t Teensy41_Util::getSdDirFileInfo(const char* suffix, uint64_t filesizes[], char (*fileNameText) [32], char** fileNamePtrs) {

    uint16_t fileCount = 0;

    // B250308-1 Work Around
    if (sdAudioActive) {
        AudioNoInterrupts();
    }

    File dir = SD.open("/");

    while (true) {
        File entry = dir.openNextFile();
        if (!entry) {
            //Serial.println("** no more files **");
            break;
        }

        if (!entry.isDirectory()) {
            char *nn = (char *)entry.name();
            if (nn[0] != '.') {
                if (strstr(entry.name(), suffix)) {
                    // Copy filename to external PSRAM
                    strncpy(fileNameText[fileCount], entry.name(), MaxFileNameLen-1);
                    fileNameText[fileCount][MaxFileNameLen-1] = '\0';

                    filesizes[fileCount] = entry.size();
                    fileNamePtrs[fileCount] = &fileNameText[fileCount][0];

                    fileCount++;
                    if (fileCount >= MaxSdWavFileNames) {
                        break;
                    }
                }
            }
        }
        entry.close();
    }

    // B250308-1 Work Around
    if (sdAudioActive) {
        AudioInterrupts();
    }

    // Now sort the filenames
    if (fileCount > 1) {
        qsort(fileNamePtrs, fileCount, sizeof(char *), compareNames);
    }

    return fileCount;
}

uint16_t Teensy41_Util::getSerialFlashFileInfo(const char* suffix, uint64_t filesizes[], char (*fileNameText)[32], char** fileNamePtrs) {

    char        filename[MaxFileNameLen];
    uint32_t    filesize;
    uint16_t    fileCount = 0;

    SerialFlash.opendir();

    while (true) {
        if (SerialFlash.readdir(filename, sizeof(filename), filesize)) {
            if (filename[0] != '.') {
                if (strstr(filename, suffix)) {
                    // Copy filename to external PSRAM
                    strncpy(fileNameText[fileCount], filename, MaxFileNameLen);
                    fileNameText[fileCount][MaxFileNameLen-1] = '\0';

                    filesizes[fileCount] = filesize;
                    fileNamePtrs[fileCount] = &fileNameText[fileCount][0];

                    fileCount++;
                    if (fileCount >= MaxSfRawFileNames) {
                        break;
                    }
                }
            }

        } else {
            break; // no more files
        }
    }


    // Now sort the filenames
    if (fileCount > 1) {
        qsort(fileNamePtrs, fileCount, sizeof(char *), compareNames);
    }

    return fileCount;
}

// printSdDirFileInfoToBigBuf()
//
uint16_t Teensy41_Util::printSdDirFileInfoToBigBuf(const char* suffix) {

    // B250308-1 Work Around
    if (sdAudioActive) {
        AudioNoInterrupts();
    }

    uint16_t lineCount = printDirFileInfoToBigBuf(SD.open("/"), suffix);

    // B250308-1 Work Around
    if (sdAudioActive) {
        AudioInterrupts();
    }

    return lineCount;
}

// printDirFileInfoToBigBuf()
//
uint16_t Teensy41_Util::printDirFileInfoToBigBuf(File dir, const char* suffix) {

    char numberStr[32];
    uint32_t totalSize = 0;
    uint16_t lineCount = 0;

    char* offset = &bigBuffer[lineCount++];
    snprintf(offset, BigBufferLineLength-1, "Directory %s", dir.name());

    while (lineCount < BigBufferLineCount) {
        File entry = dir.openNextFile();
        if (!entry) {
            // No more files
            break;
        }
        if (!entry.isDirectory()) {
            UtilFns::uint32ToString(entry.size(), numberStr);

            char *nn = (char *)entry.name();
            if (nn[0] != '.') {
                if ((strlen(suffix) == 0) || (strstr(entry.name(), suffix))) {
                    UtilFns::uint32ToString(entry.size(), numberStr);
                    totalSize += entry.size();
                    DateTimeFields dt;
                    offset += BigBufferLineLength;
                    lineCount++;
                    if (entry.getModifyTime(dt)) {
                        snprintf(offset, BigBufferLineLength-1, "%-*s%13s  %2d:%02d  %2d/%02d/%02d", FileNameFieldWidth, entry.name(), numberStr,
                            dt.hour, dt.min, dt.mon + 1, dt.mday, dt.year - 100);
                    } else {
                        snprintf(offset, BigBufferLineLength-1, "%-*s%13s", FileNameFieldWidth, entry.name(), numberStr);
                    }
                }
            }
        }
        entry.close();
    }

    if (totalSize > 0) {
        UtilFns::uint32ToString(totalSize, numberStr);
        offset += BigBufferLineLength;
        lineCount++;
        snprintf(offset, BigBufferLineLength-1, "%*s %12s", FileNameFieldWidth, "Total Size", numberStr);
    }

    return lineCount;
}

// printFlashDirFileInfoToBigBuf()
//
uint16_t Teensy41_Util::printFlashDirFileInfoToBigBuf(const char* suffix) {

//    Serial.printf("Teensy41_Util::printFlashDirFileInfoToBigBuf:\n");

    char numberStr[32];
    uint32_t totalSize = 0;
    uint16_t lineCount = 0;

    char* offset = &bigBuffer[lineCount++];
    snprintf(offset, BigBufferLineLength-1, "Directory /");

    char filename[64];
    uint32_t filesize;

    SerialFlash.opendir();

    while (lineCount < BigBufferLineCount) {
        if (SerialFlash.readdir(filename, sizeof(filename), filesize)) {
            UtilFns::uint32ToString(filesize, numberStr);
            totalSize += filesize;
            offset += BigBufferLineLength;
            lineCount++;
            snprintf(offset, BigBufferLineLength-1, "%-*s%13s", FileNameFieldWidth, filename, numberStr);

        } else {
            break; // no more files
        }
    }

    if (totalSize > 0) {
        UtilFns::uint32ToString(totalSize, numberStr);
        offset += BigBufferLineLength;
        lineCount++;
        snprintf(offset, BigBufferLineLength-1, "%*s %12s", FileNameFieldWidth, "Total Size", numberStr);
    }

    return lineCount;
}

// printSdDirFileListToBigBuf()
//
uint16_t Teensy41_Util::printSdDirFileListToBigBuf(const char* suffix) {

    // B250308-1 Work Around
    if (sdAudioActive) {
        AudioNoInterrupts();
    }

    File dir = SD.open("/");

    uint16_t lineCount = 0;
    char* offset = &bigBuffer[0];

    while (lineCount < BigBufferLineCount) {
        File entry = dir.openNextFile();
        if (!entry) {
            // No more files
            break;
        }
        if (!entry.isDirectory()) {
            char *nn = (char *)entry.name();
            if (nn[0] != '.') {
                if ((strlen(suffix) == 0) || (strstr(entry.name(), suffix))) {
                    lineCount++;
                    memset(offset, 0, BigBufferLineLength);
                    snprintf(offset, BigBufferLineLength-1, "%s", entry.name());
                    offset += BigBufferLineLength;
                }
            }
        }
        entry.close();
    }

    // B250308-1 Work Around
    if (sdAudioActive) {
        AudioInterrupts();
    }

    return lineCount;
}

// copyToBigBuffer()
//
void Teensy41_Util::copyToBigBuffer(uint32_t bigBufferOffset, char *fromBuf, size_t copyLength) {

    memcpy(bigBuffer + bigBufferOffset, fromBuf, copyLength);
}

// copyFromBigBuffer()
//
void Teensy41_Util::copyFromBigBuffer(uint32_t bigBufferOffset, char *toBuf, size_t copyLength) {

    memcpy(toBuf, bigBuffer + bigBufferOffset, copyLength);
}

// waitSerial()
//
void Teensy41_Util::waitSerial(const char* prompt) {

    Serial.println(prompt);
    while (!Serial.available()) {
    }

    char inBuf[32];

    Serial.readBytesUntil('\n', inBuf, 32);
}

/************************************************************************************
*                   P R I V A T E   C L A S S   F U N C T I O N S                   *
*************************************************************************************
*/

