/************************************************************************************
*                                 SetTimeScreen.cpp                                 *
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

// Standard C++
//
//#include <Math.h>
//#include <string.h>
//#include <limits.h>
//#include <stdio.h>
//#include <stdlib.h>

// TeensyDuino Libraries
//
#include <TimeLib.h>
#include <ElapsedMillis.h>

// User Arduino Libraries
//
#include "SetTimeScreen.h"
#include "ArduinoEclipseDefs.h"

/************************************************************************************
*                    S T A T I C   C L A S S   V A R I A B L E S                    *
*************************************************************************************
*/

PROGMEM const char currentTimeLabel[]   = "Current Time";
PROGMEM const char newTimeLabel1[]      = "New Time Value (24-Hour Format)";
PROGMEM const char newTimeLabel2[]      = "Touch and Drag Numbers Up/Down";

ScreenButtonInfo SetTimeScreen::setTimeBtnInfo = (ScreenButtonInfo ) {
     0,                             // btnX
     0,                             // btnY
     140,                           // btnWid
     40,                            // btnHgt
     SCREENBUTTON_COLOR,            // btnColor
     SCREENBUTTON_OUTLINE_COLOR,    // btnOutlineColor
     SCREENBUTTON_OUTLINE_RADIUS,   // btnOutlineRadius
     "Set New Time",                // btnLabel
     SCREENBUTTON_TEXT_COLOR,       // btnLabelColor
     SCREENBUTTON_TEXT_FONT,        // btnLabelFont (note: must set again in constructor B241001-1)
     SCREENBUTTON_TEXT_ALIGN,       // btnLabelAlign
     SCREENBUTTON_TEXT_POSITION,    // btnLabelPosition
     0,                             // bitMapInfo
};

/************************************************************************************
*                    P U B L I C   C L A S S   F U N C T I O N S                    *
*************************************************************************************
*/

// Static Member Initializations
//

// Constructor/Destructor
//
SetTimeScreen::SetTimeScreen(DisplayScreenInfo& displayInfo, DateTimeDisplayInfo& dtDisplayInfo, DateTimeDisplayInfo& dtEnterInfo) :
    DisplayScreenBase(displayInfo),
    setTimeBtn(setTimeBtnInfo),
    dtDisplay(dtDisplayInfo, 100, 100),
    dtEnter(dtEnterInfo, 100, 250) {

    // Workaround for B241001-1
    setTimeBtnInfo.btnLabelFont = SCREENBUTTON_TEXT_FONT;
}

SetTimeScreen::~SetTimeScreen() {

}

// activateScreen
//
void SetTimeScreen::activateScreen() {

//    Serial.printf("SetTimeScreen::activateScreen:\n");

    setTimeBtnInfo.btnX   = (DISPLAY_WIDTH - setTimeBtnInfo.btnWid) / 2;
    setTimeBtnInfo.btnY   = dtEnter.displayY + dtEnter.displayInfo.cellHeight + 80;

//    Serial.printf("SetTimeScreen::activateScreen: textToEdit %s\n", textToEdit);
    forceUpdate = true;

    // Clear and display screen
    clearScreen();
    showScreenHeading();
    showScreen();
}

// deActivateScreen
//
void SetTimeScreen::deActivateScreen() {

}

// updateScreen
//
void SetTimeScreen::updateScreen(StatusUpdatePacketType pktType, void* pkt) {

    if (pktType == DateTimePacket) {
    DateTimeFields* dt = (DateTimeFields*)pkt;
//    Serial.printf("SetTimeScreen::updateScreen: %02d:%02d:%02d\n", dt->hour, dt->min, dt->sec);
    dtDisplay.updateDisplay(dt, forceUpdate);
    forceUpdate = false;
    }
}

// pollScreen
//
ButtonId SetTimeScreen::pollScreen() {

    ButtonId touchedBtn = BtnNone;

    uint16_t pointX, pointY;

    if (DisplayScreenBase::touched()) {

        DisplayScreenBase::getTouchedPoint(pointX, pointY);

        dtEnter.pollDisplay(pointX, pointY);

        // First check for Home/Back button. If pressed, we'll return that value to main application.
        if (DisplayScreenBase::prevBtnTouched(pointX, pointY)) {
            touchedBtn = BtnPrev;

        } else if (!dtEnter.touchActive && setTimeBtn.buttonTouched(pointX, pointY)) {
//            Serial.printf("SetTimeScreen::pollScreen: setTimeBtn touched\n");
            setDateAndTime();
        }
    }

    return touchedBtn;
}

/************************************************************************************
*                   P R I V A T E   C L A S S   F U N C T I O N S                   *
*************************************************************************************
*/

// showScreen
//
void SetTimeScreen::showScreen() {

    setTimeBtn.drawButton();

    dtDisplay.drawDisplay();
    dtEnter.drawDisplay();

    DateTimeFields dt;
    Teensy41_Util::parseRtcDateTime(dt);
    dtEnter.updateDisplay(&dt, true);
    dtDisplay.updateDisplay(&dt, true);

    DisplayScreenBase::setFont(Arial_12_Bold);
    DisplayScreenBase::setTextColor(COLOR_WHITE);

    TextArea labelDisp;

    labelDisp.wid = 200;
    labelDisp.hgt = 40;
    labelDisp.align = TextAlignCenter;
    labelDisp.posX = (DISPLAY_WIDTH - labelDisp.wid) / 2;

    labelDisp.posY = dtDisplay.displayY - (labelDisp.hgt + 5);
    DisplayScreenBase::printInArea(currentTimeLabel, labelDisp);

    labelDisp.posY = dtEnter.displayY - (labelDisp.hgt + 5);
    DisplayScreenBase::printInArea(newTimeLabel1, labelDisp);

    labelDisp.posY = dtEnter.displayY + dtEnter.displayInfo.cellHeight + 5;
    DisplayScreenBase::printInArea(newTimeLabel2, labelDisp);
}

// setDateAndTime
//
void SetTimeScreen::setDateAndTime() {

    tmElements_t dt;

    dt.Month    = dtEnter.getCellValue(CELL_MONTH);
    dt.Day      = dtEnter.getCellValue(CELL_DAY);
    dt.Year     = dtEnter.getCellValue(CELL_YEAR) + 2000 - 1970;
    dt.Hour     = dtEnter.getCellValue(CELL_HOUR);
    dt.Minute   = dtEnter.getCellValue(CELL_MINUTE);
    dt.Second   = dtEnter.getCellValue(CELL_SECOND);

//    Serial.printf("SetTimeScreen::setDateAndTime: Month %d Day %d Year %d, Hour %d Minute %d Second %d\n", dt.Month, dt.Day, dt.Year, dt.Hour, dt.Minute, dt.Second);

    time_t userTime = makeTime(dt);

    Teensy3Clock.set(userTime);     // Teensy RTC time (reference)
    setTime(userTime);              // Arduino Time library time
}


