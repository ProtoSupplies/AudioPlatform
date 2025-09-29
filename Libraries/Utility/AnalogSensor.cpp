/************************************************************************************
*                                  AnalogSensor.cpp                                 *
*                                                                                   *
*                      Copyright (c) 2025, Donald Swearingen                        *
*                              All Rights Reserved                                  *
*************************************************************************************
*
* Description
*
*   On a call to its getNextOutput(float& outVal) function, AnalogSensor inputs signed
*   integer values (int32_t), representing voltages sampled from a specific analog input
*   pin on the MCU in use, and subjects the input values to a number of transformations:
*
*       analog in ->  dejitter -> inChanged? -> moving average ->
*       scale -> outChanged? -> output value -> return true
*
*       • analog in         read analog input (with possible oversampling)
*       • dejitter          subject inputs to a minimum change +/- from previous
*       • inChanged?        pass only changed inputs
*       • moving average    apply moving average to input
*       • scale             scale value from analog in range to output range
*       • outChanged?       pass only changed outputs
*       • output value      set the value of the user's reference
*
*   If the input passes all the stages above, it is stored in the caller's "outVal"
*   reference and getNextOutput() returns true. If the input fails to pass any stage,
*   no value is stored, and getNextOutput() returns false.
*
*
* Oversampling
*
*   On certain sensor/ADC circuits, the input values can be noisy to varying degrees.
*   It has been found that, in certain cases, this can be corrected by sampling the
*   input not once, but a number of times in succession, and taking the average of
*   the sampled values as the input. This approach is more feasible on MCU types that
*   run at high clock rates, such as that on Teens7 4.x processors at 600 MHz. Note
*   this is a process distinct from that of moving average.
*
* Moving Average
*
*   Though the sensor values are assumed to be of an integer type, the moving average
*   process will often result in floating point values (ex: (5 + 3 + 3) / 2 = 5.5,
*   thus the value of getNextInput() is returned as a float type.
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

#include <algorithm>

#include "AnalogSensor.h"
#include "MovingAverage.h"

/************************************************************************************
*                 P R I V A T E   M A C R O S  /  C O N S T A N T S                 *
*************************************************************************************
*/

/************************************************************************************
*             P R I V A T E   D A T A / T Y P E   D E F I N I T I O N S             *
*************************************************************************************
*/

/************************************************************************************
*                          P U B L I C   F U N C T I O N S                          *
*************************************************************************************
*/

// AnalogSensorIn
//
// Class Constructor
//
AnalogSensor::AnalogSensor(uint8_t inputPin, int32_t inputMin, int32_t inputMax, float outputMin, float outputMax, uint8_t mAvgLen, uint8_t inThresh) :
    mAvg(mAvgLen), inPin(inputPin), analogInMin(inputMin), analogInMax(inputMax), inputThreshold(inThresh), outMin(outputMin), outMax(outputMax) {

    if (analogInMax < analogInMin) {
        int32_t temp = analogInMin;
        analogInMax = analogInMin;
        analogInMin = temp;
    }

    lastInDir = DIR_DOWN;

    inVal = 0;
    lastInVal = 0;
    lastOutVal = 0.0;

    overSampleCount = 1;        // Default: No oversampling
}

// AnalogSensorIn~
//
// Class Destructor
//
AnalogSensor::~AnalogSensor() {
}

// getNextOutput(float&)
//
// Process sensor input value, and attempt to generate a new output value.
//
bool AnalogSensor::getNextOutput(float& outVal) {

    // Default return value: no new output
    bool newOutVal = false;

    // Ensure input is valid
    if (inPin != INPIN_NONE) {

        // Get new input from ADC, with possible oversampling
        int32_t overSampleSum = 0;
        for (int i = 0; i < overSampleCount; i++) {
            overSampleSum += analogRead(inPin);
        }
        analogInVal = overSampleSum / overSampleCount;

        // Limit input range
        inVal = max(analogInVal, analogInMin);
        inVal = min(analogInVal, analogInMax);

        // Normalize to input range
        inVal -= analogInMin;

        // DeJitter the new input value; if it passes (return true), set new
        // output value and remember new input value for comparison with next sample.
        if (UtilFns::deJitter(inVal, lastInVal, lastInDir, inputThreshold)) {

            if (inVal != lastInVal) {
                lastInVal = inVal;

                // Pass normalized value through moving average smoothing
                float inAvg = mAvg.getNextOutput(inVal);

                // Scale output value
                outVal = UtilFns::scale(inAvg, (float)analogInMin, (float)analogInMax, outMin, outMax);

                // Return true if new outVal != lastOutVal
                if (outVal != lastOutVal) {
                    lastOutVal = outVal;
                    newOutVal = true;;
                }
            }
        }
    }

    return newOutVal;
}

// setInMin
//
void AnalogSensor::setInMin(int32_t adcInMin) {

    analogInMin = max(adcInMin, analogInMin);
}

// setInMax
//
void AnalogSensor::setInMax(int32_t adcInMax) {

    analogInMax = min(adcInMax, analogInMax);
}

// setInMinMax
//
// Set the range of values of the ADC input, generally [0, 2^(n-1) where n is the
// number of bits of the ADC input range.
//
// Note that setting the range to values [analogInMin > 0, analogInMax < 2^(n-1)]
// may sometimes be employed in order to ensure that the min and max values are
// reached in the input stream.
//
void AnalogSensor::setInMinMax(int32_t analogInMin, int32_t analogInMax) {

    analogInMin = min(analogInMin, analogInMax);
    analogInMax = max(analogInMin, analogInMax);
}

// setInThreshold
//
// Set the change in input value required to pass sample in processing chain
//
void AnalogSensor::setInThreshold(uint8_t inThresh) {

    inputThreshold = inThresh;
}

// setOutMin
//
void AnalogSensor::setOutMin(float outputMin) {

    outMin = outputMin;
}

// setOutMax
//
void AnalogSensor::setOutMax(float outputMax) {

    outMax = outputMax;
}

// setOutMinMax
//
// Set the range of output values used in call to UtilFns:Scale()
//
void AnalogSensor::setOutMinMax(float outputMin, float outputMax) {

    outMin = min(outputMin, outputMax);
    outMax = max(outputMin, outputMax);
}

// setMAvgLen
//
void AnalogSensor::setMAvgLen(uint8_t length) {

    mAvg.setLength(length);
}

// setOverSampleCount
//
void AnalogSensor::setOverSampleCount(uint8_t count) {

    // Ensure count is in range [1,OVERSAMPLE_MAX]
    overSampleCount = max(1, count);
    overSampleCount = min(OVERSAMPLE_MAX, overSampleCount);
}

/************************************************************************************
*                          P R I V A T E   F U N C T I O N S                        *
*************************************************************************************
*/
