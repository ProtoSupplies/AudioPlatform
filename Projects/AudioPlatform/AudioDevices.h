/************************************************************************************
*                                   AudioDevices.h                                  *
*                                                                                   *
*                      Copyright (c) 2025, Donald Swearingen                        *
*                              All Rights Reserved                                  *
*************************************************************************************
*
* Description
*
*   AudioDevices.h is a component of the Arduino application AudioPlatform.
*   The definitions, structures, and functions defined here are specific to that
*   application.
*
*   See AudioDevices.ino for further details.
*
* Modifications List
*
*   • TODO
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

#ifndef AUDIODEVICES_H_
#define AUDIODEVICES_H_

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

// Application Components
//
#include "AudioPlatform.h"

// Teensyduino
//
#include <Audio.h>
#include "elapsedMillis.h"

// User Arduino Libraries
//
#include "SdWavPlayerMulti.h"
#include "FlashRawPlayerMulti.h"

#include "effect_platereverb_i16.h"
#include "StereoPingPongDelay.h"
#include "FutureEfx.h"

#include "AudioMixer8.h"
#include "AudioInput.h"

// Synthesizers
//
#include "FmSynth.h"        // Dexed FM Synthesizer Emulation
#include "BasicSynth.h"     // Customized Version

/************************************************************************************
*                         M A C R O S  /  C O N S T A N T S                         *
*************************************************************************************
*/

// FM Synth Constructor Args
//
const uint8_t       DexedVoiceCount = 8;
const uint16_t      DexedSampleRate = 44100;

/********************************
* Audio Device Default Settings *
*********************************
*
* Definitions here are applied in data structures associated with each audio device
* type (TODO: Create these structures, which will also be used to storage and retrieval
* of device presets).
*
*/

// Audio Device dB Range
//
#define GAIN_MIN_DB ((int16_t) -70)
#define GAIN_MAX_DB ((int16_t) 12)

// Audio Codec Settings
//
// NOTE: Enabling surround affects LR positioning of stereo inputs such that a portion
// of a sound sent to, for example, the left output only, will also be heard in the right
// output, even when the surround width is set to 0.
//
// TODO: Disabled by default. More experimentation dictated, especially when Audio System
// is used primarily with monaurol sound sources.
//
#define CODEC_POST_PROCESS_ENABLE   false
#define CODEC_SURROUND_ENABLE       false
#define CODEC_SURROUND_WIDTH        0           // Virtual surround width 0-mono to 7-widest
#define CODEC_SURROUND_DISABLE_IN   1           // Surround Select parameter
#define CODEC_SURROUND_MONO_IN      2           // Surround Select parameter
#define CODEC_SURROUND_STEREO_IN    3           // Surround Select parameter
#define CODEC_PHONES_VOLUME         0.8f
#define CODEC_TEST_SURROUND         1           // Set width with serial ASCII input 0-7 when surround is enabled

// Audio Memory Settings Using Internal RAM
//
// The argument "n" to the AudioMemory(n) macro indicates the number
// memory "blocks" (256 bytes) to be reserved for audio usage.
//
// At 44.1 kHz sample rate, 2 bytes per sample, 88.2 kBytes per second,
// a 256 byte "block" represents 256 / 88200 = ~2.9 ms of sample
// memory.
//
// In particular, for a digital delay of maximum length 2 seconds, the
// required number of blocks would be:
//
//      (2.0 * 88200.0) = 176,280 bytes of RAM.
//         / 256.0      = 689 audio blocks
//
// The maximum number of bytes in internal RAM available for audio memory,
// MAX_AUDIO_MEMORY, is defined in module AudioStream.cpp as 229,376, or
// ~882 blocks.
//
// Thus a delay of 2 seconds is possible (or 2 delays of 1 second each),
// with room left for other memory usage by other Audio Library devices.
// So we calculate the argument to the AudioMemory macro accordingly,
// adding an additional number of blocks to ensure that other operations
// are supported.
//
// FFS: When implementing a new StereoDelay module in v0.3.4.4, it was
// found that allocating on the basis of 2.0 seconds maximum, and setting
// both the left and right delay channels to a maximum of 1000ms (1 sec)
// caused the output of sound modules (Basic Synth in this example) to
// become distorted, with repeated notes on the same key sounding at
// different pitches, noise, or not at all). It was found necessary to
// increase the allocation to 2.1 secs or more (staying within the 882
// block limit) in order to eliminate the problem. It is hoped that use
// of external memory for delays (see R240809-1) will remove this
// restriction.
//
#define AUDIO_DELAY_MAX_LENGTH          2.1f        // seconds
#define AUDIO_MEMORY_BYES_PER_BLOCK     256.0f      // bytes
#define AUDIO_BYTES_PER_SECOND          88200.0f    // bytes/second
#define AUDIO_DELAY_BLOCK_COUNT         (uint32_t) ((AUDIO_DELAY_MAX_LENGTH * AUDIO_BYTES_PER_SECOND) / AUDIO_MEMORY_BYES_PER_BLOCK)
#define AUDIO_RESERVE_BLOCK_COUNT       50          // additional blocks

// Audio Memory Settings Using External RAM
//
// Lower block count when uxing AudioEffectDelayExternal
#define AUDIO_MEMORY_BLOCK_TOTAL        (uint32_t) (100)

// WAV and RAW Player Sound Bank Selection
//
#define SOUND_BANK_MIN                  ((int16_t) 0)       // int
#define SOUND_BANK_MAX                  ((int16_t) 3)       // int
#define SOUND_BANK_DEFAULT              ((int16_t) 0)       // int

// Note Shift Constants (Common to all player devices: WAV, RAW, BasicSynth, FmSynth)
//
#define NOTE_SHIFT_MIN_SYNTH            ((int16_t) -127)    // note limits (per preset) FM/Basic Synths
#define NOTE_SHIFT_MAX_SYNTH            ((int16_t) 127)
#define NOTE_SHIFT_MIN_PLYR             ((int16_t) -99)     // note limits (per preset) Media Players
#define NOTE_SHIFT_MAX_PLYR             ((int16_t) 99)
#define NOTE_SHIFT_DEFAULT              ((int16_t) 0)
#define NOTE_RANGE_MIN_SYNTH            ((int16_t) 0)       // note range (per preset) FM/Basic Synth
#define NOTE_RANGE_MAX_SYNTH            ((int16_t) 127)
#define NOTE_RANGE_MIN_PLYR             ((int16_t) 0)       // note range (per preset) Media Players
#define NOTE_RANGE_MAX_PLYR             ((int16_t) 99)

// Audio Device Default Values
//
#define DELAY_TIME_MIN                  ((int16_t) 10)      // ms
#define DELAY_TIME_MAX                  ((int16_t) 10000)   // ms
#define DELAY_TIME_DEFAULT              ((int16_t) 500)     // ms
#define DELAY_FEEDBACK_MIN              ((int16_t) 0)       // %
#define DELAY_FEEDBACK_MAX              ((int16_t) 90)      // %
#define DELAY_FEEDBACK_DEFAULT          ((int16_t) 35)      // %
#define DELAY_XFEEDBACK_MIN             ((int16_t) 0)       // % Cross-Feedback
#define DELAY_XFEEDBACK_MAX             ((int16_t) 50)      // % Cross-Feedback
#define DELAY_XFEEDBACK_DEFAULT         ((int16_t) 20)      // % Cross-Feedback
#define DELAY_LEVEL_MIN                 GAIN_MIN_DB         // dB
#define DELAY_LEVEL_MAX                 GAIN_MAX_DB         // dB
#define DELAY_LEVEL_DEFAULT             ((int16_t) -12)     // dB
#define DELAY_RVBSEND_MIN               ((int16_t) 0)       // %
#define DELAY_RVBSEND_MAX               ((int16_t) 100)     // %
#define DELAY_RVBSEND_DEFAULT           ((int16_t) 80)      // %
#define DELAY_EFX3SEND_MIN              ((int16_t) 0)       // %
#define DELAY_EFX3SEND_MAX              ((int16_t) 100)     // %
#define DELAY_EFX3SEND_DEFAULT          ((int16_t) 80)      // %

#define EFX_LEVEL_MIN                   GAIN_MIN_DB         // dB
#define EFX_LEVEL_MAX                   GAIN_MAX_DB         // dB
#define EFX_LEVEL_DEFAULT               ((int16_t) 0)       // dB
#define EFX_PARAM_MIN                   ((int16_t) 0)       // %
#define EFX_PARAM_MAX                   ((int16_t) 100)     // %
#define EFX_PARAM_DEFAULT               ((int16_t) 25)      // %

#define REVERB_LEVEL_MIN                GAIN_MIN_DB         // dB
#define REVERB_LEVEL_MAX                GAIN_MAX_DB         // dB
#define REVERB_LEVEL_DEFAULT            ((int16_t) 0)       // dB
// Following factor to increase base output level to make reverb more apparent
#define REVERB_DB_ADJUST_OFFSET         ((float)6.0)        // dB Boost
#define REVERB_ROOMSIZE_MIN             ((int16_t) 0)       // %
#define REVERB_ROOMSIZE_MAX             ((int16_t) 100)     // %
#define REVERB_ROOMSIZE_DEFAULT         ((int16_t) 60)      // %
#define REVERB_DIFFUSION_MIN            ((int16_t) 0)       // %
#define REVERB_DIFFUSION_MAX            ((int16_t) 100)     // %
#define REVERB_DIFFUSION_DEFAULT        ((int16_t) 70)      // %
#define REVERB_PCHSHFT_MIN              ((int16_t) -12)     // Semitones
#define REVERB_PCHSHFT_MAX              ((int16_t) 24)      // Semitones
#define REVERB_PCHSHFT_DEFAULT          ((int16_t) 0)       // Semitones
#define REVERB_PCHSHFTAMT_MIN           ((int16_t) 0)       // %
#define REVERB_PCHSHFTAMT_MAX           ((int16_t) 100)     // %
#define REVERB_PCHSHFTAMT_DEFAULT       ((int16_t) 0)       // %
#define REVERB_SHIMPCHSHFT_MIN          ((int16_t) -12)     // Semitones
#define REVERB_SHIMPCHSHFT_MAX          ((int16_t) 24)      // Semitones
#define REVERB_SHIMPCHSHFT_DEFAULT      ((int16_t) 0)       // Semitones
#define REVERB_SHIMSHFTRATIO_MIN        ((int16_t) 0)       // %
#define REVERB_SHIMSHFTRATIO_MAX        ((int16_t) 200)     // %
#define REVERB_SHIMSHFTRATIO_DEFAULT    ((int16_t) 0)       // %
#define REVERB_SHIMSHFTAMT_MIN          ((int16_t) 0)       // %
#define REVERB_SHIMSHFTAMT_MAX          ((int16_t) 100)     // %
#define REVERB_SHIMSHFTAMT_DEFAULT      ((int16_t) 70)      // %

#define SDWAVPLAY_STATUS_DEFAULT        Device_Enabled      // 1
#define SDWAVPLAY_STATUS_MIN            Device_Disabled     // 0
#define SDWAVPLAY_STATUS_MAX            Device_Enabled      // 1
#define SDWAVPLAY_LEVEL_MIN             GAIN_MIN_DB         // dB
#define SDWAVPLAY_LEVEL_MAX             GAIN_MAX_DB         // dB
#define SDWAVPLAY_LEVEL_DEFAULT         ((int16_t) 0)       // dB
#define SDWAVPLAY_DLY_SEND_MIN          ((int16_t) 0)       // %
#define SDWAVPLAY_DLY_SEND_MAX          ((int16_t) 90)      // %
#define SDWAVPLAY_DLY_SEND_DEFAULT      ((int16_t) 0)       // %
#define SDWAVPLAY_RVB_SEND_MIN          ((int16_t) 0)       // %
#define SDWAVPLAY_RVB_SEND_MAX          ((int16_t) 100)     // %
#define SDWAVPLAY_RVB_SEND_DEFAULT      ((int16_t) 0)       // %

#define FLASHRAWPLAY_STATUS_DEFAULT     Device_Enabled      // 1
#define FLASHRAWPLAY_STATUS_MIN         Device_Disabled     // 0
#define FLASHRAWPLAY_STATUS_MAX         Device_Enabled      // 1
#define FLASHRAWPLAY_LEVEL_MIN          GAIN_MIN_DB         // dB
#define FLASHRAWPLAY_LEVEL_MAX          GAIN_MAX_DB         // dB
#define FLASHRAWPLAY_LEVEL_DEFAULT      ((int16_t) 0)       // dB
#define FLASHRAWPLAY_DLY_SEND_MIN       ((int16_t) 0)       // %
#define FLASHRAWPLAY_DLY_SEND_MAX       ((int16_t) 90)      // %
#define FLASHRAWPLAY_DLY_SEND_DEFAULT   ((int16_t) 0)       // %
#define FLASHRAWPLAY_RVB_SEND_MIN       ((int16_t) 0)       // %
#define FLASHRAWPLAY_RVB_SEND_MAX       ((int16_t) 100)     // %
#define FLASHRAWPLAY_RVB_SEND_DEFAULT   ((int16_t) 0)       // %

#define BASICSYNTH_STATUS_DEFAULT       Device_Disabled     // 0
#define BASICSYNTH_STATUS_MIN           Device_Disabled     // 0
#define BASICSYNTH_STATUS_MAX           Device_Enabled      // 1
#define BASICSYNTH_LEVEL_MIN            GAIN_MIN_DB         // dB
#define BASICSYNTH_LEVEL_MAX            GAIN_MAX_DB         // dB
#define BASICSYNTH_LEVEL_DEFAULT        ((int16_t) -12)     // dB
#define BASICSYNTH_DLY_SEND_MIN         ((int16_t) 0)       // %
#define BASICSYNTH_DLY_SEND_MAX         ((int16_t) 100)     // %
#define BASICSYNTH_DLY_SEND_DEFAULT     ((int16_t) 0)       // %
#define BASICSYNTH_RVB_SEND_MIN         ((int16_t) 0)       // %
#define BASICSYNTH_RVB_SEND_MAX         ((int16_t) 100)     // %
#define BASICSYNTH_RVB_SEND_DEFAULT     ((int16_t) 80)      // %
// TODO: Map following to appropriate ranges rather than [0,127]
#define BASICSYNTH_MOD_FREQ_MIN         ((int16_t) 0)       // Hz
#define BASICSYNTH_MOD_FREQ_MAX         ((int16_t) 127)     // Hz
#define BASICSYNTH_MOD_FREQ_DEFAULT     ((int16_t) 0)       // Hz
#define BASICSYNTH_MOD_AMT_MIN          ((int16_t) 0)       // %
#define BASICSYNTH_MOD_AMT_MAX          ((int16_t) 100)     // %
#define BASICSYNTH_MOD_AMT_DEFAULT      ((int16_t) 0)       // %
#define BASICSYNTH_ATTACK_MIN           ((int16_t) 0)       // ms
#define BASICSYNTH_ATTACK_MAX           ((int16_t) 3000)    // ms
#define BASICSYNTH_ATTACK_DEFAULT       ((int16_t) 0)       // ms
#define BASICSYNTH_DECAY_MIN            ((int16_t) 0)       // ms
#define BASICSYNTH_DECAY_MAX            ((int16_t) 3000)    // ms
#define BASICSYNTH_DECAY_DEFAULT        ((int16_t) 0)       // ms
#define BASICSYNTH_SUSTAIN_MIN          ((int16_t) 0)       // %
#define BASICSYNTH_SUSTAIN_MAX          ((int16_t) 100)     // %
#define BASICSYNTH_SUSTAIN_DEFAULT      ((int16_t) 100)     // %
#define BASICSYNTH_RELEASE_MIN          ((int16_t) 0)       // ms
#define BASICSYNTH_RELEASE_MAX          ((int16_t) 3000)    // ms
#define BASICSYNTH_RELEASE_DEFAULT      ((int16_t) 0)       // ms
#define BASICSYNTH_TYPE_MIN             ((int16_t) 0)       // char array index
#define BASICSYNTH_TYPE_MAX             ((int16_t) 4)       // char array index
#define BASICSYNTH_TYPE_DEFAULT         ((int16_t) 0)       // char array index

#define FMSYNTH_STATUS_DEFAULT          Device_Disabled     // 0
#define FMSYNTH_STATUS_MIN              Device_Disabled     // 0
#define FMSYNTH_STATUS_MAX              Device_Enabled      // 1
#define FMSYNTH_LEVEL_MIN               GAIN_MIN_DB         // dB * 2
#define FMSYNTH_LEVEL_MAX               GAIN_MAX_DB         // dB * 2
#define FMSYNTH_LEVEL_DEFAULT           ((int16_t) -12)     // dB * 2
#define FMSYNTH_DLY_SEND_MIN            ((int16_t) 0)       // %
#define FMSYNTH_DLY_SEND_MAX            ((int16_t) 100)     // %
#define FMSYNTH_DLY_SEND_DEFAULT        ((int16_t) 0)       // %
#define FMSYNTH_RVB_SEND_MIN            ((int16_t) 0)       // %
#define FMSYNTH_RVB_SEND_MAX            ((int16_t) 100)     // %
#define FMSYNTH_RVB_SEND_DEFAULT        ((int16_t) 80)      // %
#define FMSYNTH_PRESET_NUM_MIN          ((int16_t) 0)       //
#define FMSYNTH_PRESET_NUM_MAX          ((int16_t) 511)     // FFS / FIXME: Another duplication
#define FMSYNTH_PRESET_NUM_DEFAULT      ((int16_t) 0)       //
#define FMSYNTH_PITCH_BEND_MIN          ((int16_t) 1)       // SemiTones
#define FMSYNTH_PITCH_BEND_MAX          ((int16_t) 12)      // SemiTones
#define FMSYNTH_PITCH_BEND_DEFAULT      ((int16_t) 1)       // SemiTones
//#define FMSYNTH_PAN_TYPE_MIN            ((int16_t) 1)       // SemiTones
//#define FMSYNTH_PAN_TYPE_MAX            ((int16_t) 1)       // SemiTones
//#define FMSYNTH_PAN_TYPE_DEFAULT        ((int16_t) 1)       // SemiTones
//#define FMSYNTH_PAN_PARAM_MIN           ((int16_t) 1)       // SemiTones
//#define FMSYNTH_PAN_PARAM_MAX           ((int16_t) 1)       // SemiTones
//#define FMSYNTH_PAN_PARAM_DEFAULT       ((int16_t) 1)       // SemiTones

#define AUDIOIO_LINEIN_STATUS_MIN       Device_Disabled     // 0 char array index
#define AUDIOIO_LINEIN_STATUS_MAX       Device_Enabled      // 1 char array index
#define AUDIOIO_LINEIN_STATUS_DEFAULT   Device_Disabled     // 1 char array index
#define AUDIOIO_LINEIN_LVL_MIN          ((int16_t) 0)       // P-P voltage table index
#define AUDIOIO_LINEIN_LVL_MAX          ((int16_t) 15)      // P-P voltage table index
#define AUDIOIO_LINEIN_LVL_DEFAULT      ((int16_t) 5)       // P-P voltage table index (1.33V)
#define AUDIOIO_LINEOUT_LVL_MIN         ((int16_t) 0)       // P-P voltage table index
#define AUDIOIO_LINEOUT_LVL_MAX         ((int16_t) 18)      // P-P voltage table index
#define AUDIOIO_LINEOUT_LVL_DEFAULT     ((int16_t) 16)      // P-P voltage table index (2.83V, see AudioDevices::setInputOutputParam())
#define AUDIOIO_PHONEVOLUME_MIN         ((int16_t) 0)       // %
#define AUDIOIO_PHONEVOLUME_MAX         ((int16_t) 100)     // %
#define AUDIOIO_PHONEVOLUME_DEFAULT     ((int16_t) 80)      // %
#define AUDIOIO_LINEIN_DLY_SEND_MIN     ((int16_t) 0)       // %
#define AUDIOIO_LINEIN_DLY_SEND_MAX     ((int16_t) 100)     // %
#define AUDIOIO_LINEIN_DLY_SEND_DEFAULT ((int16_t) 0)       // %
#define AUDIOIO_LINEIN_RVB_SEND_MIN     ((int16_t) 0)       // %
#define AUDIOIO_LINEIN_RVB_SEND_MAX     ((int16_t) 100)     // %
#define AUDIOIO_LINEIN_RVB_SEND_DEFAULT ((int16_t) 0)       // %

#define AUDIOIO_INPUT_MIN               ((int16_t) AUDIO_INPUT_LINEIN)  // char array index
#define AUDIOIO_INPUT_MAX               ((int16_t) AUDIO_INPUT_MIC)     // char array index
#define AUDIOIO_INPUT_DEFAULT           ((int16_t) AUDIO_INPUT_LINEIN)  // char array index
#define AUDIOIO_MIC_GAIN_MIN            ((int16_t) 0)       // dB (2 x actual value)
#define AUDIOIO_MIC_GAIN_MAX            ((int16_t) 126)     // dB (2 x actual value)
#define AUDIOIO_MIC_GAIN_DEFAULT        ((int16_t) 0)       // dB (2 x actual value)

/************************************************************************************
*            E X P O R T E D   D A T A / T Y P E   D E F I N I T I O N S            *
*************************************************************************************
*
* Exported Data/Types referenced with namespace prefix "AudioDevices::"
*
*/

/******************
* Begin Namespace *
*******************
*/
namespace AudioDevices {

AudioMixer8     OutMixerLeft;       // Referenced in Activity Monitor Screen
AudioMixer8     OutMixerRight;      // Referenced in Activity Monitor Screen

/************************************************************************************
*              E X P O R T E D   F U N C T I O N   P R O T O T Y P E S              *
*************************************************************************************
*
* Exported functions referenced with namespace prefix "AudioDevices::"
*
*/

void        begin();
void        initializeDeviceSettings();

void        setMediaPlayerSoundFiles();

// Parameter change handlers for AudioPLatform devices called from Presets Manager (Presets.ino).
// Presets Manager calls these functions in all AudioPlatform core components in two cases:
//
//  1. When new presets are loaded, to apply preset settings in other core components.
//  2. When processing applyFn() callbacks from ParamButton screens when preset
//     parameters managed by rotary encoders are modified, again to apply the modified
//     settings in other core components
//
void        setWavPlayerParam(uint16_t paramId, float paramVal);
void        setRawPlayerParam(uint16_t paramId, float paramVal);
void        setBasicSynthParam(uint16_t paramId, float paramVal);
void        setFmSynthParam(uint16_t paramId, float paramVal);
void        setMidiParam(uint16_t paramId, float paramVal);
void        setRvbParam(uint16_t paramId, float paramVal);
void        setEfxParam(uint16_t paramId, float paramVal);
void        setStereoDelayParam(uint16_t paramId, float paramVal);
void        setInputOutputParam(uint16_t paramId, float paramVal);

// TODO: Move to Preset domain
void        setPrestBanksPtr(uint8_t* banksAddress);
void        setFmSynthVoicePreset(uint16_t voicePresetNum);
const char* getFmSynthVoicePresetName(uint16_t voicePresetNum);

void        handleNoteOn(uint8_t note, uint8_t velocity);
void        handleNoteOff(uint8_t note);
void        handleIgnoreNoteOff(bool ignore);
void        handlePitchBend(int16_t bend);
void        handleSustainPedal(bool);
void        handlePanControl(uint8_t value);
void        handleAllNotesOff();
void        handleVolumeControl(uint8_t value);

void        pollAudioDevices();

/****************
* End Namespace *
*****************
*/
} // namespace AudioDevices


#endif // AUDIODEVICES_H_
