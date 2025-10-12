/************************************************************************************
*                                 FlashRawPlayer.h                                  *
*                       Copyright (c) 2025, Donald Swearingen                       *
*                                All Rights Reserved                                *
*************************************************************************************
*
* Description
*
*   FlashRawPlayer a subclass of MediaPlayer that implements the methods of that base
*   class to play audio files from Flash storage media.
*
*   See FlashRawPlayer.cpp for further details.
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

/************************************************************************************
*                       E X T E R N A L   R E F E R E N C E S                       *
*************************************************************************************
*/

#ifndef FLASHRAWPLAYER_H_
#define FLASHRAWPLAYER_H_

// Standard C++
//
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

// Teensyduino
//
#include <Audio.h>
#include <SPI.h>
#include <SerialFlash.h>

// User Arduino Libraries
//
#include "MediaPlayer.h"
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

class FlashRawPlayer : public MediaPlayer {

// Constructor/Destructor
//
public:

    FlashRawPlayer(uint8_t id = 0);
    virtual ~FlashRawPlayer();

// Class Variables
//
public:

    // Flash Chip Mono Raw File Player instance
    AudioPlaySerialflashRaw _flashRawPlayer;

    // Optional ID (defaults to 0) useful in identifying
    // player when player is used in a multi-player setting
    uint8_t                 _playerId;

private:


protected:


// Class Methods
//
public:

    // Virtual methods overrides of MediaPlayer methods to
    // manage playback for FlashRawPlayer instances.
    void startPlay(const char* soundFile, float gainValue, float panPosition) override;
    void stopPlay() override;
    bool isPlaying() override;
    bool isStopped() override;

private:


protected:


};

#endif // FLASHRAWPLAYER_H_
