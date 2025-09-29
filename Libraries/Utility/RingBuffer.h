/************************************************************************************
*                                  RingBuffer.h                                     *
*                      Copyright (c) 2025, Donald Swearingen                        *
*                               All Rights Reserved                                 *
*************************************************************************************
*
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
#ifndef RINGBUFFER_H_
#define RINGBUFFER_H_

/************************************************************************************
*                       E X T E R N A L   R E F E R E N C E S                       *
*************************************************************************************
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdint.h>

/************************************************************************************
*                         M A C R O S  /  C O N S T A N T S                         *
*************************************************************************************
*/

/************************************************************************************
*                     D A T A / T Y P E   D E F I N I T I O N S                     *
*************************************************************************************
*/

// Ring buffer error codes
//
typedef enum {
    RBUF_ERR_NONE,
    RBUF_ERR_NOTPOW2,       // specified buffer size is not a power of 2
    RBUF_ERR_TXOVERRUN,     // Tx buffer overrun. Characters lost.
    RBUF_ERR_RXOVERRUN,     // Rx buffer overrun. Characters lost.
    RBUF_ERR_LESSDATA,      // less data written/read than requested
    RBUF_ERR_BUFFULL,       // buffer is full; can't write
    RBUF_ERR_BUFEMPTY       // buffer is empty; can't read
} RingBufferError;

/************************************************************************************
*                         C L A S S   D E F I N I T I O N S                         *
*************************************************************************************
*/

class RingBuffer {

// Constructor/Destructor
//
public:
    RingBuffer();
    virtual ~RingBuffer();

// Class Variables
//
public:

private:

    uint16_t            bufferSize;     // Number of values in FIFO. Must be power of 2.
    volatile int32_t    writeIndex;     // Index of next writable value.
    volatile int32_t    readIndex;      // Index of next readable value.
    int32_t             bigMask;        // Used for wrapping indices with extra bit
                                        // to distinguish full/empty.
    int32_t             smallMask;      // Used for fitting indices to buffer.
    int32_t             *buffer;

protected:

// Class Methods
//
public:

    RingBufferError     init(uint16_t bufferSize);
    RingBufferError     putVal(int32_t val);
    RingBufferError     getVal(int32_t *val);
    uint16_t            getWriteAvailable();
    uint16_t            getReadAvailable();
    void                setWriteIndex(int32_t newIndex);
    void                flush();

private:

    int32_t             advanceWriteIndex(int32_t numValues);
    int32_t             advanceReadIndex(int32_t numValues);

protected:

};
#endif // RINGBUFFER_H_
