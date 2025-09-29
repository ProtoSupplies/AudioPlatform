/************************************************************************************
*                                 RingBuffer.cpp                                    *
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
*
*/

/************************************************************************************
*                       E X T E R N A L   R E F E R E N C E S                       *
*************************************************************************************
*/

#include "RingBuffer.h"

/************************************************************************************
*                 P R I V A T E   F U N C T I O N   P R O T O T Y P E S             *
*************************************************************************************
*/
/************************************************************************************
*                            P U B L I C   F U N C T I O N S                        *
*************************************************************************************
*/

RingBuffer::RingBuffer() {
    // TODO Auto-generated constructor stub

}

RingBuffer::~RingBuffer() {
    // TODO Auto-generated destructor stub
}

// init()
//
// numValues must be power of 2, returns error if not.
//
RingBufferError RingBuffer::init(uint16_t size)
{
    if (((size-1) & size) != 0)
        return RBUF_ERR_NOTPOW2;      // Not Power of two

    bufferSize = size;
//    buffer = (int32_t *)dataPtr;
    buffer = (int32_t *)malloc(size * sizeof(int32_t));
    flush();
    bigMask = (bufferSize * 2) - 1;
    smallMask = (bufferSize) - 1;

    return RBUF_ERR_NONE;
}

// putVal()
//
// Write a value, if space available, to the specified buffer
//
RingBufferError RingBuffer::putVal(int32_t val)
{
    int32_t index;

    if (!getWriteAvailable()) {
        return RBUF_ERR_BUFFULL;
    }

    index = writeIndex & smallMask;
    buffer[index] = val;
    advanceWriteIndex(1);

    return RBUF_ERR_NONE;
}

// getVal()
//
// Read a value, if available, from the  specified buffer
//
RingBufferError RingBuffer::getVal(int32_t *val)
{
    int32_t index;

    if (!getReadAvailable()) {
        return RBUF_ERR_BUFEMPTY;
    }

    index = readIndex & smallMask;
    *val = buffer[index];
    advanceReadIndex(1);

    return RBUF_ERR_NONE;
}


// getReadAvailable()
//
// Return number of values available for reading.
//
uint16_t RingBuffer::getReadAvailable()
{
    return ((writeIndex - readIndex) & bigMask);
}

// getWriteAvailable()
//
// Return number of values available for writing.
//
uint16_t RingBuffer::getWriteAvailable()
{
    return (bufferSize - getReadAvailable());
}

// flush()
//
// Clear buffer. Should only be called when buffer is NOT being read.
//
void RingBuffer::flush()
{
    writeIndex = readIndex = 0;
}

/************************************************************************************
*                           P R I V A T E   F U N C T I O N S                       *
*************************************************************************************
*/

// advanceWriteIndex()
//
int32_t RingBuffer::advanceWriteIndex(int32_t numValues)
{
    return writeIndex = (writeIndex + numValues) & bigMask;
}

// advanceReadIndex()
//
int32_t RingBuffer::advanceReadIndex(int32_t numValues)
{
    return readIndex = (readIndex + numValues) & bigMask;
}

