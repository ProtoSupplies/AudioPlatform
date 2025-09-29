/************************************************************************************
*                               RotaryEncoderPB.cpp                                 *
*                      Copyright (c) 2025, Donald Swearingen                        *
*                               All Rights Reserved                                 *
*************************************************************************************
*
* Description
*
*   RotaryEncoderPB (RotaryEncoder "ParamButton"), is a subclass of RotaryEncoder.
*   RobaryEncoderPB overrides RotaryEncoder's virtual methods reportValueChange() and
*   reportIncrChange() to add the capability to report encoder changes directly to a
*   ParamButtonScreen instance in which it is embedded, rather than external handlers
*   (which are ordinarily specified as the final 2 (optional) arguments (not used here)
*   of the class instantiator.
*
*   For applications that make extensive use of ParamButtonScreen instances, this provides
*   a more efficient and transparent means of handling encoder settings associated with
*   objects and buttons on these screens.
*
*   Operations and methods of the base class are detailed in module RotaryEncoder.cpp.
*
* Update/Callback Protocols
*
*   There are two means of determining changes in the encoder's value or switch state
*   (in both cases, the base function encoder.tick() is called to update the state of
*   the encoder). These mechanisms are detailed in the header comments of base class
*   RotaryEncoder.cpp.
*
*   In the case of RotaryEncoderPB, method 2 of the base class (see RotaryEncoder.cpp header)
*   has been extended as follows:
*
*       2) A call to void function pollEncoder() which, when it's callback handler functions
*       have been set, in its overriding versions of the virtual reportValueChange() and
*       reportIncrChange() methods, invokes the specified callback functions (incrChangeFn,
*       and valueChangeFn) as appropriate to the value that has changed.
*
*       In this case, there are two possible callbacks:
*
*           - First, if the encoder's variable ParamButtonScreen *pbScreen is set, the callback
*             will be directly to the ParamButtonScreen currently associated with the encoder
*             (as set by the ParamButtonScreen in its activateScreen() method).
*           - Otherwise, if the value of the encoder's internal callback functions (incrChangeFn
*             and valueChangeFn) are defined, these callbacks are used.
*
*       This scheme allows callbacks to functions in the Arduino C-language program space, defined
*       as non-class functions:
*
*           void (*incrChgFn)(uint8_t, uint16_t))
*
*       and also to the C++ class-oriented handlers such as in individual instances of the
*       ParamButtonScreen class:
*
*           void ParamButtonScreen::rotaryIncrChange(uint8_t, uint16_t)
*
*       Circumventing the tricky issue of passing a direct pointer to an instance method of
*       an individual C++ class instance (much discussed on forums, with few simple or clean
*       solutions, if any).
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

#include <Arduino.h>
#include <stdlib.h>

#include "RotaryEncoderPB.h"

// ParamButtonScreen.h Reference
//
// As is sometimes the case, we cannot include "ParamButtonScreen.h" in RotaryEncoderPB.h because
// doing so would create a circular/recursive problem since ParamButtonScreen.h also
// includes RotaryEncoderPB.h. We therefore define only a reference to the class in RotaryEncoderPB.h
// so that we can declare a pointer variable "pbScreen" that can be set by a ParamButtonScreen
// instance in its startup code.
//
// In order that the encoder can have access via this pointer to the internal class objects
// in the ParamButtonScreen, the file "ParamButtonScreen.h" is included here.
//
#include "ParamButtonScreen.h"

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
// Initialize class variables and allocate buffer.
//
RotaryEncoderPB::RotaryEncoderPB(uint8_t id, uint8_t pinA_CLK, uint8_t pinB_DT, uint8_t pin_Switch, void (*valueChgFn)(uint8_t, int16_t), void (*incrChgFn)(uint8_t, uint16_t)) :
    RotaryEncoder(id, pinA_CLK, pinB_DT, pin_Switch, valueChgFn, incrChgFn), pbScreen(0) {

}

// Destructor
//
RotaryEncoderPB::~RotaryEncoderPB() {

}

/************************************************************************************
*                        P R O T E C T E D   F U N C T I O N S                      *
*************************************************************************************
*/

// These overriding methods provide for both the original callback mechanism, and also
// callbacks into the ParamButtonScreen instance in which an encoder may be embedded.
// See header comments above.
//

// reportValueChange()
//
void RotaryEncoderPB::reportValueChange(uint8_t rotaryId, int16_t encoderVal) {

    // Send new encoder value to callbacks if defined,
    // priority to ParamButtonScreen
    if (pbScreen) {
        // C++ space
        pbScreen->rotaryValueChange(rotaryId, encoderVal);
    } else if (valueChangeFn) {
        // C space
        valueChangeFn(rotaryId, encoderVal);
    }
}

// reportIncrChange()
//
void RotaryEncoderPB::reportIncrChange(uint8_t rotaryId, uint16_t incrVal) {

    // Send new encoder increment to callbacks if defined,
    // priority to ParamButtonScreen
    if (pbScreen) {
        // C++ space
        pbScreen->rotaryIncrChange(rotaryId, incrVals[incrValIndex]);
    } else if (incrChangeFn) {
        // C space
        incrChangeFn(rotaryId, incrVals[incrValIndex]);
    }
}

