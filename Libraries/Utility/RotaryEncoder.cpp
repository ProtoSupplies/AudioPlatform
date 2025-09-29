/************************************************************************************
*                                RotaryEncoder.cpp                                  *
*                      Copyright (c) 2025, Donald Swearingen                        *
*                               All Rights Reserved                                 *
*************************************************************************************
*
* Description
*
*   KY-040 driver/control class for Teensy 4.x, based on the TeensyDuino library
*   EncoderTool library.
*
* Pullup Resistors
*
*   Many KY-O40 encoders available commercially are mounted on small circuit boards
*   that include pullup resistors, and an input for +V that is connected to one side
*   of the resistors, with the other connected to a digital input port on the Arduino.
*
*   The Teensy 4.0 and 4.1 both provide pullups on all digital pins, so that it is
*   not necessary to include pullup resistors in circuit designs. In this case, the
*   +V input of the circuit board can be left unconnected, and the digital outputs
*   connected directly to Teensy digital inputs. However, in this case it has been
*   found that the performance of the encoder is affected, taking a longer "arc" of
*   rotation to achieve a change in output value.
*
*   The KY-050 encoders that are the target of the drivers here and in the EncoderTool
*   library are assumed to be NON-PCB devices, with no pullup resistors and no +V pin.
*
* Methodology
*
*   Implements an interface to the Arduino RotaryEncoder Library, using a polled
*   discipline, and providing a structured profile of the value states being managed
*   via its RotaryParams structure.
*
*   In order to allow the encoder to be used for different value ranges and applications,
*   the design is such that the profile can be changed dynamically by passing a
*   new RotaryParams structure via the function setParams().
*
*   Other public functions allow access for setting and retrieving various local
*   values under management by the encoder.
*
* Tick Function
*
*   In order to avoid missing any change in the encoder state, the base function
*   encoder.tick() must be invoked as often as possible. See the section below on
*   Calling Protocols.
*
* Calling Protocols
*
*   When an encoder's value or switch states change, users can obtain the latest values
*   in 2 ways:
*
*       1) A call to bool function newVal(), which returns true if the encoder value has
*       changed, storing the new encoder value in the caller's reference variable "newVal&".
*       newVal() also attempts callbacks as described in 2) below, but is primarily intended
*       for use in which the callback functions are not defined.

*       2) A call to void function pollEncoder() which, when it's callback handler functions
*       have been set, in its virtual reportValueChange() and reportIncrChange() methods,
*       invokes the specified callback functions (incrChangeFn, and valueChangeFn) as
*       appropriate to the value that has changed.
*
*   In both cases, the base function encoder.tick() is called to update the state of
*   the encoder.
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
*                          P U B L I C   F U N C T I O N S                          *
*************************************************************************************
*/

// Constructor
//
// Initialize class variables and allocate buffer.
//
RotaryEncoder::RotaryEncoder(uint8_t id, uint8_t pinA_CLK, uint8_t pinB_DT, uint8_t pin_Switch, void (*valueChgFn)(uint8_t, int16_t), void (*incrChgFn)(uint8_t, uint16_t)) :
    rotaryId(id), _pinCLK(pinA_CLK), _pinDT(pinB_DT), _pinSwitch(pin_Switch),
    encoderVal(0), encoderValPrev(0), encoderValMin(-32768), encoderValMax(32767), valueChangeFn(valueChgFn),
    incr(1), incrVals(0), incrValSize(0), incrValIndex(0), incrChangeFn(incrChgFn) {

    encoder.begin(_pinCLK, _pinDT, _pinSwitch);
}

// Destructor
//
RotaryEncoder::~RotaryEncoder() {

}

// pollEncoder()
//
// Regularly poll the encoder, reporting state changes via the user's specified
// callback functions, if defined.
//
void RotaryEncoder::pollEncoder() {

    // Polled encoder. Call tick() on every poll.
    encoder.tick();

    // Check for press of Encoder Button
    if (encoder.buttonChanged()) {
        if (encoder.getButton() == LOW) {
            nextIncrementValue();
        }
    }

    // Handle new encoder value
    if (encoder.valueChanged()) {
        encoderPosPrev = encoderPos;
        encoderPos = encoder.getValue();
//        Serial.printf("RotaryEncoder::pollEncoder: encoderPos %d\n", encoderPos);
        if (encoderPos > encoderPosPrev) {
            encoderVal += incr;
        } else {
            encoderVal -= incr;
        }
//        Serial.printf("RotaryEncoder::pollEncoder: encoderVal %d\n", encoderVal);
        // Limit to [min,max] range
        if (encoderVal < encoderValMin) {
            encoderVal = encoderValMin;
        } else if (encoderVal > encoderValMax) {
            encoderVal = encoderValMax;
        }
//        Serial.printf("RotaryEncoder::pollEncoder: adjusted encoderVal %d encoderValPrev %d\n", encoderVal, encoderValPrev);

        // Output new value, if changed
        if (encoderVal != encoderValPrev) {
            encoderValPrev = encoderVal;
            reportValueChange(rotaryId, encoderVal);
//            Serial.printf("RotaryEncoder::pollEncoder: reportValueChange %d\n", encoderVal);
        }
    }
}

// nextIncrementValue()
//
// Select the next value in the incrVals[] array and report the
// change to the parent/owner.
//
void RotaryEncoder::nextIncrementValue() {

    if (incrVals != 0 && incrValSize != 0) {
        incrValIndex = (incrValIndex + 1) % incrValSize;
        uint16_t incrVal = incrVals[incrValIndex];

        setIncrement(incrVal);
        reportIncrChange(rotaryId, incrVal);
    }
}

// newVal()
//
// Regularly poll the encoder, reporting state changes via a return value of true,
// and storing the new encoder value in the caller's reference variable "newVal".
//
bool RotaryEncoder::newVal(int16_t& newVal) {

    bool isNew = false;


    // Polled encoder. Call tick() on every query to newVal()
    encoder.tick();

    // Check for press of Encoder Button
    if (encoder.buttonChanged()) {
        if (encoder.getButton() == LOW) {
            nextIncrementValue();
        }
    }

    // Handle new encoder value
    if (encoder.valueChanged()) {
        encoderPosPrev = encoderPos;
        encoderPos = encoder.getValue();
        if (encoderPos > encoderPosPrev) {
            encoderVal += incr;
        } else {
            encoderVal -= incr;
        }

        // Limit to [min,max] range
        if (encoderVal < encoderValMin) {
            encoderVal = encoderValMin;
        } else if (encoderVal > encoderValMax) {
            encoderVal = encoderValMax;
        }

        // Output new value, if changed
        if (encoderVal != encoderValPrev) {
            encoderValPrev = encoderVal;
            newVal = encoderVal;
            isNew = true;
            reportValueChange(rotaryId, encoderVal);
        }
    }

    return isNew;
}

// setVal()
//
void RotaryEncoder::setVal(int16_t val) {

    if (val > encoderValMax) {
        val = encoderValMax;
    }
    if (val < encoderValMin) {
        val = encoderValMin;
    }
    encoderVal = val;
}

// getVal()
//
int16_t RotaryEncoder::getVal() {

    return encoderVal;
}

// setValMin()
//
void RotaryEncoder::setValMin(int16_t valMin) {

    encoderValMin = valMin;
}

// setValMax()
//
void RotaryEncoder::setValMax(int16_t valMax) {

    encoderValMax = valMax;
}

// setValMinMax()
//
void RotaryEncoder::setValMinMax(int16_t valMin, int16_t valMax) {

    setValMin(valMin);
    setValMax(valMax);
}

// setIncrement()
//
void RotaryEncoder::setIncrement(uint16_t increment) {

    incr = increment;
}

// getIncrement()
//
uint16_t RotaryEncoder::getIncrement() {

    return incr;
}

// setIncrementValues()
//
void RotaryEncoder::setIncrementValues(uint16_t* incrementVals, uint8_t size) {

    if (incrementVals != 0 && size != 0) {
        incrVals = incrementVals;
        incrValSize = (size < incrValSizeMax) ? size : incrValSizeMax;
        incrValIndex = 0;
        incr = incrVals[incrValIndex];

    } else {
        incr = 1;
        incrVals = 0;
        incrValSize = 0;
        incrValIndex = 0;
    }
}

// setIncrementValuesIndex()
//
void RotaryEncoder::setIncrementValuesIndex(uint8_t index) {

    incrValIndex = index;
    incr = incrVals[incrValIndex];
}

// getIncrementValuesIndex()
//
uint8_t RotaryEncoder::getIncrementValuesIndex() {

    return incrValIndex;
}

// setParams()
//
void RotaryEncoder::setParams(RotaryParams& params) {

    setValMinMax(params.valueMin, params.valueMax);
    setIncrementValues(params.incrs, params.incrCount);
    setIncrementValuesIndex(params.incrIndex);
    setVal(params.value);
    encoderValPrev = params.value;  // Added in v0.4.0.1 11/5/24 as correction for M240722-4 (actually a bug)
}

/************************************************************************************
*                        P R O T E C T E D   F U N C T I O N S                      *
*************************************************************************************
*/

// reportValueChange()
//
void RotaryEncoder::reportValueChange(uint8_t rotaryId, int16_t encoderVal) {

    if (valueChangeFn != NULL) {
        valueChangeFn(rotaryId, encoderVal);
    }
}

// reportIncrChange()
//
void RotaryEncoder::reportIncrChange(uint8_t rotaryId, uint16_t incrVal) {

    if (incrChangeFn != NULL) {
        incrChangeFn(rotaryId, incrVal);
    }
}

