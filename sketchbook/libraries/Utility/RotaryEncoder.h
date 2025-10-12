/************************************************************************************
*                                 RotaryEncoder.h                                   *
*                      Copyright (c) 2025, Donald Swearingen                        *
*                               All Rights Reserved                                 *
*************************************************************************************
*
* Description
*
*   Structure and function definitions for the RotaryEncoder class.
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

#ifndef ROTARYENCODER_H_
#define ROTARYENCODER_H_

#include <Arduino.h>
#include <stdint.h>

#include "EncoderTool.h"
using namespace EncoderTool;

/************************************************************************************
*                         M A C R O S  /  C O N S T A N T S                         *
*************************************************************************************
*/

#define ROTARY_MAX_INCRS        8   // Maximum size of increment values array
#define ROTARY_COUNT            4   // # of installed rotaries
#define ROTARY_COUNT_MAX        4   // Maximum encoders (limits fixed array size)

// Rotary Conversion Factors
//
// Encoders only use integers, whereas application values are often float values
// representing various quantities. In these cases, a conversion factor is required
// when going back and forth between encoder values and application values.
//
// These constants are generally specified in application data structures, and applied
// either explicitly at runtime, or statically using simple mathematical expressions
// in the data structure definitions.
//
#define ROTARY_DEFAULT_FACTOR   1.0f    // 1-to-1 mapping
#define ROTARY_TO_DB_FACTOR     2.0f    // Rotary [-140,24] -> dB [-70, +12] in 0.5 dB increments
#define ROTARY_TO_PCT_FACTOR    0.01f   // Rotary [0, 100] -> % [
#define ROTARY_TO_HZ_FACTOR     0.1f    // Rotary [0, 127] -> Hz [0, 12.7] in 0.1 Hz increments

#define DB_TO_ROTARY(dbVal)     ((int16_t)((float) dbVal * ROTARY_TO_DB_FACTOR))
#define PCT_TO_FLOAT(pctVal)    ((float) pctVal * ROTARY_TO_PCT_FACTOR)
#define HZ_TO_ROTARY(hzVal)     ((int16_t)((float) hzVal * ROTARY_TO_HZ_FACTOR))

/************************************************************************************
*                     D A T A / T Y P E   D E F I N I T I O N S                     *
*************************************************************************************
*/

typedef struct {

    int16_t         value;
    int16_t         valueMin;
    int16_t         valueMax;
    uint16_t        incrs[ROTARY_MAX_INCRS];
    uint8_t         incrCount;
    uint8_t         incrIndex;

    void            (*applyFn)(uint16_t, float);  // Function to call when encoder value changes (deviceId, paramVal)

} RotaryParams;

/************************************************************************************
*                         C L A S S   D E F I N I T I O N S                         *
*************************************************************************************
*/

class RotaryEncoder {

// Constructor/Destructor
//
public:

    RotaryEncoder(uint8_t id, uint8_t pinA_CLK, uint8_t pinB_DT, uint8_t pin_Switch, void (*valueChgFn)(uint8_t, int16_t) = 0, void (*incrChgFn)(uint8_t, uint16_t) = 0);
    virtual ~RotaryEncoder();

// Class Variables
//
public:


private:

    PolledEncoder   encoder;

    uint8_t         rotaryId;
    uint8_t         _pinCLK;
    uint8_t         _pinDT;
    uint8_t         _pinSwitch;

    int16_t         encoderPos;     // current encoder position
    int16_t         encoderPosPrev; // previous encoder position
    int16_t         encoderVal;     // current encoder value
    int16_t         encoderValPrev; // previous encoder value
    int16_t         encoderValMin;  // minimum encoder value
    int16_t         encoderValMax;  // maximum encoder value

    const uint8_t   incrValSizeMax = ROTARY_MAX_INCRS;

protected:

    void            (*valueChangeFn)(uint8_t, int16_t); // called when encoder value changes

    uint16_t        incr;           // increment value
    uint16_t*       incrVals;       // array of increment values
    uint8_t         incrValSize;    // length of array of increment values
    uint8_t         incrValIndex;   // index into array of increment values

    void            (*incrChangeFn)(uint8_t, uint16_t); // called when increment value changes

// Class Methods
//
public:

    void            pollEncoder();

    bool            newVal(int16_t& newVal);

    void            setVal(int16_t val);
    int16_t         getVal();
    void            setValMin(int16_t valMin);
    void            setValMax(int16_t valMax);
    void            setValMinMax(int16_t valMin, int16_t valMax);

    void            setIncrement(uint16_t increment);
    uint16_t        getIncrement();
    void            setIncrementValues(uint16_t* incrementVals, uint8_t size);
    void            nextIncrementValue();
    void            setIncrementValuesIndex(uint8_t index);
    uint8_t         getIncrementValuesIndex();
    void            setParams(RotaryParams& params);

private:


protected:

    virtual void    reportValueChange(uint8_t rotaryId, int16_t encoderVal);
    virtual void    reportIncrChange(uint8_t rotaryId, uint16_t incrVal);

};

#endif /* ROTARYENCODER_H_ */

