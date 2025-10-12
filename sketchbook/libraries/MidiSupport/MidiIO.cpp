/************************************************************************************
*                                    MidiIO.cpp                                     *
*                      Copyright (c) 2025, Donald Swearingen                        *
*                               All Rights Reserved                                 *
*************************************************************************************
*
* Description
*
*   USB Host MIDI Input/Output class for Teensy 4.1. Generally deployed as a single
*   instance in AudioPlatform Arduino application.
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

/************************************************************************************
*                       E X T E R N A L   R E F E R E N C E S                       *
*************************************************************************************
*/

#include "MidiIO.h"

/************************************************************************************
*                    S T A T I C   C L A S S   V A R I A B L E S                    *
*************************************************************************************
*/

// Initialization of static class variables
//

/************************************************************************************
*                    P U B L I C   C L A S S   F U N C T I O N S                    *
*************************************************************************************
*/

// Constructor/Destructor
//
MidiIO::MidiIO() : usbHub(usbHost), midiDevice(usbHost) {

}

MidiIO::~MidiIO() {

}

// begin()
//
void MidiIO::begin() {

    usbHost.begin();
}

// update()
//
bool MidiIO::update() {

    // The handler functions are called when midiDevice reads data. They
    // will not be called automatically---you must call midiDevice.read()
    // regularly from loop() for midiDevice to actually read incoming
    // data and invoke the handler functions as messages arrive.
    usbHost.Task();
    return midiDevice.read();
}

// setHandleNoteOff()
//
void MidiIO::setHandleNoteOff(void (*fptr)(uint8_t channel, uint8_t note, uint8_t velocity)) {
    // type: 0x80  NoteOff
    midiDevice.setHandleNoteOff(fptr);
}

// setHandleNoteOn()
//
void MidiIO::setHandleNoteOn(void (*fptr)(uint8_t channel, uint8_t note, uint8_t velocity)) {
    // type: 0x90  NoteOn
    midiDevice.setHandleNoteOn(fptr);
}

// setHandleControlChange()
//
void MidiIO::setHandleControlChange(void (*fptr)(uint8_t channel, uint8_t control, uint8_t value)) {
    // type: 0xB0  ControlChange
    midiDevice.setHandleControlChange(fptr);
}

// setHandlePitchBend()
//
void MidiIO::setHandlePitchBend(void (*fptr)(uint8_t channel, int value)) {
    // type: 0xE0  PitchChange
    midiDevice.setHandlePitchChange(fptr);
}
// setHandleProgramChange()
//
void MidiIO::setHandleProgramChange(void (*fptr)(uint8_t channel, uint8_t value)) {
    // type: 0xX0  ControlChange
    midiDevice.setHandleProgramChange(fptr);
}

// setHandleStop()
//
void MidiIO::setHandleStop(void (*fptr)(void)) {
    midiDevice.setHandleStop(fptr);
}

// setHandleSystemReset()
//
void MidiIO::setHandleSystemReset(void (*fptr)(void)) {
    midiDevice.setHandleSystemReset(fptr);
}

/************************************************************************************
*                          P R I V A T E   F U N C T I O N S                        *
*************************************************************************************
*/


