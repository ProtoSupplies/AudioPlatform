/************************************************************************************
*                                 InputDevices.h                                    *
*                                                                                   *
*                       Copyright (c) 2025, Donald Swearingen                       *
*                                All Rights Reserved                                *
*************************************************************************************
*
* Description
*
*   InputDevices.h is a component of the Arduino application AudioPlatform.
*   The definitions, structures, and functions defined here are specific to that
*   application.
*
*   The InputDevices component encompasses the elements related to the application's
*   physical input devices.
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

#ifndef INPUTDEVICES_H_
#define INPUTDEVICES_H_

/****************
* "Pre-Defines" *
*****************
*
* Definitions that affect subsequent external references
*
*/

// Test assignment of MIDI controls to parameters by moving control on connected MIDI device
//
#define TEST_MIDI_CTRL_ASSIGN   0

/************************************************************************************
*                       E X T E R N A L   R E F E R E N C E S                       *
*************************************************************************************
*/

// Application Components
//
#include "AudioPlatform.h"

// User Arduino Libraries
//
#include "RotaryEncoderPB.h"    // See note below on use of RotaryEncoderPB vs RotaryEncoder

/************************************************************************************
*                E X P O R T E D   M A C R O S  /  C O N S T A N T S                *
*************************************************************************************
*/


/******************
* Rotary Encoders *
*******************
*/
#define ROTARY0_CLK_A_PIN           ((uint8_t) 3)
#define ROTARY0_DT_B_PIN            ((uint8_t) 4)
#define ROTARY0_SWITCH_PIN          ((uint8_t) 24)
#define ROTARY1_CLK_A_PIN           ((uint8_t) 25)
#define ROTARY1_DT_B_PIN            ((uint8_t) 30)
#define ROTARY1_SWITCH_PIN          ((uint8_t) 31)
#define ROTARY2_CLK_A_PIN           ((uint8_t) 33)
#define ROTARY2_DT_B_PIN            ((uint8_t) 34)
#define ROTARY2_SWITCH_PIN          ((uint8_t) 35)
#define ROTARY3_CLK_A_PIN           ((uint8_t) 36)
#define ROTARY3_DT_B_PIN            ((uint8_t) 37)
#define ROTARY3_SWITCH_PIN          ((uint8_t) 38)

/************************************************************************************
*            E X P O R T E D   D A T A / T Y P E   D E F I N I T I O N S            *
*************************************************************************************
*
* Exported Data/Types referenced with namespace prefix "InputDevices::"
*
*/

/******************
* Begin Namespace *
*******************
*/
namespace InputDevices {

/******************
* Rotary Encoders *
*******************
*
* NOTE: We are using RotaryEncoderPB (RotaryEncoder "ParamButton"), a subclass of RotaryEncoder, in
* preference to RotaryEncoder. RotaryEncoderPB overrides RotaryEncoder's virtual methods reportValueChange()
* and reportIncrChange() to add the capability to report encoder changes directly to a ParamButtonScreen
* instance in which it is embedded, rather than external handlers (which are ordinarily specified as
* the final 2 optional arguments of the class instantiator).
*
* Since the AudioPlatform application makes extensive use of ParamButtonScreen instances (see Screens.h),
* this provides a more efficient and transparent means of redisplaying encoder settings associated with objects
* and buttons on these screens.
*
* However, in many if not most situations, a change to a screen value will also need to be reflected in
* settings of other modules. So, in these cases, modules using encoders (most all parameter screens) may
* also set the "applyFn" argument in their RotaryParams definitions so that an external handler will also be
* invoked in ParamButtonScreen::rotaryValueChange(). (See the RotaryParams definitions in Screens.ino.)
*
*/

// Encoder profiles
RotaryEncoderPB     rotary0(0, ROTARY0_DT_B_PIN, ROTARY0_CLK_A_PIN, ROTARY0_SWITCH_PIN);
RotaryEncoderPB     rotary1(1, ROTARY1_DT_B_PIN, ROTARY1_CLK_A_PIN, ROTARY1_SWITCH_PIN);
RotaryEncoderPB     rotary2(2, ROTARY2_DT_B_PIN, ROTARY2_CLK_A_PIN, ROTARY2_SWITCH_PIN);
RotaryEncoderPB     rotary3(3, ROTARY3_DT_B_PIN, ROTARY3_CLK_A_PIN, ROTARY3_SWITCH_PIN);

// Array of available encoders
RotaryEncoderPB     *encoders[ROTARY_COUNT] = {&rotary0, &rotary1, &rotary2, &rotary3};

/************************************************************************************
*              E X P O R T E D   F U N C T I O N   P R O T O T Y P E S              *
*************************************************************************************
*
* Exported functions referenced with namespace prefix "InputDevices::"
*
*/

void    initInputDevices();
void    pollInputDevices();

// Parameter change handlers for AudioPLatform devices called from Presets Manager (Presets.ino).
// Presets Manager calls these functions in all AudioPlatform core components in two cases:
//
//  1. When new presets are loaded, to apply preset settings in other core components.
//  2. When processing applyFn() callbacks from ParamButton screens when preset
//     parameters managed by rotary encoders are modified, again to apply the modified
//     settings in other core components
//
void    setWavPlayerParam(uint16_t paramId, float paramVal);
void    setRawPlayerParam(uint16_t paramId, float paramVal);
void    setBasicSynthParam(uint16_t paramId, float paramVal);
void    setFmSynthParam(uint16_t paramId, float paramVal);
void    setMidiParam(uint16_t paramId, float paramVal);
void    setRvbParam(uint16_t paramId, float paramVal);
void    setEfxParam(uint16_t paramId, float paramVal);
void    setStereoDelayParam(uint16_t paramId, float paramVal);
void    setInputOutputParam(uint16_t paramId, float paramVal);

/****************
* End Namespace *
*****************
*/
} // namespace InputDevices

#endif // INPUTDEVICES_H_
