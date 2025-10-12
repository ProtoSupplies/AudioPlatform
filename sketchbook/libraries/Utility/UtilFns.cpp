/************************************************************************************
*                                    UtilFns.cpp                                    *
*                           General Use Utility Fuunctions                          *
*                                                                                   *
*                      Copyright (c) 2025, Donald Swearingen                        *
*                              All Rights Reserved                                  *
*************************************************************************************
*
* Description
*
*   UtilFns is a STATIC class designed providing a number of general-purpose pure
*   functions useful in a variety of circumstances.
*
* Modifications List
*
*   • TODO
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
#include "UtilFns.h"

/************************************************************************************
*                    S T A T I C   C L A S S   V A R I A B L E S                    *
*************************************************************************************
*/

// Initialization of static class variables
//

/************************************************************************************
*                           C L A S S   F U N C T I O N S                           *
*************************************************************************************
*/


// Constructor/Destructor
//
UtilFns::UtilFns() {

    // Initialize random number generator
    srand(time(0));
}

UtilFns::~UtilFns() {
}

// scale
//
// Map a value from one range to another
//
// TODO: Handle rounding (or not)?
//
float UtilFns::scale(float val, float fromMin, float fromMax, float toMin, float toMax) {

    if (val < fromMin) val = fromMin;
    if (val > fromMax) val = fromMax;

    float fromRange = fromMax - fromMin;
    float toRange = toMax - toMin;

    float fromSpan = 0.5;
    if (fromRange > 0.0) {
        fromSpan = (val - fromMin) / fromRange;  // 0.0-1.0
    }

    return toMin + fromSpan * toRange;
}

// deJitter
//
bool UtilFns::deJitter(int32_t newVal, int32_t lastVal, uint8_t& direction, uint8_t threshold) {

    bool newOut = false;

    long diff = newVal - lastVal;
    if (diff == 0) {
        //
    } else if (direction == DIR_UP) {
        // current dir is up
        if (diff < 0) {
            // and new direction is down
            // but it must be by more than DEJITTER_THRESHOLD
            if (diff < -threshold) {
                direction = DIR_DOWN;
                newOut = true;
            }
        } else {
            // direction is still up
            newOut = true;
        }
    } else {
        // current dir is down
        if (diff > 0) {
            // and new direction is up
            // but it must be by more than DEJITTER_THRESHOLD
            if (diff > threshold) {
                direction = DIR_UP;
                newOut = true;
            }
        } else {
            // direction is still down
            newOut = true;
        }
    }

    return newOut;
}

// dbToGain
//
// Convert a value in deciBels to a gain value
//
float UtilFns::dbToGain(float dbVal) {

    return pow(10.0, dbVal / 20.0);
}

// gainToDb
//
// Convert a gain value to a deciBel value
//
float UtilFns::gainToDb(float gainVal) {

    return 20.0 * log10(gainVal);
}

// uint32ToString
//
// Convert a uint32_t number to a string with commas in the 1000s digits
// (Note that uint32_t numbers are <= 10 digits, so the output buffer
// should be at least of this length + 1)
//
void UtilFns::uint32ToString(uint32_t number, char* outBuf) {

    char tempBuf[16];
    uint8_t outBufLen = 0;

    // Print number in LR order to temporary character buffer
    uint8_t nDigits = (uint8_t)log10(number) + 1;       // # of digits in number
    snprintf(tempBuf, nDigits + 1, "%lu", number);      // print to buffer

    // Scan buffered number in RL (nDigits - i) order, creating output with commas
    for (int i = nDigits; i > 0; i--) {
        // If this digit (except for last in number) is a
        // multiple of 3, precede with comma in outBuf
        if ((i < nDigits) && (i % 3 == 0)) {
            outBuf[outBufLen] = ',';
            outBufLen++;
        }
        // Add input digit from current (leftmost) position in
        // tempBuf to current (rightmost) position of outBuf
        outBuf[outBufLen] = tempBuf[nDigits - i];
        outBufLen++;
    }
    // Terminate output string
    outBuf[outBufLen] = 0;
}

// floatToInt
//
// Convert a floating point value to integer with proper rounding
//
int UtilFns::floatToInt(float floatVal) {

    int retVal = 0;

    if (floatVal < 0.0) {
        retVal = (int) (floatVal - 0.5);
    } else {
        retVal = (int) (floatVal + 0.5);
    }

    return retVal;
}

// getRand
//
// Return a random number in the specified range
//
int UtilFns::getRand(int randMin, int randMax) {

    return randMin + (rand() % (randMax - randMin));
}

