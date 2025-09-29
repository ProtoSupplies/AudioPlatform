/************************************************************************************
*                                   SegmentMeter.h                                  *
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

#ifndef SEGMENTMETERHV_H_
#define SEGMENTMETERHV_H_

// User Arduino Libraries
//
#include "DisplayScreenBase.h"
#include "UtilFns.h"

/************************************************************************************
*                         M A C R O S  /  C O N S T A N T S                         *
*************************************************************************************
*/

#define METER_OUTLINE_COLOR COLOR_WHITE
#define METER_BGND_COLOR    COLOR_BLACK
#define LEVEL_LOW_COLOR     COLOR_CORNFLOWERBLUE
#define LEVEL_NORMAL_COLOR  COLOR_LIMEGREEN
#define LEVEL_HIGH_COLOR    COLOR_DARKORANGE
#define LEVEL_PEAK_COLOR    COLOR_CRIMSON

/************************************************************************************
*                     D A T A / T Y P E   D E F I N I T I O N S                     *
*************************************************************************************
*/

typedef enum {

    meterVertical,
    meterHorizontal

} MeterOrientation;

typedef enum {

    meterTicsLeft,
    meterTicsRight,
    meterTicsBelow,
    meterTicsAbove,
    meterTicsNone

} MeterTicPlacement;

typedef struct {

    uint16_t            ticCount;
    uint16_t*           ticIndexes;
    const char**        ticLabels;

} MeterTicInfo;

typedef struct {

    uint16_t            barSegmentWidth;
    uint16_t            barSegmentHeight;
    uint16_t            barSegmentGap;

    uint16_t            levelLowSegmentCount;
    uint16_t            levelNormalSegmentCount;
    uint16_t            levelHighSegmentCount;
    uint16_t            levelPeakSegmentCount;

    int16_t             minValue;
    int16_t             maxValue;

    MeterOrientation    orientation;
    MeterTicPlacement   ticPlacement;
    MeterTicInfo        ticInfo;
    bool                showLabels;

} SegmentMeterProfile;

/************************************************************************************
*                         C L A S S   D E F I N I T I O N S                         *
*************************************************************************************
*/

class SegmentMeter {

// Constructor/Destructor
//
public:

    SegmentMeter(SegmentMeterProfile& _profile);
    virtual ~SegmentMeter();

// Class Variables
//
public:

    int16_t            meterX;
    int16_t            meterY;
    uint16_t           meterWidth;
    uint16_t           meterHeight;

private:


protected:

    SegmentMeterProfile&  meterProfile;

    uint16_t            segmentCount;
    uint16_t            currentSegment;
    int16_t             currentSegmentX;
    int16_t             currentSegmentY;
    int16_t             incrementX;
    int16_t             incrementY;

    uint16_t            levelLowIndex;
    uint16_t            levelNormIndex;
    uint16_t            levelHighIndex;
    uint16_t            levelPeakIndex;

    uint16_t            meterRange;

    bool                meterInitialized;


// Class Methods
//
public:

    void                initialize();
    void                clearMeter();
    void                drawMeter(uint16_t x, uint16_t y);
    void                updateMeter(int16_t newValue);

private:


protected:

    void                segmentXY(uint16_t segment, int16_t& x, int16_t& y);
    uint16_t            segmentColor(uint16_t segment);

    void                drawTics();
    void                labelTics();

    void                drawMeterVert(uint16_t x, uint16_t y);
    void                updateMeterVert(int16_t newValue);
    void                drawTicsVert();
    void                labelTicsVert();
    void                drawMeterHzntl(uint16_t x, uint16_t y);
    void                updateMeterHzntl(int16_t newValue);
    void                drawTicsHzntl();
    void                labelTicsHzntl();
};

#endif // SEGMENTMETERHV_H_
