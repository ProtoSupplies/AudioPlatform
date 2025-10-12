/************************************************************************************
*                              FlashRawPlayerMulti.cpp                              *
*                       Copyright (c) 2025, Donald Swearingen                       *
*                                All Rights Reserved                                *
*************************************************************************************
*
* Description
*
*   FlashRawPlayerMulti is a subclass of MediaPlayerMulti that implements the methods
*   of that base class to play multiple simultaneous audio files from flash storage
*   media.
*
*   FlashRawPlayerMulti is designed to playback multiple instances (set by constructor
*   argument _playerCount) of RAW mono audio streams from a serial Flash IC installed
*   in a Teensy 4.1 system.
*
*   Among the various types of media streams available in the Teensy Audio Library,
*   AudioPlaySerialflashRaw (the audio base of class FlashRawPlayer) is by far the most
*   efficient and conservative in its requirements for CPU and memory usage.
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

#include <stdlib.h>

#include "ArduinoEclipseDefs.h"
#include "UtilFns.h"
#include "FlashRawPlayerMulti.h"

/************************************************************************************
*                         M A C R O S  /  C O N S T A N T S                         *
*************************************************************************************
*/


/************************************************************************************
*                     D A T A / T Y P E   D E F I N I T I O N S                     *
*************************************************************************************
*/

/************************************************************************************
*                          P U B L I C   F U N C T I O N S                          *
*************************************************************************************
*/

// Constructor
//
FlashRawPlayerMulti::FlashRawPlayerMulti(uint8_t _playerCount) : MediaPlayerMulti(_playerCount) {

    // Allocate media players and connect to mixers in MediPlayerMulti base class
    for (int i = 0; i < _playerCount; i++) {
        mediaPlayers[i] = new FlashRawPlayer(i);
        _playersToPlayerMixerLeft[i].connect(*mediaPlayers[i]->getLeftOutput(), 0, _playerMixerLeft, i);
        _playersToPlayerMixerRight[i].connect(*mediaPlayers[i]->getRightOutput(), 0, _playerMixerRight, i);
    }
}

// Destructor
//
FlashRawPlayerMulti::~FlashRawPlayerMulti() {

}

/************************************************************************************
*                          P R I V A T E   F U N C T I O N S                        *
*************************************************************************************
*/
