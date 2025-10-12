 /************************************************************************************
*                                     BitMap.h                                      *
*                                                                                   *
*                       Copyright (c) 2025, Donald Swearingen                       *
*                                All Rights Reserved                                *
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

/****************
* "Pre-Defines" *
*****************
*
* Definitions that affect subsequent external references
*
*/


/************************************************************************************
*                       E X T E R N A L   R E F E R E N C E S                       *
*************************************************************************************
*/

#ifndef BITMAP_H_
#define BITMAP_H_

// User Arduino Libraries
//
#include "DisplayScreenBase.h"

/************************************************************************************
*                         M A C R O S  /  C O N S T A N T S                         *
*************************************************************************************
*/

/************************************************************************************
*                     D A T A / T Y P E   D E F I N I T I O N S                     *
*************************************************************************************
*/

typedef struct __attribute__ ((__packed__)) {

    uint16_t    signature;
    uint32_t    fileSize;
    uint32_t    reserved;
    uint32_t    dataOffset;

    uint32_t    infoHdrSize;
    uint32_t    bitmapWidth;
    uint32_t    bitmapHeight;
    uint16_t    planeCount;
    uint16_t    bitsPerPixel;
    uint32_t    compressionType;
    uint32_t    compressedImageSize;
    uint32_t    pixelsPerMeterX;
    uint32_t    pixelsPerMeterY;
    uint32_t    colorsUsed;
    uint32_t    colorsSignificant;

} BitmapHeader;

typedef uint8_t* BitMapDataBlock;


/************************************************************************************
*                         C L A S S   D E F I N I T I O N S                         *
*************************************************************************************
*/

// NOTE: Basically a static class

class BitMap {

// Constructor/Destructor
//
public:

    BitMap();
    virtual ~BitMap();

// Class Variables
//
public:


private:


protected:


// Class Methods
//
public:

    static void         drawBitMap(BitMapDataBlock bmpSrc, uint16_t posX, uint16_t posY);
    static void         showHdrInfo(BitmapHeader* bitmapHdr);

private:

    static uint16_t     Color565(uint8_t r,uint8_t g, uint8_t b);
    static uint16_t     arrRead16(uint8_t arr[], uint32_t& arrIndex);
    static uint32_t     arrRead32(uint8_t arr[], uint32_t& arrIndex);
    static void         writeClippedRect(int32_t x, int32_t y, int32_t cx, int32_t cy, uint16_t *pixels);

protected:

};

#endif // BITMAP_H_
