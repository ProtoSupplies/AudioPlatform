/************************************************************************************
*                               FlashRawPlayerMulti.h                               *
*                       Copyright (c) 2025, Donald Swearingen                       *
*                                All Rights Reserved                                *
*************************************************************************************
*
* Description
*
*   FlashRawPlayerMulti is a subclass of MediaPlayerMulti that implements the methods
*   of that base class to play multiple simultaneous audio files from Flash storage
*   media.
*
*   See FlashRawPlayerMulti.cpp for further details.
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

#ifndef FLASHRAWPLAYERMULTI_H_
#define FLASHRAWPLAYERMULTI_H_

#include <stdint.h>

#include <Audio.h>
#include <SerialFlash.h>

#include "FlashRawPlayer.h"
#include "MediaPlayerMulti.h"

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

class FlashRawPlayerMulti : public MediaPlayerMulti {

// Constructor/Destructor
//
public:

    FlashRawPlayerMulti(uint8_t _playerCount = 8);
    virtual ~FlashRawPlayerMulti();

// Class Variables
//
public:


protected:


private:

// Class Methods
//
public:

protected:

private:

};

#endif // FLASHRAWPLAYERMULTI_H_
