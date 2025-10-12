/************************************************************************************
*                                RotaryEncoderPB.h                                  *
*                      Copyright (c) 2025, Donald Swearingen                        *
*                               All Rights Reserved                                 *
*************************************************************************************
*
* Description
*
*   Structure and function definitions for the RotaryEncoderPB class.
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
*
*/

/************************************************************************************
*                       E X T E R N A L   R E F E R E N C E S                       *
*************************************************************************************
*/

#ifndef ROTARYENCODERPB_H_
#define ROTARYENCODERPB_H_

#include "RotaryEncoder.h"

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

// ParamButtonScreen Reference
//
// As is sometimes the case, we cannot include "ParamButtonScreen.h" in this module because
// doing so would create a circular/recursive problem since ParamButtonScreen.h also
// includes RotaryEncoderPB.h. We therefore define only a reference to the class here so that
// we can declare a pointer variable "pbScreen" below that can be set by a ParamButtonScreen
// instance in its startup code.
//
// In order that the encoder can have access via this pointer to the internal class objects
// in the ParamButtonScreen, the file "ParamButtonScreen.h" is included instead in
// RotaryEncoderPB.cpp.
//
class ParamButtonScreen;

class RotaryEncoderPB : public RotaryEncoder {

// Constructor/Destructor
//
public:

    RotaryEncoderPB(uint8_t id, uint8_t pinA_CLK, uint8_t pinB_DT, uint8_t pin_Switch, void (*valueChgFn)(uint8_t, int16_t) = 0, void (*incrChgFn)(uint8_t, uint16_t) = 0);
    virtual ~RotaryEncoderPB();

// Class Variables
//
public:

    // Transient pointer to an instance of ParamButtonScreen currently on display
    // used by encoder to call directly into ParamButtonScreen's encoder update methods.
    ParamButtonScreen *pbScreen;

private:


protected:

// Class Methods
//
public:


private:


protected:

    // The overriding methods provide for both the original callback mechanism, and also
    // callbacks into the ParamButtonScreen instance in which an encoder may be embedded.
    void            reportValueChange(uint8_t rotaryId, int16_t encoderVal) override;
    void            reportIncrChange(uint8_t rotaryId, uint16_t incrVal) override;

};

#endif /* ROTARYENCODERPB_H_ */

