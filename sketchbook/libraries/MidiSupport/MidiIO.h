/************************************************************************************
*                                    MidiIO.h                                       *
*                      Copyright (c) 2025, Donald Swearingen                        *
*                               All Rights Reserved                                 *
*************************************************************************************
*
* Description
*
*   USB Host MIDI Input/Output class for Teensy 4.1. Generally deployed as a single
*   instance in Arduino application.
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

#ifndef MIDIIO_H_
#define MIDIIO_H_

#include <USBHost_t36.h>

#include "ArduinoEclipseDefs.h"

/************************************************************************************
*                         M A C R O S  /  C O N S T A N T S                         *
*************************************************************************************
*/

/************************************************************************************
*                     D A T A / T Y P E   D E F I N I T I O N S                     *
*************************************************************************************
*/

/************************************************************************************
*                         C L A S S   D E F I N I T I O N S                         *
*************************************************************************************
*/

class MidiIO {

// Constructor/Destructor
//
public:

    MidiIO();
    virtual ~MidiIO();

// Class Variables
//
public:


private:

    // Base USB Host
    // Defines/Instantiates T4.1 USB Host Port
    //
    USBHost                 usbHost;

    // First USB Hub
    // For each Hub connected (possibly in series), am additional USBHub instance must be added here.
    //
    USBHub                  usbHub;

    // Primary MIDI IO Handler
    //
    // On Teensy4.1 Proto Board, if midiDevice is defined as MIDIDevice, only note and control messages
    // are received and displayed (no pitch wheel or program changes). If defined as MIDIDevice_BigBuffer
    // pitch wheel and program change are handled. Hmmmm.... There really needs to be something of a
    // user guide for USBHost MIDI (and also other USBHost topics).
    //
    MIDIDevice_BigBuffer    midiDevice; // Used for support of additional MIDI messages

protected:


// Class Methods
//
public:

    void    begin();
    bool    update();

    void    setHandleNoteOff(void (*fptr)(uint8_t channel, uint8_t note, uint8_t velocity));
    void    setHandleNoteOn(void (*fptr)(uint8_t channel, uint8_t note, uint8_t velocity));
    void    setHandleControlChange(void (*fptr)(uint8_t channel, uint8_t control, uint8_t value));
    void    setHandlePitchBend(void (*fptr)(uint8_t channel, int value));
    void    setHandleProgramChange(void (*fptr)(uint8_t channel, uint8_t value));
    void    setHandleStop(void (*fptr)(void));
    void    setHandleSystemReset(void (*fptr)(void));

private:


protected:


};
#endif // MIDIIO_H_
