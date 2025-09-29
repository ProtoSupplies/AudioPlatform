/************************************************************************************
*                                 MovingAverage.h                                   *
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

#ifndef MOVINGAVERAGE_H_
#define MOVINGAVERAGE_H_

#include <stdint.h>

/************************************************************************************
*                         M A C R O S  /  C O N S T A N T S                         *
*************************************************************************************
*/

#define MAVG_MAX_LEN    32

/************************************************************************************
*                     D A T A / T Y P E   D E F I N I T I O N S                     *
*************************************************************************************
*/

/************************************************************************************
*                         C L A S S   D E F I N I T I O N S                         *
*************************************************************************************
*/

class MovingAverage {

// Constructor/Destructor
//
public:
    MovingAverage(uint8_t length);
    virtual ~MovingAverage();

// Class Variables
//
public:

private:

    // Moving Average state variables
    int32_t     *mAvgData;
    int32_t     mAvgSum;        // Sum of current contents
    float       mAvgVal;        // Current output value
    uint8_t     mAvgIndex;      // Current position in mAvgData
    uint8_t     mAvgLen;        // Maximum length (set in setLength())

protected:

// Class Methods
//
public:

    float   getNextOutput(int32_t inVal);
    void    setLength(uint8_t length);
    uint8_t getLength();
    void    getContents(int32_t *buffer);

private:

    void    clearContents(uint8_t length);

protected:

};

#endif /* MOVINGAVERAGE_H_ */

