/************************************************************************************
*                               DisplayScreenBase.cpp                               *
*                                                                                   *
*                       Copyright (c) 2025, Donald Swearingen                       *
*                                All Rights Reserved                                *
*************************************************************************************
*
* Description
*
*   DisplayScreenBase is the base class from which all screen display classes are ultimately
*   derived. In particular, it defines a number of static elements common to all base
*   classes. Since there is only a single display screen, certain methods and structures
*   are shared among all screens through the static "tftDIsplay" and "touchScreen"
*   and other static objects, which provide a layer of abstraction between screens,
*   display types, and elements defined in application subclasses.
*
*   Additional, non-static objects and functions provide a means for sub-classes to
*   implement their own definitions specific to their intent and requirements.
*
* Display Type RA8875
*
*   • tftDisplay: 7″ TFT color 800x480 RGB SPI display w/ RA8875 controller
*   • touchScreen: RA8875 capacitive touch controller
*
*   TODO: Note on modification of RA8875 modules in TeensyDuino Library
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
#include "ArduinoEclipseDefs.h"
#include "DisplayScreenBase.h"
#include "BitMapData.h"
#include "ScreenButton.h"

/************************************************************************************
*                    S T A T I C   C L A S S   V A R I A B L E S                    *
*************************************************************************************
*/

// Initialization of static class variables
//

// Static Member Initializations
//
RA8875          DisplayScreenBase::tftDisplay(RA8875_CS, RA8875_RESET, RA8875_MOSI, RA8875_SCLK, RA8875_MISO);

bool            DisplayScreenBase::baseInitialized = false;
char            DisplayScreenBase::stringDispBuf[StringDispBufLength];

bool            DisplayScreenBase::reportTouchRelease = false;
elapsedMillis   DisplayScreenBase::touchTimer = 0;;

uint16_t        DisplayScreenBase::lastTouchedY = DISPLAY_WIDTH + 10;   // Initialize to off-screen point
uint16_t        DisplayScreenBase::lastTouchedX = DISPLAY_HEIGHT + 10;  // Initialize to off-screen point
uint16_t        DisplayScreenBase::touchInterval = TOUCH_INTERVAL_DEFAULT;

static ScreenButtonInfo homeBtnInfo = (ScreenButtonInfo) {
    0,                              // btnX (calculated at run time)
    0,                              // btnY (calculated at run time)
    36,                             // btnWid for 45x45
    36,                             // btnHgt for 45x45
    COLOR_BLACK,                    // btnColor
    SCREENBUTTON_OUTLINE_COLOR,     // btnOutlineColor
    0,                              // btnOutlineRadius
    "",                             // btnLabel (set at runtime)
    SCREENBUTTON_TEXT_COLOR,        // btnLabelColor
    SCREENBUTTON_TEXT_FONT,         // btnLabelFont (note: must set again in constructor B241001-1)
    SCREENBUTTON_TEXT_ALIGN,        // btnLabelAlign
    SCREENBUTTON_TEXT_POSITION,     // btnLabelPosition
    HomeIcon_35x35                  // bitMapInfo
};

static ScreenButton     homeBtn(homeBtnInfo);

static ScreenButtonInfo backPrevBtnInfo = (ScreenButtonInfo) {
    0,                              // btnX (calculated at run time)
    0,                              // btnY (calculated at run time)
    36,                             // btnWid for 45x45
    36,                             // btnHgt for 45x45
    COLOR_BLACK,                    // btnColor
    SCREENBUTTON_OUTLINE_COLOR,     // btnOutlineColor
    0,                              // btnOutlineRadius
    "",                             // btnLabel (set at runtime)
    SCREENBUTTON_TEXT_COLOR,        // btnLabelColor
    SCREENBUTTON_TEXT_FONT,         // btnLabelFont (note: must set again in constructor B241001-1)
    SCREENBUTTON_TEXT_ALIGN,        // btnLabelAlign
    SCREENBUTTON_TEXT_POSITION,     // btnLabelPosition
    BackPrevIcon_35x35,             // bitMapInfo
};

ScreenButton    backPrevBtn(backPrevBtnInfo);
ScreenButton*   homePrevBtn;

/************************************************************************************
*                    P U B L I C   C L A S S   F U N C T I O N S                    *
*************************************************************************************
*/

// Constructor/Destructor
//
DisplayScreenBase::DisplayScreenBase() :
    displayBgdColor(DISPLAY_BGD_COLOR_DEFAULT),
    marginTop(DEFAULT_SCRN_MARGIN_TOP),
    marginBottom(DEFAULT_SCRN_MARGIN_BOTTOM),
    marginLeft(DEFAULT_SCRN_MARGIN_LEFT),
    marginRight(DEFAULT_SCRN_MARGIN_RIGHT),
    headerFont(HDR_FONT_DEFAULT),
    headerFontColor(HDR_TEXT_COLOR_DEFAULT),
    subHeaderFont(SUBHDR_FONT_DEFAULT),
    subHeaderFontColor(SUBHDR_TEXT_COLOR_DEFAULT),
    screenHeader(0),
    screenSubHeader(0),
    textAlign(TextAlignLeft) {

    showPrevBtn = true;

    homePrevBtn = &homeBtn;
}

DisplayScreenBase::DisplayScreenBase(DisplayScreenInfo& screenInfo) :
    displayBgdColor(screenInfo.displayBgdColor),
    marginTop(screenInfo.marginTop),
    marginBottom(screenInfo.marginBottom),
    marginLeft(screenInfo.marginLeft),
    marginRight(screenInfo.marginRight),
    headerFont(screenInfo.headerFont),
    headerFontColor(screenInfo.headerFontColor),
    subHeaderFont(screenInfo.subHeaderFont),
    subHeaderFontColor(screenInfo.subHeaderFontColor),
    screenHeader(screenInfo.screenHeader),
    screenSubHeader(screenInfo.screenSubHeader),
    textAlign(screenInfo.textAlign),
    showPrevBtn(screenInfo.showPrevBtn),
    prevBtnText(screenInfo.prevBtnText) {

}

DisplayScreenBase::~DisplayScreenBase() {

}

// setupDisplay()
//
void DisplayScreenBase::setupDisplay() {

    if (!baseInitialized) {
        // Setup LCD screen
        tftDisplay.begin(RA8875_800x480);
        tftDisplay.setRotation(DISPLAY_ROTATION);
        tftDisplay.brightness(255);                 // Set brightness to max (default)
#if defined(USE_FT5206_TOUCH)
        tftDisplay.useCapINT(RA8875_INT);           // We use the capacitive chip Interrupt out
        tftDisplay.setTouchLimit(MAXTOUCHLIMIT);    // Set max touches we will recognize
        tftDisplay.enableCapISR(true);              // Arm capacitive touch screen interrupt
#else
        tftDisplay.print("you should open RA8875UserSettings.h file and uncomment USE_FT5206_TOUCH!");
#endif // USE_FT5206_TOUCH
        tftDisplay.fillWindow(COLOR_BLUE);          // Fill window blue
        tftDisplay.setTextColor(COLOR_WHITE);       // White text, transparent background
        tftDisplay.setCursor(1, 110);               // Set initial cursor position
        //tft.setFontScale(0);                      // If using the built-in fonts
        tftDisplay.setFont(Arial_14);               // If using custom fonts like we are here

        baseInitialized = true;
    }
}

// setScreenInfo()
//
void DisplayScreenBase::setScreenInfo(DisplayScreenInfo* displayInfo) {

    displayBgdColor =   displayInfo->displayBgdColor;
    marginTop =         displayInfo->marginTop;
    marginBottom =      displayInfo->marginBottom;
    marginLeft =        displayInfo->marginLeft;
    marginRight =       displayInfo->marginRight;
    headerFont =        displayInfo->headerFont;
    headerFontColor =   displayInfo->headerFontColor;
    subHeaderFont =     displayInfo->subHeaderFont;
    subHeaderFontColor = displayInfo->subHeaderFontColor;
    screenHeader =      displayInfo->screenHeader;
    screenSubHeader =   displayInfo->screenSubHeader;
    textAlign =         displayInfo->textAlign;
    showPrevBtn =       displayInfo->showPrevBtn;
    prevBtnText =       displayInfo->prevBtnText;
}

// clearScreen()
//
void DisplayScreenBase::clearScreen() {

    fillScreen(displayBgdColor);
}

// showScreenHeading()
//
void DisplayScreenBase::showScreenHeading() {

    setFont(headerFont);
    setTextColor(headerFontColor);

    uint16_t headerTextWidth, headerTextHeight;
    measureText(screenHeader, headerTextWidth, headerTextHeight);

//    Serial.printf("showScreenHeading: headerTextHeight: %d\n", headerTextHeight);

    uint16_t subHeaderTextWidth = 0, subHeaderTextHeight = 0;
    if (screenSubHeader) {

//        Serial.printf("showScreenHeading: subHeader present: %s\n", screenSubHeader);

        setFont(subHeaderFont);
        measureText(screenSubHeader, subHeaderTextWidth, subHeaderTextHeight);
    }

    uint16_t hdrX = 0, hdrY = 0, subHdrX = 0, subHdrY = headerTextHeight;
    uint16_t totalTextHeight = headerTextHeight + subHeaderTextHeight;
    if (screenSubHeader) {
        totalTextHeight += (subHeaderTextHeight / 3);
    }

    hdrY = (float)(marginTop - totalTextHeight) / 2.0;
    subHdrY = hdrY + headerTextHeight + ((float)subHeaderTextHeight / 3.0) + 2.5;

    switch (textAlign) {
        case TextAlignLeft:
            hdrX = subHdrX = marginLeft;
            break;

        case TextAlignCenter:
            hdrX = marginLeft + (float)((DISPLAY_WIDTH - (marginRight + marginLeft)) - headerTextWidth) / 2.0;
            subHdrX = marginLeft + (float)((DISPLAY_WIDTH - (marginRight + marginLeft)) - subHeaderTextWidth) / 2.0;
            break;

        case TextAlignRight:
            hdrX = (DISPLAY_WIDTH - marginRight) - headerTextWidth;
            subHdrX = (DISPLAY_WIDTH - marginRight) - subHeaderTextWidth;
            break;
    }

    setFont(headerFont);
    setTextColor(headerFontColor);
    setCursor(hdrX, hdrY);
    print(screenHeader);

    if (screenSubHeader) {
        setFont(subHeaderFont);
        setTextColor(subHeaderFontColor);
        setCursor(subHdrX, subHdrY);
        print(screenSubHeader);
    }

    drawPrevBtn();
}

// setDisplayMargins()
//
void DisplayScreenBase::setDisplayMargins(uint16_t top, uint16_t bottom, uint16_t left, uint16_t right) {

    marginTop = top;
    marginBottom = bottom;
    marginLeft = left;
    marginRight = right;
}

// setDisplayFonts()
//
void DisplayScreenBase::setDisplayFonts(ILI9341_t3_font_t hdrFont, uint16_t hdrFontColor, ILI9341_t3_font_t subHdrFont, uint16_t subHdrFontColor) {

    headerFont = hdrFont;
    headerFontColor = hdrFontColor;
    subHeaderFont = subHdrFont;
    subHeaderFontColor = subHdrFontColor;
}

// prevBtnTouched()
//
bool DisplayScreenBase::prevBtnTouched(int16_t pX, int16_t pY) {

    bool prevTouched = false;

    if (showPrevBtn) {
        if (homePrevBtn->buttonTouched(pX, pY)) {
            prevTouched = true;
        }
    }

    return prevTouched;
}

/****************************
* Indirect Access Functions *
*****************************
*
* Following public static functions provide a layer of indirection between applications,
* subclasses and the physical display in use (either ILI9341 or RA8875 in initial release).
*
* For those functions that turn out to be identical in the two libraries, the associated
* functions below do not require a check for display type.
*
*/

// setRotation()
//
void DisplayScreenBase::setRotation(uint8_t rotation) {

    tftDisplay.setRotation(rotation);
}

// width()
//
int16_t DisplayScreenBase::width(void) {

    return DISPLAY_WIDTH;
}

// height()
//
int16_t DisplayScreenBase::height(void) {

    return DISPLAY_HEIGHT;
}

// fillScreen()
//
void DisplayScreenBase::fillScreen(uint16_t color) {

    tftDisplay.fillScreen(color);
}

// drawRect()
//
void DisplayScreenBase::drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {

    tftDisplay.drawRect(x, y, w, h, color);
}

// fillRect()
//
void DisplayScreenBase::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {

//    Serial.printf("DisplayScreenBase::fillRect %d %d %d %d %d\n", x, y, w, h, color);

    tftDisplay.fillRect(x, y, w, h, color);
}

// drawRoundRect()
//
void DisplayScreenBase::drawRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t radius, uint16_t color) {

    tftDisplay.drawRoundRect(x, y, w, h, radius, color);
}

// fillRoundRect()
//
void DisplayScreenBase::fillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t radius, uint16_t color) {

    tftDisplay.fillRoundRect(x, y, w, h, radius, color);
}

// writeRect()
//
void DisplayScreenBase::writeRect(int16_t x, int16_t y, int16_t w, int16_t h, const uint16_t *pcolors) {

    tftDisplay.writeRect(x, y, w, h, pcolors);
}

// drawCircle()
//
void DisplayScreenBase::drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color) {

    tftDisplay.drawCircle(x0, y0, r, color);
}

// fillCircle()
//
void DisplayScreenBase::fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color) {

    tftDisplay.fillCircle(x0, y0, r, color);
}

// drawLine()
//
void DisplayScreenBase::drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) {

    tftDisplay.drawLine(x0, y0, x1, y1, color);
}

// setFont()
//
void DisplayScreenBase::setFont(const ILI9341_t3_font_t &f) {

    tftDisplay.setFont(f);
}

// setTextColor()
//
void DisplayScreenBase::setTextColor(uint16_t c) {

    tftDisplay.setTextColor(c);
}

void DisplayScreenBase::setTextColor(uint16_t c, uint16_t bg) {

    tftDisplay.setTextColor(c, bg);
}

// measureText()
//
void DisplayScreenBase::measureText(const char* text, uint16_t& textWidth, uint16_t& textHeight) {

    int16_t x1, y1;
    tftDisplay.getTextBounds(text, 0, 0, &x1, &y1, &textWidth, &textHeight);
}

// setCursor()
//
void DisplayScreenBase::setCursor(int16_t x, int16_t y) {

    tftDisplay.setCursor(x, y);
}

// print()
//
size_t DisplayScreenBase::print(const char s[]) {

    return tftDisplay.print(s);
}

// printInArea()
//
size_t DisplayScreenBase::printInArea(const char s[], TextArea& area) {

    uint16_t textWidth, textHeight;
    DisplayScreenBase::measureText(s, textWidth, textHeight);
    uint16_t textX = 0;
    float yOffset = (((float)area.hgt - (float)textHeight) / 2.0) + 0.5;
    uint16_t textY = area.posY + (int)yOffset;
//    uint16_t textY = area.posY + (((float)area.hgt - (float)textHeight) / 2.0);

    switch (area.align) {
        case TextAlignLeft:
            textX = area.posX;
            break;

        case TextAlignCenter:
            textX = area.posX + (((float)area.wid - (float)textWidth) / 2.0);
            break;

        case TextAlignRight:
            textX = area.posX + area.wid - textWidth;
            break;
    }

    tftDisplay.setCursor(textX, textY);

    return tftDisplay.print(s);
}

// touched()
//
bool DisplayScreenBase::touched(uint16_t touchWait, bool repeat) {

    bool screenTouched = false;

    touchInterval = touchWait;

    if (touchTimer > touchInterval) {
        touchTimer = 0;
        tftDisplay.updateTS();

        if (tftDisplay.touched()) {
                screenTouched = true;
                if (repeat) {
                    reportTouchRelease = false;

                } else {
                    reportTouchRelease = true;
                }
//            }

        } else {
            if (!reportTouchRelease) {
                screenTouched = false;
                reportTouchRelease = true;
            }
        }

        if (screenTouched) {
            uint16_t x, y;
            getTouchedPoint(x, y);

            if ((x > DISPLAY_WIDTH) || (y > DISPLAY_HEIGHT)) {
                screenTouched = false;      // Reject points reported off-screen

            } else if (!repeat && (abs(lastTouchedX - x) <= TOUCHPOINT_MAX_X_DIST) && (abs(lastTouchedY - y) <= TOUCHPOINT_MAX_Y_DIST)) {
                screenTouched = false;      // Reject points too close to last reported point

            } else {
                lastTouchedX = x;           // Record new point
                lastTouchedY = y;
            }
        }
    }

    return screenTouched;
}

// getTouchedPoint()
//
void DisplayScreenBase::getTouchedPoint(uint16_t& x, uint16_t& y) {

    uint16_t coordinates[MAXTOUCHLIMIT][2];  // to hold touch coordinates
    tftDisplay.getTScoordinates(coordinates);  // Get touch coordinates in pixels
    x = coordinates[0][0];
    y = coordinates[0][1];
}

// getTScoordinates()
//
uint8_t DisplayScreenBase::getTScoordinates(uint16_t (*touch_coordinates)[2]) {

    return tftDisplay.getTScoordinates(touch_coordinates);
}

// rectTouched()
//
bool DisplayScreenBase::rectTouched(int16_t touchX, int16_t touchY, int16_t rectX, int16_t rectY, uint16_t rectWid, uint16_t rectHgt) {

    bool touched = false;

    if ((touchX > rectX) && (touchX <= (rectX + rectWid))) {
        if ((touchY > rectY) && (touchY <= (rectY + rectHgt))) {
            touched = true;
        }
    }

    return touched;
}

// circleTouched()
//
bool DisplayScreenBase::circleTouched(uint16_t touchX, uint16_t touchY, uint16_t circleX, uint16_t circleY, uint16_t circleRadius) {

    bool touched = false;

    uint32_t distSq = ((touchX - circleX)^2) + ((touchY - circleY)^2);
    uint32_t radiusSq = circleRadius^2;

    if (distSq <= radiusSq) {
        touched = true;
    }

    return touched;
}

/************************************************************************************
*                   P R I V A T E   C L A S S   F U N C T I O N S                   *
*************************************************************************************
*/

// drawPrevBtn()
//
void DisplayScreenBase::drawPrevBtn() {

//    Serial.printf("DisplayScreenBase::drawPrevBtn: showPrevBtn %d\n", showPrevBtn);

    if (showPrevBtn) {
        const char *prevBtnLabel = prevBtnText;
//        Serial.printf("DisplayScreenBase::drawPrevNextBtns1: prevBtnLabel %s\n", prevBtnLabel);

        if (strstr(prevBtnLabel, "Home")) {
            homePrevBtn = &homeBtn;
        } else {
            homePrevBtn = &backPrevBtn;
        }

        ScreenButtonInfo& btnInfo = homePrevBtn->getBtnInfo();
        btnInfo.btnX = DISPLAY_WIDTH - (marginRight + btnInfo.btnWid);
        btnInfo.btnY = ((float)marginTop - (float) btnInfo.btnHgt) / 2.0;
        homePrevBtn->drawButton();
    }
}

