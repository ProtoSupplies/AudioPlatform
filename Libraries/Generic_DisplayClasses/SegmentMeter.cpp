/************************************************************************************
*                                  SegmentMeter.cpp                                 *
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
#include "SegmentMeter.h"
#include "ArduinoEclipseDefs.h"

/************************************************************************************
*                         M A C R O S  /  C O N S T A N T S                         *
*************************************************************************************
*/

/************************************************************************************
*                    S T A T I C   C L A S S   V A R I A B L E S                    *
*************************************************************************************
*/

/************************************************************************************
*                    P U B L I C   C L A S S   F U N C T I O N S                    *
*************************************************************************************
*/

// Static Member Initializations
//

// Constructor/Destructor
//
SegmentMeter::SegmentMeter(SegmentMeterProfile& _profile) :
    meterProfile(_profile) {

    segmentCount = 0;
    currentSegment = 0;

    meterInitialized = false;
}

SegmentMeter::~SegmentMeter() {

}

// initialize
//
void SegmentMeter::initialize() {

    segmentCount    = meterProfile.levelLowSegmentCount
                    + meterProfile.levelNormalSegmentCount
                    + meterProfile.levelHighSegmentCount
                    + meterProfile.levelPeakSegmentCount;

//    Serial.printf("\n\nSegmentMeter::initialize: segmentCount %d\n", segmentCount);

    levelLowIndex   = meterProfile.levelLowSegmentCount;
    levelNormIndex  = levelLowIndex + meterProfile.levelNormalSegmentCount;
    levelHighIndex  = levelNormIndex + meterProfile.levelHighSegmentCount;
    levelPeakIndex  = levelHighIndex + meterProfile.levelPeakSegmentCount;

//    Serial.printf("SegmentMeter::initialize: levelLowIndex %d levelNormalIndex %d levelHighIndex %d levelPeakIndex %d\n", levelLowIndex, levelNormIndex, levelHighIndex, levelPeakIndex);

    meterRange      = meterProfile.maxValue - meterProfile.minValue;

    currentSegment  = 0;

    currentSegmentX = meterX;
    currentSegmentY = meterY;

//    Serial.printf("SegmentMeter::initialize: meterRange %d\n", meterRange);
//    Serial.printf("SegmentMeter::initialize: currentSegmentX %d currentSegmentY %d\n", currentSegmentX, currentSegmentY);
//    Serial.printf("SegmentMeter::initialize: incrementX %d incrementY %d\n", incrementX, incrementY);

    if (meterProfile.orientation == meterVertical) {
        incrementX      = 0;
        incrementY      = (meterProfile.barSegmentHeight + meterProfile.barSegmentGap);
        meterHeight     = incrementY * segmentCount;
        meterWidth      = meterProfile.barSegmentWidth;

    } else {
        incrementX      = (meterProfile.barSegmentWidth + meterProfile.barSegmentGap);
        incrementY      = 0;
        meterWidth      = incrementX * segmentCount;
        meterHeight     = meterProfile.barSegmentHeight;
    }
}

// clearMeter
//
void SegmentMeter::clearMeter() {

    DisplayScreenBase::drawRect(meterX-1, meterY-1, meterWidth+2, meterHeight+2, METER_OUTLINE_COLOR);
    DisplayScreenBase::drawRect(meterX-2, meterY-2, meterWidth+4, meterHeight+4, METER_OUTLINE_COLOR);
    DisplayScreenBase::fillRect(meterX, meterY, meterWidth, meterHeight, METER_BGND_COLOR);

    currentSegment  = 0;
    segmentXY(currentSegment, currentSegmentX, currentSegmentY);
}

// drawMeter
//
void SegmentMeter::drawMeter(uint16_t x, uint16_t y) {

    if (meterProfile.orientation == meterVertical) {
        drawMeterVert(x, y);

    } else {
        drawMeterHzntl(x, y);
    }

    drawTics();
    labelTics();
}

// updateMeter
//
void SegmentMeter::updateMeter(int16_t newValue) {

    if (meterProfile.orientation == meterVertical) {
        updateMeterVert(newValue);
    } else {
        updateMeterHzntl(newValue);
    }
}

/************************************************************************************
*        P R I V A T E / P R O T E C T E D   C L A S S   F U N C T I O N S          *
*************************************************************************************
*/


// segmentXY
//
void SegmentMeter::segmentXY(uint16_t segment, int16_t& x, int16_t& y) {

    if (meterProfile.orientation == meterVertical) {
        x = meterX;
        y = meterY + meterHeight - segment * incrementY;

    } else {
        x = meterX + segment * incrementX;
        y = meterY;
    }
}

// segmentColor
//
uint16_t SegmentMeter::segmentColor(uint16_t segment) {

    uint16_t color = LEVEL_LOW_COLOR;

    if (segment >= levelHighIndex) {
        color = LEVEL_PEAK_COLOR;
    } else if (segment >= levelNormIndex) {
        color = LEVEL_HIGH_COLOR;
    } else if (segment >= levelLowIndex) {
        color = LEVEL_NORMAL_COLOR;
    }

    return color;
}

// drawTics
//
void SegmentMeter::drawTics() {

    if (meterProfile.orientation == meterVertical && meterProfile.ticPlacement != meterTicsNone) {
        drawTicsVert();
    } else if (meterProfile.orientation == meterHorizontal && meterProfile.ticPlacement != meterTicsNone) {
        drawTicsHzntl();
    }
}

// labelTics
//
void SegmentMeter::labelTics() {

    if (meterProfile.orientation == meterVertical && meterProfile.ticPlacement != meterTicsNone && meterProfile.showLabels) {
        labelTicsVert();
    } else if (meterProfile.orientation == meterHorizontal && meterProfile.ticPlacement != meterTicsNone && meterProfile.showLabels) {
        labelTicsHzntl();
    }
}

// drawMeterVert
//
void SegmentMeter::drawMeterVert(uint16_t x, uint16_t y) {

    meterX = x;
    meterY = y;

    DisplayScreenBase::drawRect(meterX-1, meterY-1, meterWidth+2, meterHeight+2, METER_OUTLINE_COLOR);
    DisplayScreenBase::drawRect(meterX-2, meterY-2, meterWidth+4, meterHeight+4, METER_OUTLINE_COLOR);

    uint16_t segmentX;
    uint16_t segmentY;

    segmentX = meterX;
    segmentY = meterY + meterHeight - incrementY;

    for (uint16_t segment = 0; segment < segmentCount; segment++) {
        DisplayScreenBase::fillRect(segmentX, segmentY, meterProfile.barSegmentWidth, meterProfile.barSegmentHeight, segmentColor(segment));
        segmentX += incrementX;
        segmentY -= incrementY;
    }

    currentSegmentX = meterX;
    currentSegmentY = meterY;
    currentSegment = segmentCount - 1;
}

// updateMeterVert
//
void SegmentMeter::updateMeterVert(int16_t newValue) {

    if (newValue < meterProfile.minValue) {
        newValue = meterProfile.minValue;
    } else if (newValue > meterProfile.maxValue) {
        newValue = meterProfile.maxValue;
    }

    uint16_t newSegment = UtilFns::scale(newValue, meterProfile.minValue, meterProfile.maxValue, 0, segmentCount-1);
    int16_t newSegmentX, newSegmentY;
    segmentXY(newSegment+1, newSegmentX, newSegmentY);

    // High to Low (Simply erase area above new segment)
    if (newSegment < currentSegment) {
        uint16_t fillX = newSegmentX;
        uint16_t fillY = currentSegmentY;
        uint16_t fillWid =  meterProfile.barSegmentWidth;
        uint16_t fillHgt = newSegmentY - currentSegmentY;
        if (newSegment == 0) {
            fillHgt += incrementY;
        }

        DisplayScreenBase::fillRect(fillX, fillY, fillWid, fillHgt, METER_BGND_COLOR);
        currentSegmentY = newSegmentY;

        currentSegment = newSegment;

    // Low to High (Fill each segment above individually)
    } else if (newSegment > currentSegment) {
        int16_t segmentY = currentSegmentY;

        for (uint16_t segment = currentSegment; segment <= newSegment; segment++) {
            uint16_t fillX = meterX;
            uint16_t fillY = segmentY; // + ((segment == 0) ?  1 : 0);
            uint16_t fillWid = meterProfile.barSegmentWidth;
            uint16_t fillHgt = meterProfile.barSegmentHeight + ((segment == 0) ?  1 : 0);

            DisplayScreenBase::fillRect(fillX, fillY, fillWid, fillHgt, segmentColor(segment));
            currentSegmentY = segmentY;
            segmentY -= incrementY;
        }
    }

    currentSegment = newSegment;
}

// drawTicsVert
//
void SegmentMeter::drawTicsVert() {

    uint16_t ticWid = 6;
    uint16_t ticCount = meterProfile.ticInfo.ticCount;
    int16_t ticX = meterX;
    int16_t ticY = meterY;

    for (uint16_t ticNum = 0; ticNum < ticCount; ticNum++) {
        uint16_t segmentNum = meterProfile.ticInfo.ticIndexes[ticNum];
        segmentXY(segmentNum, ticX, ticY);
//        Serial.printf("SegmentMeter::drawTicsVert: ticNum %d ticX %d ticY %d\n", ticNum, ticX, ticY);

        if (meterProfile.ticPlacement == meterTicsLeft) {
            ticX = ticX - ticWid;
        } else if (meterProfile.ticPlacement == meterTicsRight) {
            ticX = ticX + meterProfile.barSegmentWidth;
        }

        DisplayScreenBase::drawLine(ticX, ticY, ticX + ticWid, ticY, COLOR_WHITE);
    }
}

// labelTicsVert
//
void SegmentMeter::labelTicsVert() {

    uint16_t labelCount = meterProfile.ticInfo.ticCount;
    const char* labelValue;
    int16_t labelX;
    int16_t labelY;
    uint16_t ticWid = 6;

    // Display label values using a text area object as a boundary for the text
    TextArea tArea;
    tArea.wid = 20;
    tArea.hgt = 20;
    if (meterProfile.ticPlacement == meterTicsLeft) {
        tArea.align = TextAlignRight;
    } else if (meterProfile.ticPlacement == meterTicsRight) {
        tArea.align = TextAlignLeft;
    }

    for (uint16_t labelNum = 0; labelNum < labelCount; labelNum++) {
        uint16_t segmentNum = meterProfile.ticInfo.ticIndexes[labelNum];
        segmentXY(segmentNum, labelX, labelY);

        labelValue = meterProfile.ticInfo.ticLabels[labelNum];
        if (meterProfile.ticPlacement == meterTicsLeft) {
            labelX = meterX - ticWid - tArea.wid - 5;
            labelY = labelY - (tArea.hgt / 2);
        } else if (meterProfile.ticPlacement == meterTicsRight) {
            labelX = meterX + meterProfile.barSegmentWidth + ticWid + 5;
            labelY = labelY - (tArea.hgt / 2);
        }

        tArea.posX = labelX;
        tArea.posY = labelY;
        DisplayScreenBase::setFont(Arial_9);
        DisplayScreenBase::printInArea(labelValue, tArea);
    }
}

// drawMeterHzntl
//
void SegmentMeter::drawMeterHzntl(uint16_t x, uint16_t y) {

    meterX = x;
    meterY = y;

    DisplayScreenBase::drawRect(meterX-1, meterY-1, meterWidth+2, meterHeight+2, METER_OUTLINE_COLOR);
    DisplayScreenBase::drawRect(meterX-2, meterY-2, meterWidth+4, meterHeight+4, METER_OUTLINE_COLOR);

    uint16_t segmentX = meterX;

    for (uint16_t segment = 0; segment < segmentCount; segment++) {
        DisplayScreenBase::fillRect(segmentX, meterY, meterProfile.barSegmentWidth, meterProfile.barSegmentHeight, segmentColor(segment));
        segmentX += incrementX;
    }

    currentSegment = segmentCount - 1;
}

// updateMeterHzntl
//
void SegmentMeter::updateMeterHzntl(int16_t newValue) {

    if (newValue < meterProfile.minValue) {
        newValue = meterProfile.minValue;
    } else if (newValue > meterProfile.maxValue) {
        newValue = meterProfile.maxValue;
    }

    segmentXY(currentSegment, currentSegmentX, currentSegmentY);

    uint16_t newSegment = UtilFns::scale(newValue, meterProfile.minValue, meterProfile.maxValue, 0, segmentCount-1);
    int16_t newSegmentX, newSegmentY;;
    segmentXY(newSegment, newSegmentX, newSegmentY);

    if (newSegment < currentSegment) {
        // Adjust for segment 0 which has no previous segments
        uint16_t fillX = newSegmentX + ((newSegment == 0) ? 0 : meterProfile.barSegmentWidth);
        uint16_t fillY = meterY;
        uint16_t fillWid = currentSegmentX - newSegmentX + incrementX;
        // Adjust for segment 0 which has no previous segments
        fillWid -= ((newSegment == 0) ? 0 : incrementX);
        uint16_t fillHgt = meterProfile.barSegmentHeight;

        DisplayScreenBase::fillRect(fillX, fillY, fillWid, fillHgt, METER_BGND_COLOR);

        currentSegment = newSegment;

    } else if (newSegment > currentSegment) {
        uint16_t segmentX = currentSegmentX;

        for (uint16_t segment = currentSegment; segment <= newSegment; segment++) {
            uint16_t fillX = segmentX;
            uint16_t fillY = meterY;
            uint16_t fillWid = meterProfile.barSegmentWidth;
            uint16_t fillHgt = meterProfile.barSegmentHeight;

            DisplayScreenBase::fillRect(fillX, fillY, fillWid, fillHgt, segmentColor(segment));
            segmentX += incrementX;
        }
    }

    currentSegment = newSegment;
}

// drawTicsHzntl
//
void SegmentMeter::drawTicsHzntl() {

    uint16_t ticHgt = 6;
    uint16_t ticCount = meterProfile.ticInfo.ticCount;
    int16_t ticX = meterX;
    int16_t ticY = meterY;

    for (uint16_t ticNum = 0; ticNum < ticCount; ticNum++) {
        uint16_t segmentNum = meterProfile.ticInfo.ticIndexes[ticNum];
        segmentXY(segmentNum, ticX, ticY);
//        Serial.printf("SegmentMeter::drawTicsHzntl: ticNum %d ticX %d ticY %d\n", ticNum, ticX, ticY);

        if (meterProfile.ticPlacement == meterTicsBelow) {
            ticY = ticY + meterProfile.barSegmentHeight;
        } else if (meterProfile.ticPlacement == meterTicsAbove) {
            ticY = ticY - ticHgt;
        }

        DisplayScreenBase::drawLine(ticX, ticY, ticX, ticY + ticHgt, COLOR_WHITE);
    }
}

// labelTicsHzntl
//
void SegmentMeter::labelTicsHzntl() {

    uint16_t labelCount = meterProfile.ticInfo.ticCount;
    const char* labelValue;
    int16_t labelX;
    int16_t labelY;

    // Display label values using a text area object as a boundary for the text
    TextArea tArea;
    tArea.wid = 20;
    tArea.hgt = 20;
    tArea.align = TextAlignCenter;

    for (uint16_t labelNum = 0; labelNum < labelCount; labelNum++) {
        uint16_t segmentNum = meterProfile.ticInfo.ticIndexes[labelNum];
        segmentXY(segmentNum, labelX, labelY);
//        Serial.printf("SegmentMeter::labelTicsHzntl: segmentNum %d labelX %d labelY %d\n", labelNum, labelX, labelY);

        labelValue = meterProfile.ticInfo.ticLabels[labelNum];
        tArea.posX = labelX - 10;
        if (meterProfile.ticPlacement == meterTicsBelow) {
            labelY = labelY + meterProfile.barSegmentHeight + 9;
        } else if (meterProfile.ticPlacement == meterTicsAbove) {
            labelY = labelY - 25;
        }
        tArea.posY = labelY;

        DisplayScreenBase::setFont(Arial_9);
        DisplayScreenBase::printInArea(labelValue, tArea);
    }
}


