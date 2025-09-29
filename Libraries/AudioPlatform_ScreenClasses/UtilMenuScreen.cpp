/************************************************************************************
*                                UtilMenuScreen.cpp                                 *
*                                                                                   *
*                       Copyright (c) 2025, Donald Swearingen                       *
*                                All Rights Reserved                                *
*************************************************************************************
*
* Description
*
*   UtilMenuScreen is a custom AudioPlatform screen derived from the generic base class
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

#include "UtilMenuScreen.h"
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
UtilMenuScreen::UtilMenuScreen(DisplayScreenInfo& displayInfo, ButtonPanelInfo& screenPanelInfo, LabelIdButtonInfo *screenPanelBtnInfo) :

    DisplayScreenBase(displayInfo),
    screenPanel(screenPanelInfo), screenPanelLabelIdInfo(screenPanelBtnInfo) {
}

UtilMenuScreen::~UtilMenuScreen() {

}

// activateScreen
//
// Reset/display dynamic screen elements each time screen is displayed
//
void UtilMenuScreen::activateScreen() {

    // Clear and display basic screen
    clearScreen();
    screenPanel.clearPanel();
    showScreenHeading();        // Must come _after_ clear panels

    int16_t screenPanelX = marginLeft + (DISPLAY_WIDTH - (marginLeft + marginRight) - screenPanel.panelWidth) / 2;
    int16_t screenPanelY = marginTop + 30;
    screenPanel.setPanelPosition(screenPanelX, screenPanelY);
    screenPanel.drawPanel(screenPanelLabelIdInfo, BtnNone);
}

// deActivateScreen
//
void UtilMenuScreen::deActivateScreen() {

}

// pollScreen
//
ButtonId UtilMenuScreen::pollScreen() {

    ButtonId touchedBtn = BtnNone;
    uint16_t pointX, pointY;

    // Check for touched buttons
    if (DisplayScreenBase::touched()) {
        DisplayScreenBase::getTouchedPoint(pointX, pointY);

        // First check for Home/Back button. If pressed, we'll return that value to main application.
        if (DisplayScreenBase::prevBtnTouched(pointX, pointY)) {
//            Serial.printf("UtilMenuScreen::pollScreen: prevBtnTouched\n");
            touchedBtn = BtnPrev;

        // Check for touch on command selection button
        } else if (screenPanel.isPanelButton(pointX, pointY, touchedBtn)) {
            ButtonId btnId = screenPanelLabelIdInfo[touchedBtn].buttonId;
//            Serial.printf("UtilMenuScreen::pollScreen: screenPanelBtnTouched label %s btnId %d\n", screenPanelLabelIdInfo[touchedBtn], btnId);
            touchedBtn = btnId;
        }
    }

    return touchedBtn;
}

/************************************************************************************
*                   P R I V A T E   C L A S S   F U N C T I O N S                   *
*************************************************************************************
*/

