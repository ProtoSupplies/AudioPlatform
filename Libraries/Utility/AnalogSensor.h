/************************************************************************************
*                                  AnalogSensor.h                                   *
*                                                                                   *
*                      Copyright (c) 2025, Donald Swearingen                        *
*                              All Rights Reserved                                  *
*************************************************************************************
*
* Description
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

#ifndef ANALOGSENSOR_H_
#define ANALOGSENSOR_H_

/************************************************************************************
*                       E X T E R N A L   R E F E R E N C E S                       *
*************************************************************************************
*/

#include <Arduino.h>
#include <stdint.h>

#include "UtilFns.h"
#include "MovingAverage.h"

/************************************************************************************
*                E X P O R T E D   M A C R O S  /  C O N S T A N T S                *
*************************************************************************************
*/

#define INPIN_NONE      255
#define OVERSAMPLE_MAX  16

/************************************************************************************
*            E X P O R T E D   D A T A / T Y P E   D E F I N I T I O N S            *
*************************************************************************************
*/

/************************************************************************************
*                         C L A S S   D E F I N I T I O N S                         *
*************************************************************************************
*/

class AnalogSensor {

// Constructor/Destructor
//
public:
    AnalogSensor(uint8_t inPin, int32_t analogInMin, int32_t analogInMax, float outputMin, float outputMax, uint8_t mAvgLen, uint8_t inThresh);
    virtual ~AnalogSensor();

// Class Variables
//
public:

private:

    // External Class Instances
    MovingAverage   mAvg;

    // Input State
    uint8_t         inPin;                  // Hardware analog input pin
    int32_t         analogInMin;            // Fixed ADC input minimum value
    int32_t         analogInMax;            // Fixed ADC input maximum value
    int32_t         analogInVal;            // current analog input
    int32_t         inVal;                  // current input value
    uint8_t         inputThreshold;         // input change threshold (for deJitter)
    int32_t         lastInVal;              // last input value
    uint8_t         lastInDir;              // up/down direction of output samples

    // Output State
    float           outMin;                 // minimum output value
    float           outMax;                 // maximum output value
    float           lastOutVal;             // last output value float

    uint8_t         overSampleCount;        // # samples to take on each call to getNextOutput()

protected:

// Class Methods
//
public:
    bool            getNextOutput(float& outVal);
    void            setInMin(int32_t adcInMin); // Fixed ADC input minimum
    void            setInMax(int32_t adcInMax); // Fixed ADC input maximum
    void            setInMinMax(int32_t adcInMin, int32_t adcInMax);
    void            setInThreshold(uint8_t inThresh);
    void            setOutMin(float outputMin); // User-determined output minimum
    void            setOutMax(float outputMax); // User-determined output maximum
    void            setOutMinMax(float outputMin, float outputMax);
    void            setMAvgLen(uint8_t length);
    void            setOverSampleCount(uint8_t count);

private:

protected:

};

#endif // ANALOGSENSOR_H_

