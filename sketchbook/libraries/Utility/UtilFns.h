/************************************************************************************
*                                     UtilFns.h                                     *
*                           General Use Utility Fuunctions                          *
*                                                                                   *
*                      Copyright (c) 2025, Donald Swearingen                        *
*                               All Rights Reserved                                 *
*************************************************************************************
*
* Description
*
*   UtilFns is a STATIC class designed providing a number of general-purpose pure
*   functions useful in a variety of circumstances.
*
* Modifications List
*
*   • Something to do
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

#ifndef UTILFNS_H_
#define UTILFNS_H_

/************************************************************************************
*                       E X T E R N A L   R E F E R E N C E S                       *
*************************************************************************************
*/

// Standard C++
//
#include <Math.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

/************************************************************************************
*                         M A C R O S  /  C O N S T A N T S                         *
*************************************************************************************
*/

#define DIR_UP              0
#define DIR_DOWN            1

/************************************************************************************
*                     D A T A / T Y P E   D E F I N I T I O N S                     *
*************************************************************************************
*/

/************************************************************************************
*                         C L A S S   D E F I N I T I O N S                         *
*************************************************************************************
*/

class UtilFns {

public:

    UtilFns();
    virtual ~UtilFns();

    static float    scale(float val, float fromMin, float fromMax, float toMin, float toMax);
    static bool     deJitter(int32_t newVal, int32_t lastVal, uint8_t& direction, uint8_t threshold);
    static float    dbToGain(float dbVal);
    static float    gainToDb(float gainVal);
    static void     uint32ToString(uint32_t number, char* outBuf);
    static int      floatToInt(float floatVal);
    static int      getRand(int randMin, int randMax);
};

#endif // UTILFNS_H_
