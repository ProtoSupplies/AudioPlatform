/************************************************************************************
*                                MainMenuScreen.cpp                                 *
*                                                                                   *
*                       Copyright (c) 2025, Donald Swearingen                       *
*                                All Rights Reserved                                *
*************************************************************************************
*
* Description
*
*   MainMenuScreen is a custom AudioPlatform screen derived from the generic base class
*   DisplayScreenBase.
*
* Single Instance Class
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

#include "MainMenuScreen.h"
#include "ArduinoEclipseDefs.h"
#include "BitMap.h"
#include "BitMapData.h"

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
MainMenuScreen::MainMenuScreen(DisplayScreenInfo& displayInfo, ButtonPanelInfo& screenPanelInfo, LabelIdButtonInfo *screenPanelBtnInfo) :

    DisplayScreenBase(displayInfo),
    screenPanel(screenPanelInfo),
    screenPanelLabelIdInfo(screenPanelBtnInfo),
    statusDisplay() {

}

MainMenuScreen::~MainMenuScreen() {

}

// activateScreen
//
// Reset/display dynamic screen elements each time screen is displayed
//
void MainMenuScreen::activateScreen() {

    // Clear and display basic screen
    clearScreen();
    screenPanel.clearPanel();
    showScreenHeading();        // Must come _after_ clear panels
    const char* timeStr = Teensy41_Util::parseRtcDateTime();
    updateDateAndTime(timeStr);

    int16_t screenPanelX = marginLeft + (DISPLAY_WIDTH - (marginLeft + marginRight) - screenPanel.panelWidth) / 2;
    int16_t screenPanelY = marginTop + 20;
    screenPanel.setPanelPosition(screenPanelX, screenPanelY);
    screenPanel.drawPanel(screenPanelLabelIdInfo, BtnNone);

    uint16_t bitMapY = 30 + (marginTop - 100) / 2;
    BitMap::drawBitMap(MusicKbdIcon_100x70, 100, bitMapY);
    BitMap::drawBitMap(SoundWaveIcon_90x90, 610, bitMapY - 8);

    // Draw footer status display
    statusDisplay.initialize();
}

// deActivateScreen
//
void MainMenuScreen::deActivateScreen() {

}

// updateScreen
//
// This demonstrates a means of generically sending update info to a screen, whose packet definition
// can be unique to a single screen type, or shared by several screen types.
//
void MainMenuScreen::updateScreen(StatusUpdatePacketType pktType, void* updateInfoPkt) {

//    Serial.printf("MainMenuScreen::updateScreen: \n");

    StatusUpdatePacket* pkt = (StatusUpdatePacket*) updateInfoPkt;

    if (pktType == DateTimePacket) {
        if (strlen(pkt->timeStr) != 0) {
            updateDateAndTime(pkt->timeStr);
        }

    } else if (pktType == StatusPacket && statusDisplay.displayInitialized) {
        statusDisplay.updateDisplay(pktType, pkt);
    }
}

// pollScreen
//
ButtonId MainMenuScreen::pollScreen() {

    ButtonId touchedBtn = BtnNone;
    uint16_t pointX, pointY;

    // Check for touched buttons
    if (DisplayScreenBase::touched()) {
        DisplayScreenBase::getTouchedPoint(pointX, pointY);

        // First check for Home/Back button. If pressed, we'll return that value to main application.
        if (DisplayScreenBase::prevBtnTouched(pointX, pointY)) {
            touchedBtn = BtnPrev;

        // Check for touch on command selection button
        } else if (screenPanel.isPanelButton(pointX, pointY, touchedBtn)) {
            ButtonId btnId = screenPanelLabelIdInfo[touchedBtn].buttonId;
            touchedBtn = btnId;
        }
    }

    return touchedBtn;
}

/************************************************************************************
*                   P R I V A T E   C L A S S   F U N C T I O N S                   *
*************************************************************************************
*/

// updateDateAndTime
//
void MainMenuScreen::updateDateAndTime(const char* dateTimeStr) {

    static uint16_t lastDateTimeWid = 0;

//    Serial.printf("MainMenuScreen::updateDateAndTime: %s\n", dateTimeStr);

    DisplayScreenBase::setFont(DATETIME_DISPLAY_FONT);

    uint16_t dateTimeWid, dateTimeHgt;
    DisplayScreenBase::measureText(dateTimeStr, dateTimeWid, dateTimeHgt);

    uint16_t wid = dateTimeWid;
    uint16_t hgt = 30;
    uint16_t xPos = (DISPLAY_WIDTH - dateTimeWid) / 2;  // centered horizontally
    uint16_t yPos = 98;    // FIXME: empirical, works, but still magic number

    if (dateTimeWid < lastDateTimeWid) {
        DisplayScreenBase::fillRect(((DISPLAY_WIDTH - lastDateTimeWid) / 2) - 2, yPos, lastDateTimeWid + 4, hgt, displayBgdColor);
    }
    lastDateTimeWid = dateTimeWid;

    TextArea tArea;
    tArea.posX = xPos;
    tArea.posY = yPos;
    tArea.wid = wid;
    tArea.hgt = hgt;
    tArea.align = TextAlignCenter;

    DisplayScreenBase::setTextColor(COLOR_WHITE, COLOR_BLACK);
    DisplayScreenBase::printInArea(dateTimeStr, tArea);
}
