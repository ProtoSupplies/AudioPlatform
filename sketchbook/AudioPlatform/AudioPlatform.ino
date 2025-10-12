/************************************************************************************
*                                 AudioPlatform.ino                                 *
*                       Copyright (c) 2025, Donald Swearingen                       *
*                                All Rights Reserved                                *
*************************************************************************************
*
* Description
*
*   AudioPlatform is first and foremost a hardware platform for experimentation and
*   development of simple to sophisticated, stand-alone, audio systems and applications
*   built around the Teensy4.1 environment.
*
*   AudioPlatform is a response to the difficulty that many potential creators of
*   Teensy audio systems face: the challenge of acquiring or themselves building some
*   sort of hardware appropriate to their projects.  many of whom do not have the
*   skills or resources to complete this process, and are thus restricted to the
*   use of breadboards and/or simple soldered wiring to do so.
*
*   AudioPlatform tries to address that need by offering a system that includes
*   a significant set of resources for audio applications. These include:
*
*       - Teensy 4.1 @600 MHz with 1 MB RAM, 8 GB Program Flash, 16 MB PSRAM, 128 MB Serial Flash Chip
*       - Teensy 4.1 SD Card Reader
*       - USB I/O on both MIDI Host and USB-C Connections
*       - Midi I/O on both USB and MIDI DIN Ports
*       - Line In/Out on 3.5 mm Female Connectors
*       - Headphone Out on 3.5mm Female Connector
*
* Application Core Components
*
*   • AudioPlatform.h / AudioPlatform.ino
*
*   In the Arduino environment, it is frequently the case that most or all of an
*   application's functions and objects are simply included in the baseline ".ino"
*   module, often resulting in .ino modules that are hundreds of lines or more in
*   length.
*
*   For increased readability and clarity in AudioPlatform.ino, the bulk of its
*   functionality is represented in a collection of external software components,
*   modules, and libraries, the most fundamental of which are grouped together with
*   AudioPlatform.ino in the base folder, and referenced in this module via their
*   include files.
*
*   These application components include:
*
*       • InputDevices.h / InputDevices.ino
*
*       • AudioDevices.h / AudioDevices.cpp
*
*       • Screens.h / Screens.ino
*
*       • Presets.h / Presets.ino
*
* Application Library Dependencies
*
*   TODO
*
* General Use of Switch/Case for Selecting Among Instances of Common Base Class
*
*   TODO: Here and in User Guide/Reference discuss lack of ability to do this in a
*   more sophisticated manner because of disabling of dynamic_cast in Arduino c++:
*
*   • https://forum.arduino.cc/t/casting-from-base-class-to-derived-class/849723
*   • https://eliwinkelman.github.io/Arduino-DIY-Dynamic-Cast/
*   • https://forum.arduino.cc/t/using-dynamic-cast-incorrectly/991606/7
*   • https://forum.arduino.cc/t/using-dynamic_cast-enabling-rtti/481141
*   • https://arduino.stackexchange.com/questions/55157/dynamic-cast-not-permitted-with-fno-rtti
*
* LittleFS in Program Flash
*
*   LittleFS reserves a specified area of Program Flash (PROGMEM) for non-volatile storage
*   of data across subsequent compilations and loading of program code and data. The size
*   of the reserved area is specified in the begin() function of LittleFS, and the contents
*   of the reserved area can thereafter only be accessed and stored to via functions of the
*   LittleFS API.
*
*   Uses of LittleFS in AudioPlatform include:
*       • Storage of preset data
*       •
*
*   TODO: Investigate: After creation with a given size, will a later change of reserved area size
*   affect the current storage contents? It would seem especially so when the size was reduced,
*   but the effect of reducing or increasing the size remains to be determined.
*
* Application Memory RAM1, RAM2, Program Flash (PROGMEM) Usage
*
*       - AudioPlatform 3/15/25 Build (before selective use of FLASHMEM)
*       Memory Usage on Teensy 4.1:
*       FLASH: code:309540, data:266176, headers:8984   free for files:7541764
*       RAM1: variables:181760, code:302264, padding:25416   free for local variables:14848
*       RAM2: variables:169952  free for malloc/new:354336
*       EXTRAM: variables:457280
*
*       - AudioPlatform 3/15/25 Build (after selective use of FLASHMEM)
*       Memory Usage on Teensy 4.1:
*       FLASH: code:309620, data:266176, headers:8904   free for files:7541764
*       RAM1: variables:181760, code:301304, padding:26376   free for local variables:14848
*       RAM2: variables:169952  free for malloc/new:354336
*       EXTRAM: variables:457280
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
* Definitions that affect subsequent references
*
*/

/************************************************************************************
*                       E X T E R N A L   R E F E R E N C E S                       *
*************************************************************************************
*/

// Application Components
//
#include "AudioPlatform.h"

// Teensyduino
//
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <FS.h>
#include <SerialFlash.h>
#include <LittleFS.h>
#include <Time.h>
#include <TimeLib.h>
#include <elapsedMillis.h>

extern "C" uint8_t external_psram_size;

// User Arduino Libraries
//
#include "ArduinoEclipseDefs.h"
#include "UtilFns.h"
#include "Teensy41_Util.h"
#include "SystemProfileScreen.h"

/************************************************************************************
*                         M A C R O S  /  C O N S T A N T S                         *
*************************************************************************************
*/

// Monitor presence of SD card at startup and
// periodically during program execution
//
#define MONITOR_SD_PRESENCE     1

/************************************************************************************
*                     D A T A / T Y P E   D E F I N I T I O N S                     *
*************************************************************************************
*/

/****************
* Types/Structs *
*****************
*/

/************
* Variables *
*************
*/

#if MONITOR_SD_PRESENCE
elapsedMillis           sdUpdateTimer       = 0;
const uint32_t          sdUpdateInterval    = 250;
#endif // MONITOR_SD_PRESENCE

/******************
* Begin Namespace *
*******************
*/
// TODO: Consolidate namespace and clean up overall module organization.
namespace AudioPlatform {

EXTMEM SystemHardwareProfile   systemProfile;

/****************
* End Namespace *
*****************
*/
} // namespace AudioPlatform

// WAV and RAW Audio Filenames
//
// The names of WAV, RAW, and preset files used by AudioPlatform components are located (macro
// EXTMEM) and maintained in external PSRAM (16MB PSRAM on AudioPlatform) rather than in the more
// limited 1MB internal RAM space of the Teensy 4.1's IMXRT1060 processor. These structures are
// allocated and owned by AudioPlatform.ino, and exported for access in other AudioPlatform components
// and classes through the SystemHardwareProfile structure (also located in EXTMEM).
//
EXTMEM char             sdWavFileNames[MaxSoundFileNames][MaxSoundFileNameLen];
EXTMEM char             serialFlashFileNames[MaxSoundFileNames][MaxSoundFileNameLen];
EXTMEM char             presetBankFileNames[MaxPresetBankFileNames][MaxPresetBankFileNameLen];

bool fullyInitialized = false;

/************************************************************************************
*                 L O C A L   F U N C T I O N   P R O T O T Y P E S                 *
*************************************************************************************
*
* Local prototype prototypes to avoid forward references (in namespace, but not exported
* in .h file).
*
*/

/******************
* Begin Namespace *
*******************
*/
// TODO: Consolidate namespace and clean up overall module organization.
namespace AudioPlatform {

void    getSystemProfile() FLASHMEM;
void    showSystemProfile() FLASHMEM;

void    updateSdWavFilenames();
void    updateSerialFlashRawFilenames();
void    updatePresetBankFilenames();

/****************
* End Namespace *
*****************
*/
} // namespace AudioPlatform

/************************************************************************************
*                               A P P L I C A T I O N                               *
*************************************************************************************
*
*/

/********************************
* Standard Arduino Setup / Loop *
*********************************
*
* NOTE!! setup() and loop() CANNOT be included in AudioPlatform namespace. This causes
* the startup code to fail with the Teensy 4.1 LED flashing (i.e. Blink program).
*
*/

// setup()
//
// Perform one-time application/system initialization and startup actions
//
void setup() {

    // Initialize USB serial port to computer
    // (Happens automatically on Teensy4.1)
    Serial.begin(115200);

    // Should precede other module inits since it opens/initializes
    // storage system handles (SD, ProgMem, Serial Flash) used or
    // assumed in later component inits.
    AudioPlatform::getSystemProfile();

    // Change the speed of the QSPI clock (and, therefore, EXTMEM
    // access) from its default 88MHz to 132MHz (a 1.5x, 50% increase)
    //
    // Reset QSPI clock from 88 Mhz to 132 MHz (M240310-1)
    CCM_CCGR7 |= CCM_CCGR7_FLEXSPI2(CCM_CCGR_OFF);
    CCM_CBCMR = (CCM_CBCMR & ~(CCM_CBCMR_FLEXSPI2_PODF_MASK | CCM_CBCMR_FLEXSPI2_CLK_SEL_MASK))
              | CCM_CBCMR_FLEXSPI2_PODF(4) | CCM_CBCMR_FLEXSPI2_CLK_SEL(2);
    CCM_CCGR7 |= CCM_CCGR7_FLEXSPI2(CCM_CCGR_ON);

    // Initialize Input Devices
    InputDevices::initInputDevices();

    // Audio Library Device Initialization
    AudioDevices::begin();
    AudioDevices::initializeDeviceSettings();   // ESSENTIAL. Performs actions not done when presets are loaded.

    // Initialize Display Objects
    DisplayScreenBase::setupDisplay();      // Display base class, called once on startup
    Screens::setActiveScreen(ScreenMainMenu);

    // Load Presets last, after all other components up and running, because at startup,
    // presets init will load a default preset and send data to ALL other components.
    fullyInitialized = Presets::initialize();

    // Display basic application info on serial console.
    AudioPlatform::showSystemProfile();

#if MONITOR_SD_PRESENCE
    if (!AudioPlatform::systemProfile.sdCardPresent) {
        Serial.printf("\n\nNo SD Card at Startup\n\n");
    }
#endif // MONITOR_SD_PRESENCE

    // Something Wrong
    if (!fullyInitialized) {
        Serial.printf("AudioPlatform::setup: Initialization failed.\n");
    }
}

// loop()
//
// Simply poll user-interactive devices and screens for new activity, taking
// appropriate actions when new inputs or changes are detected.
//
// No delay() is inserted at the end of loop() (as is often the case in Arduino code)
// in order that polling proceed at the fastest possible rate.
//
void loop() {

    if (fullyInitialized) {
        // Poll physical device interaction (MIDI, Rotary Encoders)
        InputDevices::pollInputDevices();

        // Poll Audio Devices
        // (Some, like synth, need regular update calls)
        AudioDevices::pollAudioDevices();

    // Poll screen activity/interaction
    Screens::pollActiveScreen();

#if MONITOR_SD_PRESENCE
        // Check for presence of SD card if not found
        // on startup or if removed after startup
        if (sdUpdateTimer >= sdUpdateInterval) {
            if (!AudioPlatform::systemProfile.sdCardPresent) {
                if (SD.mediaPresent()) {
                    Serial.printf("\n\nSD Card Insertion Detected\n\n");
                    AudioPlatform::getSystemProfile();
                }
            }
            sdUpdateTimer = 0;
        }
#endif // MONITOR_SD_PRESENCE
    }
}

/************************************************************************************
*                        E X P O R T E D   F U N C T I O N S                        *
*************************************************************************************
*
* Exported functions referenced with namespace prefix "AudioPlatform::"
*
*/

/******************
* Begin Namespace *
*******************
*/
// TODO: Consolidate namespace and clean up overall module organization.
namespace AudioPlatform {

/************************************************************************************
*                           L O C A L   F U N C T I O N S                           *
*************************************************************************************
*
*/

/*****************
* System Profile *
******************
*
* System Profile structure shared among core AudioPlatform components and also
* provides system info to screens and support classes when passed as a constructor
* argument.
*
*/

// getSystemProfile
//
void getSystemProfile() {

    // Application
    //
    systemProfile.appName = ApplicationName;
    systemProfile.appVersion = ApplicationVersion;
    systemProfile.buildDate = BuildDate;
    systemProfile.buildTime = BuildTime;

    // Teensy 4.1 MAC
    //
    uint8_t teensyMAC[6];       // Unique Teensy MAC / Serial Number
    Teensy41_Util::getTeensyMac(teensyMAC);
    memset(systemProfile.teensyMAC, 0, sizeof(systemProfile.teensyMAC));
    for (int i = 0;  i < 6; i++) {
        snprintf(&systemProfile.teensyMAC[3*i], 4, (i == 5) ? "%02x" : "%02x:", teensyMAC[i]);
    }

    // PSRAM
    //
    systemProfile.psRamSize = external_psram_size;

    // Audio Block Allocation
    //
    systemProfile.audioBlockCount = AUDIO_MEMORY_BLOCK_TOTAL;

    // NAND/NOR Flash
    //
    systemProfile.qspiNandFlashPresent = false;
    systemProfile.qspiNandFlashSize = 0;
    systemProfile.qspiNorFlashPresent = false;
    systemProfile.qspiNorFlashSize = 0;
    if (systemProfile.myfs_NOR.begin()) {
        systemProfile.qspiNorFlashPresent = true;
        systemProfile.qspiNorFlashSize =  systemProfile.myfs_NOR.totalSize() / BytesPerMegaByte;
    }
    else if (systemProfile.myfs_NAND.begin()) {
        systemProfile.qspiNandFlashPresent = true;
        systemProfile.qspiNandFlashSize =  systemProfile.myfs_NAND.totalSize() / BytesPerMegaByte;
    }

    // Serial Flash
    //
    systemProfile.serialFlashPresent = false;
    systemProfile.serialFlashSizeMB = 0;
    systemProfile.sfRawFileCount = 0;
    if (SerialFlash.begin(SERIALFLASH_CS_PIN)) {
        systemProfile.serialFlashPresent = true;
        unsigned char chipId[256];
        SerialFlash.readID(chipId);
        systemProfile.serialFlashSizeMB = SerialFlash.capacity(chipId) / BytesPerMegaByte;
        systemProfile.sfRawFileUpdateFn = updateSerialFlashRawFilenames;
        (*systemProfile.sfRawFileUpdateFn)();
    }

    // Program Flash (PROGMEM)
    //
    systemProfile.progmemFsPresent = false;
    if (systemProfile.progmemFs.begin(TotalProgmemStorageSize)) {
        systemProfile.progmemFsPresent = true;
    }

    // SD Card
    //
    systemProfile.sdCardPresent = false;
    systemProfile.sdWavFileCount = 0;
    systemProfile.sdPresetBankFileCount = 0;
    if ((SD.begin(SD_CS_PIN))) {
        systemProfile.sdCardPresent = true;
        uint64_t totalBytes = SD.totalSize();
        uint64_t usedBytes = SD.usedSize();
        systemProfile.sdCardSizeMB = totalBytes / BytesPerMegaByte;
        systemProfile.sdUsedSizeMB = usedBytes / BytesPerMegaByte;
        systemProfile.sdWavFileUpdateFn = updateSdWavFilenames;
        (*systemProfile.sdWavFileUpdateFn)();
        systemProfile.sdPresetBankFileUpdateFn = updatePresetBankFilenames;
        (*systemProfile.sdPresetBankFileUpdateFn)();
    }

    // Large Buffer for temporary storage
    // print content and other data
    for (uint32_t i = 0; i < BigBufferLineCount; i++) {
        systemProfile.largeTempBufLines[i] = &Teensy41_Util::bigBuffer[i * BigBufferLineLength];
    }
}

// showSystemProfile
//
// Display System Profile info on serial console
//
void showSystemProfile() {

    Serial.printf("\nAudioPlatform System Profile...\n\n");

    // Application
    //
    Serial.printf("    %-32s %s\n", "Application:", systemProfile.appName);
    Serial.printf("    %-32s %s %s\n", "Build Date:", systemProfile.buildDate, systemProfile.buildTime);
    Serial.printf("    %-32s %s\n", "Version:", systemProfile.appVersion);
    Serial.printf("    %-32s %s\n", "Teensy MAC:", systemProfile.teensyMAC);

    // PSRAM / Audio Blocks
    //
    Serial.printf("    %-32s %d MB\n", "PSRAM / EXTMEM Size:", systemProfile.psRamSize);
    Serial.printf("    %-32s %d MB\n", "Audio Blocks Allocated:", systemProfile.audioBlockCount);

    // NAND/NOR Flash
    //
    Serial.printf("    %-32s ", "QSPI Flash Size: ");
    if (systemProfile.qspiNorFlashPresent) {
        Serial.printf("%lu MB (NOR)\n", systemProfile.qspiNorFlashSize);
    }
    else if (systemProfile.qspiNandFlashPresent) {
        Serial.printf("%lu MB (NAND)\n", systemProfile.qspiNandFlashSize);
    }
    else {
        Serial.printf("0 MB (NOR/NAND Flash Not Found)\n");
    }

    // Serial Flash
    //
    Serial.printf("    %-32s ", "Serial Flash Size: ");
    if (systemProfile.serialFlashPresent) {
        uint32_t serialFlashSizeMB = systemProfile.serialFlashSizeMB;
        Serial.printf("%lu MB (Winbond %s)\n", serialFlashSizeMB, serialFlashSizeMB == 128 ? "W25Q01JV*Q" : "W25Q512JV*Q");
        Serial.printf("    %-32s %u\n", "Serial Flash RAW File Count: ", systemProfile.sfRawFileCount);
    } else {
        Serial.printf("0 MB (Serial Flash Not Found)\n");
    }

    // SD Card
    //
    Serial.printf("    %-32s ", "SD Card Size: ");
    if (systemProfile.sdCardPresent) {
        Serial.printf("Total %lu MB, Used %lu MB\n", systemProfile.sdCardSizeMB, systemProfile.sdUsedSizeMB);
        Serial.printf("    %-32s %u\n", "SD Card WAV File Count: ", systemProfile.sdWavFileCount);
    } else {
        Serial.printf("0 MB (Not Found)\n");
    }

    if (systemProfile.sdCardPresent) {
        Serial.printf("    %-32s %u\n", "SD Card Preset Bank File Count: ", systemProfile.sdPresetBankFileCount);
    }

    // Object Addresses (for confirmation of placement)
    //
    Serial.printf("\n    PSRAM/EXTMEM Data Addresses...\n");
#if SHOW_PRESET_SETTINGS
    Presets::showAddresses();
#endif // SHOW_PRESET_SETTINGS
    Serial.printf("    %-32s 0x%8x\n", "sfRawFileNames:",   systemProfile.sfRawFileNames[0]);
    Serial.printf("    %-32s 0x%8x\n", "&sfRawFileNames:",  &systemProfile.sfRawFileNames[0]);
    Serial.printf("    %-32s 0x%8x\n", "sdWavFileNames:",   systemProfile.sdWavFileNames[0]);
    Serial.printf("    %-32s 0x%8x\n", "&sdWavFileNames:",  &systemProfile.sdWavFileNames[0]);
    Serial.printf("    %-32s 0x%8x\n", "systemProfile:",    &systemProfile);

    char tempBuf[16];
    UtilFns::uint32ToString(sizeof(systemProfile), tempBuf);
    Serial.printf("    %-32s %10s\n", "sizeof(systemProfile):", tempBuf);
}

/***********************************
* System Profile Function Pointers *
************************************
*
* Core component functions are made available to support classes via pointers in System Profile
*
*/

// updateSdWavFilenames()
//
void updateSdWavFilenames() {

//    Serial.printf("\n\nAudioPlatform::updateSdWavFilenames: sizeof(sdWavFileNames) %d\n\n", sizeof(sdWavFileNames));

    memset(sdWavFileNames, 0, sizeof(sdWavFileNames));
    memset(systemProfile.sdWavFileNames, 0, sizeof(systemProfile.sdWavFileNames));
    systemProfile.sdWavFileCount = Teensy41_Util::getSdWavFileInfo(WavFileSuffix, systemProfile.sdWavFileSizes, &sdWavFileNames[0], systemProfile.sdWavFileNames);
//    AudioDevices::setMediaPlayerSoundFiles();
}

// updateSerialFlashRawFilenames()
//
void updateSerialFlashRawFilenames() {

//    Serial.printf("\n\nAudioPlatform::updateSerialFlashRawFilenames: sizeof(serialFlashFileNames) %d\n\n", sizeof(serialFlashFileNames));

    memset(serialFlashFileNames, 0, sizeof(serialFlashFileNames));
    memset(systemProfile.sfRawFileNames, 0, sizeof(systemProfile.sfRawFileNames));
    systemProfile.sfRawFileCount = Teensy41_Util::getSerialFlashRawFileInfo(RawFileSuffix, systemProfile.sfRawFileSizes, &serialFlashFileNames[0], systemProfile.sfRawFileNames);
//    Serial.printf("AudioPlatform::updateSerialFlashRawFilenames: systemProfile.sfRawFileCount %d\n", systemProfile.sfRawFileCount);
//    AudioDevices::setMediaPlayerSoundFiles();
}

// updatePresetBankFilenames()
//
void updatePresetBankFilenames() {

//    Serial.printf("\n\nAudioPlatform::updatePresetBankFilenames:\n\n");

    systemProfile.sdPresetBankFileCount = Teensy41_Util::getSdDirFileInfo(PresetBankFileSuffix, systemProfile.sdPresetBankFileSizes, &presetBankFileNames[0], systemProfile.sdPresetBankFileNames);
}


/****************
* End Namespace *
*****************
*/
} // namespace AudioPlatform


