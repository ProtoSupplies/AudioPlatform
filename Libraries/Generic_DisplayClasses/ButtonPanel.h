/************************************************************************************
*                                   ButtonPanel.h                                   *
*                                                                                   *
*                      Copyright (c) 2025, Donald Swearingen                        *
*                               All Rights Reserved                                 *
*************************************************************************************
*
* Description
*
*   THe ButtonPanel class creates and manages a panel (rectangular array) of individually
*   touchable screen "buttons".
*
*   See ButtonPanel.cpp for additional details.
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

#ifndef BUTTONPANEL_H_
#define BUTTONPANEL_H_

#include "DisplayScreenBase.h"

/************************************************************************************
*                         M A C R O S  /  C O N S T A N T S                         *
*************************************************************************************
*/

/*****************************
* Display/TouchScreen Layout *
******************************
*/

// TFT Display Parameter Display Buttons
//
#define BTN_COUNT_DEFAULT           12
#define BTNS_PER_ROW_DEFAULT        4
#define BTN_COLUMNS_DEFAULT         BTNS_PER_ROW_DEFAULT
#define BTN_WIDTH_DEFAULT           156
#define BTN_HEIGHT_DEFAULT          77
#define BTN_HEIGHT_DEFAULT_2ROW     77
#define BTN_HEIGHT_DEFAULT_3ROW     77
#define BTN_HEIGHT_DEFAULT_4ROW     55
#define BTN_GAP_VERTICAL_DEFAULT    12
#define BTN_GAP_HORIZONTAL_DEFAULT  12
#define BTN_PANEL_BGD_COLOR_DEFAULT DISPLAY_BGD_COLOR_DEFAULT
#define BTN_FONT_DEFAULT            Arial_12_Bold
#define BTN_COLOR_DEFAULT           COLOR_DARKGOLDENROD
#define BTN_SEL_COLOR_DEFAULT       COLOR_OLIVE
#define BTN_TEXT_COLOR_DEFAULT      COLOR_BLACK
#define BTN_SEL_TEXT_COLOR_DEFAULT  COLOR_WHITE
#define BTN_OUTLINE_COLOR_DEFAULT   COLOR_WHITE
#define BTN_OUTLINE_COLOR_MODIFIED  COLOR_RED

/************************************************************************************
*                     D A T A / T Y P E   D E F I N I T I O N S                     *
*************************************************************************************
*/

typedef struct {

    uint8_t             btnCount;
    uint8_t             btnsPerRow;
    uint16_t            btnWidth;
    uint16_t            btnHeight;
    uint8_t             btnGapHorizontal;
    uint8_t             btnGapVertical;
    uint16_t            panelBgdColor;
    uint16_t            btnColor;
    uint16_t            btnActiveColor;
    ILI9341_t3_font_t   btnFont;
    uint16_t            btnTextColor;
    uint16_t            btnActiveTextColor;
    uint16_t            btnOutlineColor;

} ButtonPanelInfo;

typedef struct {

    const char*         buttonLabel;    // Text to display on button
    ButtonId            buttonId;       // Unique identifier

} LabelIdButtonInfo;

const uint8_t   ButtonLabelBufSize  = 64;
const int16_t   RoundedFillRadius   = 4;

/************************************************************************************
*                         C L A S S   D E F I N I T I O N S                         *
*************************************************************************************
*/

class ButtonPanel {

// Constructor/Destructor
//
public:

    ButtonPanel();
    ButtonPanel(ButtonPanelInfo& panelInfo);
    virtual ~ButtonPanel();

// Class Variables
//
public:

    // Attributes provided in ButtonPanelInfo struct
    uint8_t             btnCount;
    uint8_t             btnsPerRow;
    uint16_t            btnWidth;
    uint16_t            btnHeight;
    uint8_t             btnGapHorizontal;
    uint8_t             btnGapVertical;
    uint16_t            panelBgdColor;
    uint16_t            btnColor;
    uint16_t            btnActiveColor;
    ILI9341_t3_font_t   btnFont;
    uint16_t            btnTextColor;
    uint16_t            btnActiveTextColor;
    uint16_t            btnOutlineColor;

    // Attributes provided in setPanelPosition()
    int16_t             panelX;
    int16_t             panelY;

    // Attributes provided in setButtonShape()
    int16_t             roundingValue;

    // Derived attributes established in setDerivedAttributes()
    uint8_t             btnRowCount;
    uint8_t             btnColumnCount;
    uint16_t            panelWidth;     // width, height based on screen margins
    uint16_t            panelHeight;

    ButtonId            selectedButton;

private:

    // Temporary buffer for sprintf() of labels before display
    char                btnLabelBuf[ButtonLabelBufSize];

protected:


// Class Methods
//
public:

    void        setBgdColor(int16_t color);
    void        setPanelInfo(ButtonPanelInfo& panelInfo);
    void        showPanelInfo();
    void        setPanelPosition(int16_t x, int16_t y);
    void        setPanelAttributes(uint8_t count, uint8_t perRow, uint8_t hGap, uint8_t vGap);
    void        setButtonAttributes(uint16_t color, uint16_t activeColor, ILI9341_t3_font_t font, uint16_t textColor, uint16_t textActiveColor, uint16_t btnOutlineColor);
    void        setButtonShape(bool rounded);
    void        setSelected(LabelIdButtonInfo* btnInfo, ButtonId selectedBtn = BtnNone);
    void        setSelected(const char* btnLabels[], ButtonId btnNum);

    void        clearPanel();
    void        drawPanel(const char* btnLabels[]);
    void        drawPanel(LabelIdButtonInfo* btnInfo, ButtonId selectedBtn = BtnNone);
    void        drawPanelButtons();
    void        drawButton(ButtonId btnNum, bool isActive);
    void        drawButtonOutline(ButtonId btnNum, uint16_t outlineColor = BTN_OUTLINE_COLOR_DEFAULT);
    void        showButtonLabel(ButtonId btnNum, const char* btnLabel, bool isActive);
    void        showButtonName(ButtonId btnNum, const char* paramName, bool isActive);
    void        showButtonValue(ButtonId btnNum, const char* valueStr, bool isActive);
    void        getButtonXY(ButtonId btnNum, uint16_t& objectX, uint16_t& objectY);
    bool        virtual isPanelButton(int16_t btnX, int16_t btnY, ButtonId& btnNum);

private:

    void        setDerivedAttributes();

protected:

};

#endif // BUTTONPANEL_H_
