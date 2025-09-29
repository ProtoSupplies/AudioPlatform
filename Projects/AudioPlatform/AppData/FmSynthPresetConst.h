/************************************************************************************
*                                FmSynthPresetConst.h                               *
*                                                                                   *
*                       Copyright (c) 2025, Donald Swearingen                       *
*                                All Rights Reserved                                *
*************************************************************************************
*
* Description
*
*   Definitions of constants related to the storage and management of FM Synth voice
*   banks and presets by the AudioPlatform software application.
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

#ifndef FMSYNTHPRESETCONST_H_
#define FMSYNTHPRESETCONST_H_

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

/************************************************************************************
*                E X P O R T E D   M A C R O S  /  C O N S T A N T S                *
*************************************************************************************
*/

const uint16_t  FmSynthPresetBankedLength       = 128;
const uint16_t  FmSynthPresetNameLength         = 10;
const uint16_t  FmSynthBankNameLength           = 10;

const uint8_t   FmSynthPresetsPerBank           = 32;

const uint8_t   FmSynthProgMemPresetsBankCount  = 10;
const uint16_t  FmSynthProgMemPresetCount       = (FmSynthProgMemPresetsBankCount * FmSynthPresetsPerBank);
const uint8_t   FmSynthUserPresetsBankCount     = 6;
const uint16_t  FmSynthUserPresetCount          = (FmSynthUserPresetsBankCount * FmSynthPresetsPerBank);

const uint16_t  FmSynthPresetsBankCount         = (FmSynthProgMemPresetsBankCount + FmSynthUserPresetsBankCount);
const uint16_t  FmSynthPresetCount              = (FmSynthProgMemPresetCount + FmSynthUserPresetCount);

const uint16_t  FmSynthSysxBankSize             = FmSynthPresetsPerBank * FmSynthPresetBankedLength;
const uint16_t  FmSynthSysxBankFileSize         = FmSynthSysxBankSize + 8; // Bank + SYSX headers and Checksum

const uint16_t  FmSynthProgMemPresetBanksSize   = FmSynthSysxBankSize * FmSynthProgMemPresetsBankCount;
#endif  // FMSYNTHPRESETCONST_H_
