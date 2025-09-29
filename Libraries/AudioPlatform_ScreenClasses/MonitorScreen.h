/************************************************************************************
*                                  MonitorScreen.h                                  *
*                                                                                   *
*                       Copyright (c) 2025, Donald Swearingen                       *
*                                All Rights Reserved                                *
*************************************************************************************
*
* Description
*
*   Create and manage a screen for the dynamic display of Teensy Audio System activity.
*
*   See MonitorScreen.cpp for additional details.
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

#ifndef MONITORSCREEN_H_
#define MONITORSCREEN_H_

// Teensyduino
//
#include <Audio.h>
#include <ElapsedMillis.h>

// User Arduino Libraries
//
#include "DisplayScreenBase.h"
#include "UtilFns.h"
#include "AudioMixer8.h"
#include "SegmentMeter.h"
#include "AudioPlatformGlobal.h"
#include "StatusDisplay.h"


/************************************************************************************
*                         M A C R O S  /  C O N S T A N T S                         *
*************************************************************************************
*/

const bool LedOn  = true;
const bool LedOff = false;

/************************************************************************************
*                     D A T A / T Y P E   D E F I N I T I O N S                     *
*************************************************************************************
*/

// Peak Display Bar IDs
typedef enum {
    peakBarLeft,
    peakBarRight
} PeakBarId;

// Profile of individual range settings for peak value bar display
typedef struct {
    int16_t     topTickValue;       // Value of top tick
    int16_t     tickValueDelta;     // Difference between tick values
} TickRangeSetting;

/************************************************************************************
*                         C L A S S   D E F I N I T I O N S                         *
*************************************************************************************
*/

class MonitorScreen : public DisplayScreenBase {

// Constructor/Destructor
//
public:

    MonitorScreen(DisplayScreenInfo& displayInfo, AudioMixer8& outMixerLeft, AudioMixer8& outMixerRight);
    virtual ~MonitorScreen();

// Class Variables
//
public:


private:

    // Panels for Display of Usage and Peak Range Selection
//    ButtonPanel             usageInfoPanel;

    static AudioAnalyzePeak peakLeft;
    static AudioAnalyzePeak peakRight;

    static AudioConnection  patchCordPeakLeft;
    static AudioConnection  patchCordPeakRight;

    AudioMixer8*            _outMixerLeft;
    AudioMixer8*            _outMixerRight;

    const char*             usageLabels[4] = {"CPU Usage", "CPU Usage Max", "Memory Usage","Memory Usage Max"};

    // Display Update Timers and Limits (all in ms)
    elapsedMillis           splUpdateTime;                          // time since last update
    const uint16_t          splUpdateInterval = 20;                 // update period
    elapsedMillis           statsUpdateTime;
    const uint16_t          statsUpdateInterval = 500;
    elapsedMillis           midiInUpdateTime;
    const uint16_t          midiInUpdateInterval = 250;
    bool                    midiReceived;
    bool                    midiInLedState;

    SegmentMeter            leftChanMeter;
    SegmentMeter            rightChanMeter;

    SegmentMeter            cpuAvgMeter;
    SegmentMeter            cpuMaxMeter;
    SegmentMeter            memBlkAvgMeter;
    SegmentMeter            memBlkMaxMeter;

protected:

    // Footer status display
    StatusDisplay           statusDisplay;

// Class Methods
//
public:

    // Public functions visible to main application
    void                    activateScreen() override;      // clear and display screen
    void                    deActivateScreen() override;    // save screen parameters before closing
    ButtonId                pollScreen();
    void                    updateScreen(StatusUpdatePacketType pktType, void* updateInfoPkt) override;

private:

    // Class internal functions
    void                    monitorActivity();
    void                    drawSplMeters();
    void                    updateSplMeters();
    void                    drawUsageMeters();
    void                    updateUsageMeters();
    void                    drawMidiIndicator();
    void                    updateMidiIndicator();

protected:


};

#endif // MONITORSCREEN_H_
