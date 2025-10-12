/************************************************************************************
*                                 StatusDisplay.cpp                                 *
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
#include "StatusDisplay.h"

/************************************************************************************
*                         M A C R O S  /  C O N S T A N T S                         *
*************************************************************************************
*/

const uint16_t statusInfoDisplayHgt   = 35;

/************************************************************************************
*                    S T A T I C   C L A S S   V A R I A B L E S                    *
*************************************************************************************
*/

static OnOffDisplayInfo statusDisplayOnOffInfo = (OnOffDisplayInfo) {
    COLOR_BLACK,                // bgndColor
    Arial_11_Bold,              // font
    COLOR_WHITE,                // textColor
    ONOFF_TEXT_MARGIN_DEFAULT,  // textMargin
    COLOR_BLACK                 // outlineColor
};

static NumberNameDisplayInfo statusDisplayPresetDisplayInfo = (NumberNameDisplayInfo) {
    COLOR_BLACK,                // bgndColor
    Arial_11_Bold,              // font
    COLOR_WHITE,                // textColor
    ONOFF_TEXT_MARGIN_DEFAULT,  // textMargin
    COLOR_BLACK                 // outlineColor
};


/************************************************************************************
*                    P U B L I C   C L A S S   F U N C T I O N S                    *
*************************************************************************************
*/

// Static Member Initializations
//

// Constructor/Destructor
//
StatusDisplay::StatusDisplay() :
    wavPlyrStatusDisplay(statusDisplayOnOffInfo,     0,  450, statusInfoDisplayHgt, "WavPlyr"),
    rawPlyrStatusDisplay(statusDisplayOnOffInfo,     10, 450, statusInfoDisplayHgt, "RawPlyr"),
    synthStatusDisplay(statusDisplayOnOffInfo,       20, 450, statusInfoDisplayHgt, "Synth"),
    fmSynthStatusDisplay(statusDisplayOnOffInfo,     30, 450, statusInfoDisplayHgt, "FmSynth"),
    lineInStatusDisplay(statusDisplayOnOffInfo,      40, 450, statusInfoDisplayHgt, "LineIn"),
    presetDisplay(statusDisplayPresetDisplayInfo,    50, 450, statusInfoDisplayHgt) {

    currentPresetNumber = 1;
    currentPresetName[0] = '\0';

    wavPlayerEnabled    = false;
    rawPlayerEnabled    = false;
    basicSynthEnabled   = false;
    fmSynthEnabled      = false;
    lineInEnabled       = false;

    displayInitialized  = false;
}

StatusDisplay::~StatusDisplay() {

}

// initialize
//
void StatusDisplay::initialize() {

    // Initialize calls to set object widths
    lineInStatusDisplay.initialize();
    fmSynthStatusDisplay.initialize();
    synthStatusDisplay.initialize();
    rawPlyrStatusDisplay.initialize();
    wavPlyrStatusDisplay.initialize();

    drawDisplay();

    displayInitialized  = true;
}

// updateDisplay
//
void StatusDisplay::updateDisplay(StatusUpdatePacketType pktType, void* updateInfoPkt) {

    StatusUpdatePacket* pkt = (StatusUpdatePacket*) updateInfoPkt;

    if (pktType == StatusPacket) {
        if (currentPresetNumber != pkt->presetNumber || strcmp(currentPresetName, pkt->presetName) != 0) {
//            Serial.printf("StatusDisplay::updateDisplay: Preset %d: %s\n", currentPresetNumber, currentPresetName);
//            Serial.printf("StatusDisplay::updateDisplay: New Preset %d: %s\n", pkt->presetNumber, pkt->presetName);

            strncpy(currentPresetName, pkt->presetName, sizeof(currentPresetName) - 1);
            currentPresetNumber = pkt->presetNumber;
            presetDisplay.clearToWidth();
            presetDisplay.setPrefix("Preset");
            presetDisplay.setNumberAndName(currentPresetNumber, currentPresetName);
            presetDisplay.drawDisplay();
//            Serial.printf("StatusDisplay::updateDisplay: Preset %d: %s\n", currentPresetNumber, currentPresetName);
        }

        if (wavPlayerEnabled != pkt->wavPlayerEnabled) {
            wavPlayerEnabled = pkt->wavPlayerEnabled;
            wavPlyrStatusDisplay.setState(wavPlayerEnabled);
//            Serial.printf("StatusDisplay::updateDisplay: wavPlayerEnabled %d\n", pkt->wavPlayerEnabled);
        }

        if (rawPlayerEnabled != pkt->rawPlayerEnabled) {
            rawPlayerEnabled = pkt->rawPlayerEnabled;
            rawPlyrStatusDisplay.setState(rawPlayerEnabled);
//            Serial.printf("StatusDisplay::updateDisplay: rawPlayerEnabled %d\n", pkt->rawPlayerEnabled);
        }

        if (basicSynthEnabled != pkt->basicSynthEnabled) {
            basicSynthEnabled = pkt->basicSynthEnabled;
            synthStatusDisplay.setState(basicSynthEnabled);
//            Serial.printf("StatusDisplay::updateDisplay: basicSynthEnabled %d\n", pkt->basicSynthEnabled);
        }

        if (fmSynthEnabled != pkt->fmSynthEnabled) {
            fmSynthEnabled = pkt->fmSynthEnabled;
            fmSynthStatusDisplay.setState(fmSynthEnabled);
//            Serial.printf("StatusDisplay::updateDisplay: fmSynthEnabled %d\n", pkt->fmSynthEnabled);
        }

        if (lineInEnabled != pkt->lineInEnabled) {
            lineInEnabled = pkt->lineInEnabled;
            lineInStatusDisplay.setState(lineInEnabled);
//            Serial.printf("StatusDisplay::updateDisplay: lineInEnabled %d\n", pkt->lineInEnabled);
        }
    }
}

/************************************************************************************
*                   P R I V A T E   C L A S S   F U N C T I O N S                   *
*************************************************************************************
*/

// drawDisplay
//
void StatusDisplay::drawDisplay() {

    // OnOffDisplay Classes
    //
    uint16_t objX = DISPLAY_WIDTH - 10;
    uint16_t objY = DISPLAY_HEIGHT - 35;

    // Right most first
    objX -= lineInStatusDisplay.getWidth();
    lineInStatusDisplay.setPosition(objX, objY);
    lineInStatusDisplay.drawDisplay();
    lineInStatusDisplay.setState(lineInEnabled);

    objX -= fmSynthStatusDisplay.getWidth();
    fmSynthStatusDisplay.setPosition(objX, objY);
    fmSynthStatusDisplay.drawDisplay();
    fmSynthStatusDisplay.setState(fmSynthEnabled);

    objX -= synthStatusDisplay.getWidth();
    synthStatusDisplay.setPosition(objX, objY);
    synthStatusDisplay.drawDisplay();
    synthStatusDisplay.setState(basicSynthEnabled);

    objX -= rawPlyrStatusDisplay.getWidth();
    rawPlyrStatusDisplay.setPosition(objX, objY);
    rawPlyrStatusDisplay.drawDisplay();
    rawPlyrStatusDisplay.setState(rawPlayerEnabled);

    objX -= wavPlyrStatusDisplay.getWidth();
    wavPlyrStatusDisplay.setPosition(objX, objY);
    wavPlyrStatusDisplay.drawDisplay();
    wavPlyrStatusDisplay.setState(wavPlayerEnabled);

    // NumberNameDisplay Class
    //
    presetDisplay.setPosition(10, objY);
    presetDisplay.drawDisplay();
}



