/************************************************************************************
*                               AudioPlatformGlobal.h                               *
*                AudioPlatform Global Shared Objects and Definitions                *
*                                                                                   *
*                       Copyright (c) 2025, Donald Swearingen                       *
*                                All Rights Reserved                                *
*************************************************************************************
*
* Description
*
*   Objects Allocated/Owned by main AudioPlatform components listed here
*   so as to be visible in AudioPlatform_CustomClasses library modules, which
*   have no direct knowledge AudioPlatform components internal objects.
*
*/

#ifndef AUDIOPLATFORMGLOBAL_H_
#define AUDIOPLATFORMGLOBAL_H_

/****************
* "Pre-Defines" *
*****************
*
* Definitions that affect subsequent external references
*
*/

// Initialize memory during development to prevent items from remaining present
// (esp. in EXTMEM) when re-compiling without power cycling.
//
#define INIT_MEM_DURING_DEV 1

#if INIT_MEM_DURING_DEV
#else // NO_INIT
#endif // INIT_MEM_DURING_DEV / NO_INIT

/************************************************************************************
*                       E X T E R N A L   R E F E R E N C E S                       *
*************************************************************************************
*/

// Standard C++
//
#include <Math.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

// Teensyduino (FFS: List/consolidate all used in system here? Maybe less efficient to compile?
//
#include <LittleFS.h>
#include <pgmspace.h>
#include <SD.h>

/************************************************************************************
*                E X P O R T E D   M A C R O S  /  C O N S T A N T S                *
*************************************************************************************
*/

// System Constants
//
// Note: Following constants apply separately for SD and Flash Banks,
// that is, MaxSoundFileNames in each of the two banks (total 2 * MaxSoundBanks)
//
const int       MaxSoundBanks               = 4;
const int       SoundFilesPerBank           = 100;
const int       MaxSoundFileNames           = 400;
const int       MaxSoundFileNameLen         = 32;   // Maximum filename length of WAV or RAW Files

const int       MaxPresetBankFileNames      = 64;   // .pbnk Files (each; total 512)
const int       MaxPresetBankFileNameLen    = 32;   // Maximum filename length of .pbnk
const uint8_t   MaxPresetBtnNameLen         = 11;   // Maximum display chars on preset buttons

const uint64_t  BytesPerMegaByte            = (1024 * 1024);  // 1 MB;
const uint32_t  TotalProgmemStorageSize     = BytesPerMegaByte;

const char      MonthNames[12][4]           = {"Jan","Feb","Mar","Apr","May","Jun",
                                               "Jul","Aug","Sep","Oct","Nov","Dec"};

const uint32_t  MillisPerSecond             = 1000;
const uint32_t  SecondsPerHour              = 3600;
const uint32_t  SecondsPerDay               = 86400;

const uint32_t  TempBufLength               = 128;
const uint32_t  LargeTempBufferLineCount    = 1024;
const uint32_t  LargeTempBufferLineLength   = 128;
const uint32_t  LargeTempBufferSize         = LargeTempBufferLineCount * LargeTempBufferLineLength;

// Preset Constants
//
// Shared mainly by Presets.ino and PresersScreen.cpp, but also available to other
// application components and classes
//
const char      DefaultPresetsBankFilename[] = "DefaultBank.pbnk";

const uint16_t  PresetNameSize              = 32;
const uint16_t  PresetFileNameSize          = 32;
const uint16_t  PresetDeviceSize            = 128;
const uint16_t  PresetDeviceCount           = 32;
const uint16_t  PresetCount                 = 32;

// Application File Directories on SD
//
// Support for proposed new organization of SD files (M250802-2)
//
const char      PresetBankFileDir[]         = "/Presets/";
const char      FmSynthBankFileDir[]        = "/Dexed/";
const char      WavFileDir[]                = "/";
const char      RawFileDir[]                = "/Raw/";
const char      BitmapFileDir[]             = "/Bitmaps/";
const char      SystemFileDir[]             = "/System/";

// Application File Type Suffixes
//
// Don't understand this, but if string constants are define as pointers, as is commonly
// done in code, for example:
//
//      const char* PresetBankFileSuffix = ".pbnk";
//
// the compiler generates an error "previously defined here" for every instance of inclusion
// of AudioPlatform.h, producing a very long list of error messages.
//
// If, however, they are defined as below, there is no issue, and the compiler apparently adds
// a trailing 0 to the strings, as would be the case with the definitions in normal code.
//
const char      PresetBankFileSuffix[]      = ".pbnk";
const char      FmSynthBankFileSuffix[]     = ".syx";
const char      WavFileSuffix[]             = ".wav";
const char      RawFileSuffix[]             = ".raw";
const char      BitmapFileSuffix[]          = ".bmp";

// Field widths for Directory Listings
//
const uint16_t  DirFullInfoWidth            = 540;
const uint16_t  DirPartiaInfoWidth          = 385;
const uint16_t  DirShortInfoWidth           = 250;

/************************************************************************************
*            E X P O R T E D   D A T A / T Y P E   D E F I N I T I O N S            *
*************************************************************************************
*
*/

// System Hardware Profile
//
//  Profile of system hardware set at startup and shared among AudioPlatform components
//  and libraries.
//
// WAV and RAW Audio Filenames
//
//  Because of their large memory requirements (16384 bytes as of 7/15/24), the names of
//  WAV and RAW files used by AudioPlatform components are stored in EXTRAM (8MB
//  PSRAM on AudioPlatform) rather than in the more limited 1MB internal RAM space
//  of the Teensy 4.1's IMXRT1060 processor. These are allocated and owned by
//  AudioPlatform.ino, and not presently (7/12/24) exported for access in other
//  AudioPlatform components and classes.
//
//  Pointers/references to the filenames (sdWavFileNames, sfRawFileNames)
//  are maintained in SystemHardwareProfile, in internal memory, for use in situations where
//  a filename is required (the MediaPlayer classes, for example). Additional pointers,
//  set at startup, provide access to core component functions for update of dynamic
//  lists (sound filenames/sizes, preset banks, etc.)
//
typedef struct {

    // Application
    const char*         appName;            // From app. via getSystemInfo()
    const char*         appVersion;         // From app. via getSystemInfo()
    char                teensyMAC[20];      // Unique Teensy MAC "xx:xx:xx:xx:xx:xx"
    const char*         buildDate;
    const char*         buildTime;

    // System Memory
    uint32_t            psRamSize;
    uint32_t            audioBlockCount;

    LittleFS_QSPIFlash  myfs_NOR;           // NOR FLASH
    bool                qspiNorFlashPresent;
    uint32_t            qspiNorFlashSize;
    LittleFS_QPINAND    myfs_NAND;          // NAND FLASH
    bool                qspiNandFlashPresent;
    uint32_t            qspiNandFlashSize;

    // Serial Flash
    bool                serialFlashPresent;
    uint32_t            serialFlashSizeMB;
    uint16_t            sfRawFileCount;
    // pointers to sound file names in EXTMEM
    char*               sfRawFileNames[MaxSoundFileNames];
    uint64_t            sfRawFileSizes[MaxSoundFileNames];
    // assignable pointer to function to update sfRawFileNames
    void                (*sfRawFileUpdateFn)();

    // SD Card
    // TODO: Also count RAW files on SD card
    bool                sdCardPresent;
    uint32_t            sdCardSizeMB;
    uint32_t            sdUsedSizeMB;
    uint16_t            sdWavFileCount;
                        // pointers to sound file names in EXTMEM
    char*               sdWavFileNames[MaxSoundFileNames];
    uint64_t            sdWavFileSizes[MaxSoundFileNames];
    // assignable pointer to function to update sdWavFileNames
    void                (*sdWavFileUpdateFn)();

    // Preset Bank Files on SD Card
    uint16_t            sdPresetBankFileCount;
                        // pointers to sound file names in EXTMEM
    char*               sdPresetBankFileNames[MaxPresetBankFileNames];
    uint64_t            sdPresetBankFileSizes[MaxPresetBankFileNames];
    // assignable pointer to function to update sdPresetBankFileNames
    void                (*sdPresetBankFileUpdateFn)();

    // Program Flash (PROGMEM)
    LittleFS_Program    progmemFs;
    bool                progmemFsPresent;

    // Real Time Clock (RTC)
    bool                rtcSet;
    float               rtcBatteryLevel;

    // Pointer to "lines" (1024 lines of 128 bytes each) in Temp Buffer in EXTMEM.
    // Generally used as sources for display in textColumn slots of more limited number.
    // TODO: Rename bigBufLinePtrs
    char*               largeTempBufLines[LargeTempBufferLineCount];

    // Active MIDI interfaces ??
    // What else??
} SystemHardwareProfile;

// SoundFileNumber type allows for up to 65,536 files
//
typedef uint16_t        SoundFileNumber;

/************************************************************************************
*              E X P O R T E D   F U N C T I O N   P R O T O T Y P E S              *
*************************************************************************************
*
*/

#endif // AUDIOPLATFORMGLOBAL_H_
