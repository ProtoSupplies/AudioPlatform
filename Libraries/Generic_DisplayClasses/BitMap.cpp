/************************************************************************************
*                                    BitMap.cpp                                     *
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

// User Arduino Libraries
//
#include "BitMap.h"
#include "ArduinoEclipseDefs.h"

/************************************************************************************
*                         M A C R O S  /  C O N S T A N T S                         *
*************************************************************************************
*/

/******************
* BitMap Defaults *
*******************
*/

#define RGB888_BYTES_PER_PIXEL  3
#define RGB565_BYTES_PER_PIXEL  2

/************************************************************************************
*                    S T A T I C   C L A S S   V A R I A B L E S                    *
*************************************************************************************
*/

// Buffer for single image line of converted to RGB565
EXTMEM uint16_t imageLinePixels[DISPLAY_WIDTH * RGB565_BYTES_PER_PIXEL];

/************************************************************************************
*                    P U B L I C   C L A S S   F U N C T I O N S                    *
*************************************************************************************
*/

// Static Member Initializations
//

// Constructor/Destructor
//
BitMap::BitMap() {

}

BitMap::~BitMap() {

}

// showHdrInfo
//
void BitMap::showHdrInfo(BitmapHeader* bitmapHdr) {

    if (bitmapHdr) {
        Serial.println();
        Serial.printf("%20s: %x\n", "signature",             bitmapHdr->signature);
        Serial.printf("%20s: %d\n", "fileSize",              bitmapHdr->fileSize);
        Serial.printf("%20s: %d\n", "reserved",              bitmapHdr->reserved);
        Serial.printf("%20s: %d\n", "dataOffset",            bitmapHdr->dataOffset);
        Serial.printf("%20s: %d\n", "infoHdrSize",           bitmapHdr->infoHdrSize);
        Serial.printf("%20s: %d\n", "bitmapWidth",           bitmapHdr->bitmapWidth);
        Serial.printf("%20s: %d\n", "bitmapHeight",          bitmapHdr->bitmapHeight);
        Serial.printf("%20s: %d\n", "planeCount",            bitmapHdr->planeCount);
        Serial.printf("%20s: %d\n", "bitsPerPixel",          bitmapHdr->bitsPerPixel);
        Serial.printf("%20s: %d\n", "compressionType",       bitmapHdr->compressionType);
        Serial.printf("%20s: %d\n", "compressedImageSize",   bitmapHdr->compressedImageSize);
        Serial.printf("%20s: %d\n", "pixelsPerMeterX",       bitmapHdr->pixelsPerMeterX);
        Serial.printf("%20s: %d\n", "pixelsPerMeterY",       bitmapHdr->pixelsPerMeterY);
        Serial.printf("%20s: %d\n", "colorsUsed",            bitmapHdr->colorsUsed);
        Serial.printf("%20s: %d\n", "colorsSignificant",     bitmapHdr->colorsSignificant);
    }
}

// drawBitMap
//
void BitMap::drawBitMap(BitMapDataBlock bmpData, uint16_t posX, uint16_t posY) {

    if (!bmpData) {
        return;
    }

    uint32_t rowSize;                     // Not always = image_width; may have padding
    uint8_t r, g, b;

    uint32_t pos = 0;
    uint32_t srcIndex = 0;

    BitmapHeader* bmpHdr = (BitmapHeader*) bmpData;

    // Parse BMP header
    if (bmpHdr->signature == 0x4D42) {
        uint32_t bmpImageoffset = bmpHdr->dataOffset;
        int32_t image_width     = bmpHdr->bitmapWidth;
        int32_t image_height    = bmpHdr->bitmapHeight;

        if ((bmpHdr->planeCount == 1) && (bmpHdr->bitsPerPixel == 24) && (bmpHdr->compressionType == 0)) {  // 0 = uncompressed
            // BMP rows are padded (if needed) to 4-byte boundary
            rowSize = (image_width * 3 + 3) & ~3;

            // If image_height is negative, image is in top-down order.
            // This is not canon but has been observed in the wild.
            if (image_height < 0) {
                image_height = -image_height;
            }

            for (uint16_t row = 0; row < image_height; row++) {  // For each scanline...
                pos = bmpImageoffset + row * rowSize;
                srcIndex = pos;

                for (uint16_t col = 0; col < image_width; col++) {  // For each pixel...
                    // Convert pixel from BMP to TFT format, push to display
                    b = bmpData[srcIndex++];
                    g = bmpData[srcIndex++];
                    r = bmpData[srcIndex++];
                    imageLinePixels[col] = Color565(r, g, b);

                }  // end pixel

                writeClippedRect(posX + 0, posY + row, image_width, 1, imageLinePixels);

            } // end scanline
        }
    }
}

/************************************************************************************
*                   P R I V A T E   C L A S S   F U N C T I O N S                   *
*************************************************************************************
*/

// Color565
//
uint16_t BitMap::Color565(uint8_t r, uint8_t g, uint8_t b) {

    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

// arrRead16
//
uint16_t BitMap::arrRead16(uint8_t arr[], uint32_t& arrIndex) {

    uint16_t result;

    ((uint8_t *)&result)[0] = arr[arrIndex++];  // LSB
    ((uint8_t *)&result)[1] = arr[arrIndex++];  // MSB

    return result;
}

// arrRead32
//
uint32_t BitMap::arrRead32(uint8_t arr[], uint32_t& arrIndex) {

    uint32_t result;

    ((uint8_t *)&result)[0] = arr[arrIndex++];  // LSB
    ((uint8_t *)&result)[1] = arr[arrIndex++];
    ((uint8_t *)&result)[2] = arr[arrIndex++];
    ((uint8_t *)&result)[3] = arr[arrIndex++];  // MSB

    return result;
}

// writeClippedRect
//
void BitMap::writeClippedRect(int32_t x, int32_t y, int32_t cx, int32_t cy, uint16_t *pixels) {

    int32_t end_x = x + cx;
    int32_t end_y = y + cy;

//    Serial.printf("BitMap::writeClippedRect: x %d y %d end_x %d end_y %d\n", x, y, end_x, end_y);

    if ((x >= 0) && (y >= 0) && (end_x <= DISPLAY_WIDTH) && (end_y <= DISPLAY_HEIGHT)) {
        DisplayScreenBase::writeRect(x, y, cx, cy, pixels);

    } else if ((end_x >= 0) && (end_y >= 0) && (x < DISPLAY_WIDTH) && (y < DISPLAY_HEIGHT)) {
        int16_t cx_out = cx;
        int16_t cy_out = cy;

        if (x < 0) {
            pixels += -x;  // point to first word we will use.
            cx_out += x;
            x = 0;
        }

        if (end_x > DISPLAY_WIDTH) {
            cx_out -= (end_x - DISPLAY_WIDTH);
        }

        if (y < 0) {
            pixels += (-y * cx);  // point to first word we will use.
            cy_out += y;
            y = 0;
        }

        if (end_y > DISPLAY_HEIGHT) {
            cy_out -= (end_y - DISPLAY_HEIGHT);
        }

        if (cx_out && cy_out) {
            if (cy_out > 1) {
                //compress the buffer
                uint16_t* pixels_out = pixels;
                uint16_t* p = pixels;
                end_y = cy_out;  // reuse variable
                while (--end_y) {
                    p += cx_out;       // increment to where we will copy the pixels to
                    pixels_out += cx;  // increment by one full row
                    memcpy(p, pixels_out, cx_out * sizeof(uint16_t));
                }
            }
            DisplayScreenBase::writeRect(x, y, cx_out, cy_out, pixels);
        }
    }
}
