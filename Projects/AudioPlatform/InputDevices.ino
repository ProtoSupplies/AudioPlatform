/************************************************************************************
*                                 InputDevices.ino                                  *
*                                                                                   *
*                       Copyright (c) 2025, Donald Swearingen                       *
*                                All Rights Reserved                                *
*************************************************************************************
*
* Description
*
*   InputDevices.ino contains the Arduino C-language functions associated with the
*   input device (encoders, buttons, MIDI, etc.) components of the main Arduino
*   application AudioPlatform.ino (also a .ino C-language space module).
*
*   Even though the code herein is standard C code, when placed in the same directory
*   as the main Arduino application (the .ino containing functions setup() and loop())
*   it must have the suffix ".ino" rather than ".c" in order to compile correctly in
*   the Arduino IDE, in conformance with the standards required by that environment
*   (a quirk/deficiency of the original Arduino scheme for user-friendliness).
*
* MIDI In/Out Selection
*
*   Notes on AudioPlatforn MIDI interfaces and device selection using compiler defines.
*   Could this be a run-time selection rather than compile-time?
*
* MIDI Controller Class
*
*   FFS: A MIDI controller base class with virtual functions implemented in derived classes
*   for individual controllers (MK3, LK25, ML3, etc.). (see A240716-1).
*
* RTC (Real Time Clock)
*
*   Initially (perhaps permanently?), RTC considered an input device. Initialization and polling
*   done here. Time display handled in Screens.ino.
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

#define USING_TEENSY_USB_MIDI   0       // MIDI IO on Teensy USB-C Port to Computer Connection (TODO Future. Not yet implemented.)
#define SHOW_MIDI_IN            0       // Enable/disable Serial display of MIDI inputs

/************************************************************************************
*                       E X T E R N A L   R E F E R E N C E S                       *
*************************************************************************************
*/

// Application Components
//
#include "InputDevices.h"

// User Arduino Libraries
//
#include "ArduinoEclipseDefs.h"
#include "MidiIO.h"
#include "MidiDefines.h"
#include "Teensy41_Util.h"

// TeensyDuino Libraries
//
#include <MIDI.h>
#include <TimeLib.h>
#include <ElapsedMillis.h>
#include <core_pins.h>
#include <SerialFlash.h>
#include <SPI.h>

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

/************************************************************************************
*               L O C A L   D A T A / T Y P E   D E F I N I T I O N S               *
*************************************************************************************
*
* Data types and variables visible only within this module
*
*/

// Current date/time string to pass time to screen for display.
// Defined as static in order to retain value between updates
//static char  dateTimeStr[DateTimeStringLength];

// State variables for Program Change shift
//
bool            progShift1 = false;
bool            progShift2 = false;

/***************
* MIDI Devices *
****************
*/

MidiIO          usbMidiIO;
                MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, dinMidiIO);

                bool            ignoreNoteOff   = false;
bool            midiInReceived  = false;

/******
* RTC *
*******
*/

elapsedMillis   midiRxUpdateTime;           // time since last update
const uint16_t  midiRxUpdateInterval = 100; // update period (ms)
//const uint16_t  midiRxUpdateInterval = 1000; // update period (ms)
elapsedMillis   rtcUpdateTime;              // time since last update
const uint16_t  rtcUpdateInterval = 500;    // update period 2x/second
time_t          lastEpochTime;              // seconds since 1/1/1970
time_t          currentEpochTime;

/************************************************************************************
*                        E X P O R T E D   F U N C T I O N S                        *
*************************************************************************************
*
* Exported functions referenced with namespace prefix "InputDevices::"
*
*/

/******************
* Begin Namespace *
*******************
*/
namespace InputDevices {

// Device polling
void        pollRotaryEncoders();
void        pollRtc();
void        pollMidiIn();

// MIDI IO
void        midiNoteOn(byte channel, byte note, byte velocity);
void        midiNoteOff(byte channel, byte note, byte velocity);
float       adsrRampFn(uint8_t ctrlVal);
void        midiCtrlChange(byte channel, byte control, byte value);
void        midiProgramChange(byte channel, byte value);
void        midiPitchBend(byte channel, int value);

// RTC (Real Time Clock)
time_t      getTeensy3Time();


// initInputDevices
//
void initInputDevices() {

    // MIDI
    usbMidiIO.begin();
    usbMidiIO.setHandleNoteOn(midiNoteOn);
    usbMidiIO.setHandleNoteOff(midiNoteOff);
    usbMidiIO.setHandleControlChange(midiCtrlChange);
    usbMidiIO.setHandlePitchBend(midiPitchBend);
    usbMidiIO.setHandleProgramChange(midiProgramChange);

    dinMidiIO.begin(MIDI_CHANNEL_OMNI);
    dinMidiIO.setHandleNoteOn(midiNoteOn);
    dinMidiIO.setHandleNoteOff(midiNoteOff);
    dinMidiIO.setHandleControlChange(midiCtrlChange);
    dinMidiIO.setHandlePitchBend(midiPitchBend);
    dinMidiIO.setHandleProgramChange(midiProgramChange);

    midiRxUpdateTime = 0;

    // RTC
    setSyncProvider(getTeensy3Time);    // Uuse Teensy 3.0's RTC to keep time
    rtcUpdateTime = 0;                  // time since last update
    lastEpochTime = now();
}

// pollInputDevices
//
void pollInputDevices() {

    // Encoders
    pollRotaryEncoders();

    // Real Time Clock
    pollRtc();

    // MIDI
    pollMidiIn();
}

/**************************************
* ParamButtonScreen Callback Handlers *
***************************************
*
* Handlers called from screen Presets (DataMgr) component when users make changes to
* ParamButtonScreen instances representing parameter values for various system variables
* and quantities (primarily, though not exclusively, audio devices).
*
* The screen sends the changed value first to the DataMgr (ex. Presets::setWavPlayerParam())
* which records the value and in turn relays the value to each of the main application
* components through the functions listed below.
*
*/

// setWavPlayerParam
//
// Callback from Presets component (application data manager/coordinator)
//
//
void setWavPlayerParam(uint16_t paramId, float paramVal) {

//    Serial.printf("InputDevices::setWavPlayerParam paramId: %d paramVal: %f\n", paramId, paramVal);

}

// setRawPlayerParam
//
// Callback from Presets component (application data manager/coordinator)
//
//
void setRawPlayerParam(uint16_t paramId, float paramVal) {

//    Serial.printf("InputDevices::setRawPlayerParam paramId: %d paramVal: %f\n", paramId, paramVal);

}

// setBasicSynthParam
//
// Callback from Presets component (application data manager/coordinator)
//
void setBasicSynthParam(uint16_t paramId, float paramVal) {

//    Serial.printf("InputDevices::setBasicSynthParam paramId: %d paramVal: %f\n", paramId, paramVal);

#if TEST_MIDI_CTRL_ASSIGN // Testing for MIDI control assignment. Will find a different way via mapping.
      // since basicSynthScreen is no longer visible here
    if (Screens::activeScreen == ScreenBasicSynth && paramId == BasicSynth_Type) {
        (Screens::getActiveScreenProfile())->scrnInstance.setBtnProperties(paramId, paramVal, true);
    }
#endif // TEST_MIDI_CTRL_ASSIGN
}

// setFmSynthParam
//
// Callback from Presets component (application data manager/coordinator)
//
void setFmSynthParam(uint16_t paramId, float paramVal) {

//    Serial.printf("InputDevices::setFmSynthParam paramId: %d paramVal: %f\n", paramId, paramVal);

}

// setMidiParam
//
// Callback from Presets component (application data manager/coordinator)
//
void setMidiParam(uint16_t paramId, float paramVal) {

//    Serial.printf("InputDevices::setMidiParam paramId: %d paramVal: %f\n", paramId, paramVal);

}

// setRvbParam
//
// Callback from Presets component (application data manager/coordinator)
//
void setRvbParam(uint16_t paramId, float paramVal) {

//    Serial.printf("InputDevices::setRvbParam paramId: %d paramVal: %f\n", paramId, paramVal);

}

// setEfxParam
//
// Callback from Presets component (application data manager/coordinator)
//
void setEfxParam(uint16_t paramId, float paramVal) {

//    Serial.printf("InputDevices::setEfxParam paramId: %d paramVal: %f\n", paramId, paramVal);

}

// setStereoDelayParam
//
void setStereoDelayParam(uint16_t paramId, float paramVal) {

//    Serial.printf("InputDevices::setStereoDelayParam paramId: %d paramVal: %f\n", paramId, paramVal);

}

// setInputOutputParam
//
// Callback from Presets component (application data manager/coordinator)
//
void setInputOutputParam(uint16_t paramId, float paramVal) {

//    Serial.printf("InputDevices::setInputOutputParam paramId: %d paramVal: %f\n", paramId, paramVal);

}

/************************************************************************************
*                           L O C A L   F U N C T I O N S                           *
*************************************************************************************
*
* Functions visible only within this module
*
*/

/******************
* Encoder Polling *
*******************
*/

// pollRotaryEncoders()
//
// Regularly poll the encoders. When their values change, the callbacks
// for value and/or increment changes, pollRotaryEncoders() and
// rotaryIncrChange(), will be invoked to handle the changes.
//
void pollRotaryEncoders() {

    for (int i = 0; i < ROTARY_COUNT; i++) {
        encoders[i]->pollEncoder();
    }
}

/*************
* RTC Update *
**************
*/

// pollRtc()
//
// On regular intervals, check for change in RTC time. If
// changed, inform Main Screen to update its time display.
//
void pollRtc() {

    // Update time expired
    if (rtcUpdateTime > rtcUpdateInterval) {

        // Get RTC epoch time and check for change
        currentEpochTime = now();
        if (currentEpochTime != lastEpochTime) {
            lastEpochTime = currentEpochTime;

            // Pass string to screens that display time
            Screens::updateDateTimeDisplay();

            rtcUpdateTime = 0;
        }
    }
}

/******************
* MIDI In Polling *
*******************
*/

// pollMidiIn()
//
void pollMidiIn() {

    // Update MIDI on every poll
    bool usbMidiInState;
    bool dinMidiInState;

    usbMidiInState = usbMidiIO.update();
    dinMidiInState = dinMidiIO.read();

    // Record reception of MIDI
    if (usbMidiInState || dinMidiInState) {
        midiInReceived = true;
    }

    // Check for MIDI reception at regular intervals
    if (midiRxUpdateTime > midiRxUpdateInterval) {

        // MIDI reception since last check?
        if (midiInReceived) {

            // Report reception of MIDI input
//            Serial.printf("InputDevices::pollMidiIn: RX\n");
            Screens::updateMidiRx();

            // Reset MIDI received flag
            midiInReceived = false;
        }

        // Reset uppdate period
        midiRxUpdateTime = 0;
    }
}

/*****************
* MIDI Callbacks *
******************
*
* TODO: MIDI channel currently being ignored. Modify MIDI scheme to check channel and respond appropriately.
*/
#if USING_USBHOST_MIDI
// When a USB device with multiple virtual cables is used,
// midi1.getCable() can be used to read which of the virtual
// MIDI cables received this message.
#endif // USING_USBHOST_MIDI

// Lk25NoteMap()
//
// FFS: A quick-and-dirty workaround for LaunchKey25's wacko mapping of pads to MIDI note numbers.
// Likely to be superseded by A240716-1 addition if/when implemented.
//
// Remaps factory arrangement:
//
//      -----------------------------------------
//      | 40 | 41 | 42 | 43 | 48 | 49 | 50 | 51 |
//      -----------------------------------------
//      | 36 | 37 | 38 | 39 | 44 | 45 | 46 | 47 |
//      -----------------------------------------
//
// To reasonable/normal arrangement:
//
//      -----------------------------------------
//      |  1 |  2 |  3 |  4 |  5 |  6 |  7 |  8 |
//      -----------------------------------------
//      |  9 | 10 | 11 | 12 | 13 | 14 | 15 | 16 |
//      -----------------------------------------
//

// Array is accessed by mapping noteIn [MIDI_PADS_MIN, MIDI_PADS_MAX] (36,51)
// to noteIn-MIDI_PADS_MIN (0,15) to access elements of the map array.
//
const uint8_t Lk25NoteToProgMap[16] = {8,  9, 10, 11, 0, 1, 2, 3, 12, 13, 14, 15, 4, 5, 6, 7};

uint8_t Lk25NoteMap(uint8_t noteIn) {

    if (noteIn < MIDI_PADS_MIN) {
        noteIn = MIDI_PADS_MIN;
    } else if (noteIn > MIDI_PADS_MAX) {
        noteIn = MIDI_PADS_MAX;
    }

    return Lk25NoteToProgMap[noteIn - MIDI_PADS_MIN];
}

// midiNoteOn()
//
void midiNoteOn(uint8_t channel, uint8_t note, uint8_t velocity) {

#if (USING_MINILAB3 || USING_LAUNCHKEY_25)
    // MiniLab3 and LaunchKey25 send pads as note numbers on MIDI
    // channel 10 which we are interpreting as program change for
    // assigned MIDI channel.
    if (channel == 10) {
#if USING_LAUNCHKEY_25
        note = Lk25NoteMap(note);
#endif // USING_LAUNCHKEY_25
        midiProgramChange(MIDI_CHANNEL, note);
        return;
    }
#endif // (USING_MINILAB3 || USING_LAUNCHKEY_25)

#if SHOW_MIDI_IN
    Serial.printf("Note On: ch = %d, note = %d, velocity = %d\n", channel, note, velocity);
#endif // SHOW_MIDI_IN

    if (velocity == 0) {
        midiNoteOff(channel, note, velocity);
    } else {
        AudioDevices::handleNoteOn(note, velocity);
    }
}

// midiNoteOff()
//
void midiNoteOff(uint8_t channel, uint8_t note, uint8_t velocity) {

#if (USING_MINILAB3 || USING_LAUNCHKEY_25)
    // MiniLab3 and LaunchKey25 send pads as note numbers on MIDI
    // channel 10 which we are interpreting as program change for
    // assigned MIDI channel.
    if (channel == 10) {
        return;
    }
#endif // (USING_MINILAB3 || USING_LAUNCHKEY_25)

#if SHOW_MIDI_IN
    Serial.printf("Note Off: ch = %d, note = %d, velocity = %d\n", channel, note, velocity);
#endif // SHOW_MIDI_IN

    AudioDevices::handleNoteOff(note);
//    if (!ignoreNoteOff) {
//        AudioDevices::handleNoteOff(note);
//    }
}

// ADSR Ramp Parameters
//
// Determined experimentally to smooth output of linear faders and avoid
// distributing values too closely in upper range. Produces outputs in range
// of app. 0.1 to 3.0 seconds.
//
const float AdsrDivisor     = 15.0;
const float AdsrRange       = 3000.0;
const float AdsrRangeOffest = 96.0;
const float AdsrBase        = 1.5;

// adsrRampFn()
//
// Apply exponential curve converting MIDI control value [0,127] to adsr time range (ms)
//

float adsrRampFn(uint8_t ctrlVal) {

#if 1 // Outputs 3 -3000

    float val = (AdsrRange + AdsrRangeOffest) * pow((double)AdsrBase, -((double)((127.0 - (float) ctrlVal) / AdsrDivisor)));
    val -= AdsrRangeOffest;

#else // Outputs 96-3000

    float val = AdsrRange * pow((double)AdsrBase, -((double)((127.0 - (float) ctrlVal) / AdsrDivisor)));

#endif // Outputs 0-3000 / // Outputs 0-3000

//    Serial.printf("InputDevices::adsrRampFn: %d = %f\n", ctrlVal, val);

    return val;
}

// midiCtrlChange()
//
void midiCtrlChange(uint8_t channel, uint8_t control, uint8_t value) {

#if SHOW_MIDI_IN
    Serial.printf("InputDevices::midiCtrlChange: ch = %d, control = %d, value = %d\n", channel, control, value);
#endif // SHOW_MIDI_IN

    // Controller mappings
    // TODO: Implement in GUI assignable mappings
    switch (control) {

        // Map mod wheel to pan mode
        case PAN_CTRL:
            AudioDevices::handlePanControl(value);
            break;

        // Silence all playing notes
        case ALLOFF_CTRL:
        case ALLOFF_CTRL_ALT:
            AudioDevices::handleAllNotesOff();
            // Note: Leave ignoreNoteOff alone. If pedal is down during and after
            // MidiAllNotesOff, still should be observed on new notes.
            break;

        // Ignore note off / release for playing and new voices
        case IGNORE_NOTEOFF_CTRL:
            ignoreNoteOff = (value > 63);
            AudioDevices::handleIgnoreNoteOff(ignoreNoteOff);
            break;

        // MK3 Toggle Switch (toggles between 0 and 127)
        case SUSTAIN_CTRL:
        case SUSTAIN_CTRL_ALT:
            AudioDevices::handleSustainPedal(value > 63); // TODO: Fix sustain issue in Dexed
            break;

        case VOLUME_CTRL:
            AudioDevices::handleVolumeControl(value);
            break;

        // Synthesizer-Specific
        case SYNTH_MODFREQ_CTRL:
            // TODO: Scale MIDI value to output more appropriate t0 0-22kHz target
            Presets::setBasicSynthParam(BasicSynth_ModFreq, UtilFns::scale(value, 0, 127, (float)BASICSYNTH_MOD_FREQ_MIN, BASICSYNTH_MOD_FREQ_MAX));
            break;
        case SYNTH_MODAMT_CTRL:
            Presets::setBasicSynthParam(BasicSynth_ModAmt, UtilFns::scale(value, 0, 127, 0.0, 1.0));
            break;
        case SYNTH_ATTACK_CTRL:
            Presets::setBasicSynthParam(BasicSynth_Attack, adsrRampFn(value));
            break;
        case SYNTH_DECAY_CTRL:
            Presets::setBasicSynthParam(BasicSynth_Decay, adsrRampFn(value));
            break;
        case SYNTH_SUSTAIN_CTRL:
            Presets::setBasicSynthParam(BasicSynth_Sustain, UtilFns::scale(value, 0, 127, (float)BASICSYNTH_SUSTAIN_MIN / 100.0, (float)BASICSYNTH_SUSTAIN_MAX / 100.0));
            break;
        case SYNTH_RELEASE_CTRL:
            Presets::setBasicSynthParam(BasicSynth_Release, adsrRampFn(value));
            break;

        // Program Change shift buttons
        case MIDI_PROG_SHIFT1_CTRL:
            progShift1 = (value != 0);
            break;

        case MIDI_PROG_SHIFT2_CTRL:
            progShift2 = (value != 0);
            break;

        default:
            break;
    }

#if TEST_MIDI_CTRL_ASSIGN // Testing for MIDI control assignment. Will find a different way via mapping.
    // Test setting param in active window
    Serial.printf("Control Change: activeScreen = %d\n", Screens::activeScreen);
    InputDevices::setBasicSynthParam(BasicSynth_Type, (float) control);
#endif // TEST_MIDI_CTRL_ASSIGN
}

// midiProgramChange()
//
void midiProgramChange(byte channel, byte value) {

#if SHOW_MIDI_IN
    Serial.printf("Program Change: ch = %d, value = %d\n", channel, value);
#endif // SHOW_MIDI_IN

    // Adjust program # range by 16 for each
    // combination of shift buttons pressed
    uint16_t progShiftOffset = 0;
    if (progShift1 & !progShift2) {
        progShiftOffset = 16;
    } else if (!progShift1 & progShift2) {
        progShiftOffset = 32;
    } else if (progShift1 & progShift2) {
        progShiftOffset = 48;
    }
    value += progShiftOffset;

#if SHOW_MIDI_IN
    if (progShiftOffset > 0) {
        Serial.printf("Program Change: ch = %d, value (shifted) = %d\n", channel, value);
    }
#endif // SHOW_MIDI_IN

    // Select preset
    if ((value >= MIDI_PROG_MIN) && (value <= MIDI_PROG_MAX)) {
        Presets::selectPreset(value - MIDI_PROG_MIN);
    }
}

// midiPitchBend()
//
// TODO: Implement in Media Player and BasicSynth
//
void midiPitchBend(byte channel, int value) {

#if SHOW_MIDI_IN
    Serial.printf("Pitch Bend: ch = %d, value = %d\n", channel, value);
#endif // SHOW_MIDI_IN

    AudioDevices::handlePitchBend(value);
}

/***************
* RTC Handling *
****************
*/

// getTeensy3Time()
//
time_t getTeensy3Time() {

    return Teensy3Clock.get();
}



/****************
* End Namespace *
*****************
*/
} // namespace InputDevices



