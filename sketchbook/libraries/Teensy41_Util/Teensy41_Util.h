/************************************************************************************
*                                 Teensy41_Util.h                                   *
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

#ifndef TEENSY4UTIL_H_
#define TEENSY4UTIL_H_

/************************************************************************************
*                       E X T E R N A L   R E F E R E N C E S                       *
*************************************************************************************
*/

// Arduino Core
//
#include <Arduino.h>

// User Arduino Libraries
//
#include "UtilFns.h"

// Teensyduino
//
#include <SPI.h>
#include <FS.h>
#include <SD.h>
#include <SerialFlash.h>
#include <LittleFS.h>
#include <TimeLib.h>
#include <Audio.h>

/************************************************************************************
*                         M A C R O S  /  C O N S T A N T S                         *
*************************************************************************************
*/

#define TEENSYUTIL          Teensy41_Util
#define TEENSYVERSION       "4.1"

#define NO_OUTPUT           -1

#define RX1_PIN             0
#define TX1_PIN             1
#define RX2_PIN             7
#define TX2_PIN             8
#define RX3_PIN             15
#define TX3_PIN             14
#define RX4_PIN             16
#define TX4_PIN             17
#define RX5_PIN             21
#define TX5_PIN             20
#define RX6_PIN             25
#define TX6_PIN             24
#define RX7_PIN             28
#define TX7_PIN             29
#define RX8_PIN             34
#define TX8_PIN             35

#define SCL0_PIN            19
#define SDA0_PIN            18
#define SCL1_PIN            16
#define SDA1_PIN            17

#define SPDIF_IN_PIN        15
#define SPDIF_OUT_PIN       14

#define SPI_CS_PIN          10
#define SPI_MOSI_PIN        11
#define SPI_MISO_PIN        12
#define SPI_SCK_PIN         13
#define SPI_CS1_PIN         0
#define SPI_MOSI1_PIN       26
#define SPI_MISO1_PIN       1
#define SPI_SCK1_PIN        27

#define ARDUINO_LED_PIN     13

#define DIGITAL_IO_0_PIN    0
#define DIGITAL_IO_1_PIN    1
#define DIGITAL_IO_2_PIN    2
#define DIGITAL_IO_3_PIN    3
#define DIGITAL_IO_4_PIN    4
#define DIGITAL_IO_5_PIN    5
#define DIGITAL_IO_6_PIN    6
#define DIGITAL_IO_7_PIN    7
#define DIGITAL_IO_8_PIN    8
#define DIGITAL_IO_9_PIN    9
#define DIGITAL_IO_10_PIN   10
#define DIGITAL_IO_11_PIN   11
#define DIGITAL_IO_12_PIN   12
#define DIGITAL_IO_13_PIN   13
#define DIGITAL_IO_14_PIN   14
#define DIGITAL_IO_15_PIN   15
#define DIGITAL_IO_16_PIN   16
#define DIGITAL_IO_17_PIN   17
#define DIGITAL_IO_18_PIN   18
#define DIGITAL_IO_19_PIN   19
#define DIGITAL_IO_20_PIN   20
#define DIGITAL_IO_21_PIN   21
#define DIGITAL_IO_22_PIN   22
#define DIGITAL_IO_23_PIN   23
#define DIGITAL_IO_24_PIN   24
#define DIGITAL_IO_25_PIN   25
#define DIGITAL_IO_26_PIN   26
#define DIGITAL_IO_27_PIN   27
#define DIGITAL_IO_28_PIN   28
#define DIGITAL_IO_29_PIN   29
#define DIGITAL_IO_30_PIN   30
#define DIGITAL_IO_31_PIN   31
#define DIGITAL_IO_32_PIN   32
#define DIGITAL_IO_33_PIN   33
#define DIGITAL_IO_34_PIN   34
#define DIGITAL_IO_35_PIN   35
#define DIGITAL_IO_36_PIN   36
#define DIGITAL_IO_37_PIN   37
#define DIGITAL_IO_38_PIN   38
#define DIGITAL_IO_39_PIN   39
#define DIGITAL_IO_40_PIN   40
#define DIGITAL_IO_41_PIN   41

#define ANALOG_IN_0_PIN     14
#define ANALOG_IN_1_PIN     15
#define ANALOG_IN_2_PIN     16
#define ANALOG_IN_3_PIN     17
#define ANALOG_IN_4_PIN     18
#define ANALOG_IN_5_PIN     19
#define ANALOG_IN_6_PIN     20
#define ANALOG_IN_7_PIN     21
#define ANALOG_IN_8_PIN     22
#define ANALOG_IN_9_PIN     23
#define ANALOG_IN_10_PIN    24
#define ANALOG_IN_11_PIN    25
#define ANALOG_IN_12_PIN    26
#define ANALOG_IN_13_PIN    27
#define ANALOG_IN_14_PIN    38
#define ANALOG_IN_15_PIN    39
#define ANALOG_IN_16_PIN    40
#define ANALOG_IN_17_PIN    41

#define ANALOG_IN_MIN_VAL   0
#define ANALOG_IN_MAX_VAL   1023


/************************************************************************************
*                     D A T A / T Y P E   D E F I N I T I O N S                     *
*************************************************************************************
*/

const uint32_t  BigBufferLineCount      = 1024;
const uint32_t  BigBufferLineLength     = 128;
const uint32_t  BigBufferSize           = BigBufferLineCount * BigBufferLineLength;

const uint8_t   DateTimeStringLength    = 32;

// SD and Serial Flash can each contain up to 4 banks of soundfiles each with up to
// 100 files (1-100, 101-200, 201-300, 301-400. Filenames must follow the convention
// of numeric prefixes of 3 digits (with leading 0s) followed by a dash '-', the name,
// and with suffix ".wav" or ".raw" (suffix in lower-case). The numbers need not be
// consecutive, and gaps are permitted. For example:
//
//      "004-SomeSoundFile.wav"
//      "015-ARawFile.raw"
//      "103-AnotherWAVFile.wav"
//      "305-RawFileInBank3.raw"
//
const uint16_t  MaxSdWavFileNames       = 400;  // Maximum WAV or RAW Files (each; total 512)
const uint16_t  MaxSfRawFileNames       = 400;  // Maximum RAW Files on Serial Flash
const uint16_t  MaxFileNameLen          = 32;   // Maximum filename length of WAV or RAW Files

/************************************************************************************
*                         C L A S S   D E F I N I T I O N S                         *
*************************************************************************************
*/

class Teensy41_Util {

// Constructor/Destructor
//
public:

    Teensy41_Util();
    virtual ~Teensy41_Util();

// Class Variables
//
public:

    // Large Buffer (32768 as of v3.4.5) in External PSRAM
    // for temporary use for storage and as destination for transient
    // line-by-line assembly of content to be displayed on screen
    EXTMEM static char  bigBuffer[];

private:

    EXTMEM static char  dateTimeStr[];
    static bool         sdAudioActive;

protected:


// Class Methods
//
public:

    static void         getTeensyMac(unsigned char *buf) FLASHMEM;
    static void         setTeensySdAudioActive(bool active);
    static void         parseRtcDateTime(DateTimeFields& dt) FLASHMEM;
    static char*        parseRtcDateTime() FLASHMEM;

    static void         printlnToBuf(const char* textLine, char **buf, uint16_t maxLineLen, uint16_t& lineCount);
    static void         printlnToBigBuf(const char* textLine, uint16_t& lineCount);

    static uint16_t     getSdWavFileInfo(const char* suffix, uint64_t filesizes[], char (*fileNameText) [32], char** fileNamePtrs);
    static uint16_t     getSerialFlashRawFileInfo(const char* suffix, uint64_t filesizes[], char (*fileNameText)[32], char** fileNamePtrs);
    static uint16_t     getSdDirFileInfo(const char* suffix, uint64_t filesizes[], char (*fileNameText)[32], char** fileNamePtrs);
    static uint16_t     getSerialFlashFileInfo(const char* suffix, uint64_t filesizes[], char (*fileNameText)[32], char** fileNamePtrs);

    static uint16_t     printSdDirFileInfoToBigBuf(const char* suffix);
    static uint16_t     printDirFileInfoToBigBuf(File dir, const char* suffix);
    static uint16_t     printFlashDirFileInfoToBigBuf(const char* suffix);
    static uint16_t     printSdDirFileListToBigBuf(const char* suffix);

    static void         copyToBigBuffer(uint32_t bigBufferOffset, char *fromBuf, size_t copyLength);
    static void         copyFromBigBuffer(uint32_t bigBufferOffset, char *toBuf, size_t copyLength);

    static void         waitSerial(const char* prompt);

private:


protected:


};

#endif // TEENSY4UTIL_H_
