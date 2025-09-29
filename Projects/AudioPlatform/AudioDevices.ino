/************************************************************************************
*                                 AudioDevices.ino                                  *
*                                                                                   *
*                       Copyright (c) 2025, Donald Swearingen                       *
*                                All Rights Reserved                                *
*************************************************************************************
*
* Description
*
*   AudioDevices defines the structural elements (variables, types, methods, and
*   terms) of the AudioDevices class, a component of the Teensy/Arduino application
*   AudioPlatform. The application is in turn based on classes, and objects defined
*   in the Teensy Audio Library:
*
*       (https://www.pjrc.com/teensy/td_libs_Audio.html)
*
*   as well as classes and objects defined in the Arduino user Libraries folder,
*   including the DisplayScreens, EncoderTool, MediaPlayers, MidiSupport, BasicSynth,
*   Utility, and AudioPlatform CustomScreens.
*
*   References to AudioDevices public objects and methods are prefixed with the namespace
*   "AudioDevices::", as in  AudioDevices::begin().
*
* Modifications List
*
* Copyright Notices
*
*   AudioDevices.ino
*   StereoPingPongDelay
*   Copyright (c) 2025, Donald Swearingen
*
*   Stereo Plate Reverb for Teensy 4 (AudioEffectPlateReverb_i16)
*   Copyright (c) 2020 by Piotr Zapart
*   https://github.com/hexeguitar/hexefx_audiolib_i16
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

// Application Components
//
#include "AudioDevices.h"

// TeensyDuino Libraries
//
#include "ElapsedMillis.h"

// User Arduino Libraries
//
#include "SystemProfileScreen.h"
#include "MidiDefines.h"
#include "UtilFns.h"

/************************************************************************************
*                         M A C R O S  /  C O N S T A N T S                         *
*************************************************************************************
*/

/**************************************
* Audio Device Mixer Channel Routings *
***************************************
*/

#define DELAYIN_CHAN_WAVPLAYER          0
#define DELAYIN_CHAN_RAWPLAYER          1
#define DELAYIN_CHAN_LINEIN             2
#define DELAYIN_CHAN_BASICSYNTH         3
#define DELAYIN_CHAN_FMSYNTH            4
#define DELAYIN_CHAN_EFX                5

#define REVERBIN_CHAN_WAVPLAYER         0
#define REVERBIN_CHAN_RAWPLAYER         1
#define REVERBIN_CHAN_LINEIN            2
#define REVERBIN_CHAN_BASICSYNTH        3
#define REVERBIN_CHAN_FMSYNTH           4
#define REVERBIN_CHAN_EFX               5
#define REVERBIN_CHAN_DELAY             6

#define EFXIN_CHAN_WAVPLAYER            0
#define EFXIN_CHAN_RAWPLAYER            1
#define EFXIN_CHAN_LINEIN               2
#define EFXIN_CHAN_BASICSYNTH           3
#define EFXIN_CHAN_FMSYNTH              4
#define EFXIN_CHAN_DELAY                6

#define AUDIO_OUT_CHAN_WAVPLAYER        0
#define AUDIO_OUT_CHAN_RAWPLAYER        1
#define AUDIO_OUT_CHAN_LINEIN           2
#define AUDIO_OUT_CHAN_BASICSYNTH       3
#define AUDIO_OUT_CHAN_FMSYNTH          4
#define AUDIO_OUT_CHAN_EFX              5
#define AUDIO_OUT_CHAN_DELAY            6
#define AUDIO_OUT_CHAN_REVERB           7

/************************************************************************************
*               L O C A L   D A T A / T Y P E   D E F I N I T I O N S               *
*************************************************************************************
*
* Data types and variables visible only within this module
*
*/

// Audio Codec
//
// Main audio IO device for system
//
AudioControlSGTL5000    AudioCodec;

// Audio Source Device Instances
//
AudioInput              AudioLineIn;
SdWavPlayerMulti        WavPlayer(8);
FlashRawPlayerMulti     RawPlayer(8);
FmSynth                 FrqModSynth(DexedVoiceCount, DexedSampleRate);
BasicSynth              CoreSynth;

// Audio Mixer Device Instances
//
AudioMixer8             ReverbInMixerLeft;
AudioMixer8             ReverbInMixerRight;
AudioMixer8             DelayInMixerLeft;
AudioMixer8             DelayInMixerRight;
AudioMixer8             EfxInMixerLeft;
AudioMixer8             EfxInMixerRight;

// Audio Effects Device Instances
//
StereoPingPongDelay     StereoDelay(DelayInMixerLeft, DelayInMixerRight);
AudioEffectPlateReverb_i16 PlateReverb;
FutureEfx               FutureAudioEfx(EfxInMixerLeft, EfxInMixerRight);

// Audio Output Device Instances
//
AudioOutputI2S          AudioOut;

// Audio Connections
//
// NOTE: For a diagram of application device interconnections,
// see the accompanying document "AudioPlayformAudioDevices.png"
//
AudioConnection         WavPlayerLeftToReverbInMixerLeft;
AudioConnection         WavPlayerRightToReverbInMixerRight;
AudioConnection         WavPlayerLeftToEfxInMixerLeft;
AudioConnection         WavPlayerRightToEfxInMixerRight;
AudioConnection         WavPlayerLeftToDelayInMixerLeft;
AudioConnection         WavPlayerRightToDelayInMixerRight;
AudioConnection         WavPlayerLeftToOutMixerLeft;
AudioConnection         WavPlayerRightToOutMixerRight;

AudioConnection         RawPlayerLeftToReverbInMixerLeft;
AudioConnection         RawPlayerRightToReverbInMixerRight;
AudioConnection         RawPlayerLeftToEfxInMixerLeft;
AudioConnection         RawPlayerRightToEfxInMixerRight;
AudioConnection         RawPlayerLeftToDelayInMixerLeft;
AudioConnection         RawPlayerRightToDelayInMixerRight;
AudioConnection         RawPlayerLeftToOutMixerLeft;
AudioConnection         RawPlayerRightToOutMixerRight;

AudioConnection         AudioLineInLeftToReverbInMixerLeft;
AudioConnection         AudioLineInRightToReverbInMixerRight;
AudioConnection         AudioLineInLeftToEfxInMixerLeft;
AudioConnection         AudioLineInRightToEfxInMixerRight;
AudioConnection         AudioLineInLeftToDelayInMixerLeft;
AudioConnection         AudioLineInRightToDelayInMixerRight;
AudioConnection         AudioLineInLeftToOutMixer;
AudioConnection         AudioLineInRightToOutMixer;

AudioConnection         CoreSynthLeftToReverbInMixerLeft;
AudioConnection         CoreSynthRightToReverbInMixerRight;
AudioConnection         CoreSynthLeftToEfxInMixerLeft;
AudioConnection         CoreSynthRightToEfxInMixerRight;
AudioConnection         CoreSynthLeftToDelayInMixerLeft;
AudioConnection         CoreSynthRightToDelayInMixerRight;
AudioConnection         CoreSynthLeftToOutMixerLeft;
AudioConnection         CoreSynthRightToOutMixerRight;

AudioConnection         FmSynthLeftToReverbInMixerLeft;
AudioConnection         FmSynthRightToReverbInMixerRight;
AudioConnection         FmSynthLeftToEfxInMixerLeft;
AudioConnection         FmSynthRightToEfxInMixerRight;
AudioConnection         FmSynthLeftToDelayInMixerLeft;
AudioConnection         FmSynthRightToDelayInMixerRight;
AudioConnection         FmSynthLeftToOutMixerLeft;
AudioConnection         FmSynthRightToOutMixerRight;

AudioConnection         DelayLeftToReverbInMixerLeft;
AudioConnection         DelayRightToReverbInMixerRight;
AudioConnection         DelayLeftToEfxInMixerLeft;
AudioConnection         DelayRightToEfxInMixerRight;
AudioConnection         DelayLeftFdbkToDelayInMixerLeft;
AudioConnection         DelayRightFdbkToDelayInMixerRight;
AudioConnection         DelayLeftToOutMixerLeft;
AudioConnection         DelayRightToOutMixerRight;
AudioConnection         DelayInMixerLeftToDelayInMixerLeft;
AudioConnection         DelayInMixerRightToDelayInMixerRight;

AudioConnection         EfxLeftToReverbInMixerLeft;
AudioConnection         EfxRightToReverbInMixerRight;
AudioConnection         EfxLeftToDelayInMixerLeft;
AudioConnection         EfxRightToDelayInMixerRight;
AudioConnection         EfxLeftToOutMixerLeft;
AudioConnection         EfxRightToOutMixerRight;

AudioConnection         ReverbLeftToPlateReverbLeft;
AudioConnection         ReverbRightToPlateReverbRight;
AudioConnection         PlateReverbLeftToOutMixerLeft;
AudioConnection         PlateReverbRightToOutMixerRight;

AudioConnection         OutMixerLeftToI2sOutLeft;
AudioConnection         OutMixerRightToI2sOutRight;

// Player Status Data
//
typedef struct {

    bool    enabled;
    bool    ignoreNoteOff;

} PlayerStatus;

PlayerStatus            coreSynthStatus = {false, false};
PlayerStatus            fmSynthStatus   = {false, false};
PlayerStatus            wavPlayerStatus = {false, false};
PlayerStatus            rawPlayerStatus = {false, false};

// CoreSynth Update Timer
//
elapsedMillis           coreSynthUpdateTimer = 0;
const uint32_t          coreSynthUpdateInterval = 20;      // ms

// Misc
//
bool                    initialized = false;

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

/************************************************************************************
*                 L O C A L   F U N C T I O N   P R O T O T Y P E S                 *
*************************************************************************************
*
* Local prototype prototypes to avoid forward references (in namespace, but not exported
* in .h file).
*
*/
void    wavFileNote(uint8_t note, uint8_t midiVelocity);
void    rawFileNote(uint8_t note, uint8_t midiVelocity);

/************************************************************************************
*                        E X P O R T E D   F U N C T I O N S                        *
*************************************************************************************
*
* Exported functions referenced with namespace prefix "InputDevices::"
*
*/

/***************************
* Startup / Initialization *
****************************
*/

// begin()
//
// Allocate audio memory and establish initial codec settings
//
void begin() {

    if (!initialized) {
        Serial.printf("\nAudioDevices::begin: Allocating %d blocks of audio memory\n", AUDIO_MEMORY_BLOCK_TOTAL);
        AudioMemory(AUDIO_MEMORY_BLOCK_TOTAL);
        AudioCodec.enable();
        AudioCodec.volume(CODEC_PHONES_VOLUME);
        if (CODEC_POST_PROCESS_ENABLE) {
            AudioCodec.audioPostProcessorEnable();
            if (CODEC_SURROUND_ENABLE) {
                AudioCodec.surroundSoundEnable();
                AudioCodec.surroundSound(CODEC_SURROUND_WIDTH, CODEC_SURROUND_STEREO_IN);
            }
        } else {
            AudioCodec.audioProcessorDisable();
        }

        WavPlayerLeftToReverbInMixerLeft.connect(*WavPlayer.getLeftOutput(), 0, ReverbInMixerLeft, REVERBIN_CHAN_WAVPLAYER);
        WavPlayerRightToReverbInMixerRight.connect(*WavPlayer.getRightOutput(), 0, ReverbInMixerRight, REVERBIN_CHAN_WAVPLAYER);

        WavPlayerLeftToEfxInMixerLeft.connect(*WavPlayer.getLeftOutput(), 0, EfxInMixerLeft, EFXIN_CHAN_WAVPLAYER);
        WavPlayerRightToEfxInMixerRight.connect(*WavPlayer.getRightOutput(), 0, EfxInMixerRight, EFXIN_CHAN_WAVPLAYER);

        WavPlayerLeftToDelayInMixerLeft.connect(*WavPlayer.getLeftOutput(), 0, DelayInMixerLeft, DELAYIN_CHAN_WAVPLAYER);
        WavPlayerRightToDelayInMixerRight.connect(*WavPlayer.getRightOutput(), 0, DelayInMixerRight, DELAYIN_CHAN_WAVPLAYER);

        WavPlayerLeftToOutMixerLeft.connect(*WavPlayer.getLeftOutput(), 0, OutMixerLeft, AUDIO_OUT_CHAN_WAVPLAYER);
        WavPlayerRightToOutMixerRight.connect(*WavPlayer.getRightOutput(), 0, OutMixerRight, AUDIO_OUT_CHAN_WAVPLAYER);


        RawPlayerLeftToReverbInMixerLeft.connect(*RawPlayer.getLeftOutput(), 0, ReverbInMixerLeft, REVERBIN_CHAN_RAWPLAYER);
        RawPlayerRightToReverbInMixerRight.connect(*RawPlayer.getRightOutput(), 0, ReverbInMixerRight, REVERBIN_CHAN_RAWPLAYER);

        RawPlayerLeftToEfxInMixerLeft.connect(*RawPlayer.getLeftOutput(), 0, EfxInMixerLeft, EFXIN_CHAN_WAVPLAYER);
        RawPlayerRightToEfxInMixerRight.connect(*RawPlayer.getRightOutput(), 0, EfxInMixerRight, EFXIN_CHAN_WAVPLAYER);

        RawPlayerLeftToDelayInMixerLeft.connect(*RawPlayer.getLeftOutput(), 0, DelayInMixerLeft, DELAYIN_CHAN_RAWPLAYER);
        RawPlayerRightToDelayInMixerRight.connect(*RawPlayer.getRightOutput(), 0, DelayInMixerRight, DELAYIN_CHAN_RAWPLAYER);

        RawPlayerLeftToOutMixerLeft.connect(*RawPlayer.getLeftOutput(), 0, OutMixerLeft, AUDIO_OUT_CHAN_RAWPLAYER);
        RawPlayerRightToOutMixerRight.connect(*RawPlayer.getRightOutput(), 0, OutMixerRight, AUDIO_OUT_CHAN_RAWPLAYER);


        AudioLineInLeftToReverbInMixerLeft.connect(*AudioLineIn.getLeftOutput(), 0, ReverbInMixerLeft, REVERBIN_CHAN_LINEIN);
        AudioLineInRightToReverbInMixerRight.connect(*AudioLineIn.getRightOutput(), 0, ReverbInMixerRight, REVERBIN_CHAN_LINEIN);

        AudioLineInLeftToEfxInMixerLeft.connect(*AudioLineIn.getLeftOutput(), 0, EfxInMixerLeft, EFXIN_CHAN_LINEIN);
        AudioLineInRightToEfxInMixerRight.connect(*AudioLineIn.getRightOutput(), 0, EfxInMixerRight, EFXIN_CHAN_LINEIN);

        AudioLineInLeftToDelayInMixerLeft.connect(*AudioLineIn.getLeftOutput(), 0, DelayInMixerLeft, DELAYIN_CHAN_LINEIN);
        AudioLineInRightToDelayInMixerRight.connect(*AudioLineIn.getRightOutput(), 0, DelayInMixerRight, DELAYIN_CHAN_LINEIN);

        AudioLineInLeftToOutMixer.connect(*AudioLineIn.getLeftOutput(), 0, OutMixerLeft, AUDIO_OUT_CHAN_LINEIN);
        AudioLineInRightToOutMixer.connect(*AudioLineIn.getRightOutput(), 0, OutMixerRight, AUDIO_OUT_CHAN_LINEIN);


        FmSynthLeftToReverbInMixerLeft.connect(*FrqModSynth.getLeftOutput(), 0, ReverbInMixerLeft, REVERBIN_CHAN_FMSYNTH);
        FmSynthRightToReverbInMixerRight.connect(*FrqModSynth.getRightOutput(), 0, ReverbInMixerRight, REVERBIN_CHAN_FMSYNTH);

        FmSynthLeftToEfxInMixerLeft.connect(*FrqModSynth.getLeftOutput(), 0, EfxInMixerLeft, EFXIN_CHAN_FMSYNTH);
        FmSynthRightToEfxInMixerRight.connect(*FrqModSynth.getRightOutput(), 0, EfxInMixerRight, EFXIN_CHAN_FMSYNTH);

        FmSynthLeftToDelayInMixerLeft.connect(*FrqModSynth.getLeftOutput(), 0, DelayInMixerLeft, DELAYIN_CHAN_FMSYNTH);
        FmSynthRightToDelayInMixerRight.connect(*FrqModSynth.getRightOutput(), 0, DelayInMixerRight, DELAYIN_CHAN_FMSYNTH);

        FmSynthLeftToOutMixerLeft.connect(*FrqModSynth.getLeftOutput(), 0, OutMixerLeft, AUDIO_OUT_CHAN_FMSYNTH);
        FmSynthRightToOutMixerRight.connect(*FrqModSynth.getRightOutput(), 0, OutMixerRight, AUDIO_OUT_CHAN_FMSYNTH);



        CoreSynthLeftToReverbInMixerLeft.connect(*CoreSynth.getLeftOutput(), 0, ReverbInMixerLeft, REVERBIN_CHAN_BASICSYNTH);
        CoreSynthRightToReverbInMixerRight.connect(*CoreSynth.getRightOutput(), 0, ReverbInMixerRight, REVERBIN_CHAN_BASICSYNTH);

        CoreSynthLeftToEfxInMixerLeft.connect(*CoreSynth.getLeftOutput(), 0, EfxInMixerLeft, EFXIN_CHAN_FMSYNTH);
        CoreSynthRightToEfxInMixerRight.connect(*CoreSynth.getRightOutput(), 0, EfxInMixerRight, EFXIN_CHAN_FMSYNTH);

        CoreSynthLeftToDelayInMixerLeft.connect(*CoreSynth.getLeftOutput(), 0, DelayInMixerLeft, EFXIN_CHAN_BASICSYNTH);
        CoreSynthRightToDelayInMixerRight.connect(*CoreSynth.getRightOutput(), 0, DelayInMixerRight, EFXIN_CHAN_BASICSYNTH);

        CoreSynthLeftToOutMixerLeft.connect(*CoreSynth.getLeftOutput(), 0, OutMixerLeft, AUDIO_OUT_CHAN_BASICSYNTH);
        CoreSynthRightToOutMixerRight.connect(*CoreSynth.getRightOutput(), 0, OutMixerRight, AUDIO_OUT_CHAN_BASICSYNTH);



        DelayLeftToReverbInMixerLeft.connect(*StereoDelay.getLeftOutput(), 0, ReverbInMixerLeft, REVERBIN_CHAN_DELAY);
        DelayRightToReverbInMixerRight.connect(*StereoDelay.getRightOutput(), 0, ReverbInMixerRight, REVERBIN_CHAN_DELAY);

        DelayLeftToEfxInMixerLeft.connect(*StereoDelay.getLeftOutput(), 0, EfxInMixerLeft, EFXIN_CHAN_DELAY);
        DelayRightToEfxInMixerRight.connect(*StereoDelay.getRightOutput(), 0, EfxInMixerRight, EFXIN_CHAN_DELAY);

        DelayLeftToEfxInMixerLeft.connect(*StereoDelay.getLeftOutput(), 0, ReverbInMixerLeft, REVERBIN_CHAN_DELAY);
        DelayRightToEfxInMixerRight.connect(*StereoDelay.getRightOutput(), 0, ReverbInMixerRight, REVERBIN_CHAN_DELAY);

        DelayLeftToOutMixerLeft.connect(*StereoDelay.getLeftOutput(), 0, OutMixerLeft, AUDIO_OUT_CHAN_DELAY);
        DelayRightToOutMixerRight.connect(*StereoDelay.getRightOutput(), 0, OutMixerRight, AUDIO_OUT_CHAN_DELAY);

        StereoDelay.setLeftInput(DelayInMixerLeft);
        StereoDelay.setRightInput(DelayInMixerRight);

        EfxLeftToReverbInMixerLeft.connect(*FutureAudioEfx.getLeftOutput(), 0, ReverbInMixerLeft, REVERBIN_CHAN_EFX);
        EfxRightToReverbInMixerRight.connect(*FutureAudioEfx.getRightOutput(), 0, ReverbInMixerRight, REVERBIN_CHAN_EFX);

        EfxLeftToDelayInMixerLeft.connect(*FutureAudioEfx.getLeftOutput(), 0, DelayInMixerLeft, DELAYIN_CHAN_EFX);
        EfxRightToDelayInMixerRight.connect(*FutureAudioEfx.getRightOutput(), 0, DelayInMixerRight, DELAYIN_CHAN_EFX);

        EfxLeftToOutMixerLeft.connect(*FutureAudioEfx.getLeftOutput(), 0, OutMixerLeft, AUDIO_OUT_CHAN_EFX);
        EfxRightToOutMixerRight.connect(*FutureAudioEfx.getRightOutput(), 0, OutMixerRight, AUDIO_OUT_CHAN_EFX);

        ReverbLeftToPlateReverbLeft.connect(ReverbInMixerLeft, 0, PlateReverb, 0);
        ReverbRightToPlateReverbRight.connect(ReverbInMixerRight, 0, PlateReverb, 1);

        PlateReverbLeftToOutMixerLeft.connect(PlateReverb, 0, OutMixerLeft, AUDIO_OUT_CHAN_REVERB);
        PlateReverbRightToOutMixerRight.connect(PlateReverb, 1, OutMixerRight, AUDIO_OUT_CHAN_REVERB);

        OutMixerLeftToI2sOutLeft.connect(OutMixerLeft, 0, AudioOut, 0);
        OutMixerRightToI2sOutRight.connect(OutMixerRight, 0, AudioOut, 1);

        setMediaPlayerSoundFiles();

        initialized = true;
    }
}

// initializeDeviceSettings()
//
// Initial gain and effects settings at startup
//
// NOTE: Pan-related constants defined in MidiDefines.h
//
void initializeDeviceSettings() {

    // Effects and Output Mixer Gain Settings
    //
    // NOTE: Delay and Reverb input mixer channel levels set to 0 initially.
    // Individual levels will be adjusted at runtime by effects send controls.
    // Initial send levels of 0 may also be overridden below by individual
    // device default settings.
    //
    DelayInMixerLeft.gain(DELAYIN_CHAN_WAVPLAYER, 0.0);     // Direct Signal Left Chan. to Delay
    DelayInMixerLeft.gain(DELAYIN_CHAN_RAWPLAYER, 0.0);     // Direct Signal Left Chan. to Delay
    DelayInMixerLeft.gain(DELAYIN_CHAN_BASICSYNTH, 0.0);    // Synth Signal Right Chan. to Delay
    DelayInMixerLeft.gain(DELAYIN_CHAN_FMSYNTH, 0.0);       // Synth Signal Right Chan. to Delay
    DelayInMixerLeft.gain(DELAYIN_CHAN_EFX, 0.0);           // Efx Signal Right Chan. to Delay
    DelayInMixerLeft.gain(DELAYIN_CHAN_LINEIN, 0.0);        // Line In Signal Left Chan. to Delay

    DelayInMixerRight.gain(DELAYIN_CHAN_WAVPLAYER, 0.0);    // Direct Signal Left Chan. to Delay
    DelayInMixerRight.gain(DELAYIN_CHAN_RAWPLAYER, 0.0);    // Direct Signal Left Chan. to Delay
    DelayInMixerRight.gain(DELAYIN_CHAN_BASICSYNTH, 0.0);   // Synth Signal Right Chan. to Delay
    DelayInMixerRight.gain(DELAYIN_CHAN_FMSYNTH, 0.0);      // Synth Signal Right Chan. to Delay
    DelayInMixerRight.gain(DELAYIN_CHAN_EFX, 0.0);          // Efx Signal Right Chan. to Delay
    DelayInMixerRight.gain(DELAYIN_CHAN_LINEIN, 0.0);       // Line In Signal Left Chan. to Delay

    EfxInMixerLeft.gain(EFXIN_CHAN_WAVPLAYER, 1.0);         // SD Player Direct Left
    EfxInMixerLeft.gain(EFXIN_CHAN_RAWPLAYER, 1.0);         // RAW Player Direct Left
    EfxInMixerLeft.gain(EFXIN_CHAN_LINEIN, 1.0);            // Line In Direct Left
    EfxInMixerLeft.gain(EFXIN_CHAN_BASICSYNTH, 1.0);        // Synth Direct Left
    EfxInMixerLeft.gain(EFXIN_CHAN_FMSYNTH, 1.0);           // Synth Direct Left
    EfxInMixerLeft.gain(EFXIN_CHAN_DELAY, 1.0);             // Delay Out Left

    EfxInMixerRight.gain(EFXIN_CHAN_WAVPLAYER, 1.0);        // SD Player Direct Right
    EfxInMixerRight.gain(EFXIN_CHAN_RAWPLAYER, 1.0);        // RAW Player Direct Right
    EfxInMixerRight.gain(EFXIN_CHAN_LINEIN, 1.0);           // Line In Direct Left
    EfxInMixerRight.gain(EFXIN_CHAN_BASICSYNTH, 1.0);       // Synth Direct Right
    EfxInMixerRight.gain(EFXIN_CHAN_FMSYNTH, 1.0);          // Synth Direct Right
    EfxInMixerRight.gain(EFXIN_CHAN_DELAY, 1.0);            // Delay Out Right

    ReverbInMixerLeft.gain(REVERBIN_CHAN_WAVPLAYER, 0.0);   // SD Player Left to Reverb
    ReverbInMixerLeft.gain(REVERBIN_CHAN_RAWPLAYER, 0.0);   // RAW Player Left to Reverb
    ReverbInMixerLeft.gain(REVERBIN_CHAN_BASICSYNTH, 0.0);  // Synth Left to Reverb
    ReverbInMixerLeft.gain(REVERBIN_CHAN_FMSYNTH, 0.0);     // FM Synth Left to Reverb
    ReverbInMixerLeft.gain(REVERBIN_CHAN_LINEIN, 0.0);      // Line In Left to Reverb
    ReverbInMixerLeft.gain(REVERBIN_CHAN_EFX, 0.0);         // Efx Left to Reverb
    ReverbInMixerLeft.gain(REVERBIN_CHAN_DELAY, 0.0);       // Delay Left to Reverb

    ReverbInMixerRight.gain(REVERBIN_CHAN_WAVPLAYER, 0.0);  // SD Player Right to Reverb
    ReverbInMixerRight.gain(REVERBIN_CHAN_RAWPLAYER, 0.0);  // SD Player Right to Reverb
    ReverbInMixerRight.gain(REVERBIN_CHAN_BASICSYNTH, 0.0); // Synth Right to Reverb
    ReverbInMixerRight.gain(REVERBIN_CHAN_FMSYNTH, 0.0);    // Synth Right to FmSynth
    ReverbInMixerRight.gain(REVERBIN_CHAN_LINEIN, 0.0);     // Line In Right to Reverb
    ReverbInMixerRight.gain(REVERBIN_CHAN_EFX, 0.0);        // Efx Right to Reverb
    ReverbInMixerRight.gain(REVERBIN_CHAN_DELAY, 0.0);      // Delay Right to Reverb

    OutMixerLeft.gain(AUDIO_OUT_CHAN_WAVPLAYER, 1.0);       // SD Player Direct Left
    OutMixerLeft.gain(AUDIO_OUT_CHAN_RAWPLAYER, 1.0);       // SD Player Direct Left
    OutMixerLeft.gain(AUDIO_OUT_CHAN_BASICSYNTH, 1.0);      // Basic Synth Direct Left
    OutMixerLeft.gain(AUDIO_OUT_CHAN_FMSYNTH, 1.0);         // FM Synth Direct Left
    OutMixerLeft.gain(AUDIO_OUT_CHAN_LINEIN, 1.0);          // Line In Direct Left
    OutMixerLeft.gain(AUDIO_OUT_CHAN_REVERB, 1.0);          // Reverb Out Left
    OutMixerLeft.gain(AUDIO_OUT_CHAN_EFX, 1.0);             // Efx Out Left
    OutMixerLeft.gain(AUDIO_OUT_CHAN_DELAY, 1.0);           // Delay Out Left

    OutMixerRight.gain(AUDIO_OUT_CHAN_WAVPLAYER, 1.0);      // SD Player Direct Right
    OutMixerRight.gain(AUDIO_OUT_CHAN_RAWPLAYER, 1.0);      // SD Player Direct Right
    OutMixerRight.gain(AUDIO_OUT_CHAN_BASICSYNTH, 1.0);     // Basic Synth Direct Right
    OutMixerRight.gain(AUDIO_OUT_CHAN_FMSYNTH, 1.0);        // FM Synth Direct Right
    OutMixerRight.gain(AUDIO_OUT_CHAN_LINEIN, 1.0);         // Line In Direct Right
    OutMixerRight.gain(AUDIO_OUT_CHAN_REVERB, 1.0);         // Reverb Out Right
    OutMixerRight.gain(AUDIO_OUT_CHAN_EFX, 1.0);            // Efx Out Right
    OutMixerRight.gain(AUDIO_OUT_CHAN_DELAY, 1.0);          // Delay Out Right

    // Plate Reverb Settings (TODO A250201-1: Add some of these to reverb screen)
    PlateReverb.bypass_set(false);
    PlateReverb.wet_level(1.0);
    PlateReverb.dry_level(0.0);

    // WAV Player Initial Default Settings
    setWavPlayerParam(MediaPlayWav_Status, SDWAVPLAY_STATUS_DEFAULT);
    setWavPlayerParam(MediaPlayWav_Level, SDWAVPLAY_LEVEL_DEFAULT);
    setWavPlayerParam(MediaPlayWav_DlySend, (float)SDWAVPLAY_DLY_SEND_DEFAULT / 100.0);
    setWavPlayerParam(MediaPlayWav_RvbSend, (float)SDWAVPLAY_RVB_SEND_DEFAULT / 100.0);
    setWavPlayerParam(MediaPlayWav_SoundBank, SOUND_BANK_DEFAULT);
    setWavPlayerParam(MediaPlayWav_NoteShift, NOTE_SHIFT_DEFAULT);
    setWavPlayerParam(MediaPlayWav_NoteRangeMin, NOTE_RANGE_MIN_PLYR);
    setWavPlayerParam(MediaPlayWav_NoteRangeMax, NOTE_RANGE_MAX_PLYR);
    setWavPlayerParam(MediaPlayWav_PanType, PanTypeDefault);
    setWavPlayerParam(MediaPlayWav_PanParameter, PanParamDefault);

    // RAW Player Initial Default Settings
    setRawPlayerParam(MediaPlayRaw_Status,  FLASHRAWPLAY_STATUS_DEFAULT);
    setRawPlayerParam(MediaPlayRaw_Level,   FLASHRAWPLAY_LEVEL_DEFAULT);
    setRawPlayerParam(MediaPlayRaw_DlySend, (float)FLASHRAWPLAY_DLY_SEND_DEFAULT / 100.0);
    setRawPlayerParam(MediaPlayRaw_RvbSend, (float)FLASHRAWPLAY_RVB_SEND_DEFAULT / 100.0);
    setRawPlayerParam(MediaPlayRaw_SoundBank, SOUND_BANK_DEFAULT);
    setRawPlayerParam(MediaPlayRaw_NoteShift, NOTE_SHIFT_DEFAULT);
    setRawPlayerParam(MediaPlayRaw_NoteRangeMin, NOTE_RANGE_MIN_PLYR);
    setRawPlayerParam(MediaPlayRaw_NoteRangeMax, NOTE_RANGE_MAX_PLYR);
    setRawPlayerParam(MediaPlayRaw_PanType, PanTypeDefault);
    setRawPlayerParam(MediaPlayRaw_PanParameter, PanParamDefault);

    // Basic Synth Initial Default Settings
    setBasicSynthParam(BasicSynth_Status,   BASICSYNTH_STATUS_DEFAULT);
    setBasicSynthParam(BasicSynth_Level,    BASICSYNTH_LEVEL_DEFAULT);
    setBasicSynthParam(BasicSynth_DlySend, (float)BASICSYNTH_DLY_SEND_DEFAULT / 100.0);
    setBasicSynthParam(BasicSynth_RvbSend, (float)BASICSYNTH_RVB_SEND_DEFAULT / 100.0);
    setBasicSynthParam(BasicSynth_Type,    (float)BASICSYNTH_TYPE_DEFAULT);
    setBasicSynthParam(BasicSynth_NoteShift, NOTE_SHIFT_DEFAULT);
    setBasicSynthParam(BasicSynth_NoteRangeMin, NOTE_RANGE_MIN_SYNTH);
    setBasicSynthParam(BasicSynth_NoteRangeMax, NOTE_RANGE_MAX_SYNTH);
    setBasicSynthParam(BasicSynth_Attack,  (float)BASICSYNTH_ATTACK_DEFAULT);
    setBasicSynthParam(BasicSynth_Decay,   (float)BASICSYNTH_DECAY_DEFAULT);
    setBasicSynthParam(BasicSynth_Sustain, (float)BASICSYNTH_SUSTAIN_DEFAULT);
    setBasicSynthParam(BasicSynth_Release, (float)BASICSYNTH_RELEASE_DEFAULT);
    setBasicSynthParam(BasicSynth_ModFreq, (float)BASICSYNTH_MOD_FREQ_DEFAULT);
    setBasicSynthParam(BasicSynth_ModAmt,  (float)BASICSYNTH_MOD_AMT_DEFAULT / 100.0);
    setBasicSynthParam(BasicSynth_PanType,      PanTypeDefault);
    setBasicSynthParam(BasicSynth_PanParameter, PanParamDefault);

    // FM Synth Initial Default Settings
    setFmSynthParam(FmSynth_Status,         FMSYNTH_STATUS_DEFAULT);
    setFmSynthParam(FmSynth_Level,          FMSYNTH_LEVEL_DEFAULT);
    setFmSynthParam(FmSynth_DlySend,        (float)FMSYNTH_DLY_SEND_DEFAULT / 100.0);
    setFmSynthParam(FmSynth_RvbSend,        (float)FMSYNTH_RVB_SEND_DEFAULT / 100.0);
    // Initial voice number will be selected in Presets.ino
//    setFmSynthParam(FmSynth_PresetNum,      FMSYNTH_PRESET_NUM_DEFAULT);
    setFmSynthParam(FmSynth_NoteShift,      NOTE_SHIFT_DEFAULT);
    setFmSynthParam(FmSynth_NoteRangeMin,   NOTE_RANGE_MIN_SYNTH);
    setFmSynthParam(FmSynth_NoteRangeMax,   NOTE_RANGE_MAX_SYNTH);
    setFmSynthParam(FmSynth_PitchBendRange, FMSYNTH_PITCH_BEND_DEFAULT);
    setFmSynthParam(FmSynth_PanType,        PanTypeDefault);
    setFmSynthParam(FmSynth_PanParameter,   PanParamDefault);

    // Efx Initial Default Settings
    setRvbParam(Reverb_RoomSize, (float)REVERB_ROOMSIZE_DEFAULT / 100.0);
    setRvbParam(Reverb_Diffusion, (float)REVERB_DIFFUSION_DEFAULT / 100.0);
    setRvbParam(Reverb_Level, (float)REVERB_LEVEL_DEFAULT / 2.0);

    setStereoDelayParam(StereoDelay_DlyL_DlyTime,     DELAY_TIME_DEFAULT);
    setStereoDelayParam(StereoDelay_DlyL_Gain,        (float)DELAY_LEVEL_DEFAULT / 2.0);
    setStereoDelayParam(StereoDelay_DlyL_RvbSend,     DELAY_RVBSEND_DEFAULT);
    setStereoDelayParam(StereoDelay_DlyL_Efx3Send,    DELAY_EFX3SEND_DEFAULT);
    setStereoDelayParam(StereoDelay_DlyL_FB_DlyAmt,   DELAY_FEEDBACK_MIN);
    setStereoDelayParam(StereoDelay_DlyL_xFB_DlyTime, DELAY_TIME_MIN);
    setStereoDelayParam(StereoDelay_DlyL_xFB_DlyAmt,  DELAY_FEEDBACK_MIN);

    setStereoDelayParam(StereoDelay_DlyR_DlyTime,     DELAY_TIME_DEFAULT);
    setStereoDelayParam(StereoDelay_DlyR_Gain,        (float)DELAY_LEVEL_DEFAULT / 2.0);
    setStereoDelayParam(StereoDelay_DlyR_RvbSend,     DELAY_RVBSEND_DEFAULT);
    setStereoDelayParam(StereoDelay_DlyR_Efx3Send,    DELAY_EFX3SEND_DEFAULT);
    setStereoDelayParam(StereoDelay_DlyR_FB_DlyAmt,   DELAY_FEEDBACK_MIN);
    setStereoDelayParam(StereoDelay_DlyR_xFB_DlyTime, DELAY_TIME_MIN);
    setStereoDelayParam(StereoDelay_DlyR_xFB_DlyAmt,  DELAY_FEEDBACK_MIN);

    // Audio IO Initial Default Settings
    setInputOutputParam(AudioIo_LineIn_Status, AUDIOIO_LINEIN_STATUS_DEFAULT);
    setInputOutputParam(AudioIo_LineIn_Level, AUDIOIO_LINEIN_LVL_DEFAULT);
    setInputOutputParam(AudioIo_LineOut_Level, AUDIOIO_LINEOUT_LVL_DEFAULT);
    setInputOutputParam(AudioIo_Phone_Volume, AUDIOIO_PHONEVOLUME_DEFAULT / 100.0);
    setInputOutputParam(AudioIo_InputSel, AUDIO_INPUT_LINEIN);
    setInputOutputParam(AudioIo_MicGain, 0.0);
    setInputOutputParam(AudioIo_LineIn_DlySend, AUDIOIO_LINEIN_DLY_SEND_DEFAULT);
    setInputOutputParam(AudioIo_LineIn_RvbSend, AUDIOIO_LINEIN_RVB_SEND_DEFAULT);
}

/***************************
* Device Parameter Updates *
****************************
*
* Generally available in application, but primarily invoked from parameter-setting
* screens and/or MIDI and other user input devices.
*
*/

// setMediaPlayerSoundFiles()
//
void setMediaPlayerSoundFiles() {

//    Serial.printf("AudioDevices::setMediaPlayerSoundFiles() sdWavFileNames %x sdWavFileCount %d\n", AudioPlatform::systemProfile.sdWavFileNames, AudioPlatform::systemProfile.sdWavFileCount);
//    Serial.printf("AudioDevices::setMediaPlayerSoundFiles() sfRawFileNames %x sfRawFileCount %d\n", AudioPlatform::systemProfile.sfRawFileNames, AudioPlatform::systemProfile.sfRawFileCount);

    WavPlayer.setSoundFiles(AudioPlatform::systemProfile.sdWavFileNames, AudioPlatform::systemProfile.sdWavFileCount);
    RawPlayer.setSoundFiles(AudioPlatform::systemProfile.sfRawFileNames, AudioPlatform::systemProfile.sfRawFileCount);
}

// setWavPlayerParam()
//
void setWavPlayerParam(uint16_t paramId, float paramVal) {

//    Serial.printf("AudioDevices::setWavPlayerParam paramId: %d paramVal: %f\n", paramId, paramVal);

    switch (paramId) {
        case MediaPlayWav_Status:
            wavPlayerStatus.enabled = ((int)paramVal == 1);
            WavPlayer.setEnabled((int)paramVal == 1);
            // Workaround for B250308-1
            Teensy41_Util::setTeensySdAudioActive((int)paramVal == 1);
            break;

        case MediaPlayWav_Level: {
            float sdPlayGain = UtilFns::dbToGain(paramVal);
//            Serial.printf("AudioDevices::setWavPlayerParam MediaPlayWav_Level paramVal(dB): %f gainVal %f\n", paramVal, sdPlayGain);
            WavPlayer.setGain(sdPlayGain);
            }
            break;

        case MediaPlayWav_DlySend:
            DelayInMixerLeft.gain(DELAYIN_CHAN_WAVPLAYER, paramVal);
            DelayInMixerRight.gain(DELAYIN_CHAN_WAVPLAYER, paramVal);
            break;

        case MediaPlayWav_RvbSend:
            ReverbInMixerLeft.gain(REVERBIN_CHAN_WAVPLAYER, paramVal);
            ReverbInMixerRight.gain(REVERBIN_CHAN_WAVPLAYER, paramVal);
            break;

        case MediaPlayWav_SoundBank:
            WavPlayer.setSoundBank((uint8_t) paramVal);
            break;

        case MediaPlayWav_NoteShift:
//            Serial.printf("AudioDevices::setWavPlayerParam MediaPlayWav_NoteShift paramVal %f (int) paramVal %d\n", paramVal, UtilFns::floatToInt(paramVal));
            WavPlayer.setNoteShift(UtilFns::floatToInt(paramVal));
            break;

        case MediaPlayWav_NoteRangeMin:
//            Serial.printf("AudioDevices::setWavPlayerParam MediaPlayWav_NoteRangeMin %f\n", paramVal);
            WavPlayer.setNoteRangeStart((MidiNoteNumber) paramVal);
            break;

        case MediaPlayWav_NoteRangeMax:
//            Serial.printf("AudioDevices::setWavPlayerParam MediaPlayWav_NoteRangeMax %f\n", paramVal);
            WavPlayer.setNoteRangeEnd((MidiNoteNumber) paramVal);
            break;

        // Call the following 2 symmetrically so as to make no assumptions
        // as to the order in whicn the type and parameter may be sent
        case MediaPlayWav_PanType:
            WavPlayer.setPanType((MidiPanType) paramVal, Presets::getWavPlayerParam(MediaPlayWav_PanParameter));
            break;

        case MediaPlayWav_PanParameter:
            WavPlayer.setPanType((MidiPanType) Presets::getWavPlayerParam(MediaPlayWav_PanType), paramVal);
            break;

        default:
            break;
    }
}

// setRawPlayerParam()
//
void setRawPlayerParam(uint16_t paramId, float paramVal) {

//    Serial.printf("AudioDevices::setRawPlayerParam paramId: %d paramVal: %f\n", paramId, paramVal);

    switch (paramId) {
        case MediaPlayRaw_Status:
            rawPlayerStatus.enabled = ((int)paramVal == 1);
            RawPlayer.setEnabled((int)paramVal == 1);
            break;

        case MediaPlayRaw_Level: {
            float sdPlayGain = UtilFns::dbToGain(paramVal);
            RawPlayer.setGain(sdPlayGain);
            }
            break;

        case MediaPlayRaw_DlySend:
            DelayInMixerLeft.gain(DELAYIN_CHAN_RAWPLAYER, paramVal);
            DelayInMixerRight.gain(DELAYIN_CHAN_RAWPLAYER, paramVal);
            break;

        case MediaPlayRaw_RvbSend:
            ReverbInMixerLeft.gain(REVERBIN_CHAN_RAWPLAYER, paramVal);
            ReverbInMixerRight.gain(REVERBIN_CHAN_RAWPLAYER, paramVal);
            break;

        case MediaPlayRaw_SoundBank:
            RawPlayer.setSoundBank((uint8_t) paramVal);
            break;

        case MediaPlayRaw_NoteShift:
//            Serial.printf("AudioDevices::setRawPlayerParam MediaPlayRaw_NoteShift paramVal %f (int) paramVal %d\n", paramVal, UtilFns::floatToInt(paramVal));
            RawPlayer.setNoteShift(UtilFns::floatToInt(paramVal));
            break;

        case MediaPlayRaw_NoteRangeMin:
            RawPlayer.setNoteRangeStart((MidiNoteNumber) paramVal);
            break;

        case MediaPlayRaw_NoteRangeMax:
            RawPlayer.setNoteRangeEnd((MidiNoteNumber) paramVal);
            break;

        // Call the following 2 symmetrically so as to make no assumptions
        // as to the order in whicn the type and parameter may be sent
        case MediaPlayRaw_PanType:
            RawPlayer.setPanType((MidiPanType) paramVal, Presets::getRawPlayerParam(MediaPlayRaw_PanParameter));
            break;

        case MediaPlayRaw_PanParameter:
            RawPlayer.setPanType((MidiPanType) Presets::getRawPlayerParam(MediaPlayRaw_PanType), paramVal);
            break;

        default:
            break;
    }
}

// setBasicSynthParam()
//
// TODO: Use synth MIDI channel
//
void setBasicSynthParam(uint16_t paramId, float paramVal) {

//    Serial.printf("AudioDevices::setBasicSynthParam paramId: %d paramVal: %f\n", paramId, paramVal);

    switch (paramId) {
        case BasicSynth_Status:
            coreSynthStatus.enabled = ((int)paramVal == 1);
            CoreSynth.setEnabled((int)paramVal == 1);
            break;

        case BasicSynth_Level:
            CoreSynth.setGain(UtilFns::dbToGain(paramVal));
            break;

        case BasicSynth_DlySend:
            DelayInMixerLeft.gain(DELAYIN_CHAN_BASICSYNTH, paramVal);
            DelayInMixerRight.gain(DELAYIN_CHAN_BASICSYNTH, paramVal);
            break;

        case BasicSynth_RvbSend:
            ReverbInMixerLeft.gain(REVERBIN_CHAN_BASICSYNTH, paramVal);
            ReverbInMixerRight.gain(REVERBIN_CHAN_BASICSYNTH, paramVal);
            break;

        case BasicSynth_Type:
            CoreSynth.setSynthType((SynthType) paramVal);
            break;

        case BasicSynth_NoteShift:
            CoreSynth.setNoteShift(UtilFns::floatToInt(paramVal));
            break;

        case BasicSynth_NoteRangeMin:
            CoreSynth.setNoteRangeStart((MidiNoteNumber) paramVal);
            break;

        case BasicSynth_NoteRangeMax:
            CoreSynth.setNoteRangeEnd((MidiNoteNumber) paramVal);
            break;

        case BasicSynth_Attack:
            CoreSynth.setAttack(paramVal);
            break;

        case BasicSynth_Decay:
            CoreSynth.setDecay(paramVal);
            break;

        case BasicSynth_Sustain:
            CoreSynth.setSustain(paramVal);
            break;

        case BasicSynth_Release:
            CoreSynth.setRelease(paramVal);
            break;

        case BasicSynth_ModFreq:
            CoreSynth.setModulatorFrequency(paramVal);
            break;

        case BasicSynth_ModAmt:
            CoreSynth.setModulatorAmplitude(paramVal);
            break;

        // Call the following 2 symmetrically so as to make no assumptions
        // as to the order in whicn the type and parameter may be sent
        case BasicSynth_PanType:
            CoreSynth.setPanType((MidiPanType) paramVal, Presets::getBasicSynthParam(BasicSynth_PanParameter));
            break;

        case BasicSynth_PanParameter:
            CoreSynth.setPanType((MidiPanType) Presets::getBasicSynthParam(BasicSynth_PanType), paramVal);
            break;

        default:
            break;
    }
}

// setFmSynthParam()
//
void setFmSynthParam(uint16_t paramId, float paramVal) {

//    Serial.printf("AudioDevices::setFmSynthParam paramId: %d paramVal: %f\n", paramId, paramVal);

    switch (paramId) {
        case FmSynth_Status:
            fmSynthStatus.enabled = ((int)paramVal == 1);
            FrqModSynth.setEnabled((int)paramVal == 1);
            break;

        case FmSynth_Level: {
            float gainVal = UtilFns::dbToGain(paramVal);
            OutMixerLeft.gain(AUDIO_OUT_CHAN_FMSYNTH, gainVal);
            OutMixerRight.gain(AUDIO_OUT_CHAN_FMSYNTH, gainVal);
            }
            break;

        case FmSynth_DlySend:
            DelayInMixerLeft.gain(DELAYIN_CHAN_FMSYNTH, paramVal);
            DelayInMixerRight.gain(DELAYIN_CHAN_FMSYNTH, paramVal);
            break;

        case FmSynth_RvbSend:
            ReverbInMixerLeft.gain(REVERBIN_CHAN_FMSYNTH, paramVal);
            ReverbInMixerRight.gain(REVERBIN_CHAN_FMSYNTH, paramVal);
            break;

        case FmSynth_PresetNum:
            // Can synth keep up with encoder changes? Yes!
            // FFS: But why is rounding necessary when applying a parameter
            // that has integer values? Shouldn't int values _always_ convert
            // to float and back without issues?
//            Serial.printf("AudioDevices::setFmSynthParam paramId: FmSynth_PresetNum paramVal: %f\n", paramVal);
            FrqModSynth.selectVoicePreset((uint16_t)(paramVal + 0.5) + 1);    // Round to nearest whole
            break;

        case FmSynth_NoteShift:
            FrqModSynth.setNoteShift(UtilFns::floatToInt(paramVal));
            break;

        case FmSynth_NoteRangeMin:
            FrqModSynth.setNoteRangeStart((MidiNoteNumber) paramVal);
            break;

        case FmSynth_NoteRangeMax:
            FrqModSynth.setNoteRangeEnd((MidiNoteNumber) paramVal);
            break;

        case FmSynth_PitchBendRange:
//            Serial.printf("AudioDevices::setFmSynthParam setPitchbendRange: %f\n", paramVal);
            FrqModSynth.setPitchbendRange((uint8_t) paramVal);
            break;

        case FmSynth_PanType:
            FrqModSynth.setPanType((MidiPanType) paramVal, Presets::getFmSynthParam(FmSynth_PanParameter));
            break;

        case FmSynth_PanParameter:
            FrqModSynth.setPanType((MidiPanType) Presets::getFmSynthParam(FmSynth_PanType), paramVal);
            break;

       default:
            break;
    }
}

// setMidiParam
//
void setMidiParam(uint16_t paramId, float paramVal) {

//    Serial.printf("AudioDevices::setMidiParam paramId: %d paramVal: %f\n", paramId, paramVal);

    switch (paramId) {

        default:
            break;
    }

}

// setRvbParam()
//
void setRvbParam(uint16_t paramId, float paramVal) {

//    Serial.printf("AudioDevices::setRvbParam paramId: %d paramVal: %f\n", paramId, paramVal);

    switch (paramId) {

        case Reverb_Level: {
            float reverbGain = UtilFns::dbToGain(paramVal + REVERB_DB_ADJUST_OFFSET);
            OutMixerLeft.gain(AUDIO_OUT_CHAN_REVERB, reverbGain / 2.0);
            OutMixerRight.gain(AUDIO_OUT_CHAN_REVERB, reverbGain / 2.0);
            }
            break;

        case Reverb_RoomSize:
            PlateReverb.size(paramVal); // max reverb length
            break;

        case Reverb_Diffusion:
            // 1.0 is the default setting; lower it
            // to create more "echoey" reverb.
            PlateReverb.diffusion(paramVal);
            break;

        case Reverb_PchShft:
            PlateReverb.pitchSemitones(paramVal);
            break;

        case Reverb_PchShftAmt:
            PlateReverb.pitchMix(paramVal);
            break;

        case Reverb_ShimPchShft:
            PlateReverb.shimmerPitchSemitones(paramVal);
            break;

        case Reverb_ShimShftRatio:
            PlateReverb.shimmerPitch(paramVal);
            break;

        case Reverb_ShimAmt:
            PlateReverb.shimmer(paramVal);
            break;

        default:
            break;
    }
}

// setEfxParam()
//
void setEfxParam(uint16_t paramId, float paramVal) {

//    Serial.printf("AudioDevices::setEfxParam paramId: %d paramVal: %f\n", paramId, paramVal);

    switch (paramId) {

        case FutureEfx_Level:
            FutureAudioEfx.setGain(paramVal);
            break;

        case FutureEfx_Param2:
            FutureAudioEfx.setParam2(paramVal);
            break;

        case FutureEfx_Param3:
            FutureAudioEfx.setParam3(paramVal);
            break;

        case FutureEfx_Param4:
            FutureAudioEfx.setParam4(paramVal);
            break;

        default:
            break;
    }
}

// setStereoDelayParam
//
void setStereoDelayParam(uint16_t paramId, float paramVal) {

//    Serial.printf("AudioDevices::setStereoDelayParam paramId: %d paramVal: %f\n", paramId, paramVal);

    switch (paramId) {

        // Stereo Delay Left Channel Settings
        //
        case StereoDelay_DlyL_DlyTime:
            StereoDelay.setDelayLeftOutTime(paramVal);
            break;

        case StereoDelay_DlyL_Gain: {
            // Delay level in dB. Convert to gain value.
            float delayGain = UtilFns::dbToGain(paramVal);
            StereoDelay.setDelayLeftOutGain(delayGain / 2.0);
            }
            break;

        case StereoDelay_DlyL_RvbSend:
            ReverbInMixerLeft.gain(REVERBIN_CHAN_DELAY, paramVal);    // Delay to Reverb
            break;

        case StereoDelay_DlyL_Efx3Send:
            break;

        case StereoDelay_DlyL_FB_DlyAmt:
            StereoDelay.setDelayLeftFbAmt(paramVal);
            break;

        case StereoDelay_DlyL_xFB_DlyTime:
            StereoDelay.setDelayLeftxFbTime(paramVal);
            break;

        case StereoDelay_DlyL_xFB_DlyAmt:
            StereoDelay.setDelayLeftxFbAmt(paramVal);
            break;

        // Stereo Delay Right Channel Settings
        //
        case StereoDelay_DlyR_DlyTime:
            StereoDelay.setDelayRightOutTime(paramVal);
            break;

        case StereoDelay_DlyR_Gain: {
            // Delay level in dB. Convert to gain value.
            float delayGain = UtilFns::dbToGain(paramVal);
            StereoDelay.setDelayRightOutGain(delayGain / 2.0);
            }
            break;

        case StereoDelay_DlyR_RvbSend:
            ReverbInMixerRight.gain(REVERBIN_CHAN_DELAY, paramVal);   // Delay to Reverb
            break;

        case StereoDelay_DlyR_Efx3Send:
            break;

        case StereoDelay_DlyR_FB_DlyAmt:
            StereoDelay.setDelayRightFbAmt(paramVal);
            break;

        case StereoDelay_DlyR_xFB_DlyTime:
            StereoDelay.setDelayRightxFbTime(paramVal);
            break;

        case StereoDelay_DlyR_xFB_DlyAmt:
            StereoDelay.setDelayRightxFbAmt(paramVal);
            break;
    }
}

// setInputOutputParam()
//
void setInputOutputParam(uint16_t paramId, float paramVal) {

//    Serial.printf("AudioDevices::setInputOutputParam paramId: %d paramVal: %f\n", paramId, paramVal);

    switch (paramId) {
        case AudioIo_LineIn_Status:
            AudioLineIn.setEnabled(((uint8_t)paramVal) == 1);
            break;

        case AudioIo_LineIn_Level:
            // [0.0,15.0] just pass through as uint.
            AudioCodec.lineInLevel((uint8_t)paramVal);
            break;

        case AudioIo_LineOut_Level:
            // There's some weirdness to the need to adjust the argument to lineOutLevel() from a paramVal of [0-18],
            // an index representing the lowest output level to the highest in an array of string values used for
            // display and editing the output level, arranged from low to high (Screens.ino audioIoLineOutLevelParams):
            //
            //      {"1.16", "1.22", "1.29", "1.37", "1.44", "1.53", "1.62", "1.71", "1.80", "1.91",
            //      "2.02", "2.14", "2.26", "2.39", "2.53", "2.67", "2.83", "2.98", "3.16"}
            //
            // and the values required in the Teensy Audio Library object AudioControlSGTL5000 function lineOutLevel()
            // which requires non-0-based arguments (also reversed from low to high levels) for the same values:
            //
            //      lineOutLevel(both);
            //
            //      Adjust the line level output voltage range. The following settings are possible:
            //
            //      13: 3.16 Volts p-p
            //      14: 2.98 Volts p-p
            //      15: 2.83 Volts p-p
            //      16: 2.67 Volts p-p
            //      17: 2.53 Volts p-p
            //      18: 2.39 Volts p-p
            //      19: 2.26 Volts p-p
            //      20: 2.14 Volts p-p
            //      21: 2.02 Volts p-p
            //      22: 1.91 Volts p-p
            //      23: 1.80 Volts p-p
            //      24: 1.71 Volts p-p
            //      25: 1.62 Volts p-p
            //      26: 1.53 Volts p-p
            //      27: 1.44 Volts p-p
            //      28: 1.37 Volts p-p
            //      29: 1.29 Volts p-p  (default)
            //      30: 1.22 Volts p-p
            //      31: 1.16 Volts p-p
            //
            // such that, to achieve output gains from low to high, we must index from 31 to 0, rather than 0 to 31.
            // It is important to maintain the [0,31] range as the reference parameter value (stored in presets and
            // used for display of values in the array), so we make the adjustment to the range expected by lineOutLevel()
            // here in the final output stage.
            //
            AudioCodec.lineOutLevel(31 - (uint8_t)paramVal);
            break;

        case AudioIo_Phone_Volume:
            AudioCodec.volume(paramVal);
            break;

        case AudioIo_LineIn_RvbSend:
            ReverbInMixerLeft.gain(REVERBIN_CHAN_LINEIN, paramVal);
            ReverbInMixerRight.gain(REVERBIN_CHAN_LINEIN, paramVal);
            break;

        case AudioIo_LineIn_DlySend:
            DelayInMixerLeft.gain(DELAYIN_CHAN_LINEIN, paramVal);
            DelayInMixerRight.gain(DELAYIN_CHAN_LINEIN, paramVal);
            break;

        case AudioIo_InputSel:
            // [0.0,1.0] Line/Mic pass through as uint.
            AudioCodec.inputSelect((uint8_t)paramVal);
            break;

        case AudioIo_MicGain: {
            // Param is [0.0, 63.0] dB to conform SGTL5000 0-63dB
            AudioCodec.micGain((uint8_t)paramVal);
            }
            break;

        default:
            break;
    }
}

/************************
* Player Device Control *
*************************
*
* Handle commands from MIDI and/or other user input devices that trigger sound output
* from media player(s) or synthesizers.
*
* Note that values received in these functions send the values (often transformed in
* some manner or another) _directly_ to the audio devices, bypassing the "normal"
* method where data values are sent to the Presets Manager, which stores the values
* and then relays them to the appropriate devices in AudioDevices.ino, Screns.ino,
* and InputDevices.ino. This means that these realtime settings from MIDI controls
* will not be saved in the preset data for the target devices.
*
*/

// setPrestBanksPtr()
//
void setPrestBanksPtr(uint8_t* banksAddress) {

    FrqModSynth.setPrestBanksPtr(banksAddress);
}

// setFmSynthVoicePreset()
//
void setFmSynthVoicePreset(uint16_t voicePresetNum) {

//    Serial.printf("AudioDevices::setFmSynthVoicePreset: voicePresetNum  %d\n", voicePresetNum);

    FrqModSynth.selectVoicePreset(voicePresetNum);
}

// getFmSynthVoicePresetName()
//
const char* getFmSynthVoicePresetName(uint16_t voicePresetNum) {

    return FrqModSynth.getVoicePresetName(voicePresetNum);
}

// handleNoteOn()
//
void handleNoteOn(uint8_t note, uint8_t velocity) {

//    Serial.printf("AudioDevices::handleNoteOn: note  %d, velocity %d position %d\n", note, velocity, position);

    if (fmSynthStatus.enabled) {
        FrqModSynth.noteOn(note, velocity);
    }

    if (coreSynthStatus.enabled) {
        CoreSynth.noteOn(note, velocity);
    }

    if (wavPlayerStatus.enabled) {
        wavFileNote(note, velocity);
    }

    if (rawPlayerStatus.enabled) {
        rawFileNote(note, velocity);
    }
}

// handleNoteOff()
//
void handleNoteOff(uint8_t note) {

//    Serial.printf("AudioDevices::handleNoteOff: note  %d\n", note);

    if (fmSynthStatus.enabled && !fmSynthStatus.ignoreNoteOff) {
        FrqModSynth.noteOff(note);
    }

    if (coreSynthStatus.enabled && !coreSynthStatus.ignoreNoteOff) {
        CoreSynth.noteOff(note);
    }

    if (wavPlayerStatus.enabled && !wavPlayerStatus.ignoreNoteOff) {
        WavPlayer.noteOff(note);
    }

    if (rawPlayerStatus.enabled && !rawPlayerStatus.ignoreNoteOff) {
        RawPlayer.noteOff(note);
    }
}

// handleIgnoreNoteOff()
//
void handleIgnoreNoteOff(bool ignore) {

    // BUG/FFS: Temporary workaround for B250308-2 until resolved
    fmSynthStatus.ignoreNoteOff   = false;

    coreSynthStatus.ignoreNoteOff = ignore;
    wavPlayerStatus.ignoreNoteOff = ignore;
    rawPlayerStatus.ignoreNoteOff = ignore;

}

// handlePitchBend()
//
void handlePitchBend(int16_t bend) {

//    Serial.printf("AudioDevices::handlePitchBend: bend  %d\n", bend);

    if (fmSynthStatus.enabled) {
        FrqModSynth.pitchBend(bend);
    }

    if (coreSynthStatus.enabled) {
    }

    if (wavPlayerStatus.enabled) {
    }

    if (rawPlayerStatus.enabled) {
    }
}

// handleSustainPedal()
//
void handleSustainPedal(bool pedalDown) {

    if (fmSynthStatus.enabled) {
        FrqModSynth.sustainPedal(pedalDown);
    }

    if (coreSynthStatus.enabled) {
        CoreSynth.sustainPedal(pedalDown);
    }

    if (wavPlayerStatus.enabled) {
        WavPlayer.sustainPedal(pedalDown);
    }

    if (rawPlayerStatus.enabled) {
        RawPlayer.sustainPedal(pedalDown);
    }
}

// handlePanControl()
//
void handlePanControl(uint8_t value) {

    if (fmSynthStatus.enabled) {
        FrqModSynth.setPanPosition(value);
    }

    if (coreSynthStatus.enabled) {
        CoreSynth.setPanPosition(value);
    }

    if (wavPlayerStatus.enabled) {
        WavPlayer.setPanPosition(value);
    }

    if (rawPlayerStatus.enabled) {
        RawPlayer.setPanPosition(value);
    }
}

// handleAllNotesOff()
//
void handleAllNotesOff() {

//    Serial.printf("AudioDevices::handleAllNotesOff:\n");

#if 1

    FrqModSynth.allNotesOff();
    CoreSynth.allNotesOff();
    WavPlayer.allNotesOff();
    RawPlayer.allNotesOff();

#else

    if (fmSynthStatus.enabled) {
        FrqModSynth.allNotesOff();
    }

    if (coreSynthStatus.enabled) {
        CoreSynth.allNotesOff();
    }

    if (wavPlayerStatus.enabled) {
        WavPlayer.allNotesOff();
    }

    if (rawPlayerStatus.enabled) {
        RawPlayer.allNotesOff();
    }

#endif
}

// handleVolumeControl()
//
void handleVolumeControl(uint8_t value) {

    if (fmSynthStatus.enabled) {
        float dbValue = UtilFns::scale(value, MidiDataMin, MidiDataMax, GAIN_MIN_DB, GAIN_MAX_DB);
        float gainValue = UtilFns::dbToGain(dbValue);
//        Serial.printf("AudioDevices::handleVolumeControl: MIDI %d dbVal %f gainVal %f\n", value, dbValue, gainValue);
        FrqModSynth.setGain(gainValue);
    }

    if (coreSynthStatus.enabled) {
    }

    if (wavPlayerStatus.enabled) {
    }

    if (rawPlayerStatus.enabled) {
    }
}


// pollAudioDevices()
//
void pollAudioDevices() {

    if (coreSynthUpdateTimer >= coreSynthUpdateInterval) {
        CoreSynth.updateControls();
        coreSynthUpdateTimer = 0;
    }
}

/************************************************************************************
*                           L O C A L   F U N C T I O N S                           *
*************************************************************************************
*
* Functions visible only within this module
*
*/

// wavFileNote()
//
void wavFileNote(uint8_t note, uint8_t midiVelocity) {

    // First check for note off
    if (midiVelocity == 0) {
        WavPlayer.noteOff(note);
    } else {
        // Map velocity to a range of -30dB to 0dB
        float gainValue = UtilFns::scale(midiVelocity, 0, 127, 0.03125, 1.0);
        WavPlayer.noteOn(note, gainValue);
    }
}

// rawFileNote()
//
void rawFileNote(uint8_t note, uint8_t midiVelocity) {

    // First check for note off
    if (midiVelocity == 0) {
        RawPlayer.noteOff(note);
    } else {
        // Map velocity to a range of -30dB to 0dB
        float gainValue = UtilFns::scale(midiVelocity, 0, 127, 0.03125, 1.0);
        RawPlayer.noteOn(note, gainValue);
    }
}

/****************
* End Namespace *
*****************
*/
} // namespace AudioDevices
