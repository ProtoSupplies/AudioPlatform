/************************************************************************************
*                                    Presets.ino                                    *
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
*
* FM Synth Preset Storage
*
*   • Compressed/Banked preset length 128 bytes
*   • 512 Presets 512 x 128 = 65536 bytes
*   • Preset banks = 32 presets
*   • 10 permanent banks in PROGMEM (data in "AppData/FmSynthPresetBanks.h")
*   • 6 User Banks, initialized to InitVoice. Replaced by contents of SD files
*     USER1.syx - USER6.syx, if present on SD
*   • Both permanent and user banks copied to EXTMEM for runtime access
*
* Preset Settings File Organization
*
*   Take note! Once a presets file has been created, the order in which devices are written to and
*   read from the presets block (defined in Presets.h enum PresetDevice) must be preserved in ALL
*   subsequent loads and saves of the presets file, across all application versions. If the order is
*   changed, existing files must be converted to the newer format by an external utility.
*
* Developer Disclaimer
*
*  Apologies for the abundance of switch/case handling of various device and system parameters.
*  From a coding perspective, there is likely a cleaner way to perform these actions using
*  data structures and other mechanisms. But I feel that this risks making the actions here more
*  opaque and difficult to follow.
*
*  The prolific use switch/case makes the introduction of new settings or changes to existing
*  device settings a much more tedious activity, as the changes must be applied not only in this
*  module, but often also other application modules as well. But the procedures are well-documented
*  in the AudioPlatform program documentation, and are otherwise straightforward.
*
*/

/****************
* "Pre-Defines" *
*****************
*
* Definitions that affect subsequent external references
*
*/

// RENAME_DEFAULT_BANKNAME
//
// Used only in saveDefaultPresetBank() to rename default bank:
//  1. Define NewPresetsBankFilename in AudioPlatformGlobal.h
//  2. Define RENAME_DEFAULT_BANKNAME as 1 below
//  3. Compile and restart AudioPlatform
//  4. In Presets Management screen, select Save Default (the renaming will be done here)
//  5. Take no further actions in AudioPlatform
//  6. Delete the old definition of DefaultPresetsBankFilename in AudioPlatformGlobal.h
//  7. Rename NewPresetsBankFilename to DefaultPresetsBankFilename in AudioPlatformGlobal.h
//  8. Define RENAME_DEFAULT_BANKNAME as 0 below
//  9. Compile and restart AudioPlatform

#define RENAME_DEFAULT_BANKNAME 0

#if RENAME_DEFAULT_BANKNAME
#else // Original Settings
#endif // RENAME_DEFAULT_BANKNAME / Original Settings

/************************************************************************************
*                       E X T E R N A L   R E F E R E N C E S                       *
*************************************************************************************
*/

// Application Components
//
#include "Presets.h"
#include "PresetDefs/PresetDataStructs.h"
#include "PresetDefs/PresetDefaultData.h"
#include "AppData/FmSynthPresetBanks.h"

// User Arduino Libraries
//
#include "ArduinoEclipseDefs.h"
#include "Teensy41_Util.h"
#include "PresetsScreen.h"

// Teensyduino Libraries
//
#include <LittleFS.h>
#include <SD.h>

/************************************************************************************
*                   L O C A L   M A C R O S  /  C O N S T A N T S                   *
*************************************************************************************
*/

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
namespace Presets {

bool        loadDefaultPresetBank();
bool        saveDefaultPresetBank();
bool        resetDefaultPresetBank();
void        resetPresetModifiedFields();
void        resetPresetDeviceModifiedFields();

bool        loadSdPresetBank(const char* sdFilename, CommandButtonId& loadDest);
bool        saveSdPresetBank(const char* sdFilename);
void        copyPresetSettings(uint16_t destPreset, uint16_t srcPreset, uint16_t deviceId);

bool        loadPreset(uint16_t presetNum, bool import = false);
bool        savePreset(uint16_t presetNum);
const char* getImportPresetName(ButtonId presetNum);

void        initFmSynthPresets();
void        loadFmSynthUserPresets();

void        updateSystem();

void        setAudioDeviceParams();
void        setInputOutputParams();
void        setScreenParams();

/************************************************************************************
*               L O C A L   D A T A / T Y P E   D E F I N I T I O N S               *
*************************************************************************************
*
* Data types and variables visible only within this module
*
*/

/**************
* Preset Data *
***************
*
*  Storage for presets loaded from and saved to presets banks (active or import).
*  The settings are the "official" reference for the application, and
*  must always reflect, and be reflected in all other application locations
*  (screens, physical device states, etc.).
*
*  It is the responsibility and function of application modules and components
*  to maintain and enforce this requirement.
*
*/

// Individual Device Settings
//
// When presets are loaded and saved (either from/to active or import bank buffers)
// their various preset devices are copied from/to separate individual structures
// that hold the operating copies of the current preset's devices.
//
// In other words, the currently selected preset is represented in memory, by these
// individual structures, as distinct from the preset bank buffers, which store
// the individual device settings for ALL the presets in a bank.
//
EXTMEM  PresetCommonData    presetCommon;
EXTMEM  MediaPlayerSettings wavPlayerPreset;
EXTMEM  BasicSynthSettings  basicSynthPreset;
EXTMEM  MidiSettings        midiPreset;
EXTMEM  RvbSettings         rvbPreset;
EXTMEM  EfxSettings         efxPreset;
EXTMEM  AudioIoSettings     audioIoPreset;
EXTMEM  MediaPlayerSettings rawPlayerPreset;
EXTMEM  FmSynthSettings     fmSynthPreset;
EXTMEM  StereoDelaySettings stereoDelayPreset;

// Device Modified Flags
// Indicate devices in currently loaded preset which have been modified
//
EXTMEM  bool                presetDeviceModified[PresetDeviceCount];

// Single Preset Block, Header and All Devices
//
uint8_t*                    presetBlock;

// Presets Bank File Buffer
// (All Presets + Global Bank Data)
//
EXTMEM  PresetBlock         activePresetsBankBuf[PresetBlockCount];
EXTMEM  uint8_t             activePresetsBankGlobalDataBuf[PresetBankGlobalDataSize];

// Import Presets Bank File Buffer
// ("Shadow" bank for imports of presets to active bank)
//
EXTMEM  PresetBlock         importPresetsBankBuf[PresetBlockCount];

// Populated at startup from FM synth preset banks
//
//  fmSynthPresetNameTexts  ASCII text names of presets in SYSX banks
//  fmSynthPresetNames      Pointers to entries in fmSynthPresetNameTexts, required format
//                          for access in ParamButtonInfo source field "const char** paramLabels"/
//  fmSynthBankNames
//  fmSynthPresetsBuffer
//  tempbuf
//
EXTMEM  char                fmSynthPresetNameTexts[FmSynthPresetCount][FmSynthPresetNameLength + 1];
EXTMEM  const char*         fmSynthPresetNames[FmSynthPresetCount];
EXTMEM  const char*         fmSynthBankNames[FmSynthPresetsBankCount];
EXTMEM  uint8_t             fmSynthPresetsBuffer[FmSynthPresetCount * FmSynthPresetBankedLength];
EXTMEM  uint8_t             fmSynthBankFileBuf[FmSynthSysxBankFileSize];

/******************************************
* Preset and  Application PROGMEM Storage *
*******************************************
*
*/

// Application File System and Presets File
//
LittleFS_Program&           progmemFs = AudioPlatform::systemProfile.progmemFs;
File                        presetsFile;

// Initially false, set true in startup initialize() call if file system is valid
// and presets file exists. Thereafter, all operations disabled when initialized == false.
//
bool                        initialized         = false;

// Initial preset globals state
//
PresetCmndState             presetCmndState     = cmndStateCancel;
uint16_t                    currentPresetNum    = 0;

// Current Preset Bank Info
//
char                        activePresetBankFileName[PresetFileNameSize];
char                        importPresetBankFileName[PresetFileNameSize];
bool                        presetBankModified  = false;

/************************************************************************************
*                        E X P O R T E D   F U N C T I O N S                        *
*************************************************************************************
*
* Exported functions in .h referenced with namespace prefix "Presets::"
*
*/

/***************************
* Startup / Initialization *
****************************
*/

// initialize()
//
bool initialize() {

    Serial.printf("\nPresets::initialize: \n");

    if (!initialized) {
        if (AudioPlatform::systemProfile.progmemFsPresent) {
            if (!progmemFs.exists(DefaultPresetsBankFilename)) {
                Serial.printf("\nPresets::initialize: Can't Find Default Presets Bank %s. Recreating.\n", DefaultPresetsBankFilename);
                if (!resetDefaultPresetBank()) {
                    Serial.printf("\nPresets::initialize: Reset Default Presets Bank Failed\n");
                } else {
                    initialized = loadDefaultPresetBank();
                }
            } else {
                initialized = loadDefaultPresetBank();
            }
        }

        initFmSynthPresets();
    }

    if (initialized) {
        Serial.printf("\nPresets::initialize: AudioPlatform File System is operational\n", DefaultPresetsBankFilename);

        // Select initial startup preset
        selectPreset(currentPresetNum);
    }

    return initialized;
}

// presetsInitialized
//
// Return initialization state
//
bool presetsInitialized() {

    return initialized;
}

/***********************
* Preset Access / Mgmt *
************************
*/

// presetsCallback
//
// Preset commands invoked from screens PresetScreen and PresetsLiveScreen to perform
// user-initiated actions made by interaction with these screen elements.
//
// Arguments:
//
//      CommandButtonId cmdId       Command ID from enumeration in module DisplayScreenBase.h
//      uint16_t&       numberParam Allows parameter passing in and out
//      void*           voidParam   Allows inputs/output of various data types, based on cmdId
//
const char* presetsCallback(CommandButtonId cmdId, uint16_t& numberParam, void* voidParam) {

    const char* returnVal = 0;

    switch (cmdId) {

        case CmdLoadPreset:
//            Serial.printf("Presets::presetsCallback CmdLoadPreset %d\n", numberParam);
            selectPreset(numberParam);
            resetPresetDeviceModifiedFields();
            break;

        case CmdImportPreset:
//            Serial.printf("Presets::presetsCallback CmdImportPreset %d\n", numberParam);
            selectPreset(numberParam, true);
            break;

        case CmdSavePreset:
//            Serial.printf("Presets::presetsCallback CmdSavePreset %d\n", numberParam);
            resetPresetDeviceModifiedFields();
            savePreset(numberParam);
            break;

        case CmdClearPreset:
//            Serial.printf("Presets::presetsCallback CmdClearPreset %d\n", numberParam);
            break;

        case CmdGetPresetNumber:
//            Serial.printf("Presets::presetsCallback CmdGetPresetNumber %d\n", numberParam);
            numberParam = currentPresetNum;
            break;

        case CmdGetPresetName:
//            Serial.printf("Presets::presetsCallback CmdGetPresetName %d\n", numberParam);
            returnVal = getPresetName(numberParam);
            break;

        case CmdGetImportPresetName:
//            Serial.printf("Presets::presetsCallback CmdGetPresetName %d\n", numberParam);
            returnVal = getImportPresetName(numberParam);
            break;

        case CmdSetPresetName:
//            Serial.printf("Presets::presetsCallback CmdSetPresetName %d, %s\n", numberParam, textParam);
            setPresetName(numberParam, (const char*) voidParam);
            break;

        case CmdCopyDeviceSettings: {
            uint16_t deviceId = *((uint16_t *)voidParam);
//            Serial.printf("Presets::presetsCallback CmdCopyDeviceSettings device %d from preset %d to preset %d\n", deviceId, numberParam, currentPresetNum);
            copyPresetSettings(currentPresetNum, numberParam, deviceId);
            }
            break;

        case CmdSetPresetModified:
//            Serial.printf("Presets::presetsCallback CmdSetPresetModified %d\n", numberParam);
            setPresetModified(numberParam, *((bool*) voidParam));
            presetBankModified = true;
            break;

        case CmdGetPresetModified:
            *((bool*) voidParam) = getPresetModified(numberParam);
//            Serial.printf("Presets::presetsCallback CmdGetPresetModified returning %d\n", *((bool*) voidParam));
            break;

        case CmdSetPresetDeviceModified:
//            Serial.printf("Presets::presetsCallback CmdSetPresetDeviceModified %d\n", numberParam);
            presetDeviceModified[numberParam] = true;
            break;

        case CmdGetPresetDeviceModified:
            *((bool*) voidParam) = presetDeviceModified[numberParam];
//            Serial.printf("Presets::presetsCallback CmdGetPresetDeviceModified returning %d\n", *((bool*) voidParam));
            break;

        case CmdSetBankModified:
//            Serial.printf("Presets::presetsCallback CmdSetBankModified %d\n", numberParam);
            presetBankModified = (numberParam != 0);
            break;

        case CmdGetBankModified:
//            Serial.printf("Presets::presetsCallback CmdGetModified returning %d\n", presetBankModified);
            numberParam = presetBankModified ? 1: 0;
            break;

        case CmdLoadDefaultBank:
//            Serial.printf("Presets::presetsCallback CmdLoadDefaultBank\n");
            loadDefaultPresetBank();
            resetPresetModifiedFields();
            resetPresetDeviceModifiedFields();
            presetBankModified = false;
            break;

        case CmdSaveDefaultBank:
//            Serial.printf("Presets::presetsCallback CmdSaveDefaultBank\n");
            resetPresetModifiedFields();    // should precede save
            resetPresetDeviceModifiedFields();
            saveDefaultPresetBank();
            presetBankModified = false;
            break;

        case CmdLoadSdBank:
            // TODO: Need to return success/fail code to caller
//            Serial.printf("Presets::presetsCallback CmdLoadSdBank %s\n", (const char*) voidParam);

            // Following code modified solely in support of workaround for B250308-1
            // so that audio interrupts can be disabled when accessing SD card file
            // system when SD WAV Player is active.
            if (wavPlayerPreset.enabled) {
                AudioNoInterrupts();
            }
            loadSdPresetBank((const char*) voidParam, (CommandButtonId&)numberParam);
            if (wavPlayerPreset.enabled) {
                AudioInterrupts();
            }

            resetPresetModifiedFields();
            resetPresetDeviceModifiedFields();
            presetBankModified = false;
            break;

        case CmdSaveSdBank:
            // TODO: Need to return success/fail code to caller
//            Serial.printf("Presets::presetsCallback CmdSaveSdBank %s\n", (const char*) voidParam));
            resetPresetModifiedFields();    // should precede save
            resetPresetDeviceModifiedFields();

            // Following code modified solely in support of workaround for B250308-1
            // so that audio interrupts can be disabled when accessing SD card file
            // system when SD WAV Player is active.
            if (wavPlayerPreset.enabled) {
                AudioNoInterrupts();
            }
            saveSdPresetBank((const char*) voidParam);
            if (wavPlayerPreset.enabled) {
                AudioInterrupts();
            }

            presetBankModified = false;
            break;

        case CmdGetBankFilename:
//            Serial.printf("Presets::presetsCallback CmdGetBankFilename %s\n", activePresetBankFileName);
            returnVal = ((CommandButtonId)numberParam == BtnActiveBank) ? activePresetBankFileName : importPresetBankFileName;
            break;

        // NOTE: The following 2 commands are not specifically associated with preset management
        // but are included so that they can provide additional functionality to the PresetsLive
        // screen that may be unavailable otherwise (ex.: if not defined for the selected MIDI
        // input/controller device.
        case CmdIgnoreNoteOff:
//            Serial.printf("Presets::presetsCallback CmdIgnoreNoteOff %d\n", (numberParam == 1));
            AudioDevices::handleIgnoreNoteOff(numberParam == 1);
//            InputDevices::setIgnoreNoteOff((numberParam == 1));
            break;

        case CmdAllNotesOff:
//            Serial.printf("Presets::presetsCallback CmdAllNotesOff\n");
            AudioDevices::handleAllNotesOff();
            break;

        default:
            break;
    }

    return returnVal;
}

// selectPreset
//
// Select and load preset, updating presets screen.
//
void selectPreset(uint16_t presetNum, bool import) {

//    Serial.printf("Presets::selectPreset: %d import %d\n", presetNum, import);

    if (presetNum < PresetCount) {
        // Read preset data
        loadPreset(presetNum, import);

        // Reset cmnd button state after load
        presetCmndState = cmndStateCancel;

        // Update presetNum's screen button state/appearance
        (Screens::getScreenProfile(ScreenPresets))->scrnInstance.setBtnProperties(presetNum, Screens::activeScreen == ScreenPresets);

        // Send update to any active screen. All screens are derived from DisplayScreenBase which defines
        // updateScreen() as virtual. So, only screens that override updateScreen will receive the message,
        // and only when they are the active screen.
        Screens::updateStatusDisplay();
    }
}

// setPresetName
//
// Set the name of the currently open preset
//
void setPresetName(ButtonId presetNum, const char* presetName) {

//    Serial.printf("Presets::setPresetName: presetNum %d %s (currentPresetNum %d)\n", presetNum, presetName, currentPresetNum);

    PresetCommonData* presetCommonData = (PresetCommonData*) &activePresetsBankBuf[presetNum];
    strncpy((char*)presetCommonData->presetName, presetName, PresetNameSize-1);

    if (presetNum == currentPresetNum) {

        // Copy name to preset common
        strncpy(presetCommon.presetName, presetName, PresetNameSize-1);

        // Null-terminate common name in case source name filled entire buffer
        presetCommon.presetName[PresetNameSize-1] = 0;

        // FFS: Is save on set presetname correct? Or, should user have to
        // save preset using AudioPlatform presets screen?
        savePreset(currentPresetNum);
    }
}

// getPresetName
//
// Return the name a specific presetNum [0,n-1] in the active
//
const char* getPresetName(ButtonId presetNum) {

    PresetCommonData* presetCommonData = (PresetCommonData*) &activePresetsBankBuf[presetNum];
//    Serial.printf("Presets::getPresetName: returning %s\n", presetCommonData->presetName);

    return (const char*)presetCommonData->presetName;
//    return &presetCommon.presetName[0];
}

// setPresetModified
//
// Set the preset modified value [0,1] of the specified preset
//
void setPresetModified(ButtonId presetNum, bool modified) {

//    Serial.printf("Presets::setPresetModified: presetNum %d modified %d\n", presetNum, modified);

    PresetCommonData* presetCommonData = (PresetCommonData*) &activePresetsBankBuf[presetNum];
    presetCommonData->presetModified = (float) modified;
}

// getPresetModified
//
// Return the preset modified [0, 1] value of the specified preset
//
bool getPresetModified(ButtonId presetNum) {

    PresetCommonData* presetCommonData = (PresetCommonData*) &activePresetsBankBuf[presetNum];

//    Serial.printf("Presets::getPresetModified: presetNum %d returning %d\n", presetNum, (bool)presetCommonData->presetModified);

    return (bool)presetCommonData->presetModified;
}

// getPresetNumber
//
// Return the number [0,n-1] of the currently open preset
//
uint16_t getPresetNumber() {

    return currentPresetNum;
}

// showPresetNames
//
// Print list of preset names to serial
//
void showPresetNames() {

    Serial.printf("\nPresets::showPresetNames:\n");

    for (uint16_t presetNum = 0; presetNum < PresetCount; presetNum++) {
        PresetCommonData* presetCommonData = (PresetCommonData*) &activePresetsBankBuf[presetNum];
        Serial.printf("Presets::showPresetNames: presetNum %d presetName %s\n", presetNum, (const char*)presetCommonData->presetName);
    }
    Serial.printf("\n\n");
}

/***********************************
* Screens Data -> Preset Interface *
************************************
*
* Device data generated on screens and conveyed here via callback assignments in the RotaryParams
* entries for the rotary encoders used on ParamButton/DataEntry screens. (See entries in "ScreenDef"
* files such as BasicSynthScreehDefs.h).
*
*/

// setWavPlayerParam
//
void setWavPlayerParam(uint16_t paramId, float paramVal) {

//    Serial.printf("Presets::setWavPlayerParam paramId: %d paramVal: %f\n", paramId, paramVal);

    switch (paramId) {

        case MediaPlayWav_Status:
            wavPlayerPreset.enabled = paramVal;
            Screens::updateStatusDisplay();
            break;

        case MediaPlayWav_Level:
            wavPlayerPreset.outLevel = paramVal;
            break;

        case MediaPlayWav_DlySend:
            wavPlayerPreset.dlySend = paramVal;
            break;

        case MediaPlayWav_RvbSend:
            wavPlayerPreset.rvbSend = paramVal;
            break;

        case MediaPlayWav_SoundBank:
            wavPlayerPreset.soundBank = paramVal;
            break;

        case MediaPlayWav_NoteShift:
//            Serial.printf("Presets::setWavPlayerParam MediaPlayWav_NoteShift paramVal: %f\n", paramVal);
            wavPlayerPreset.noteShift = paramVal;
            break;

        case MediaPlayWav_NoteRangeMin:
//            Serial.printf("Presets::setWavPlayerParam MediaPlayWav_NoteRangeMin paramVal: %f\n", paramVal);
            wavPlayerPreset.noteRangeMin = paramVal;
            break;

        case MediaPlayWav_NoteRangeMax:
//            Serial.printf("Presets::setWavPlayerParam MediaPlayWav_NoteRangeMax paramVal: %f\n", paramVal);
            wavPlayerPreset.noteRangeMax = paramVal;
            break;

        case MediaPlayWav_PanType:
            wavPlayerPreset.panType = paramVal;
            break;

        case MediaPlayWav_PanParameter:
            wavPlayerPreset.panParameter = paramVal;
            break;
    }

    // Propagate Settings to Other AudioPlatfom Components
    AudioDevices::setWavPlayerParam(paramId, paramVal);
    InputDevices::setWavPlayerParam(paramId, paramVal);
    Screens::setWavPlayerParam(paramId, paramVal);
}

// setRawPlayerParam
//
void setRawPlayerParam(uint16_t paramId, float paramVal) {

//    Serial.printf("Presets::setRawPlayerParam paramId: %d paramVal: %f\n", paramId, paramVal);

    switch (paramId) {

        case MediaPlayRaw_Status:
            rawPlayerPreset.enabled = paramVal;
            Screens::updateStatusDisplay();
            break;

        case MediaPlayRaw_Level:
            rawPlayerPreset.outLevel = paramVal;
            break;

        case MediaPlayRaw_DlySend:
            rawPlayerPreset.dlySend = paramVal;
            break;

        case MediaPlayRaw_RvbSend:
            rawPlayerPreset.rvbSend = paramVal;
            break;

        case MediaPlayRaw_SoundBank:
            rawPlayerPreset.soundBank = paramVal;
            break;

        case MediaPlayRaw_NoteShift:
            rawPlayerPreset.noteShift = paramVal;
            break;

        case MediaPlayRaw_NoteRangeMin:
            rawPlayerPreset.noteRangeMin = paramVal;
            break;

        case MediaPlayRaw_NoteRangeMax:
//            Serial.printf("Presets::setRawPlayerParam paramId: MediaPlayRaw_NoteRangeMax paramVal: %f\n", paramId, paramVal);
            rawPlayerPreset.noteRangeMax = paramVal;
            break;

        case MediaPlayRaw_PanType:
            rawPlayerPreset.panType = paramVal;
            break;

        case MediaPlayRaw_PanParameter:
            rawPlayerPreset.panParameter = paramVal;
   }

    // Propagate Settings to Other AudioPlatfom Components
    AudioDevices::setRawPlayerParam(paramId, paramVal);
    InputDevices::setRawPlayerParam(paramId, paramVal);
    Screens::setRawPlayerParam(paramId, paramVal);
}

// setBasicSynthParam
//
void setBasicSynthParam(uint16_t paramId, float paramVal) {

//    Serial.printf("Presets::setBasicSynthParam paramId: %d paramVal: %f\n", paramId, paramVal);

    switch (paramId) {

        case BasicSynth_Status:
            basicSynthPreset.enabled = paramVal;
            Screens::updateStatusDisplay();
            break;

        case BasicSynth_Level:
            basicSynthPreset.outLevel = paramVal;
            break;

        case BasicSynth_DlySend:
            basicSynthPreset.dlySend = paramVal;
            break;

        case BasicSynth_RvbSend:
            basicSynthPreset.rvbSend = paramVal;
            break;

        case BasicSynth_Type:
            basicSynthPreset.synthType = paramVal;
            break;

        case BasicSynth_NoteShift:
            basicSynthPreset.noteShift = paramVal;
            break;

        case BasicSynth_NoteRangeMin:
            basicSynthPreset.noteRangeMin = paramVal;
            break;

        case BasicSynth_NoteRangeMax:
            basicSynthPreset.noteRangeMax = paramVal;
            break;

        case BasicSynth_Attack:
            basicSynthPreset.attack  = paramVal;
            break;

        case BasicSynth_Decay:
            basicSynthPreset.decay   = paramVal;
            break;

        case BasicSynth_Sustain:
            basicSynthPreset.sustain = paramVal;
            break;

        case BasicSynth_Release:
            basicSynthPreset.release = paramVal;
            break;

        case BasicSynth_ModFreq:
            basicSynthPreset.modFreq = paramVal;
            break;

        case BasicSynth_ModAmt:
            basicSynthPreset.modAmt  = paramVal;
           break;

        case BasicSynth_PanType:
            basicSynthPreset.panType  = paramVal;
            break;

        case BasicSynth_PanParameter:
            basicSynthPreset.panParameter  = paramVal;
            break;
    }

    // Propagate Settings to Other AudioPlatfom Components
    AudioDevices::setBasicSynthParam(paramId, paramVal);
    InputDevices::setBasicSynthParam(paramId, paramVal);
    Screens::setBasicSynthParam(paramId, paramVal);
}

// setFmSynthParam
//
void setFmSynthParam(uint16_t paramId, float paramVal) {

//    Serial.printf("Presets::setFmSynthParam paramId: %d paramVal: %f\n", paramId, paramVal);

    switch (paramId) {

        case FmSynth_Status:
            fmSynthPreset.enabled = paramVal;
            Screens::updateStatusDisplay();
            break;

        case FmSynth_Level:
            fmSynthPreset.outLevel = paramVal;
            break;

        case FmSynth_DlySend:
            fmSynthPreset.dlySend = paramVal;
            break;

        case FmSynth_RvbSend:
            fmSynthPreset.rvbSend = paramVal;
            break;

        case FmSynth_PresetNum:
            fmSynthPreset.presetNum = paramVal;
            break;

        case FmSynth_NoteShift:
            fmSynthPreset.noteShift = paramVal;
            break;

        case FmSynth_NoteRangeMin:
            fmSynthPreset.noteRangeMin = paramVal;
            break;

        case FmSynth_NoteRangeMax:
            fmSynthPreset.noteRangeMax = paramVal;
            break;

        case FmSynth_PitchBendRange:
            fmSynthPreset.pitchBendRange = paramVal;
            break;

        case FmSynth_PanType:
            fmSynthPreset.panType = paramVal;
            break;

        case FmSynth_PanParameter:
            fmSynthPreset.panParameter = paramVal;
            break;
    }

    // Propagate Settings to Other AudioPlatfom Components
    AudioDevices::setFmSynthParam(paramId, paramVal);
    InputDevices::setFmSynthParam(paramId, paramVal);
    Screens::setFmSynthParam(paramId, paramVal);
}

// setMidiParam
//
void setMidiParam(uint16_t paramId, float paramVal) {

//    Serial.printf("Presets::setMidiParam paramId: %d paramVal: %f\n", paramId, paramVal);

    switch (paramId) {

        default:
            break;
    }

    // Propagate Settings to Other AudioPlatfom Components
    AudioDevices::setMidiParam(paramId, paramVal);
    InputDevices::setMidiParam(paramId, paramVal);
    Screens::setMidiParam(paramId, paramVal);
}

// setRvbParam
//
void setRvbParam(uint16_t paramId, float paramVal) {

//    Serial.printf("Presets::setRvbParam paramId: %d paramVal: %f\n", paramId, paramVal);

    switch (paramId) {

        case Reverb_Level:
            rvbPreset.rvbLevel = paramVal;
            break;

        case Reverb_RoomSize:
            rvbPreset.rvbRoomSize = paramVal;
            break;

        case Reverb_Diffusion:
            rvbPreset.rvbDiffusion = paramVal;
            break;

        case Reverb_PchShft:
            rvbPreset.rvbPchShft = paramVal;
            break;

        case Reverb_PchShftAmt:
            rvbPreset.rvbPchShftAmt = paramVal;
            break;

        case Reverb_ShimPchShft:
            rvbPreset.rvbShimShft = paramVal;
            break;

        case Reverb_ShimShftRatio:
            rvbPreset.rvbShimShftRto = paramVal;
            break;

        case Reverb_ShimAmt:
            rvbPreset.rvbShimAmt = paramVal;
            break;
    }

    // Propagate Settings to Other AudioPlatfom Components
    AudioDevices::setRvbParam(paramId, paramVal);
    InputDevices::setRvbParam(paramId, paramVal);
    Screens::setRvbParam(paramId, paramVal);
}

// setEfxParam
//
void setEfxParam(uint16_t paramId, float paramVal) {

//    Serial.printf("Presets::setEfxParam paramId: %d paramVal: %f\n", paramId, paramVal);

    // Set values of dummy place holders added in rev 0.3.4.4
    //
    switch (paramId) {

        case FutureEfx_Level:
            efxPreset.efxGain = paramVal;
            break;

        case FutureEfx_Param2:
            efxPreset.param2 = paramVal;
            break;

        case FutureEfx_Param3:
            efxPreset.param3 = paramVal;
            break;

        case FutureEfx_Param4:
            efxPreset.param4 = paramVal;
            break;
    }

    // Propagate Settings to Other AudioPlatfom Components
    AudioDevices::setEfxParam(paramId, paramVal);
    InputDevices::setEfxParam(paramId, paramVal);
    Screens::setEfxParam(paramId, paramVal);
}

// setStereoDelayParam
//
void setStereoDelayParam(uint16_t paramId, float paramVal) {

//    Serial.printf("Presets::setStereoDelayParam paramId: %d paramVal: %f\n", paramId, paramVal);

    switch (paramId) {

        // Delay Left
        //
        case StereoDelay_DlyL_DlyTime:
            stereoDelayPreset.delayLeftDelayTime = paramVal;
            break;

        case StereoDelay_DlyL_Gain:
            stereoDelayPreset.delayLeftLevel = paramVal;
            break;

        case StereoDelay_DlyL_RvbSend:
            stereoDelayPreset.delayLeftRvbSend = paramVal;
            break;

        case StereoDelay_DlyL_Efx3Send:
            stereoDelayPreset.delayLeftEfx3Send  = paramVal;
           break;

        case StereoDelay_DlyL_FB_DlyAmt:
            stereoDelayPreset.delayLeftFbDelayAmt   = paramVal;
            break;

        case StereoDelay_DlyL_xFB_DlyTime:
            stereoDelayPreset.delayLeftxFbDelayTime = paramVal;
            break;

        case StereoDelay_DlyL_xFB_DlyAmt:
            stereoDelayPreset.delayLeftxFbDelayAmt = paramVal;
            break;

        // Delay Right
        //
        case StereoDelay_DlyR_DlyTime:
            stereoDelayPreset.delayRightDelayTime = paramVal;
            break;

        case StereoDelay_DlyR_Gain:
            stereoDelayPreset.delayRightLevel = paramVal;
            break;

        case StereoDelay_DlyR_RvbSend:
            stereoDelayPreset.delayRightRvbSend = paramVal;
            break;

        case StereoDelay_DlyR_Efx3Send:
            stereoDelayPreset.delayRightEfx3Send = paramVal;
            break;

        case StereoDelay_DlyR_FB_DlyAmt:
            stereoDelayPreset.delayRightFbDelayAmt = paramVal;
            break;

        case StereoDelay_DlyR_xFB_DlyTime:
            stereoDelayPreset.delayRightxFbDelayTime = paramVal;
            break;

        case StereoDelay_DlyR_xFB_DlyAmt:
            stereoDelayPreset.delayRightxFbDelayAmt = paramVal;
            break;
    }

    // Propagate Settings to Other AudioPlatfom Components
    AudioDevices::setStereoDelayParam(paramId, paramVal);
    InputDevices::setStereoDelayParam(paramId, paramVal);
    Screens::setStereoDelayParam(paramId, paramVal);
}

// setInputOutputParam
//
void setInputOutputParam(uint16_t paramId, float paramVal) {

//    Serial.printf("Presets::setInputOutputParam paramId: %d paramVal: %f\n", paramId, paramVal);

    switch (paramId) {

        case AudioIo_LineIn_Status:
            audioIoPreset.lineInEnabled = paramVal;
            Screens::updateStatusDisplay();
            break;

        case AudioIo_LineIn_Level:
            audioIoPreset.lineInLevel = paramVal;
            break;

        case AudioIo_LineOut_Level:
            audioIoPreset.lineOutLevel = paramVal;
            break;

        case AudioIo_Phone_Volume:
            audioIoPreset.phoneLevel = paramVal;
            break;

        case AudioIo_LineIn_DlySend:
            audioIoPreset.lineInDlySend = paramVal;
            break;

        case AudioIo_LineIn_RvbSend:
            audioIoPreset.lineInRvbSend = paramVal;
            break;
    }

    // Propagate Settings to Other AudioPlatfom Components
    AudioDevices::setInputOutputParam(paramId, paramVal);
    InputDevices::setInputOutputParam(paramId, paramVal);
    Screens::setInputOutputParam(paramId, paramVal);
}

/***********************************
* Preset Data -> Screens Interface *
************************************
*
* Preset data requests from screen elements conveyed here via callbacks within
* various device screens, or directly from Screens.ino.
*
*/

// getWavPlayerParam
//
float getWavPlayerParam(uint16_t paramId) {

    float paramVal = -1;

    //    Serial.printf("Presets::getWavPlayerParam paramId: %d paramVal: %f\n", paramId, paramVal);

    switch (paramId) {

        case MediaPlayWav_Status:
            paramVal = wavPlayerPreset.enabled;
            break;

        case MediaPlayWav_Level:
            paramVal = wavPlayerPreset.outLevel;
            break;

        case MediaPlayWav_DlySend:
            paramVal = wavPlayerPreset.dlySend;
            break;

        case MediaPlayWav_RvbSend:
            paramVal = wavPlayerPreset.rvbSend;
            break;

        case MediaPlayWav_SoundBank:
            paramVal = wavPlayerPreset.soundBank;
            break;

        case MediaPlayWav_NoteShift:
            paramVal = wavPlayerPreset.noteShift;
            break;

        case MediaPlayWav_NoteRangeMin:
            paramVal = wavPlayerPreset.noteRangeMin;
            break;

        case MediaPlayWav_NoteRangeMax:
            paramVal = wavPlayerPreset.noteRangeMax;
            break;

        case MediaPlayWav_PanType:
            paramVal = wavPlayerPreset.panType;
            break;

        case MediaPlayWav_PanParameter:
            paramVal = wavPlayerPreset.panParameter;
            break;
    }

    return paramVal;
}

// getRawPlayerParam
//
float getRawPlayerParam(uint16_t paramId) {

    float paramVal = -1;

    //    Serial.printf("Presets::getRawPlayerParam paramId: %d paramVal: %f\n", paramId, paramVal);

    switch (paramId) {

        case MediaPlayRaw_Status:
            paramVal = rawPlayerPreset.enabled;
            break;

        case MediaPlayRaw_Level:
            paramVal = rawPlayerPreset.outLevel;
            break;

        case MediaPlayRaw_DlySend:
            paramVal = rawPlayerPreset.dlySend;
            break;

        case MediaPlayRaw_RvbSend:
            paramVal = rawPlayerPreset.rvbSend;
            break;

        case MediaPlayRaw_SoundBank:
            paramVal = rawPlayerPreset.soundBank;
            break;

        case MediaPlayRaw_NoteShift:
            paramVal = rawPlayerPreset.noteShift;
            break;

        case MediaPlayRaw_NoteRangeMin:
            paramVal = rawPlayerPreset.noteRangeMin;
            break;

        case MediaPlayRaw_NoteRangeMax:
            paramVal = rawPlayerPreset.noteRangeMax;
            break;

        case MediaPlayRaw_PanType:
            paramVal = rawPlayerPreset.panType;
            break;

        case MediaPlayRaw_PanParameter:
            paramVal = rawPlayerPreset.panParameter;
            break;
    }

    return paramVal;
}

// getBasicSynthParam
//
float getBasicSynthParam(uint16_t paramId) {

    float paramVal = -1;

//    Serial.printf("Presets::getBasicSynthParam paramId: %d paramVal: %f\n", paramId, paramVal);

    switch (paramId) {

        case BasicSynth_Status:
            paramVal = basicSynthPreset.enabled;
            break;

        case BasicSynth_Level:
            paramVal = basicSynthPreset.outLevel;
            break;

        case BasicSynth_DlySend:
            paramVal = basicSynthPreset.dlySend;
            break;

        case BasicSynth_RvbSend:
            paramVal = basicSynthPreset.rvbSend;
            break;

        case BasicSynth_Type:
            paramVal = basicSynthPreset.synthType;
            break;

        case BasicSynth_NoteShift:
            paramVal = basicSynthPreset.noteShift;
            break;

        case BasicSynth_NoteRangeMin:
            paramVal = basicSynthPreset.noteRangeMin;
            break;

        case BasicSynth_NoteRangeMax:
            paramVal = basicSynthPreset.noteRangeMax;
            break;

        case BasicSynth_Attack:
            paramVal = basicSynthPreset.attack;
            break;

        case BasicSynth_Decay:
            paramVal = basicSynthPreset.decay;
            break;

        case BasicSynth_Sustain:
            paramVal = basicSynthPreset.sustain;
            break;

        case BasicSynth_Release:
            paramVal = basicSynthPreset.release;
            break;

        case BasicSynth_ModFreq:
            paramVal = basicSynthPreset.modFreq;
            break;

        case BasicSynth_ModAmt:
            paramVal = basicSynthPreset.modAmt;
           break;

        case BasicSynth_PanType:
            paramVal = basicSynthPreset.panType;
           break;

        case BasicSynth_PanParameter:
            paramVal = basicSynthPreset.panParameter;
           break;
    }

    return paramVal;
}

// getFmSynthParam
//
float getFmSynthParam(uint16_t paramId) {

    float paramVal = -1;

//    Serial.printf("Presets::getFmSynthParam paramId: %d paramVal: %f\n", paramId, paramVal);

    switch (paramId) {

        case FmSynth_Status:
            paramVal = fmSynthPreset.enabled;
            break;

        case FmSynth_Level:
            paramVal = fmSynthPreset.outLevel;
            break;

        case FmSynth_DlySend:
            paramVal = fmSynthPreset.dlySend;
            break;

        case FmSynth_RvbSend:
            paramVal = fmSynthPreset.rvbSend;
            break;

        case FmSynth_PresetNum:
            paramVal = fmSynthPreset.presetNum;
            break;

        case FmSynth_NoteShift:
            paramVal = fmSynthPreset.noteShift;
            break;

        case FmSynth_NoteRangeMin:
            paramVal = fmSynthPreset.noteRangeMin;
            break;

        case FmSynth_NoteRangeMax:
            paramVal = fmSynthPreset.noteRangeMax;
            break;

        case FmSynth_PitchBendRange:
            paramVal = fmSynthPreset.pitchBendRange;
            break;

        case FmSynth_PanType:
            paramVal = fmSynthPreset.panType;
            break;

        case FmSynth_PanParameter:
            paramVal = fmSynthPreset.panParameter;
            break;
    }

    return paramVal;
}

// getMdiParam
//
float getMdiParam(uint16_t paramId) {

    float paramVal = -1;

//    Serial.printf("Presets::getMdiParam paramId: %d paramVal: %f\n", paramId, paramVal);

    switch (paramId) {

        default:
            break;
    }

    return paramVal;
}

// getRvbParam
//
float getRvbParam(uint16_t paramId) {

    float paramVal = -1;

//    Serial.printf("Presets::getRvbParam paramId: %d paramVal: %f\n", paramId, paramVal);

    switch (paramId) {

        case Reverb_Level:
            paramVal = rvbPreset.rvbLevel;
            break;

        case Reverb_RoomSize:
            paramVal = rvbPreset.rvbRoomSize;
            break;

        case Reverb_Diffusion:
            paramVal = rvbPreset.rvbDiffusion;
            break;

        case Reverb_PchShft:
            paramVal = rvbPreset.rvbPchShft;
            break;

        case Reverb_PchShftAmt:
            paramVal = rvbPreset.rvbPchShftAmt;
            break;

        case Reverb_ShimPchShft:
            paramVal = rvbPreset.rvbShimShft;
            break;

        case Reverb_ShimShftRatio:
            paramVal = rvbPreset.rvbShimShftRto;
            break;

        case Reverb_ShimAmt:
            paramVal = rvbPreset.rvbShimAmt;
            break;
    }

    return paramVal;
}

// getEfxParam
//
float getEfxParam(uint16_t paramId) {

    float paramVal = -1;

//    Serial.printf("Presets::getEfxParam paramId: %d paramVal: %f\n", paramId, paramVal);

    switch (paramId) {

        case FutureEfx_Level:
            paramVal = efxPreset.efxGain;
            break;

        case FutureEfx_Param2:
            paramVal = efxPreset.param2;
            break;

        case FutureEfx_Param3:
            paramVal = efxPreset.param3;
            break;

        case FutureEfx_Param4:
            paramVal = efxPreset.param4;
            break;
    }

    return paramVal;
}

// getStereoDelayParam
//
float getStereoDelayParam(uint16_t paramId) {

    float paramVal = -1;

    switch (paramId) {

        // Delay Left
        //
        case StereoDelay_DlyL_DlyTime:
            paramVal = stereoDelayPreset.delayLeftDelayTime;
            break;

        case StereoDelay_DlyL_Gain:
            paramVal = stereoDelayPreset.delayLeftLevel;
            break;

        case StereoDelay_DlyL_RvbSend:
            paramVal = stereoDelayPreset.delayLeftRvbSend;
            break;

        case StereoDelay_DlyL_Efx3Send:
            paramVal = stereoDelayPreset.delayLeftEfx3Send;
           break;

        case StereoDelay_DlyL_FB_DlyAmt:
            paramVal = stereoDelayPreset.delayLeftFbDelayAmt;
            break;

        case StereoDelay_DlyL_xFB_DlyTime:
            paramVal = stereoDelayPreset.delayLeftxFbDelayTime;
            break;

        case StereoDelay_DlyL_xFB_DlyAmt:
            paramVal = stereoDelayPreset.delayLeftxFbDelayAmt;
            break;

        // Delay Right
        //
        case StereoDelay_DlyR_DlyTime:
            paramVal = stereoDelayPreset.delayRightDelayTime;
            break;

        case StereoDelay_DlyR_Gain:
            paramVal = stereoDelayPreset.delayRightLevel;
            break;

        case StereoDelay_DlyR_RvbSend:
            paramVal = stereoDelayPreset.delayRightRvbSend;
            break;

        case StereoDelay_DlyR_Efx3Send:
            paramVal = stereoDelayPreset.delayRightEfx3Send;
            break;

        case StereoDelay_DlyR_FB_DlyAmt:
            paramVal = stereoDelayPreset.delayRightFbDelayAmt;
            break;

        case StereoDelay_DlyR_xFB_DlyTime:
            paramVal = stereoDelayPreset.delayRightxFbDelayTime;
            break;

        case StereoDelay_DlyR_xFB_DlyAmt:
            paramVal = stereoDelayPreset.delayRightxFbDelayAmt;
            break;
    }

    return paramVal;
}

// getInputOutputParam
//
float getInputOutputParam(uint16_t paramId) {

    float paramVal = -1;

//    Serial.printf("Presets::getInputOutputParam paramId: %d paramVal: %f\n", paramId, paramVal);

    switch (paramId) {

        case AudioIo_LineIn_Status:
            paramVal = audioIoPreset.lineInEnabled;
            break;

        case AudioIo_LineIn_Level:
            paramVal = audioIoPreset.lineInLevel;
            break;

        case AudioIo_LineOut_Level:
            paramVal = audioIoPreset.lineOutLevel;
            break;

        case AudioIo_Phone_Volume:
            paramVal = audioIoPreset.phoneLevel;
            break;

        case AudioIo_LineIn_DlySend:
            paramVal = audioIoPreset.lineInDlySend;
            break;

        case AudioIo_LineIn_RvbSend:
            paramVal = audioIoPreset.lineInRvbSend;
            break;
    }

    return paramVal;
}

/*************************
* Input Devices Messages *
**************************
*/

// midiMsgIn
//
// Receive and respond to notification of MIDI input events
//
void midiMsgIn(MidiMessageSource msgSrc, MidiMessageType msgType, uint8_t channel, uint8_t data1, uint8_t data2) {

    Serial.printf("Presets::midiMsgIn: ");

    const char* src = (msgSrc == MidiSourceUSB) ? "USB" : "DIN";
    Serial.printf("Source %s ", src);

    const char* type = "";
    switch (msgType) {
        case MidiNoteOn:
            type = "NoteOn";
            break;

        case MidiNoteOff:
            type = "NoteOff";
            break;

        case MidiControlChange:
            type = "CtrlChange";
            break;

        case MIDIProgramChange:
            type = "ProgChange";
            break;

        case MidiPitchBend:
            type = "PitchBend";
            break;
    }
    Serial.printf("Type %s ", type);

    Serial.printf("Channel %d ", channel);
    Serial.printf("Data 1 %d ", data1);
    Serial.printf("Data 2 %d ", data2);
    Serial.printf("\n");
}

/*********************************
* Serial Data Display on Console *
**********************************
*
* TODO: Make this a compile-time option
*
*/

#if SHOW_PRESET_SETTINGS

// showAddresses()
//
void showAddresses() {

    Serial.printf("    %-32s 0x%8x\n", "PresetCommonData:",         &presetCommon);
    Serial.printf("    %-32s 0x%8x\n", "MediaPlayerSettings:",      &wavPlayerPreset);
    Serial.printf("    %-32s 0x%8x\n", "BasicSynthSettings:",       &basicSynthPreset);
    Serial.printf("    %-32s 0x%8x\n", "MidiSettings:",             &midiPreset);
    Serial.printf("    %-32s 0x%8x\n", "RvbSettings:",              &rvbPreset);
    Serial.printf("    %-32s 0x%8x\n", "EfxSettings:",              &efxPreset);
    Serial.printf("    %-32s 0x%8x\n", "AudioIoSettings:",          &audioIoPreset);
    Serial.printf("    %-32s 0x%8x\n", "activePresetsBankBuf:",     &activePresetsBankBuf);
    Serial.printf("    %-32s 0x%8x\n", "fmSynthPresetNameTexts:",   &fmSynthPresetNameTexts);
    Serial.printf("    %-32s 0x%8x\n", "fmSynthPresetNames:",       &fmSynthPresetNames);
}

// showSettings()
//
void showSettings() {

//    Serial.printf("Device Settings strlen(presetCommon.presetName): %d \n", strlen(presetCommon.presetName));
    Serial.printf("Device Settings: Preset %d Name %s)\n", currentPresetNum + 1, (strlen(presetCommon.presetName) == 0) ? "not set" : presetCommon.presetName);

    Serial.printf("\nwavPlayerPreset:\n");
    Serial.printf("    &wavPlayerPreset = %x\n", &wavPlayerPreset);
    Serial.printf("    enabled:         %6.2f\n", wavPlayerPreset.enabled);
    Serial.printf("    outLevel:        %6.2f\n", wavPlayerPreset.outLevel);
    Serial.printf("    dlySend:         %6.2f\n", wavPlayerPreset.dlySend);
    Serial.printf("    rvbSend:         %6.2f\n", wavPlayerPreset.rvbSend);
    Serial.printf("    panType:         %6.2f\n", wavPlayerPreset.panType);

    Serial.printf("\rawPlayerPreset:\n");
    Serial.printf("    &rawPlayerPreset = %x\n", &rawPlayerPreset);
    Serial.printf("    enabled:         %6.2f\n", rawPlayerPreset.enabled);
    Serial.printf("    outLevel:        %6.2f\n", rawPlayerPreset.outLevel);
    Serial.printf("    dlySend:         %6.2f\n", rawPlayerPreset.dlySend);
    Serial.printf("    rvbSend:         %6.2f\n", rawPlayerPreset.rvbSend);
    Serial.printf("    panType:         %6.2f\n", rawPlayerPreset.panType);

    Serial.printf("\nbasicSynthPreset:\n");
    Serial.printf("    &basicSynthPreset = %x\n", &basicSynthPreset);
    Serial.printf("    enabled:         %6.2f\n", basicSynthPreset.enabled);
    Serial.printf("    outLevel:        %6.2f\n", basicSynthPreset.outLevel);
    Serial.printf("    modFreq:         %6.2f\n", basicSynthPreset.modFreq);
    Serial.printf("    modAmt:          %6.2f\n", basicSynthPreset.modAmt);
    Serial.printf("    attack:          %6.2f\n", basicSynthPreset.attack);
    Serial.printf("    decay:           %6.2f\n", basicSynthPreset.decay);
    Serial.printf("    sustain:         %6.2f\n", basicSynthPreset.sustain);
    Serial.printf("    release:         %6.2f\n", basicSynthPreset.release);
    Serial.printf("    dlySend:         %6.2f\n", basicSynthPreset.dlySend);
    Serial.printf("    rvbSend:         %6.2f\n", basicSynthPreset.rvbSend);
    Serial.printf("    synthType:       %6.2f\n", basicSynthPreset.synthType);

    Serial.printf("\nmidiPreset:\n");
    Serial.printf("    &midiPreset = %x\n", &midiPreset);
    Serial.printf("    dummy1:          %6.2f\n", midiPreset.dummy1);
    Serial.printf("    dummy1:          %6.2f\n", midiPreset.dummy2);

    Serial.printf("\nrvbPreset:\n");
    Serial.printf("    &rvbPreset = %x\n", &rvbPreset);
    Serial.printf("    rvbRoomSize:     %6.2f\n", rvbPreset.rvbRoomSize);
    Serial.printf("    rvbDiffusion:    %6.2f\n", rvbPreset.rvbDiffusion);
    Serial.printf("    rvbLevel:        %6.2f\n", rvbPreset.rvbLevel);

    Serial.printf("\nstereoDelayPreset:\n");
    Serial.printf("    &stereoDelayPreset = %x\n", &stereoDelayPreset);
    Serial.printf("    delayLeftDelayTime:     %6.2f\n", stereoDelayPreset.delayLeftDelayTime);
    Serial.printf("    delayLeftLevel:         %6.2f\n", stereoDelayPreset.delayLeftLevel);
    Serial.printf("    delayLeftRvbSend:       %6.2f\n", stereoDelayPreset.delayLeftRvbSend);
    Serial.printf("    delayLeftEfx3Send:      %6.2f\n", stereoDelayPreset.delayLeftEfx3Send);
    Serial.printf("    delayLeftFbDelayAmt:    %6.2f\n", stereoDelayPreset.delayLeftFbDelayAmt);
    Serial.printf("    delayLeftxFbDelayTime:  %6.2f\n", stereoDelayPreset.delayLeftxFbDelayTime);
    Serial.printf("    delayLeftxFbDelayAmt:   %6.2f\n", stereoDelayPreset.delayLeftxFbDelayAmt);
    Serial.printf("\n");

    Serial.printf("    delayRightDelayTime:     %6.2f\n", stereoDelayPreset.delayRightDelayTime);
    Serial.printf("    delayRightLevel:         %6.2f\n", stereoDelayPreset.delayRightLevel);
    Serial.printf("    delayRightRvbSend:       %6.2f\n", stereoDelayPreset.delayRightRvbSend);
    Serial.printf("    delayRightEfx3Send:      %6.2f\n", stereoDelayPreset.delayRightEfx3Send);
    Serial.printf("    delayRightFbDelayAmt:    %6.2f\n", stereoDelayPreset.delayRightFbDelayAmt);
    Serial.printf("    delayRightxFbDelayTime:  %6.2f\n", stereoDelayPreset.delayRightxFbDelayTime);
    Serial.printf("    delayRightxFbDelayAmt:   %6.2f\n", stereoDelayPreset.delayRightxFbDelayAmt);

    Serial.printf("\naudioIoPreset:\n");
    Serial.printf("    &audioIoPreset = %x\n", &audioIoPreset);
    Serial.printf("    lineInEnabled:   %6.2f\n", audioIoPreset.lineInEnabled);
    Serial.printf("    lineInLevel:     %6.2f\n", audioIoPreset.lineInLevel);
    Serial.printf("    lineOutLevel:    %6.2f\n", audioIoPreset.lineOutLevel);
    Serial.printf("    phoneLevel:      %6.2f\n", audioIoPreset.phoneLevel);
    Serial.printf("    lineInDlySend:   %6.2f\n", audioIoPreset.lineInDlySend);
    Serial.printf("    lineInRvbSend:   %6.2f\n", audioIoPreset.lineInRvbSend);

    Serial.println();
}

#endif // SHOW_PRESET_SETTINGS

/************************************************************************************
*                           L O C A L   F U N C T I O N S                           *
*************************************************************************************
*
* Functions visible only within this module
*
*/

// loadDefaultPresetBank
//
// Read default presets bank file from PROGMEM storage into preset bank buffer
//
bool loadDefaultPresetBank() {

    File pbnkFile;
    size_t bytesRead = 0;

    pbnkFile = progmemFs.open(DefaultPresetsBankFilename, FILE_READ);
    if (pbnkFile) {
        pbnkFile.seek(0, SeekSet);
        bytesRead = pbnkFile.read(activePresetsBankBuf, PresetBankSize);
        bytesRead += pbnkFile.read(activePresetsBankGlobalDataBuf, PresetBankGlobalDataSize);
        pbnkFile.close();

        strcpy(activePresetBankFileName, DefaultPresetsBankFilename);

        // On initial load, copy contents and name to import bank
        if (!initialized) {
            memcpy(importPresetsBankBuf, activePresetsBankBuf, PresetBankSize);
            strcpy(importPresetBankFileName, DefaultPresetsBankFilename);
        }
    }

    resetPresetModifiedFields();
    resetPresetDeviceModifiedFields();

    return (bytesRead == PresetBankFileSize);
}

// saveDefaultPresetBank
//
// Write preset bank buffer to PROGMEM storage default preset bank file
//
bool saveDefaultPresetBank() {

    File pbnkFile;
    size_t bytesWritten = 0;

#if RENAME_DEFAULT_BANKNAME
    progmemFs.rename(DefaultPresetsBankFilename, NewPresetsBankFilename);
    pbnkFile = progmemFs.open(NewPresetsBankFilename, FILE_WRITE_BEGIN);
#else // Original Settings
    pbnkFile = progmemFs.open(DefaultPresetsBankFilename, FILE_WRITE_BEGIN);
#endif // RENAME_DEFAULT_BANKNAME / Original Settings

    if (pbnkFile) {
        pbnkFile.seek(0, SeekSet);
        bytesWritten = pbnkFile.write((void *)activePresetsBankBuf, PresetBankSize);
        bytesWritten += pbnkFile.write((void *)activePresetsBankGlobalDataBuf, PresetBankGlobalDataSize);
        pbnkFile.close();
        strcpy(activePresetBankFileName, DefaultPresetsBankFilename);
    }

    return (bytesWritten == PresetBankFileSize);
}

// resetDefaultPresetBank
//
// Initialize and save empty default preset bank
//
// TODO: Populate with data from default structures in PresetDefaultData.h
//
bool resetDefaultPresetBank() {

//    Serial.printf("Presets::resetDefaultPresetBank: creating new default presets bank\n");

    memset(activePresetsBankBuf, 0, PresetBankSize);
    memset(activePresetsBankGlobalDataBuf, 0, PresetBankGlobalDataSize);

    return saveDefaultPresetBank();
}

// resetPresetModifiedFields
//
// Reset the preset modified fields of all presets
//
void resetPresetModifiedFields() {

//    Serial.printf("Presets::resetPresetModifiedFields: Begin\n");

    // Copy preset data from presetBlock in activePresetsBankBuf to individual device/preset settings
    for (uint16_t presetNum = 0; presetNum < PresetCount; presetNum++) {
        PresetCommonData* presetCommonData = (PresetCommonData*) &activePresetsBankBuf[presetNum];
        presetCommonData->presetModified = 0;
    }

//    Serial.printf("Presets::resetPresetModifiedFields: Finished\n");
}

// resetPresetDeviceModifiedFields
//
// Reset the current preset device modified fields
//
void resetPresetDeviceModifiedFields() {

//    Serial.printf("Presets::resetPresetDeviceModifiedFields: \n");

    memset(presetDeviceModified, 0, sizeof(presetDeviceModified));
}

// loadSdPresetBank()
//
bool loadSdPresetBank(const char* sdFilename, CommandButtonId& loadDest) {

    bool returnVal = true;

//    Serial.printf("Presets::loadSdPresetBank: %s loadDest %d\n", sdFilename, loadDest);

    if (AudioPlatform::systemProfile.sdCardPresent) {

        if (SD.exists(sdFilename)) {

            File sdFile = SD.open(sdFilename, FILE_READ);

            switch (loadDest) {
                case BtnSelect:
                case BtnSelectActive:
//                    Serial.printf("Presets::loadSdPresetBank: loading Active Bank\n");
                    sdFile.read((void*)activePresetsBankBuf, PresetBankSize);
                    sdFile.read((void*)activePresetsBankGlobalDataBuf, PresetBankGlobalDataSize);
                    strcpy(activePresetBankFileName, sdFilename);
                    break;

                case BtnSelectImport:
//                    Serial.printf("Presets::loadSdPresetBank: loading Import Bank\n");
                    sdFile.read((void*)importPresetsBankBuf, PresetBankSize);
                    strcpy(importPresetBankFileName, sdFilename);
                    break;

                default:
                    break;
            }

            sdFile.close();
//            Serial.printf("Presets::loadSdPresetBank: Copied Presets File from SD to EXTMEM\n");

        } else {
//            Serial.printf("Presets::loadSdPresetBank: Presets File not found on SD\n");
            returnVal = false;

        }
    }

    return returnVal;
}

// saveSdPresetBank()
//
bool saveSdPresetBank(const char* sdFilename) {

    bool returnVal = false;

//    Serial.printf("Presets::saveSdPresetBank:\n");

#if 0 // FFS: Implement in future. How to notify and interact with user?
    // Default, unless user overrides
    bool copyOk = true;

    // Check for presets file already present on SD
    if (AudioPlatform::systemProfile.sdCardPresent) {
        if (SD.exists(DefaultPresetsBankFilename)) {
            // Get user confirmation of overwriting existing file
            } else {
                // User overrides. No copy over existing file.
                copyOk = false;
            }
    }
#endif

    if (AudioPlatform::systemProfile.sdCardPresent) {

        File sdFile = SD.open(sdFilename, FILE_WRITE);

        if (sdFile) {
//            Serial.printf("Copying presets bank %s to SD.\n", sdFilename);

            DateTimeFields dtf;
            Teensy41_Util::parseRtcDateTime(dtf);
            sdFile.setCreateTime(dtf);
            sdFile.setModifyTime(dtf);

            sdFile.seek(0, SeekSet);
            size_t bytesWritten = sdFile.write((void *)activePresetsBankBuf, PresetBankSize);
            bytesWritten += sdFile.write((void *)activePresetsBankGlobalDataBuf, PresetBankGlobalDataSize);

            sdFile.close();

            if (bytesWritten == PresetBankFileSize) {
                returnVal = true;
                strcpy(activePresetBankFileName, sdFilename);
            }
        }
        (*AudioPlatform::systemProfile.sdPresetBankFileUpdateFn)();
    }

    return returnVal;
}

// copyPresetSettings
//
void copyPresetSettings(uint16_t destPreset, uint16_t srcPreset, uint16_t deviceId) {

//    Serial.printf("Presets::copyPresetSettings srcPreset %d destPreset %d  deviceId %d\n", srcPreset, destPreset, deviceId);

    // Copy from import to active
    uint8_t* srcPresetBlock = (uint8_t*)&importPresetsBankBuf[srcPreset];
    // Copy from active to import
//    uint8_t* srcPresetBlock = (uint8_t*)&activePresetsBankBuf[srcPreset];
    uint8_t* destPresetBlock = (uint8_t*)&activePresetsBankBuf[destPreset];
    uint16_t devicePos = deviceId * PresetDeviceSize;

    memcpy((void *)(destPresetBlock + devicePos), (void *)(srcPresetBlock + devicePos), PresetDeviceSize);

    // Re-load destination preset to update display
    loadPreset(destPreset);
}

// loadPreset()
//
// Load preset device settings from active or import bank into individual preset devices.
//
// _NOTE_ that the preset device settings are not saved to the active presets bank by this action.
//
bool loadPreset(uint16_t presetNum, bool import) {

    bool loadedOk = true;

//    Serial.printf("Presets::loadPreset Loading preset %d import %d\n", presetNum, import);

    if (import) {
        presetBlock = (uint8_t*)&importPresetsBankBuf[presetNum];
    } else {
        presetBlock = (uint8_t*)&activePresetsBankBuf[presetNum];
    }

    // Copy preset data from presetBlock in activePresetsBankBuf to individual device/preset settings
    for (uint16_t deviceId = 0; deviceId < deviceCount; deviceId++) {
        uint16_t devicePos = deviceId * PresetDeviceSize;

        switch (deviceId) {

            case devicePresetCommon:
                memcpy((void *)&presetCommon, (void *)&presetBlock[devicePos], sizeof(PresetCommonData));
                break;

            case deviceWavPlayer:
                memcpy((void *)&wavPlayerPreset, (void *)&presetBlock[devicePos], sizeof(MediaPlayerSettings));
                break;

            case deviceRawPlayer:
                memcpy((void *)&rawPlayerPreset, (void *)&presetBlock[devicePos], sizeof(MediaPlayerSettings));
                break;

            case deviceBasicSynth:
                memcpy((void *)&basicSynthPreset, (void *)&presetBlock[devicePos], sizeof(BasicSynthSettings));
                break;

            case deviceFmSynth:
                memcpy((void *)&fmSynthPreset, (void *)&presetBlock[devicePos], sizeof(FmSynthSettings));
                break;

            case deviceStereoDelay:
                memcpy((void *)&stereoDelayPreset, (void *)&presetBlock[devicePos], sizeof(StereoDelaySettings));
                break;

            case deviceRvb:
                memcpy((void *)&rvbPreset, (void *)&presetBlock[devicePos], sizeof(RvbSettings));
                break;

            case deviceEfx:
                memcpy((void *)&efxPreset, (void *)&presetBlock[devicePos], sizeof(EfxSettings));
                break;

            case deviceAudioIo:
                memcpy((void *)&audioIoPreset, (void *)&presetBlock[devicePos], sizeof(AudioIoSettings));
                break;

            // Note: As of rev 0.5.5.5, MIDI settings have been moved to the utilities screen, and should
            // be stored in PROGMEM as single-instance global settings rather than as part of each preset
            case deviceMidi:
                memcpy((void *)&midiPreset, (void *)&presetBlock[devicePos], sizeof(MidiSettings));
                break;
        }
    }

    // Propagate preset device settings to application modules
    updateSystem();
//    updateSystem();     // FIXME: call 2x in row as temporary "fix/patch" for B240711-1

    currentPresetNum = presetNum;

//    Serial.printf("Presets::loadPreset PresetNames After loadPreset()\n");

    return loadedOk;
}

// savePreset()
//
// Save individual preset device settings to selected preset block in
// active presets bank.
//
bool savePreset(uint16_t presetNum) {

    bool savedOk = true;

//    Serial.printf("Presets::savePreset: Saving preset %d %s\n", presetNum, presetCommon.presetName);

    presetBlock = (uint8_t*)&activePresetsBankBuf[presetNum];
//    Serial.printf("Presets::savePreset presetBlock %8x\n", presetBlock);

    // Write device blocks for all devices
    // (ID number assignments of devices MUST NOT BE CHANGED)
    // The IDs (0, 1, 2,...) of the devices in the PresetDevice cannot
    // be modified. Otherwise, the order/index of the presets in the preset
    // will be therefore modified, rendering the newly saved preset incompatible
    // with previously saved preset banks.
    //
    for (uint16_t deviceId = 0; deviceId < deviceCount; deviceId++) {
        uint16_t devicePos = deviceId * PresetDeviceSize;

        switch (deviceId) {

            case devicePresetCommon:
                memcpy((void *)&presetBlock[devicePos], (void *)&presetCommon, sizeof(PresetCommonData));
                break;

            case deviceWavPlayer:
                memcpy((void *)&presetBlock[devicePos], (void *)&wavPlayerPreset, sizeof(MediaPlayerSettings));
                break;

            case deviceRawPlayer:
                memcpy((void *)&presetBlock[devicePos], (void *)&rawPlayerPreset, sizeof(MediaPlayerSettings));
                break;

            case deviceBasicSynth:
                memcpy((void *)&presetBlock[devicePos], (void *)&basicSynthPreset, sizeof(BasicSynthSettings));
                break;

            case deviceFmSynth:
                memcpy((void *)&presetBlock[devicePos], (void *)&fmSynthPreset, sizeof(FmSynthSettings));
                break;

            case deviceStereoDelay:
                memcpy((void *)&presetBlock[devicePos], (void *)&stereoDelayPreset, sizeof(StereoDelaySettings));
                break;

            case deviceRvb:
                memcpy((void *)&presetBlock[devicePos], (void *)&rvbPreset, sizeof(RvbSettings));
                break;

            case deviceEfx:
                memcpy((void *)&presetBlock[devicePos], (void *)&efxPreset, sizeof(EfxSettings));
                break;

            case deviceAudioIo:
                memcpy((void *)&presetBlock[devicePos], (void *)&audioIoPreset, sizeof(AudioIoSettings));
                break;

            // Note: As of rev 0.5.5.5, MIDI settings have been moved to the utilities screen, and should
            // be stored in PROGMEM as single-instance global settings rather than as part of each preset
            case deviceMidi:
                memcpy((void *)&presetBlock[devicePos], (void *)&midiPreset, sizeof(MidiSettings));
                break;
        }
    }

    currentPresetNum = presetNum;

    return savedOk;
}

// getImportPresetName
//
// Return the name a specific presetNum [0,n-1] in the import bank
//
const char* getImportPresetName(ButtonId presetNum) {

    PresetCommonData* presetCommonData = (PresetCommonData*) &importPresetsBankBuf[presetNum];
//    Serial.printf("Presets::getImportPresetName: returning %s\n", presetCommonData->presetName);

    return (const char*)presetCommonData->presetName;
}

// initFmSynthPresets()
//
void initFmSynthPresets() {

#if 0
    // Verify Constants
    Serial.printf("Presets::initFmSynthPresets: FmSynthProgMemPresetCount %d\n", FmSynthProgMemPresetCount);
    Serial.printf("Presets::initFmSynthPresets: FmSynthUserPresetCount %d\n", FmSynthUserPresetCount);
    Serial.printf("Presets::initFmSynthPresets: FmSynthPresetsBankCount %d\n", FmSynthPresetsBankCount);
    Serial.printf("Presets::initFmSynthPresets: FmSynthPresetCount %d\n", FmSynthPresetCount);

    Serial.printf("Presets::initFmSynthPresets: FmSynthSysxBankSize %d\n", FmSynthSysxBankSize);
    Serial.printf("Presets::initFmSynthPresets: FmSynthSysxBankFileSize %d\n", FmSynthSysxBankFileSize);
    Serial.printf("Presets::initFmSynthPresets: FmSynthProgMemPresetBanksSize %d\n", FmSynthProgMemPresetBanksSize);
#endif

    // Copy all the PROGMEM presets to EXTRAM
    memcpy(fmSynthPresetsBuffer, &FmSynthPresetBanks[0][0][0], FmSynthProgMemPresetBanksSize);

    // Now fill the remainder of the butter with INIT VOICE preset
    for (int i = 0; i < FmSynthUserPresetCount; i++) {
        memcpy(&fmSynthPresetsBuffer[FmSynthProgMemPresetBanksSize + i * FmSynthPresetBankedLength], FmSynthInitPreset, FmSynthPresetBankedLength);
    }

    // If present on SD, load User1-User6 into INIT VOICE Banks
    loadFmSynthUserPresets();

    // Point FM Synth to Synth Preset Banks
    AudioDevices::setPrestBanksPtr((uint8_t*)fmSynthPresetsBuffer);

    // Get FM Synth preset names
    memset((void *)fmSynthPresetNameTexts, 0, sizeof(fmSynthPresetNameTexts));
    for (int i = 0; i < FmSynthPresetCount; i++) {

        const char* voicePresetName = AudioDevices::getFmSynthVoicePresetName(i);
        if (voicePresetName) {
            strncpy((char *)&fmSynthPresetNameTexts[i], AudioDevices::getFmSynthVoicePresetName(i), FmSynthPresetNameLength);
            fmSynthPresetNameTexts[i][FmSynthPresetNameLength] = '\0';

            // Pointers to ASCII names for access in FmSynthScreenDefs
            fmSynthPresetNames[i] = (const char*)&fmSynthPresetNameTexts[i];
        }
    }

    // Get FM Synth Bank Names
    for (int i = 0; i < FmSynthPresetsBankCount; i++) {
        fmSynthBankNames[i] = (const char*)&FmSynthPresetBankNames[i][0];
    }

}

// loadFmSynthUserPresets()
//
void loadFmSynthUserPresets() {

//    Serial.printf("Presets::loadFmSynthUserPresets:\n");

    File sdFile;
    char fileName[16];

    if (AudioPlatform::systemProfile.sdCardPresent) {
        for (int i = 1; i <= FmSynthUserPresetsBankCount; i++) {

            memset(fileName, 0, 16);
            snprintf(fileName, 15, "User%1d.syx", i);

            if (SD.exists(fileName)) {
//                Serial.printf("Presets::loadFmSynthUserPresets: Loading %s\n", fileName);

                sdFile = SD.open(fileName, FILE_READ);
                sdFile.read(fmSynthBankFileBuf, FmSynthSysxBankSize);
                sdFile.close();

                memcpy(&fmSynthPresetsBuffer[FmSynthProgMemPresetBanksSize + (i-1) * FmSynthSysxBankSize], &fmSynthBankFileBuf[6], FmSynthSysxBankSize);
            }

        }
    }
}

// updateSystem()
//
void updateSystem() {

    setInputOutputParams();
    setAudioDeviceParams();
    setScreenParams();
}

// setAudioDeviceParams()
//
void setAudioDeviceParams() {

//    Serial.printf("Presets::setAudioDeviceParams:\n");

    AudioDevices::setWavPlayerParam(MediaPlayWav_Status,        wavPlayerPreset.enabled);
    AudioDevices::setWavPlayerParam(MediaPlayWav_Level,         wavPlayerPreset.outLevel);
    AudioDevices::setWavPlayerParam(MediaPlayWav_DlySend,       wavPlayerPreset.dlySend);
    AudioDevices::setWavPlayerParam(MediaPlayWav_RvbSend,       wavPlayerPreset.rvbSend);
    AudioDevices::setWavPlayerParam(MediaPlayWav_SoundBank,     wavPlayerPreset.soundBank);
    AudioDevices::setWavPlayerParam(MediaPlayWav_NoteShift,     wavPlayerPreset.noteShift);
    AudioDevices::setWavPlayerParam(MediaPlayWav_NoteRangeMin,  wavPlayerPreset.noteRangeMin);
    AudioDevices::setWavPlayerParam(MediaPlayWav_NoteRangeMax,  wavPlayerPreset.noteRangeMax);
    AudioDevices::setWavPlayerParam(MediaPlayWav_PanType,       wavPlayerPreset.panType);
    AudioDevices::setWavPlayerParam(MediaPlayWav_PanParameter,  wavPlayerPreset.panParameter);

    AudioDevices::setRawPlayerParam(MediaPlayRaw_Status,        rawPlayerPreset.enabled);
    AudioDevices::setRawPlayerParam(MediaPlayRaw_Level,         rawPlayerPreset.outLevel);
    AudioDevices::setRawPlayerParam(MediaPlayRaw_DlySend,       rawPlayerPreset.dlySend);
    AudioDevices::setRawPlayerParam(MediaPlayRaw_RvbSend,       rawPlayerPreset.rvbSend);
    AudioDevices::setRawPlayerParam(MediaPlayRaw_SoundBank,     rawPlayerPreset.soundBank);
    AudioDevices::setRawPlayerParam(MediaPlayRaw_NoteShift,     rawPlayerPreset.noteShift);
    AudioDevices::setRawPlayerParam(MediaPlayRaw_NoteRangeMin,  rawPlayerPreset.noteRangeMin);
    AudioDevices::setRawPlayerParam(MediaPlayRaw_NoteRangeMax,  rawPlayerPreset.noteRangeMax);
    AudioDevices::setRawPlayerParam(MediaPlayRaw_PanType,       rawPlayerPreset.panType);
    AudioDevices::setRawPlayerParam(MediaPlayWav_PanParameter,  rawPlayerPreset.panParameter);

    AudioDevices::setBasicSynthParam(BasicSynth_Status,         basicSynthPreset.enabled);
    AudioDevices::setBasicSynthParam(BasicSynth_Level,          basicSynthPreset.outLevel);
    AudioDevices::setBasicSynthParam(BasicSynth_DlySend,        basicSynthPreset.dlySend);
    AudioDevices::setBasicSynthParam(BasicSynth_RvbSend,        basicSynthPreset.rvbSend);
    AudioDevices::setBasicSynthParam(BasicSynth_Type,           basicSynthPreset.synthType);
    AudioDevices::setBasicSynthParam(BasicSynth_NoteShift,      basicSynthPreset.noteShift);
    AudioDevices::setBasicSynthParam(BasicSynth_NoteRangeMin,   basicSynthPreset.noteRangeMin);
    AudioDevices::setBasicSynthParam(BasicSynth_NoteRangeMax,   basicSynthPreset.noteRangeMax);
    AudioDevices::setBasicSynthParam(BasicSynth_Attack,         basicSynthPreset.attack);
    AudioDevices::setBasicSynthParam(BasicSynth_Decay,          basicSynthPreset.decay);
    AudioDevices::setBasicSynthParam(BasicSynth_Sustain,        basicSynthPreset.sustain);
    AudioDevices::setBasicSynthParam(BasicSynth_Release,        basicSynthPreset.release);
    AudioDevices::setBasicSynthParam(BasicSynth_ModFreq,        basicSynthPreset.modFreq);
    AudioDevices::setBasicSynthParam(BasicSynth_ModAmt,         basicSynthPreset.modAmt);
    AudioDevices::setBasicSynthParam(BasicSynth_PanType,        basicSynthPreset.panType);
    AudioDevices::setBasicSynthParam(BasicSynth_PanParameter,   basicSynthPreset.panParameter);

#if 1 // FIXME: Testing work-around for B240711-1 based on observation that it seems
      // to affect mostly/only BasicSynth. So sending params 2x.
    AudioDevices::setBasicSynthParam(BasicSynth_Status,         basicSynthPreset.enabled);
    AudioDevices::setBasicSynthParam(BasicSynth_Level,          basicSynthPreset.outLevel);
    AudioDevices::setBasicSynthParam(BasicSynth_DlySend,        basicSynthPreset.dlySend);
    AudioDevices::setBasicSynthParam(BasicSynth_RvbSend,        basicSynthPreset.rvbSend);
    AudioDevices::setBasicSynthParam(BasicSynth_Type,           basicSynthPreset.synthType);
    AudioDevices::setBasicSynthParam(BasicSynth_NoteShift,      basicSynthPreset.noteShift);
    AudioDevices::setBasicSynthParam(BasicSynth_NoteRangeMin,   basicSynthPreset.noteRangeMin);
    AudioDevices::setBasicSynthParam(BasicSynth_NoteRangeMax,   basicSynthPreset.noteRangeMax);
    AudioDevices::setBasicSynthParam(BasicSynth_Attack,         basicSynthPreset.attack);
    AudioDevices::setBasicSynthParam(BasicSynth_Decay,          basicSynthPreset.decay);
    AudioDevices::setBasicSynthParam(BasicSynth_Sustain,        basicSynthPreset.sustain);
    AudioDevices::setBasicSynthParam(BasicSynth_Release,        basicSynthPreset.release);
    AudioDevices::setBasicSynthParam(BasicSynth_ModFreq,        basicSynthPreset.modFreq);
    AudioDevices::setBasicSynthParam(BasicSynth_ModAmt,         basicSynthPreset.modAmt);
    AudioDevices::setBasicSynthParam(BasicSynth_PanType,        basicSynthPreset.panType);
    AudioDevices::setBasicSynthParam(BasicSynth_PanParameter,   basicSynthPreset.panParameter);
#endif

    AudioDevices::setFmSynthParam(FmSynth_Status,               fmSynthPreset.enabled);
    AudioDevices::setFmSynthParam(FmSynth_Level,                fmSynthPreset.outLevel);
    AudioDevices::setFmSynthParam(FmSynth_DlySend,              fmSynthPreset.dlySend);
    AudioDevices::setFmSynthParam(FmSynth_RvbSend,              fmSynthPreset.rvbSend);
    AudioDevices::setFmSynthParam(FmSynth_PresetNum,            fmSynthPreset.presetNum);
    AudioDevices::setFmSynthParam(FmSynth_NoteShift,            fmSynthPreset.noteShift);
    AudioDevices::setFmSynthParam(FmSynth_NoteRangeMin,         fmSynthPreset.noteRangeMin);
    AudioDevices::setFmSynthParam(FmSynth_NoteRangeMax,         fmSynthPreset.noteRangeMax);
    AudioDevices::setFmSynthParam(FmSynth_PitchBendRange,       fmSynthPreset.pitchBendRange);
    AudioDevices::setFmSynthParam(FmSynth_PanType,              fmSynthPreset.panType);
    AudioDevices::setFmSynthParam(FmSynth_PanParameter,         fmSynthPreset.panParameter);

    AudioDevices::setStereoDelayParam(StereoDelay_DlyL_DlyTime,     stereoDelayPreset.delayLeftDelayTime);
    AudioDevices::setStereoDelayParam(StereoDelay_DlyL_Gain,        stereoDelayPreset.delayLeftLevel);
    AudioDevices::setStereoDelayParam(StereoDelay_DlyL_RvbSend,     stereoDelayPreset.delayLeftRvbSend);
    AudioDevices::setStereoDelayParam(StereoDelay_DlyL_Efx3Send,    stereoDelayPreset.delayLeftEfx3Send);
    AudioDevices::setStereoDelayParam(StereoDelay_DlyL_FB_DlyAmt,   stereoDelayPreset.delayLeftFbDelayAmt);
    AudioDevices::setStereoDelayParam(StereoDelay_DlyL_xFB_DlyTime, stereoDelayPreset.delayLeftxFbDelayTime);
    AudioDevices::setStereoDelayParam(StereoDelay_DlyL_xFB_DlyAmt,  stereoDelayPreset.delayLeftxFbDelayAmt);

    AudioDevices::setStereoDelayParam(StereoDelay_DlyR_DlyTime,     stereoDelayPreset.delayRightDelayTime);
    AudioDevices::setStereoDelayParam(StereoDelay_DlyR_Gain,        stereoDelayPreset.delayRightLevel);
    AudioDevices::setStereoDelayParam(StereoDelay_DlyR_RvbSend,     stereoDelayPreset.delayRightRvbSend);
    AudioDevices::setStereoDelayParam(StereoDelay_DlyR_Efx3Send,    stereoDelayPreset.delayRightEfx3Send);
    AudioDevices::setStereoDelayParam(StereoDelay_DlyR_FB_DlyAmt,   stereoDelayPreset.delayRightFbDelayAmt);
    AudioDevices::setStereoDelayParam(StereoDelay_DlyR_xFB_DlyTime, stereoDelayPreset.delayRightxFbDelayTime);
    AudioDevices::setStereoDelayParam(StereoDelay_DlyR_xFB_DlyAmt,  stereoDelayPreset.delayRightxFbDelayAmt);

    AudioDevices::setRvbParam(Reverb_Level,         rvbPreset.rvbLevel);
    AudioDevices::setRvbParam(Reverb_RoomSize,      rvbPreset.rvbRoomSize);
    AudioDevices::setRvbParam(Reverb_Diffusion,     rvbPreset.rvbDiffusion);
    AudioDevices::setRvbParam(Reverb_PchShft,       rvbPreset.rvbPchShft);
    AudioDevices::setRvbParam(Reverb_PchShftAmt,    rvbPreset.rvbPchShftAmt);
//    AudioDevices::setRvbParam(Reverb_ShimPchShft,   rvbPreset.rvbShimShft);
    AudioDevices::setRvbParam(Reverb_ShimShftRatio, rvbPreset.rvbShimShftRto);
    AudioDevices::setRvbParam(Reverb_ShimAmt,       rvbPreset.rvbShimAmt);
    // Testing setting this last (R250801-1)
    AudioDevices::setRvbParam(Reverb_ShimPchShft,   rvbPreset.rvbShimShft);

    AudioDevices::setEfxParam(FutureEfx_Level,  efxPreset.efxGain);
    AudioDevices::setEfxParam(FutureEfx_Param2, efxPreset.param2);
    AudioDevices::setEfxParam(FutureEfx_Param3, efxPreset.param3);
    AudioDevices::setEfxParam(FutureEfx_Param4, efxPreset.param4);

    AudioDevices::setInputOutputParam(AudioIo_LineIn_Status,        audioIoPreset.lineInEnabled);
    AudioDevices::setInputOutputParam(AudioIo_LineIn_Level,         audioIoPreset.lineInLevel);
    AudioDevices::setInputOutputParam(AudioIo_LineOut_Level,        audioIoPreset.lineOutLevel);
    AudioDevices::setInputOutputParam(AudioIo_Phone_Volume,         audioIoPreset.phoneLevel);
    AudioDevices::setInputOutputParam(AudioIo_LineIn_DlySend,       audioIoPreset.lineInDlySend);
    AudioDevices::setInputOutputParam(AudioIo_LineIn_RvbSend,       audioIoPreset.lineInRvbSend);

    // Note: As of rev 0.5.5.5, MIDI settings have been moved to the utilities screen, and should
    // be stored in PROGMEM as single-instance global settings rather than as part of each preset
//    AudioDevices::setMidiParam(paramId, midiPreset.dummy1);
//    AudioDevices::setMidiParam(paramId, midiPreset.dummy2);
}

// setInputOutputParams()
//
void setInputOutputParams() {

    InputDevices::setWavPlayerParam(MediaPlayWav_Status,        wavPlayerPreset.enabled);
    InputDevices::setWavPlayerParam(MediaPlayWav_Level,         wavPlayerPreset.outLevel);
    InputDevices::setWavPlayerParam(MediaPlayWav_DlySend,       wavPlayerPreset.dlySend);
    InputDevices::setWavPlayerParam(MediaPlayWav_RvbSend,       wavPlayerPreset.rvbSend);
    InputDevices::setWavPlayerParam(MediaPlayWav_SoundBank,     wavPlayerPreset.soundBank);
    InputDevices::setWavPlayerParam(MediaPlayWav_NoteShift,     wavPlayerPreset.noteShift);
    InputDevices::setWavPlayerParam(MediaPlayWav_NoteRangeMin,  wavPlayerPreset.noteRangeMin);
    InputDevices::setWavPlayerParam(MediaPlayWav_NoteRangeMax,  wavPlayerPreset.noteRangeMax);
    InputDevices::setWavPlayerParam(MediaPlayWav_PanType,       wavPlayerPreset.panType);
    InputDevices::setWavPlayerParam(MediaPlayWav_PanParameter,  wavPlayerPreset.panParameter);

    InputDevices::setRawPlayerParam(MediaPlayRaw_Status,        rawPlayerPreset.enabled);
    InputDevices::setRawPlayerParam(MediaPlayRaw_Level,         rawPlayerPreset.outLevel);
    InputDevices::setRawPlayerParam(MediaPlayRaw_DlySend,       rawPlayerPreset.dlySend);
    InputDevices::setRawPlayerParam(MediaPlayRaw_RvbSend,       rawPlayerPreset.rvbSend);
    InputDevices::setRawPlayerParam(MediaPlayRaw_SoundBank,     rawPlayerPreset.soundBank);
    InputDevices::setRawPlayerParam(MediaPlayRaw_NoteShift,     rawPlayerPreset.noteShift);
    InputDevices::setRawPlayerParam(MediaPlayRaw_NoteRangeMin,  rawPlayerPreset.noteRangeMin);
    InputDevices::setRawPlayerParam(MediaPlayRaw_NoteRangeMax,  rawPlayerPreset.noteRangeMax);
    InputDevices::setRawPlayerParam(MediaPlayRaw_PanType,       rawPlayerPreset.panType);
    InputDevices::setRawPlayerParam(MediaPlayWav_PanParameter,  rawPlayerPreset.panParameter);

    InputDevices::setBasicSynthParam(BasicSynth_Status,         basicSynthPreset.enabled);
    InputDevices::setBasicSynthParam(BasicSynth_Level,          basicSynthPreset.outLevel);
    InputDevices::setBasicSynthParam(BasicSynth_DlySend,        basicSynthPreset.dlySend);
    InputDevices::setBasicSynthParam(BasicSynth_RvbSend,        basicSynthPreset.rvbSend);
    InputDevices::setBasicSynthParam(BasicSynth_Type,           basicSynthPreset.synthType);
    InputDevices::setBasicSynthParam(BasicSynth_NoteShift,      basicSynthPreset.noteShift);
    InputDevices::setBasicSynthParam(BasicSynth_NoteRangeMin,   basicSynthPreset.noteRangeMin);
    InputDevices::setBasicSynthParam(BasicSynth_NoteRangeMax,   basicSynthPreset.noteRangeMax);
    InputDevices::setBasicSynthParam(BasicSynth_Attack,         basicSynthPreset.attack);
    InputDevices::setBasicSynthParam(BasicSynth_Decay,          basicSynthPreset.decay);
    InputDevices::setBasicSynthParam(BasicSynth_Sustain,        basicSynthPreset.sustain);
    InputDevices::setBasicSynthParam(BasicSynth_Release,        basicSynthPreset.release);
    InputDevices::setBasicSynthParam(BasicSynth_ModFreq,        basicSynthPreset.modFreq);
    InputDevices::setBasicSynthParam(BasicSynth_ModAmt,         basicSynthPreset.modAmt);
    InputDevices::setBasicSynthParam(BasicSynth_PanType,        basicSynthPreset.panType);
    InputDevices::setBasicSynthParam(BasicSynth_PanParameter,   basicSynthPreset.panParameter);

    InputDevices::setFmSynthParam(FmSynth_Status,               fmSynthPreset.enabled);
    InputDevices::setFmSynthParam(FmSynth_Level,                fmSynthPreset.outLevel);
    InputDevices::setFmSynthParam(FmSynth_DlySend,              fmSynthPreset.dlySend);
    InputDevices::setFmSynthParam(FmSynth_RvbSend,              fmSynthPreset.rvbSend);
    InputDevices::setFmSynthParam(FmSynth_PresetNum,            fmSynthPreset.presetNum);
    InputDevices::setFmSynthParam(FmSynth_NoteShift,            fmSynthPreset.noteShift);
    InputDevices::setFmSynthParam(FmSynth_NoteRangeMin,         fmSynthPreset.noteRangeMin);
    InputDevices::setFmSynthParam(FmSynth_NoteRangeMax,         fmSynthPreset.noteRangeMax);
    InputDevices::setFmSynthParam(FmSynth_PitchBendRange,       fmSynthPreset.pitchBendRange);
    InputDevices::setFmSynthParam(FmSynth_PanType,              fmSynthPreset.panType);
    InputDevices::setFmSynthParam(FmSynth_PanParameter,         fmSynthPreset.panParameter);

    InputDevices::setStereoDelayParam(StereoDelay_DlyL_DlyTime,     stereoDelayPreset.delayLeftDelayTime);
    InputDevices::setStereoDelayParam(StereoDelay_DlyL_Gain,        stereoDelayPreset.delayLeftLevel);
    InputDevices::setStereoDelayParam(StereoDelay_DlyL_RvbSend,     stereoDelayPreset.delayLeftRvbSend);
    InputDevices::setStereoDelayParam(StereoDelay_DlyL_Efx3Send,    stereoDelayPreset.delayLeftEfx3Send);
    InputDevices::setStereoDelayParam(StereoDelay_DlyL_FB_DlyAmt,   stereoDelayPreset.delayLeftFbDelayAmt);
    InputDevices::setStereoDelayParam(StereoDelay_DlyL_xFB_DlyTime, stereoDelayPreset.delayLeftxFbDelayTime);
    InputDevices::setStereoDelayParam(StereoDelay_DlyL_xFB_DlyAmt,  stereoDelayPreset.delayLeftxFbDelayAmt);

    InputDevices::setStereoDelayParam(StereoDelay_DlyR_DlyTime,     stereoDelayPreset.delayRightDelayTime);
    InputDevices::setStereoDelayParam(StereoDelay_DlyR_Gain,        stereoDelayPreset.delayRightLevel);
    InputDevices::setStereoDelayParam(StereoDelay_DlyR_RvbSend,     stereoDelayPreset.delayRightRvbSend);
    InputDevices::setStereoDelayParam(StereoDelay_DlyR_Efx3Send,    stereoDelayPreset.delayRightEfx3Send);
    InputDevices::setStereoDelayParam(StereoDelay_DlyR_FB_DlyAmt,   stereoDelayPreset.delayRightFbDelayAmt);
    InputDevices::setStereoDelayParam(StereoDelay_DlyR_xFB_DlyTime, stereoDelayPreset.delayRightxFbDelayTime);
    InputDevices::setStereoDelayParam(StereoDelay_DlyR_xFB_DlyAmt,  stereoDelayPreset.delayRightxFbDelayAmt);

    InputDevices::setRvbParam(Reverb_Level,         rvbPreset.rvbLevel);
    InputDevices::setRvbParam(Reverb_RoomSize,      rvbPreset.rvbRoomSize);
    InputDevices::setRvbParam(Reverb_Diffusion,     rvbPreset.rvbDiffusion);
    InputDevices::setRvbParam(Reverb_PchShft,       rvbPreset.rvbPchShft);
    InputDevices::setRvbParam(Reverb_PchShftAmt,    rvbPreset.rvbPchShftAmt);
    InputDevices::setRvbParam(Reverb_ShimPchShft,   rvbPreset.rvbShimShft);
    InputDevices::setRvbParam(Reverb_ShimShftRatio, rvbPreset.rvbShimShftRto);
    InputDevices::setRvbParam(Reverb_ShimAmt,       rvbPreset.rvbShimAmt);

    InputDevices::setEfxParam(FutureEfx_Level,  efxPreset.efxGain);
    InputDevices::setEfxParam(FutureEfx_Param2, efxPreset.param2);
    InputDevices::setEfxParam(FutureEfx_Param3, efxPreset.param3);
    InputDevices::setEfxParam(FutureEfx_Param4, efxPreset.param4);

    InputDevices::setInputOutputParam(AudioIo_LineIn_Status,  audioIoPreset.lineInEnabled);
    InputDevices::setInputOutputParam(AudioIo_LineIn_Level,   audioIoPreset.lineInLevel);
    InputDevices::setInputOutputParam(AudioIo_LineOut_Level,  audioIoPreset.lineOutLevel);
    InputDevices::setInputOutputParam(AudioIo_Phone_Volume,   audioIoPreset.phoneLevel);
    InputDevices::setInputOutputParam(AudioIo_LineIn_DlySend, audioIoPreset.lineInDlySend);
    InputDevices::setInputOutputParam(AudioIo_LineIn_RvbSend, audioIoPreset.lineInRvbSend);

    // Note: As of rev 0.5.5.5, MIDI settings have been moved to the utilities screen, and should
    // be stored in PROGMEM as single-instance global settings rather than as part of each preset
//    InputDevices::setMidiParam(paramId, midiPreset.dummy1);
//    InputDevices::setMidiParam(paramId, midiPreset.dummy2);
}

// setScreenParams()
//
void setScreenParams() {

    Screens::setWavPlayerParam(MediaPlayWav_Status,         wavPlayerPreset.enabled);
    Screens::setWavPlayerParam(MediaPlayWav_Level,          wavPlayerPreset.outLevel);
    Screens::setWavPlayerParam(MediaPlayWav_DlySend,        wavPlayerPreset.dlySend);
    Screens::setWavPlayerParam(MediaPlayWav_RvbSend,        wavPlayerPreset.rvbSend);
    Screens::setWavPlayerParam(MediaPlayWav_SoundBank,      wavPlayerPreset.soundBank);
    Screens::setWavPlayerParam(MediaPlayWav_NoteShift,      wavPlayerPreset.noteShift);
    Screens::setWavPlayerParam(MediaPlayWav_NoteRangeMin,   wavPlayerPreset.noteRangeMin);
    Screens::setWavPlayerParam(MediaPlayWav_NoteRangeMax,   wavPlayerPreset.noteRangeMax);
    Screens::setWavPlayerParam(MediaPlayWav_PanType,        wavPlayerPreset.panType);
    Screens::setWavPlayerParam(MediaPlayWav_PanParameter,   wavPlayerPreset.panParameter);

    Screens::setRawPlayerParam(MediaPlayRaw_Status,         rawPlayerPreset.enabled);
    Screens::setRawPlayerParam(MediaPlayRaw_Level,          rawPlayerPreset.outLevel);
    Screens::setRawPlayerParam(MediaPlayRaw_DlySend,        rawPlayerPreset.dlySend);
    Screens::setRawPlayerParam(MediaPlayRaw_RvbSend,        rawPlayerPreset.rvbSend);
    Screens::setRawPlayerParam(MediaPlayRaw_SoundBank,      rawPlayerPreset.soundBank);
    Screens::setRawPlayerParam(MediaPlayRaw_NoteShift,      rawPlayerPreset.noteShift);
    Screens::setRawPlayerParam(MediaPlayRaw_NoteRangeMin,   rawPlayerPreset.noteRangeMin);
    Screens::setRawPlayerParam(MediaPlayRaw_NoteRangeMax,   rawPlayerPreset.noteRangeMax);
    Screens::setRawPlayerParam(MediaPlayRaw_PanType,        rawPlayerPreset.panType);
    Screens::setRawPlayerParam(MediaPlayWav_PanParameter,   rawPlayerPreset.panParameter);

    Screens::setBasicSynthParam(BasicSynth_Status,          basicSynthPreset.enabled);
    Screens::setBasicSynthParam(BasicSynth_Level,           basicSynthPreset.outLevel);
    Screens::setBasicSynthParam(BasicSynth_DlySend,         basicSynthPreset.dlySend);
    Screens::setBasicSynthParam(BasicSynth_RvbSend,         basicSynthPreset.rvbSend);
    Screens::setBasicSynthParam(BasicSynth_Type,            basicSynthPreset.synthType);
    Screens::setBasicSynthParam(BasicSynth_NoteShift,       basicSynthPreset.noteShift);
    Screens::setBasicSynthParam(BasicSynth_NoteRangeMin,    basicSynthPreset.noteRangeMin);
    Screens::setBasicSynthParam(BasicSynth_NoteRangeMax,    basicSynthPreset.noteRangeMax);
    Screens::setBasicSynthParam(BasicSynth_Attack,          basicSynthPreset.attack);
    Screens::setBasicSynthParam(BasicSynth_Decay,           basicSynthPreset.decay);
    Screens::setBasicSynthParam(BasicSynth_Sustain,         basicSynthPreset.sustain);
    Screens::setBasicSynthParam(BasicSynth_Release,         basicSynthPreset.release);
    Screens::setBasicSynthParam(BasicSynth_ModFreq,         basicSynthPreset.modFreq);
    Screens::setBasicSynthParam(BasicSynth_ModAmt,          basicSynthPreset.modAmt);
    Screens::setBasicSynthParam(BasicSynth_PanType,         basicSynthPreset.panType);
    Screens::setBasicSynthParam(BasicSynth_PanParameter,    basicSynthPreset.panParameter);

    Screens::setFmSynthParam(FmSynth_Status,                fmSynthPreset.enabled);
    Screens::setFmSynthParam(FmSynth_Level,                 fmSynthPreset.outLevel);
    Screens::setFmSynthParam(FmSynth_DlySend,               fmSynthPreset.dlySend);
    Screens::setFmSynthParam(FmSynth_RvbSend,               fmSynthPreset.rvbSend);
    Screens::setFmSynthParam(FmSynth_PresetNum,             fmSynthPreset.presetNum);
    Screens::setFmSynthParam(FmSynth_NoteShift,             fmSynthPreset.noteShift);
    Screens::setFmSynthParam(FmSynth_NoteRangeMin,          fmSynthPreset.noteRangeMin);
    Screens::setFmSynthParam(FmSynth_NoteRangeMax,          fmSynthPreset.noteRangeMax);
    Screens::setFmSynthParam(FmSynth_PitchBendRange,        fmSynthPreset.pitchBendRange);
    Screens::setFmSynthParam(FmSynth_PanType,               fmSynthPreset.panType);
    Screens::setFmSynthParam(FmSynth_PanParameter,          fmSynthPreset.panParameter);

    Screens::setStereoDelayParam(StereoDelay_DlyL_DlyTime,      stereoDelayPreset.delayLeftDelayTime);
    Screens::setStereoDelayParam(StereoDelay_DlyL_Gain,         stereoDelayPreset.delayLeftLevel);
    Screens::setStereoDelayParam(StereoDelay_DlyL_RvbSend,      stereoDelayPreset.delayLeftRvbSend);
    Screens::setStereoDelayParam(StereoDelay_DlyL_Efx3Send,     stereoDelayPreset.delayLeftEfx3Send);
    Screens::setStereoDelayParam(StereoDelay_DlyL_FB_DlyAmt,    stereoDelayPreset.delayLeftFbDelayAmt);
    Screens::setStereoDelayParam(StereoDelay_DlyL_xFB_DlyTime,  stereoDelayPreset.delayLeftxFbDelayTime);
    Screens::setStereoDelayParam(StereoDelay_DlyL_xFB_DlyAmt,   stereoDelayPreset.delayLeftxFbDelayAmt);

    Screens::setStereoDelayParam(StereoDelay_DlyR_DlyTime,      stereoDelayPreset.delayRightDelayTime);
    Screens::setStereoDelayParam(StereoDelay_DlyR_Gain,         stereoDelayPreset.delayRightLevel);
    Screens::setStereoDelayParam(StereoDelay_DlyR_RvbSend,      stereoDelayPreset.delayRightRvbSend);
    Screens::setStereoDelayParam(StereoDelay_DlyR_Efx3Send,     stereoDelayPreset.delayRightEfx3Send);
    Screens::setStereoDelayParam(StereoDelay_DlyR_FB_DlyAmt,    stereoDelayPreset.delayRightFbDelayAmt);
    Screens::setStereoDelayParam(StereoDelay_DlyR_xFB_DlyTime,  stereoDelayPreset.delayRightxFbDelayTime);
    Screens::setStereoDelayParam(StereoDelay_DlyR_xFB_DlyAmt,   stereoDelayPreset.delayRightxFbDelayAmt);

    Screens::setRvbParam(Reverb_Level,          rvbPreset.rvbLevel);
    Screens::setRvbParam(Reverb_RoomSize,       rvbPreset.rvbRoomSize);
    Screens::setRvbParam(Reverb_Diffusion,      rvbPreset.rvbDiffusion);
    Screens::setRvbParam(Reverb_PchShft,        rvbPreset.rvbPchShft);
    Screens::setRvbParam(Reverb_PchShftAmt,     rvbPreset.rvbPchShftAmt);
    Screens::setRvbParam(Reverb_ShimPchShft,    rvbPreset.rvbShimShft);
    Screens::setRvbParam(Reverb_ShimShftRatio,  rvbPreset.rvbShimShftRto);
    Screens::setRvbParam(Reverb_ShimAmt,        rvbPreset.rvbShimAmt);

    Screens::setEfxParam(FutureEfx_Level,   efxPreset.efxGain);
    Screens::setEfxParam(FutureEfx_Param2,  efxPreset.param2);
    Screens::setEfxParam(FutureEfx_Param3,  efxPreset.param3);
    Screens::setEfxParam(FutureEfx_Param4,  efxPreset.param4);

    Screens::setInputOutputParam(AudioIo_LineIn_Status,         audioIoPreset.lineInEnabled);
    Screens::setInputOutputParam(AudioIo_LineIn_Level,          audioIoPreset.lineInLevel);
    Screens::setInputOutputParam(AudioIo_LineOut_Level,         audioIoPreset.lineOutLevel);
    Screens::setInputOutputParam(AudioIo_Phone_Volume,          audioIoPreset.phoneLevel);
    Screens::setInputOutputParam(AudioIo_LineIn_DlySend,        audioIoPreset.lineInDlySend);
    Screens::setInputOutputParam(AudioIo_LineIn_RvbSend,        audioIoPreset.lineInRvbSend);

    // Note: As of rev 0.5.5.5, MIDI settings have been moved to the utilities screen, and should
    // be stored in PROGMEM as single-instance global settings rather than as part of each preset
//    Screens::setMidiParam(paramId, midiPreset.dummy1);
//    Screens::setMidiParam(paramId, midiPreset.dummy2);
}

/****************
* End Namespace *
*****************
*/
} // namespace Presets

