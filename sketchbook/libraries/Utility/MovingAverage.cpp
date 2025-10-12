/************************************************************************************
*                                MovingAverage.cpp                                  *
*                      Copyright (c) 2025, Donald Swearingen                        *
*                               All Rights Reserved                                 *
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

/************************************************************************************
*                       E X T E R N A L   R E F E R E N C E S                       *
*************************************************************************************
*/

#include <stdlib.h>

#include "MovingAverage.h"

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
MovingAverage::MovingAverage(uint8_t length) {

    mAvgIndex = 0;
    mAvgSum = 0;
    mAvgVal = 0.0;

    setLength(length);
}

// Destructor
//
// Free buffer, if allocated.
//
MovingAverage::~MovingAverage() {

    if (mAvgData != 0) {
        free(mAvgData);
        mAvgData = 0;
    }
}

// getNextOutput
//
// Return the next output value of the moving average.
//
float MovingAverage::getNextOutput(int32_t inVal) {

    mAvgVal = (float) inVal;
    if (mAvgLen > 1) {
        mAvgSum = mAvgSum - mAvgData[mAvgIndex];
        mAvgData[mAvgIndex] = inVal;
        mAvgSum = mAvgSum + mAvgData[mAvgIndex];

        mAvgIndex++;
        if (mAvgIndex >= mAvgLen) {
            mAvgIndex = 0;
        }

        mAvgVal =  (float)mAvgSum / (float)mAvgLen;
    }

    return mAvgVal;
}

// setLength
//
// Set new buffer length, allocating if necessary.
//
void MovingAverage::setLength(uint8_t length) {

    // Restrict length to maximum
    uint8_t newLength = length;
    if (newLength > MAVG_MAX_LEN) {
        newLength = MAVG_MAX_LEN;
    }

    // If new length is 0 or 1, no buffer needed.
    // getNextOutput() will simply return its input.
    //
    if (newLength <= 1 || newLength > mAvgLen) {
        if (mAvgData != 0) {
            free(mAvgData);
            mAvgData = 0;
        }
    }

    if (newLength > 1 && newLength > mAvgLen) {
        mAvgData = (int32_t *)malloc(newLength * sizeof(int));
    }

    clearContents(newLength);
    mAvgLen = newLength;
    mAvgSum = 0;
    mAvgIndex = 0;
}

// getLength
//
// Return current buffer length.
//
uint8_t MovingAverage::getLength() {

    return mAvgLen;
}

// getContents
//
// Return current contents in order of the last mAvgLength values.
//
void MovingAverage::getContents(int32_t *buffer) {

    if (mAvgData != 0 && buffer != 0) {
        uint8_t index = mAvgIndex;
        uint8_t count = 0;

        while (count < mAvgLen) {
            buffer[count] = mAvgData[index];
            count++;
            index++;
            if (index >= mAvgLen) {
                index = 0;
            }
        }
    }
}

/************************************************************************************
*                          P R I V A T E   F U N C T I O N S                        *
*************************************************************************************
*/

// clearContents
//
// Set all values in data buffer to 0, reset indices and output value.
//
void MovingAverage::clearContents(uint8_t length) {

    if (mAvgData != 0) {
        for (uint8_t i = 0; i < length; i++) {
            mAvgData[i] = 0;
        }
    }
}
