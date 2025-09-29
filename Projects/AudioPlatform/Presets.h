/************************************************************************************
*                                     Presets.h                                     *
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
*/

#ifndef PRESETS_H_
#define PRESETS_H_

/****************
* "Pre-Defines" *
*****************
*
* Definitions that affect subsequent external references
*
*/

#define SHOW_PRESET_SETTINGS    0

#if SHOW_PRESET_SETTINGS
#endif // SHOW_PRESET_SETTINGS

/************************************************************************************
*                       E X T E R N A L   R E F E R E N C E S                       *
*************************************************************************************
*/

// Application Components
//
#include "AudioPlatform.h"
#include "AppData/FmSynthPresetConst.h"
#include "MidiDefines.h"

/************************************************************************************
*                E X P O R T E D   M A C R O S  /  C O N S T A N T S                *
*************************************************************************************
*/

/******************
* Begin Namespace *
*******************
*/
namespace Presets {

/************************************************************************************
*            E X P O R T E D   D A T A / T Y P E   D E F I N I T I O N S            *
*************************************************************************************
*
* Exported Data/Types referenced with namespace prefix "Presets::"
*
*/

/****************************
* PROGMEM Storage Constants *
*****************************
*
* Presets Data
*
* TODO: add comments to lines; discuss details here
*
*/

const uint16_t  PresetBlockSize             = PresetDeviceSize * PresetDeviceCount;     // 128 * 32 = 4,096
const uint16_t  PresetBlockCount            = PresetCount;                              // 32
const uint64_t  PresetBankSize              = PresetBlockSize * PresetBlockCount;       // 4,096 * 32 = 131,072
const uint64_t  PresetBankGlobalDataSize    = 16384;
const uint64_t  PresetBankFileSize          = PresetBankSize + PresetBankGlobalDataSize;// 131,072 + 16,384 = 147,456

typedef uint8_t PresetBlock[PresetBlockSize];

/*****************
* EXTMEM Storage *
******************
*
* Defined in Presets.ino as EXTMEM. Exported here for reference in other modules.
*
*/

// Presets File Buffer
// For use when in-memory access to entire presets file is required
//
extern PresetBlock  activePresetsBankBuf[PresetBlockCount];

extern char         fmSynthPresetNameTexts[FmSynthPresetCount][FmSynthPresetNameLength + 1];
extern const char*  fmSynthPresetNames[FmSynthPresetCount];
extern const char*  fmSynthBankNames[FmSynthPresetsBankCount];

/************************************************************************************
*              E X P O R T E D   F U N C T I O N   P R O T O T Y P E S              *
*************************************************************************************
*
* Exported functions referenced with namespace prefix "Presets::"
*
*/

bool        initialize();
bool        presetsInitialized();

const char* presetsCallback(CommandButtonId cmdId, uint16_t& numberParam, void* voidParam = 0);
//const char* presetsCallback(CommandButtonId cmdId, uint16_t& numberParam, const char* textParam = "");
void        selectPreset(uint16_t presetNum, bool import = false);
void        setPresetName(ButtonId presetNum, const char* presetName);
const char* getPresetName(ButtonId presetNum);
void        setPresetModified(ButtonId presetNum, bool modified);
bool        getPresetModified(ButtonId presetNum);
uint16_t    getPresetNumber();
void        showPresetNames();

// Following set...() functions made available in individual screen classes
// as constructor arguments passed to class in the screen's screenDef module
// in order that parameters set in screens (primarily ParamButton screen types)
// can be sent to the Presets manager for updates to device presets.
void        setWavPlayerParam(uint16_t paramId, float paramVal);
void        setRawPlayerParam(uint16_t paramId, float paramVal);
void        setBasicSynthParam(uint16_t paramId, float paramVal);
void        setFmSynthParam(uint16_t paramId, float paramVal);
void        setMidiParam(uint16_t paramId, float paramVal);
void        setRvbParam(uint16_t paramId, float paramVal);
void        setEfxParam(uint16_t paramId, float paramVal);
void        setStereoDelayParam(uint16_t paramId, float paramVal);
void        setInputOutputParam(uint16_t paramId, float paramVal);

// Following get...() functions exported so that other AudioPlatform
// application components can access device parameters.
float       getWavPlayerParam(uint16_t paramId);
float       getRawPlayerParam(uint16_t paramId);
float       getBasicSynthParam(uint16_t paramId);
float       getFmSynthParam(uint16_t paramId);
float       getMidiParam(uint16_t paramId);
float       getRvbParam(uint16_t paramId);
float       getEfxParam(uint16_t paramId);
float       getStereoDelayParam(uint16_t paramId);
float       getInputOutputParam(uint16_t paramId);

void        midiMsgIn(MidiMessageSource msgSrc, MidiMessageType msgType, uint8_t channel, uint8_t data1, uint8_t data2);

#if SHOW_PRESET_SETTINGS
void        showAddresses();
void        showSettings();
#endif // SHOW_PRESET_SETTINGS

/****************
* End Namespace *
*****************
*/
} // namespace Presets

#endif // PRESETS_H_
