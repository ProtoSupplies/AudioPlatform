/************************************************************************************
*                                  ColorsScreen.cpp                                 *
*                                                                                   *
*                       Copyright (c) 2025, Donald Swearingen                       *
*                                All Rights Reserved                                *
*************************************************************************************
*
* Description
*
*   ColorsScreen is a custom AudioPlatform screen derived from the generic base class
*   DisplayScreebBase.
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

#include "ArduinoEclipseDefs.h"
#include "ColorsScreen.h"

/************************************************************************************
*                         M A C R O S  /  C O N S T A N T S                         *
*************************************************************************************
*/

/************************************************************************************
*                    S T A T I C   C L A S S   V A R I A B L E S                    *
*************************************************************************************
*/

static const uint16_t colorData[] = {
        COLOR_BLACK               , //     0
        COLOR_BLUE                , //     1
        COLOR_ALICEBLUE           , //     2
        COLOR_BLUEVIOLET          , //     3
        COLOR_CADETBLUE           , //     4
        COLOR_CORNFLOWERBLUE      , //     5
        COLOR_DARKBLUE            , //     6
        COLOR_DARKSLATEBLUE       , //     7
        COLOR_DEEPSKYBLUE         , //     8
        COLOR_DODGERBLUE          , //     9
        COLOR_LIGHTBLUE           , //    10
        COLOR_LIGHTSKYBLUE        , //    11
        COLOR_MEDIUMBLUE          , //    12
        COLOR_MEDIUMSLATEBLUE     , //    13
        COLOR_MEDIUMPURPLE        , //    14
        COLOR_MIDNIGHTBLUE        , //    15
        COLOR_POWDERBLUE          , //    16
        COLOR_ROYALBLUE           , //    17
        COLOR_SKYBLUE             , //    18
        COLOR_SLATEBLUE           , //    19
        COLOR_STEELBLUE           , //    20
        COLOR_NAVY                , //    21
        COLOR_NAVY2               , //    22
        COLOR_CYAN                , //    23
        COLOR_DARKCYAN            , //    24
        COLOR_AQUA                , //    25
        COLOR_AQUAMARINE          , //    26
        COLOR_DARKCYAN2           , //    27
        COLOR_TEAL                , //    28
        COLOR_DARKTURQUOISE       , //    29
        COLOR_GAINSBORO           , //    30
        COLOR_LAVENDER            , //    31
        COLOR_LIGHTCYAN           , //    32
        COLOR_LIGHTSTEELBLUE      , //    33
        COLOR_MEDIUMAQUAMARINE    , //    34
        COLOR_MEDIUMTURQUOISE     , //    35
        COLOR_PALETURQUOISE       , //    36
        COLOR_TURQUOISE           , //    37
        COLOR_NAVAJOWHITE         , //    38
        COLOR_DARKGREY            , //    39
        COLOR_LIGHTGREY1          , //    40
        COLOR_LIGHTGREY2          , //    41
        COLOR_LIGHTGREY           , //    42
        COLOR_SILVER              , //    43
        COLOR_BURLYWOOD           , //    44
        COLOR_TAN                 , //    45
        COLOR_THISTLE             , //    46
        COLOR_DARKSLATEGRAY       , //    47
        COLOR_DIMGRAY             , //    48
        COLOR_GRAY                , //    49
        COLOR_LIGHTSLATEGRAY      , //    50
        COLOR_SLATEGRAY           , //    51
        COLOR_DARKGREEN           , //    52
        COLOR_GREEN               , //    53
        COLOR_GREENYELLOW         , //    54
        COLOR_DARKGREEN2          , //    55
        COLOR_DARKOLIVEGREEN      , //    56
        COLOR_DARKSEAGREEN        , //    57
        COLOR_FORESTGREEN         , //    58
        COLOR_GREEN2              , //    59
        COLOR_LAWNGREEN           , //    60
        COLOR_LIGHTGREEN          , //    61
        COLOR_LIGHTSEAGREEN       , //    62
        COLOR_LIMEGREEN           , //    63
        COLOR_MEDIUMSEAGREEN      , //    64
        COLOR_MEDIUMSPRINGGREEN   , //    65
        COLOR_PALEGREEN           , //    66
        COLOR_SEAGREEN            , //    67
        COLOR_SPRINGGREEN         , //    68
        COLOR_YELLOWGREEN         , //    69
        COLOR_CHARTREUSE          , //    70
        COLOR_LIME                , //    71
        COLOR_OLIVE               , //    72
        COLOR_OLIVE2              , //    73
        COLOR_OLIVEDRAB           , //    74
        COLOR_DARKKHAKI           , //    75
        COLOR_RED                 , //    76
        COLOR_INDIANRED           , //    77
        COLOR_CRIMSON             , //    78
        COLOR_DARKRED             , //    79
        COLOR_FIREBRICK           , //    80
        COLOR_MAROON2             , //    81
        COLOR_MAROON              , //    82
        COLOR_TOMATO              , //    83
        COLOR_CORAL               , //    84
        COLOR_LIGHTCORAL          , //    85
        COLOR_LIGHTSALMON         , //    86
        COLOR_PALEVIOLETRED       , //    87
        COLOR_SANDYBROWN          , //    88
        COLOR_SALMON              , //    89
        COLOR_DARKSALMON          , //    90
        COLOR_MAGENTA             , //    91
        COLOR_PINK                , //    92
        COLOR_PEACHPUFF           , //    93
        COLOR_PINK1               , //    94
        COLOR_PINK2               , //    95
        COLOR_FUCHSIA             , //    96
        COLOR_MEDIUMVIOLETRED     , //    97
        COLOR_DEEPPINK            , //    98
        COLOR_HOTPINK             , //    99
        COLOR_LIGHTPINK           , //   100
        COLOR_DARKORANGE          , //   101
        COLOR_ORANGE              , //   102
        COLOR_ORANGERED           , //   103
        COLOR_BROWN               , //   104
        COLOR_CHOCOLATE           , //   105
        COLOR_PERU                , //   106
        COLOR_SADDLEBROWN         , //   107
        COLOR_SIENNA              , //   108
        COLOR_YELLOW              , //   109
        COLOR_GOLD                , //   110
        COLOR_GOLDENROD           , //   111
        COLOR_DARKGOLDENROD       , //   112
        COLOR_LIGHTGOLDENRODYELLOW , //  113
        COLOR_LIGHTYELLOW         , //   114
        COLOR_KHAKI               , //   115
        COLOR_WHEAT               , //   116
        COLOR_PURPLE1             , //   117
        COLOR_PURPLE2             , //   118
        COLOR_INDIGO              , //   119
        COLOR_DARKMAGENTA         , //   120
        COLOR_DARKORCHID          , //   121
        COLOR_DARKVIOLET          , //   122
        COLOR_MEDIUMORCHID        , //   123
        COLOR_ORCHID              , //   124
        COLOR_PLUM                , //   125
        COLOR_PURPLE              , //   126
        COLOR_VIOLET              , //   127
        COLOR_ROSYBROWN           , //   128
        COLOR_WHITE               , //   129
        COLOR_ANTIQUEWHITE        , //   130
        COLOR_AZURE               , //   131
        COLOR_BEIGE               , //   132
        COLOR_BISQUE              , //   133
        COLOR_BLANCHEDALMOND      , //   134
        COLOR_CORNSILK            , //   135
        COLOR_FLORALWHITE         , //   136
        COLOR_GHOSTWHITE          , //   137
        COLOR_HONEYDEW            , //   138
        COLOR_IVORY               , //   139
        COLOR_LAVENDERBLUSH       , //   140
        COLOR_LEMONCHIFFON        , //   141
        COLOR_LINEN               , //   142
        COLOR_MINTCREAM           , //   143
        COLOR_OLDLACE             , //   144
        COLOR_PAPAYAWHIP          , //   145
        COLOR_SNOW                , //   146
        COLOR_WHITESMOKE          , //   147
        COLOR_MISTYROSE           , //   148
        COLOR_MOCCASIN            , //   149
        COLOR_PALEGOLDENROD       , //   150
        COLOR_SEASHELL            , //   151
};

static const char* colorNames[] = {
    "COLOR_BLACK"               , //     0
    "COLOR_BLUE"                , //     1
    "COLOR_ALICEBLUE"           , //     2
    "COLOR_BLUEVIOLET"          , //     3
    "COLOR_CADETBLUE"           , //     4
    "COLOR_CORNFLOWERBLUE"      , //     5
    "COLOR_DARKBLUE"            , //     6
    "COLOR_DARKSLATEBLUE"       , //     7
    "COLOR_DEEPSKYBLUE"         , //     8
    "COLOR_DODGERBLUE"          , //     9
    "COLOR_LIGHTBLUE"           , //    10
    "COLOR_LIGHTSKYBLUE"        , //    11
    "COLOR_MEDIUMBLUE"          , //    12
    "COLOR_MEDIUMSLATEBLUE"     , //    13
    "COLOR_MEDIUMPURPLE"        , //    14
    "COLOR_MIDNIGHTBLUE"        , //    15
    "COLOR_POWDERBLUE"          , //    16
    "COLOR_ROYALBLUE"           , //    17
    "COLOR_SKYBLUE"             , //    18
    "COLOR_SLATEBLUE"           , //    19
    "COLOR_STEELBLUE"           , //    20
    "COLOR_NAVY"                , //    21
    "COLOR_NAVY2"               , //    22
    "COLOR_CYAN"                , //    23
    "COLOR_DARKCYAN"            , //    24
    "COLOR_AQUA"                , //    25
    "COLOR_AQUAMARINE"          , //    26
    "COLOR_DARKCYAN2"           , //    27
    "COLOR_TEAL"                , //    28
    "COLOR_DARKTURQUOISE"       , //    29
    "COLOR_GAINSBORO"           , //    30
    "COLOR_LAVENDER"            , //    31
    "COLOR_LIGHTCYAN"           , //    32
    "COLOR_LIGHTSTEELBLUE"      , //    33
    "COLOR_MEDIUMAQUAMARINE"    , //    34
    "COLOR_MEDIUMTURQUOISE"     , //    35
    "COLOR_PALETURQUOISE"       , //    36
    "COLOR_TURQUOISE"           , //    37
    "COLOR_NAVAJOWHITE"         , //    38
    "COLOR_DARKGREY"            , //    39
    "COLOR_LIGHTGREY1"          , //    40
    "COLOR_LIGHTGREY2"          , //    41
    "COLOR_LIGHTGREY"           , //    42
    "COLOR_SILVER"              , //    43
    "COLOR_BURLYWOOD"           , //    44
    "COLOR_TAN"                 , //    45
    "COLOR_THISTLE"             , //    46
    "COLOR_DARKSLATEGRAY"       , //    47
    "COLOR_DIMGRAY"             , //    48
    "COLOR_GRAY"                , //    49
    "COLOR_LIGHTSLATEGRAY"      , //    50
    "COLOR_SLATEGRAY"           , //    51
    "COLOR_DARKGREEN"           , //    52
    "COLOR_GREEN"               , //    53
    "COLOR_GREENYELLOW"         , //    54
    "COLOR_DARKGREEN2"          , //    55
    "COLOR_DARKOLIVEGREEN"      , //    56
    "COLOR_DARKSEAGREEN"        , //    57
    "COLOR_FORESTGREEN"         , //    58
    "COLOR_GREEN2"              , //    59
    "COLOR_LAWNGREEN"           , //    60
    "COLOR_LIGHTGREEN"          , //    61
    "COLOR_LIGHTSEAGREEN"       , //    62
    "COLOR_LIMEGREEN"           , //    63
    "COLOR_MEDIUMSEAGREEN"      , //    64
    "COLOR_MEDIUMSPRINGGREEN"   , //    65
    "COLOR_PALEGREEN"           , //    66
    "COLOR_SEAGREEN"            , //    67
    "COLOR_SPRINGGREEN"         , //    68
    "COLOR_YELLOWGREEN"         , //    69
    "COLOR_CHARTREUSE"          , //    70
    "COLOR_LIME"                , //    71
    "COLOR_OLIVE"               , //    72
    "COLOR_OLIVE2"              , //    73
    "COLOR_OLIVEDRAB"           , //    74
    "COLOR_DARKKHAKI"           , //    75
    "COLOR_RED"                 , //    76
    "COLOR_INDIANRED"           , //    77
    "COLOR_CRIMSON"             , //    78
    "COLOR_DARKRED"             , //    79
    "COLOR_FIREBRICK"           , //    80
    "COLOR_MAROON2"             , //    81
    "COLOR_MAROON"              , //    82
    "COLOR_TOMATO"              , //    83
    "COLOR_CORAL"               , //    84
    "COLOR_LIGHTCORAL"          , //    85
    "COLOR_LIGHTSALMON"         , //    86
    "COLOR_PALEVIOLETRED"       , //    87
    "COLOR_SANDYBROWN"          , //    88
    "COLOR_SALMON"              , //    89
    "COLOR_DARKSALMON"          , //    90
    "COLOR_MAGENTA"             , //    91
    "COLOR_PINK"                , //    92
    "COLOR_PEACHPUFF"           , //    93
    "COLOR_PINK1"               , //    94
    "COLOR_PINK2"               , //    95
    "COLOR_FUCHSIA"             , //    96
    "COLOR_MEDIUMVIOLETRED"     , //    97
    "COLOR_DEEPPINK"            , //    98
    "COLOR_HOTPINK"             , //    99
    "COLOR_LIGHTPINK"           , //   100
    "COLOR_DARKORANGE"          , //   101
    "COLOR_ORANGE"              , //   102
    "COLOR_ORANGERED"           , //   103
    "COLOR_BROWN"               , //   104
    "COLOR_CHOCOLATE"           , //   105
    "COLOR_PERU"                , //   106
    "COLOR_SADDLEBROWN"         , //   107
    "COLOR_SIENNA"              , //   108
    "COLOR_YELLOW"              , //   109
    "COLOR_GOLD"                , //   110
    "COLOR_GOLDENROD"           , //   111
    "COLOR_DARKGOLDENROD"       , //   112
    "COLOR_LIGHTGOLDENRODYELLOW" , //  113
    "COLOR_LIGHTYELLOW"         , //   114
    "COLOR_KHAKI"               , //   115
    "COLOR_WHEAT"               , //   116
    "COLOR_PURPLE1"             , //   117
    "COLOR_PURPLE2"             , //   118
    "COLOR_INDIGO"              , //   119
    "COLOR_DARKMAGENTA"         , //   120
    "COLOR_DARKORCHID"          , //   121
    "COLOR_DARKVIOLET"          , //   122
    "COLOR_MEDIUMORCHID"        , //   123
    "COLOR_ORCHID"              , //   124
    "COLOR_PLUM"                , //   125
    "COLOR_PURPLE"              , //   126
    "COLOR_VIOLET"              , //   127
    "COLOR_ROSYBROWN"           , //   128
    "COLOR_WHITE"               , //   129
    "COLOR_ANTIQUEWHITE"        , //   130
    "COLOR_AZURE"               , //   131
    "COLOR_BEIGE"               , //   132
    "COLOR_BISQUE"              , //   133
    "COLOR_BLANCHEDALMOND"      , //   134
    "COLOR_CORNSILK"            , //   135
    "COLOR_FLORALWHITE"         , //   136
    "COLOR_GHOSTWHITE"          , //   137
    "COLOR_HONEYDEW"            , //   138
    "COLOR_IVORY"               , //   139
    "COLOR_LAVENDERBLUSH"       , //   140
    "COLOR_LEMONCHIFFON"        , //   141
    "COLOR_LINEN"               , //   142
    "COLOR_MINTCREAM"           , //   143
    "COLOR_OLDLACE"             , //   144
    "COLOR_PAPAYAWHIP"          , //   145
    "COLOR_SNOW"                , //   146
    "COLOR_WHITESMOKE"          , //   147
    "COLOR_MISTYROSE"           , //   148
    "COLOR_MOCCASIN"            , //   149
    "COLOR_PALEGOLDENROD"       , //   150
    "COLOR_SEASHELL"            , //   151
};

const uint16_t*  ColorsScreen::colorDefs    = colorData;
const char**     ColorsScreen::colorLabels  = colorNames;

/************************************************************************************
*                    P U B L I C   C L A S S   F U N C T I O N S                    *
*************************************************************************************
*/

// Static Member Initializations
//

// Constructor/Destructor
//
ColorsScreen::ColorsScreen(DisplayScreenInfo& displayInfo) :
    DisplayScreenBase(displayInfo),
    boxWid(BoxWidthDefault),
    boxHgt(BoxHeightDefault),
    boxGapHoriz(BoxGapHorizDefault),
    boxGapVert(BoxGapVertDefault) {

    boxCount = sizeof(colorData) / sizeof(uint16_t);

    uint16_t displayAreaWidth = DISPLAY_WIDTH - (marginLeft + marginRight);

    boxesPerRow = displayAreaWidth / (boxWid + boxGapHoriz);
    rowCount = boxCount / boxesPerRow;
    if ((boxCount % boxesPerRow) != 0) {
        rowCount += 1;
    }

    boxGridWid = (boxWid + boxGapHoriz) * boxesPerRow - boxGapHoriz;
    boxGridHgt = (boxHgt + boxGapVert) * rowCount - boxGapVert;

    boxGridX = marginLeft + (displayAreaWidth - boxGridWid) / 2;;
    boxGridY = marginTop - 5;

    selectedBtn = BtnNone;
}

ColorsScreen::~ColorsScreen() {

}

// activateScreen
//
// Reset/display dynamic screen elements each time screen is displayed
//
void ColorsScreen::activateScreen() {

    // Screen setup
    clearScreen();
    showScreenHeading();

//    uint16_t arraySize = sizeof(colorData) / sizeof(uint16_t);

//    Serial.printf("\nColorsScreen::activateScreen: boxCount %d\n", boxCount);
//    Serial.printf("\nColorsScreen::activateScreen: boxGridX %d boxGridY %d boxGridWid %d boxGridHgt %d\n", boxGridX, boxGridY, boxGridWid, boxGridHgt);
//    Serial.printf("\nColorsScreen::activateScreen: rowCount %d boxesPerRow %d\n", rowCount, boxesPerRow);

    TextArea boxText;

    boxText.wid = boxWid;
    boxText.hgt = boxHgt;
    boxText.align = TextAlignCenter;
    DisplayScreenBase::setFont(Arial_9_Bold);
    DisplayScreenBase::setTextColor(COLOR_BLACK);

    for (int i = 0; i < boxCount; i++) {

        uint8_t boxRow = i / boxesPerRow;
        uint8_t boxCol = i % boxesPerRow;

        uint16_t boxX = boxGridX + boxCol * (boxWid + boxGapHoriz);
        uint16_t boxY = boxGridY + boxRow * (boxHgt + boxGapVert);

        DisplayScreenBase::fillRect(boxX, boxY, boxWid, boxHgt, colorDefs[i]);

        boxText.posX = boxX;
        boxText.posY = boxY;

        snprintf(stringDispBuf, sizeof(stringDispBuf)-1, "%d", i);
        DisplayScreenBase::printInArea(stringDispBuf, boxText);
    }
}

// deActivateScreen
//
void ColorsScreen::deActivateScreen() {

}

// pollScreen
//
// In addition to monitoring touchable screen buttons, ColorsScreen's real-time
// display functions are "piggy-backed" on the pollScreen() function call.
//
// Only the "Home/Back" button will return a value other than BtnNone. Detected
// buttons related to monitoring are processed within ColorsScreen.
//
ButtonId ColorsScreen::pollScreen() {

    // Then check for button presses
    ButtonId touchedBtn = BtnNone;
    uint16_t pointX, pointY;

    // Check for touched buttons
    if (DisplayScreenBase::touched()) {
        DisplayScreenBase::getTouchedPoint(pointX, pointY);

        // Check for Prev button. If pressed, we'll return that value to main application.
        if (DisplayScreenBase::prevBtnTouched(pointX, pointY)) {
            touchedBtn = BtnPrev;
        } else if (isColorButton(pointX, pointY, touchedBtn)) {
//            Serial.printf("ColorsScreen::pollScreen: button %d %s\n", touchedBtn, colorLabels[touchedBtn]);
            setSelected(touchedBtn);
            showSelectedBtnLabel(touchedBtn);
            touchedBtn = BtnNone;   // handled locally
        } else {
            touchedBtn = BtnNone;   // handled locally
        }
    }

    return touchedBtn;
}

// setSelected()
//
void ColorsScreen::setSelected(ButtonId btnNum) {

    if (selectedBtn != BtnNone) {
        drawBtnOutline(selectedBtn, false);
    }

    // Make new selected active and redisplay
    selectedBtn = btnNum;
    drawBtnOutline(selectedBtn, true);
}

// drawBtnOutline()
//
void ColorsScreen::drawBtnOutline(ButtonId selectedBtn, bool btnActive) {

    uint8_t boxRow = selectedBtn / boxesPerRow;
    uint8_t boxCol = selectedBtn % boxesPerRow;

    uint16_t boxX = boxGridX + boxCol * (boxWid + boxGapHoriz);
    uint16_t boxY = boxGridY + boxRow * (boxHgt + boxGapVert);

    uint16_t lineColor = (btnActive) ? COLOR_RED : COLOR_BLACK;

    DisplayScreenBase::drawRect(boxX-1, boxY-1, boxWid+2, boxHgt+2, lineColor);
    DisplayScreenBase::drawRect(boxX-2, boxY-2, boxWid+4, boxHgt+4, lineColor);
//    DisplayScreenBase::drawRect(boxX+1, boxY+1, boxWid-2, boxHgt-2, lineColor);
}

// showSelectedBtnLabel()
//
void ColorsScreen::showSelectedBtnLabel(ButtonId selectedBtn) {

    // Display area
    int16_t dispAreaWid = 400;
    int16_t dispAreaHgt = 40;
    int16_t dispAreaX   = (DISPLAY_WIDTH - dispAreaWid) / 2;
    int16_t dispAreaY   = DISPLAY_HEIGHT - dispAreaHgt + 5;

    TextArea tArea;
    tArea.posX  = dispAreaX;
    tArea.posY  = dispAreaY;
    tArea.wid   = dispAreaWid;
    tArea.hgt   = dispAreaHgt;
    tArea.align = TextAlignCenter;

    DisplayScreenBase::fillRect(dispAreaX, dispAreaY, dispAreaWid, dispAreaHgt, colorDefs[selectedBtn]);

    DisplayScreenBase::setFont(Arial_14);
    DisplayScreenBase::setTextColor(COLOR_BLACK);
    snprintf(stringDispBuf, sizeof(stringDispBuf)-1, "%d: %s", selectedBtn, colorLabels[selectedBtn]);
    DisplayScreenBase::printInArea(stringDispBuf, tArea);
}

/************************************************************************************
*                   P R I V A T E   C L A S S   F U N C T I O N S                   *
*************************************************************************************
*/

// isColumnLine()
//
bool ColorsScreen::isColorButton(int16_t pointX, int16_t pointY, ButtonId& btnNum) {

    bool isColorBtn = false;

    btnNum = BtnNone;

    if ((pointX >= boxGridX) && (pointX <= (boxGridX + boxGridWid))) {
        if ((pointY >= boxGridY) && (pointY <= (boxGridY + boxGridHgt))) {
            isColorBtn = true;
            uint16_t btnCol = (pointX - boxGridX) / (boxWid + boxGapHoriz);
            uint16_t btnRow = (pointY - boxGridY) / (boxHgt + boxGapVert);
            btnNum = btnRow * boxesPerRow + btnCol;
//            Serial.printf("ColorsScreen::isColorButton:touched btnRow %d btnCol %d\n", btnRow, btnCol);
        }
    }

    return isColorBtn;
}

