/************************************************************************************
*                                 MonitorScreen.cpp                                 *
*                                                                                   *
*                       Copyright (c) 2025, Donald Swearingen                       *
*                                All Rights Reserved                                *
*************************************************************************************
*
* Description
*
*   MonitorScreen monitors the activities of the Teensy audio system in real-time,
*   based on various timers that determine update rates. The monitored quantities
*   include CPU and memory usage as well as the current peak output levels of the
*   left and right audio outputs.
*
* Single Instance Class
*
*   MonitorScreen is a custom (single instance) subclass of base class DisplayScreenBase.
*   Though it exists as a single instance, it is not defined as a static class, which
*   allows the instance to be managed by the application's screen display mechanisms
*   in the same manner as other screens which represent multiple subclass instances of
*   DisplayScreenBase and its derivatives.
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
* Modifications List
*
*   •
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

#include "MonitorScreen.h"
#include "ArduinoEclipseDefs.h"

/************************************************************************************
*                         M A C R O S  /  C O N S T A N T S                         *
*************************************************************************************
*/

// Initial Meter Layout
//
static const int32_t leftRightDistance  = 55;       // vertical
static const int32_t leftChanMeterX     = 70;
static const int32_t leftChanMeterY     = 160;
static const int32_t rightChanMeterX    = leftChanMeterX;
static const int32_t rightChanMeterY    = leftChanMeterY + leftRightDistance;

static const int32_t midiInLabelX       = rightChanMeterX + 85;
static const int32_t midiInLabelY       = rightChanMeterY + 120;
static const int32_t midiInIndicatorX   = midiInLabelX + 110;
static const int32_t midiInIndicatorY   = midiInLabelY;
static const int32_t midiInWidHgt       = 20;
static const int16_t midiInColorOff     = COLOR_RED;
static const int16_t midiInColorOn      = COLOR_GREEN;

static const int32_t cpuToMemBlkGap     = 200;      // vertical
static const int32_t avgToMaxGap        = 170;      // horizontal
static const int32_t usageMetersX       = 480;
static const int32_t usageMetersY       = 100;
static const int32_t cpuAvgMeterX       = usageMetersX;
static const int32_t cpuAvgMeterY       = usageMetersY;
static const int32_t cpuMaxMeterX       = cpuAvgMeterX + avgToMaxGap;
static const int32_t cpuMaxMeterY       = cpuAvgMeterY;
static const int32_t memBlkAvgMeterX    = usageMetersX;
static const int32_t memBlkAvgMeterY    = usageMetersY + cpuToMemBlkGap;
static const int32_t memBlkMaxMeterX    = memBlkAvgMeterX + avgToMaxGap;
static const int32_t memBlkMaxMeterY    = memBlkAvgMeterY;

/************************************************************************************
*                    S T A T I C   C L A S S   V A R I A B L E S                    *
*************************************************************************************
*/

/*********
* Screen *
**********
*/
AudioAnalyzePeak        MonitorScreen::peakLeft;
AudioAnalyzePeak        MonitorScreen::peakRight;

AudioConnection         MonitorScreen::patchCordPeakLeft;
AudioConnection         MonitorScreen::patchCordPeakRight;

/***************
* Audio Meters *
****************
*/
static uint16_t         audioMeterTicSegs[]   = {0, 8, 16, 24, 32, 40};
static const char*      audioMeterTicLabels[] = {"-80", "-60", "-40", "-20", "0", "+20"};

static MeterTicInfo audioMeterTicInfo = (MeterTicInfo) {

    6,                  // ticCount
    audioMeterTicSegs,  // ticIndexes
    audioMeterTicLabels // ticLabels

};

static SegmentMeterProfile leftChanMeterProfile = (SegmentMeterProfile) {

    6,                  // barSegmentWidth
    18,                 // barSegmentHeight
    1,                  // barSegmentGap
    16,                 // levelLowSegmentCount
    16,                 // levelNormalSegmentCount
    4,                  // levelHighSegmentCount
    4,                  // levelPeakSegmentCount
    -80,                // minValue
    20,                 // maxValue
    meterHorizontal,    // orientation
    meterTicsBelow,     // ticPlacement
    audioMeterTicInfo,  // ticInfo;
    true                // showLabels
};

static SegmentMeterProfile rightChanMeterProfile = (SegmentMeterProfile) {

    6,                  // barSegmentWidth
    18,                 // barSegmentHeight
    1,                  // barSegmentGap
    16,                 // levelLowSegmentCount
    16,                 // levelNormalSegmentCount
    4,                  // levelHighSegmentCount
    4,                  // levelPeakSegmentCount
    -80,                // minValue
    20,                 // maxValue
    meterHorizontal,    // orientation
    meterTicsAbove,     // ticPlacement
    audioMeterTicInfo,  // ticInfo;
    false               // showLabels
};

/*******************
* CPU Usage Meters *
********************
*/
static uint16_t         cpuMeterTicSegs[]   = {0, 5, 10, 15, 20};
static const char*      cpuMeterTicLabels[] = {"0", "25", "50", "75", "100"};

static MeterTicInfo cpuMeterTicInfo = (MeterTicInfo) {

    5,                  // ticCount
    cpuMeterTicSegs,    // ticIndexes
    cpuMeterTicLabels   // ticLabels

};

static SegmentMeterProfile cpuAvgMeterProfile = (SegmentMeterProfile) {

    20,                 // barSegmentWidth
    5,                  // barSegmentHeight
    1,                  // barSegmentGap
    5,                  // levelLowSegmentCount
    5 ,                 // levelNormalSegmentCount
    5,                  // levelHighSegmentCount
    5,                  // levelPeakSegmentCount
    0,                  // minValue
    100,                // maxValue
    meterVertical,      // orientation
    meterTicsLeft,      // ticPlacement
    cpuMeterTicInfo,    // ticInfo;
    true                // showLabels
};

static SegmentMeterProfile cpuMaxMeterProfile = (SegmentMeterProfile) {

    20,                 // barSegmentWidth
    5,                  // barSegmentHeight
    1,                  // barSegmentGap
    5,                  // levelLowSegmentCount
    5 ,                 // levelNormalSegmentCount
    5,                  // levelHighSegmentCount
    5,                  // levelPeakSegmentCount
    0,                  // minValue
    100,                // maxValue
    meterVertical,      // orientation
    meterTicsRight,     // ticPlacement
    cpuMeterTicInfo,    // ticInfo;
    true                // showLabels
};

/**********************
* Memory Usage Meters *
***********************
*
* NOTE: The upper tic segment counts, and corresponding labels should be set to
* correspond to the current value of the constant AUDIO_MEMORY_BLOCK_TOTAL, which
* is determined and set in module AudioDevices.h (currently 100).
*
* For example, if the value is changed to 120, in order to maintain the same meter
* height, the values should be:
*
*       memBlkMeterTicSegs[]   = {0, 5, 10, 15, 20};
*       memBlkMeterTicLabels[] = {"0", "30", "60", "90", "120"};
*
* and the maxValue fields of memBlkAvgMeterProfile and memBlkMaxMeterProfile should
* be set to 120:
*       .
*       .
*       .
*       0,                  // minValue
*       120,                // maxValue
*       .
*       .
*       .
* In this case, each segment would represent a count of 6 blocks.
*
*/

static uint16_t         memBlkMeterTicSegs[]   = {0, 5, 10, 15, 20};
static const char*      memBlkMeterTicLabels[] = {"0", "25", "50", "75", "100"};

static MeterTicInfo memBlkMeterTicInfo = (MeterTicInfo) {

    5,                  // ticCount
    memBlkMeterTicSegs, // ticIndexes
    memBlkMeterTicLabels // ticLabels

};

static SegmentMeterProfile memBlkAvgMeterProfile = (SegmentMeterProfile) {

    20,                 // barSegmentWidth
    5,                  // barSegmentHeight
    1,                  // barSegmentGap
    5,                  // levelLowSegmentCount
    5 ,                 // levelNormalSegmentCount
    5,                  // levelHighSegmentCount
    5,                  // levelPeakSegmentCount
    0,                  // minValue
    100,                // maxValue
    meterVertical,      // orientation
    meterTicsLeft,      // ticPlacement
    memBlkMeterTicInfo, // ticInfo;
    true                // showLabels
};

static SegmentMeterProfile memBlkMaxMeterProfile = (SegmentMeterProfile) {

    20,                 // barSegmentWidth
    5,                  // barSegmentHeight
    1,                  // barSegmentGap
    5,                  // levelLowSegmentCount
    5 ,                 // levelNormalSegmentCount
    5,                  // levelHighSegmentCount
    5,                  // levelPeakSegmentCount
    0,                  // minValue
    100,                // maxValue
    meterVertical,      // orientation
    meterTicsRight,     // ticPlacement
    memBlkMeterTicInfo, // ticInfo;
    true                // showLabels
};

/************************************************************************************
*                    P U B L I C   C L A S S   F U N C T I O N S                    *
*************************************************************************************
*/

// Static Member Initializations
//

// Constructor/Destructor
//
MonitorScreen::MonitorScreen(DisplayScreenInfo& displayInfo, AudioMixer8& outMixerLeft, AudioMixer8& outMixerRight) :
    DisplayScreenBase(displayInfo), _outMixerLeft(&outMixerLeft), _outMixerRight(&outMixerRight),
    leftChanMeter(leftChanMeterProfile), rightChanMeter(rightChanMeterProfile),
    cpuAvgMeter(cpuAvgMeterProfile), cpuMaxMeter(cpuMaxMeterProfile),
    memBlkAvgMeter(memBlkAvgMeterProfile), memBlkMaxMeter(memBlkMaxMeterProfile),
    statusDisplay() {

}

MonitorScreen::~MonitorScreen() {

}

// activateScreen
//
// Reset/display dynamic screen elements each time screen is displayed
//
void MonitorScreen::activateScreen() {

    // Screen setup
    clearScreen();
    showScreenHeading();

    patchCordPeakLeft.connect(*_outMixerLeft, peakLeft);
    patchCordPeakRight.connect(*_outMixerRight, peakRight);

    // Draw RealTime Display Meters
    drawSplMeters();
    drawUsageMeters();

    // Draw MIDI Input Indicator
    midiReceived = false;
    midiInLedState = LedOff;
    drawMidiIndicator();

    // Reset display update timers
    midiInUpdateTime = 0;
    splUpdateTime = 0;
    statsUpdateTime = statsUpdateInterval;      // Force immediate display on window activation

    // Draw footer status display
    statusDisplay.initialize();
}

// deActivateScreen
//
void MonitorScreen::deActivateScreen() {

    // Disconnect patchcords to reduce audio system CPU
    // usage when screen is not on display
    //
    patchCordPeakLeft.disconnect();
    patchCordPeakRight.disconnect();
}

// pollScreen
//
ButtonId MonitorScreen::pollScreen() {

    // First carry out real-time monitoring operations
    monitorActivity();

    // Then check for button presses
    ButtonId touchedBtn = BtnNone;
    uint16_t pointX, pointY;

    // Check for touched buttons
    if (DisplayScreenBase::touched()) {
        DisplayScreenBase::getTouchedPoint(pointX, pointY);

        // First check for Home/Back button. If pressed, we'll return that value to main application.
        if (DisplayScreenBase::prevBtnTouched(pointX, pointY)) {
            touchedBtn = BtnPrev;

        } else {
            touchedBtn = BtnNone;   // handled locally
        }
    }

    return touchedBtn;
}

// updateScreen
//
// Called when new MIDI data received
//
void MonitorScreen::updateScreen(StatusUpdatePacketType pktType, void* updateInfoPkt) {

//    Serial.printf("MonitorScreen::updateScreen: \n");

    StatusUpdatePacket* pkt = (StatusUpdatePacket*) updateInfoPkt;

    if (pktType == MidiRxPacket) {
        // Indicate new MIDI data. Will be reported and reset on regular intervals.
        if (!midiReceived) {
            midiReceived = true;
            updateMidiIndicator();
            midiInUpdateTime = 0;
        }

    } else if (pktType == StatusPacket && statusDisplay.displayInitialized) {
        statusDisplay.updateDisplay(pktType, pkt);
    }
}

/************************************************************************************
*                   P R I V A T E   C L A S S   F U N C T I O N S                   *
*************************************************************************************
*/

// monitorActivity
//
// Monitor and display audio system usage and output levels in real-time
//
void MonitorScreen::monitorActivity() {

    // Sample/Manage left/right SPL display indicators at regular intervals
    if (splUpdateTime > splUpdateInterval) {
        updateSplMeters();
        splUpdateTime = 0;
    }

    // Update system usage statistics at regular intervals
    if (statsUpdateTime > statsUpdateInterval) {
        updateUsageMeters();
        statsUpdateTime = 0;
    }

    // Update Midi Input Indicator
    if (midiInUpdateTime > midiInUpdateInterval) {
        updateMidiIndicator();
        midiInUpdateTime = 0;
    }
}

// drawSplMeters
//
// Draw and label SPL meters
//
void MonitorScreen::drawSplMeters() {

    char textBuf[32];
    TextArea tArea;

    // Display L/R SPL Meters
    leftChanMeter.initialize();
    leftChanMeter.drawMeter(leftChanMeterX, leftChanMeterY);
    rightChanMeter.initialize();
    rightChanMeter.drawMeter(rightChanMeterX, rightChanMeterY);

    // Left/Right Meter Labels
    DisplayScreenBase::setTextColor(COLOR_WHITE);
    DisplayScreenBase::setFont(Arial_12);
    tArea.hgt = 20;
    tArea.wid = 10;
    tArea.align = TextAlignRight;
    tArea.posX = leftChanMeter.meterX - (tArea.wid + 10);

    tArea.posY = leftChanMeter.meterY;
    snprintf(textBuf, sizeof(textBuf)-1, "L");
    DisplayScreenBase::printInArea(textBuf, tArea);
    tArea.posY = rightChanMeter.meterY;
    snprintf(textBuf, sizeof(textBuf)-1, "R");
    DisplayScreenBase::printInArea(textBuf, tArea);

    // Meter Descriptor
    DisplayScreenBase::setTextColor(COLOR_WHITE);
    DisplayScreenBase::setFont(Arial_13);
    tArea.hgt = 20;
    tArea.wid = leftChanMeter.meterWidth;
    tArea.posX = leftChanMeter.meterX;
    tArea.align = TextAlignCenter;
    tArea.posY = leftChanMeter.meterY - (tArea.hgt + 15);
    snprintf(textBuf, sizeof(textBuf)-1, "Line Out SPL Levels");
    DisplayScreenBase::printInArea(textBuf, tArea);

    // Meter Update Interval
    DisplayScreenBase::setTextColor(COLOR_WHITE);
    DisplayScreenBase::setFont(Arial_12);

    tArea.posY = rightChanMeter.meterY + rightChanMeter.meterHeight + 10;
    snprintf(textBuf, sizeof(textBuf)-1, "%d ms Peak Level", splUpdateInterval);
    DisplayScreenBase::printInArea(textBuf, tArea);
}

// updateSplMeters
//
// Periodically update SPL meter display
//
void MonitorScreen::updateSplMeters() {

    // New peak levels detected
    if (peakLeft.available() && peakRight.available()) {

        // Gain values are in range [0,1]
        float gainLeft = peakLeft.read();
        float gainRight = peakRight.read();

        // Convert gain levels to deciBels: dbValue = 20 * log10(gainValue).
        // The gain to dB conversion will return 0 dB for a gain of 1.
        // Note that log10(0) is undefined, and will return inf, which will
        // effectively be discarded by the "db >" checks applied next.
        float dbLeft = UtilFns::gainToDb(gainLeft);
        if (dbLeft < -80) {
            // cutoff at -80dB
            dbLeft = -80;
        } else if (dbLeft > 20) {
            dbLeft = 20;
        }
        float dbRight = UtilFns::gainToDb(gainRight);
        if (dbRight < -80) {
            // cutoff at -80dB
            dbRight = -80;
        } else if (dbRight > 20) {
            dbRight = 20;
        }

        leftChanMeter.updateMeter((int16_t)dbLeft);
        rightChanMeter.updateMeter((int16_t)dbRight);
    }
}

// drawUsageMeters
//
// Draw and label Resource Usage meters
//
void MonitorScreen::drawUsageMeters() {

    char textBuf[32];
    TextArea tArea;

    // Display Usage Meters
    cpuAvgMeter.initialize();
    cpuAvgMeter.drawMeter(cpuAvgMeterX, cpuAvgMeterY);
    cpuMaxMeter.initialize();
    cpuMaxMeter.drawMeter(cpuMaxMeterX, cpuMaxMeterY);
    memBlkAvgMeter.initialize();
    memBlkAvgMeter.drawMeter(memBlkAvgMeterX, memBlkAvgMeterY);
    memBlkMaxMeter.initialize();
    memBlkMaxMeter.drawMeter(memBlkMaxMeterX, memBlkMaxMeterY);

    // Avg/Max Columns Labels
    tArea.hgt = 20;
    tArea.align = TextAlignCenter;
    tArea.wid = cpuAvgMeterProfile.barSegmentWidth + 30;

    DisplayScreenBase::setTextColor(COLOR_WHITE);
    DisplayScreenBase::setFont(Arial_12);

    tArea.posX = memBlkAvgMeter.meterX - 15;
    tArea.posY = memBlkAvgMeter.meterY - 55;
    snprintf(textBuf, sizeof(textBuf)-1, "Avg");
    DisplayScreenBase::printInArea(textBuf, tArea);

    tArea.posX = memBlkMaxMeter.meterX - 15;
    tArea.posY = memBlkMaxMeter.meterY - 55;
    snprintf(textBuf, sizeof(textBuf)-1, "Max");
    DisplayScreenBase::printInArea(textBuf, tArea);

    // Update Interval
    tArea.posX = cpuAvgMeter.meterX + cpuAvgMeterProfile.barSegmentWidth;
    tArea.wid = cpuMaxMeter.meterX - tArea.posX;
    tArea.posY = memBlkMaxMeter.meterY - 55;
//    Serial.printf("MonitorScreen::activateScreen: 500ms label tArea.posX %d tArea.posY %d tArea.wid %d\n", tArea.posX, tArea.posY, tArea.wid);
    snprintf(textBuf, sizeof(textBuf)-1, "%d ms", statsUpdateInterval);
    DisplayScreenBase::printInArea(textBuf, tArea);

    // CPU/AudioBlock Usage Descriptors
    DisplayScreenBase::setFont(Arial_13);

    tArea.posY = cpuAvgMeter.meterY + (memBlkAvgMeter.meterHeight / 2) - 10;
    snprintf(textBuf, sizeof(textBuf)-1, "CPU %%");
    DisplayScreenBase::printInArea(textBuf, tArea);

    tArea.posY = memBlkAvgMeter.meterY + (memBlkAvgMeter.meterHeight / 2) - 10;
    snprintf(textBuf, sizeof(textBuf)-1, "Audio Blocks");
    DisplayScreenBase::printInArea(textBuf, tArea);
}

// updateUsageMeters
//
// Periodically update CPU and Memory Usage displays
//
void MonitorScreen::updateUsageMeters() {

    char textBuf[16];

    // TODO: Make this a callable function
    TextArea tArea;
    tArea.wid = cpuAvgMeterProfile.barSegmentWidth + 30;
    tArea.hgt = 20;
    tArea.align = TextAlignCenter;

    DisplayScreenBase::setFont(Arial_9);
    DisplayScreenBase::setTextColor(COLOR_WHITE);

    uint16_t cpuAvg = UtilFns::scale(AudioProcessorUsage(), 0, 100, 0, 100);
    cpuAvgMeter.updateMeter(cpuAvg);
    tArea.posX = cpuAvgMeterX - 15;
    tArea.posY = cpuAvgMeterY - 25;
    DisplayScreenBase::fillRect(tArea.posX, tArea.posY, tArea.wid, tArea.hgt, COLOR_BLACK);
    snprintf(textBuf, sizeof(textBuf)-1, "%d%%", cpuAvg);
    DisplayScreenBase::printInArea(textBuf, tArea);

    uint16_t cpuMax = UtilFns::scale(AudioProcessorUsageMax(), 0, 100, 0, 100);
    cpuMaxMeter.updateMeter(cpuMax);
    tArea.posX = cpuMaxMeterX - 15;
    tArea.posY = cpuMaxMeterY - 25;
    DisplayScreenBase::fillRect(tArea.posX, tArea.posY, tArea.wid, tArea.hgt, COLOR_BLACK);
    snprintf(textBuf, sizeof(textBuf)-1, "%d%%", cpuMax);
    DisplayScreenBase::printInArea(textBuf, tArea);

    uint16_t memBlkAvg = UtilFns::scale(AudioMemoryUsage(), 0, 100, 0, 100);
    memBlkAvgMeter.updateMeter(memBlkAvg);
    tArea.posX = memBlkAvgMeterX - 15;
    tArea.posY = memBlkAvgMeterY - 25;
    DisplayScreenBase::fillRect(tArea.posX, tArea.posY, tArea.wid, tArea.hgt, COLOR_BLACK);
    snprintf(textBuf, sizeof(textBuf)-1, "%d", memBlkAvg);
    DisplayScreenBase::printInArea(textBuf, tArea);

    uint16_t memBlkMax = UtilFns::scale(AudioMemoryUsageMax(), 0, 100, 0, 100);
    memBlkMaxMeter.updateMeter(memBlkMax);
    tArea.posX = memBlkMaxMeterX - 15;
    tArea.posY = memBlkMaxMeterY - 25;
    DisplayScreenBase::fillRect(tArea.posX, tArea.posY, tArea.wid, tArea.hgt, COLOR_BLACK);
    snprintf(textBuf, sizeof(textBuf)-1, "%d", memBlkMax);
    DisplayScreenBase::printInArea(textBuf, tArea);

    AudioProcessorUsageMaxReset();
    AudioMemoryUsageMaxReset();
}

// drawMidiIndicator
//
// Draw and label MIDI input indicator
//
void MonitorScreen::drawMidiIndicator() {

    char textBuf[16];

    TextArea tArea;
    tArea.wid = 100;
    tArea.hgt = 20;
    tArea.posX = midiInLabelX;
    tArea.posY = midiInLabelY;
    tArea.align = TextAlignRight;

    DisplayScreenBase::setFont(Arial_13);
    DisplayScreenBase::setTextColor(COLOR_WHITE);

    snprintf(textBuf, sizeof(textBuf)-1, "MIDI Received");
    DisplayScreenBase::printInArea(textBuf, tArea);

    DisplayScreenBase::fillRect(midiInIndicatorX, midiInIndicatorY, midiInWidHgt, midiInWidHgt, midiInColorOff);
}

// updateMidiIndicator
//
// Update MIDI input indicator
//
void MonitorScreen::updateMidiIndicator() {

    if (midiReceived) {
        if (midiInLedState == LedOff) {
            midiInLedState = LedOn;
            DisplayScreenBase::fillRect(midiInIndicatorX, midiInIndicatorY, midiInWidHgt, midiInWidHgt, midiInColorOn);
        }
        midiReceived = false;

    } else {
        if (midiInLedState == LedOn) {
            midiInLedState = LedOff;
            DisplayScreenBase::fillRect(midiInIndicatorX, midiInIndicatorY, midiInWidHgt, midiInWidHgt, midiInColorOff);
        }
    }
}
