 /************************************************************************************
*                                DisplayScreenBase.h                                *
*                                                                                   *
*                       Copyright (c) 2025, Donald Swearingen                       *
*                                All Rights Reserved                                *
*************************************************************************************
*
* Description
*
*   DisplayScreenBase is the base class from which all screen display classes in the
*   DisplayScreen library are ultimately derived.
*
*   See DisplayScreenBase.cpp for further details.
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
* Defines that affect following and external references
*
*/

#define GUI_TESTING     0

#if GUI_TESTING // Testing modifications to GUI
#else // Original Settings
#endif // GUI_TESTING / Original Settings

/************************************************************************************
*                       E X T E R N A L   R E F E R E N C E S                       *
*************************************************************************************
*/
#ifndef DISPLAYSCREENBASE_H_
#define DISPLAYSCREENBASE_H_

// Standard C++
//
#include <Math.h>
#include <stdio.h>
#include <stdint.h>

// Teensyduino
//
#include <SPI.h>
#include <RA8875.h>

// Installed Arduino Libraries
//
#include <font_Arial.h>
#include <font_ArialBold.h>
#include <elapsedMillis.h>

// User Arduino Libraries
//
#include "ScreenUpdatePackets.h"
#include "ArduinoEclipseDefs.h"

/************************************************************************************
*                         M A C R O S  /  C O N S T A N T S                         *
*************************************************************************************
*/

// Buffer for transient snprintf() usage
//
const uint16_t  StringDispBufLength = 128;

// Color Definitions
//
// Using ILI9341/RA8875 Color Scheme
//
const uint16_t  COLOR_BLACK         = 0x0000;   //   0,   0,   0
const uint16_t  COLOR_BLUE          = 0x001F;   //   0,   0, 255
const uint16_t  COLOR_CYAN          = 0x07FF;   //   0, 255, 255
const uint16_t  COLOR_DARKCYAN      = 0x03EF;   //   0, 128, 128
const uint16_t  COLOR_DARKGREEN     = 0x03E0;   //   0, 128,   0
const uint16_t  COLOR_DARKGREY      = 0x7BEF;   // 128, 128, 128
const uint16_t  COLOR_DARKORANGE    = 0xFB60;   // the experimentalist
const uint16_t  COLOR_GREEN         = 0x07E0;   //   0, 255,   0
const uint16_t  COLOR_GREENYELLOW   = 0xAFE5;   // 173, 255,  47
const uint16_t  COLOR_LIGHTGREY1    = 0xC618;;  // 192, 192, 192
const uint16_t  COLOR_LIGHTGREY2    = 0xB5B2;   // the experimentalist
const uint16_t  COLOR_MAGENTA       = 0xF81F;   // 255,   0, 255
const uint16_t  COLOR_MAROON        = 0x7800;   // 128,   0,   0
const uint16_t  COLOR_NAVY          = 0x000F;   //   0,   0, 128
const uint16_t  COLOR_OLIVE         = 0x7BE0;   // 128, 128,   0
const uint16_t  COLOR_ORANGE        = 0xFD20;   // 255, 165,   0
const uint16_t  COLOR_PINK1         = 0xFCFF;   // M.Sandercock
const uint16_t  COLOR_PINK2         = 0xF81F;
const uint16_t  COLOR_PURPLE1       = 0x780F;   // 128,   0, 128
const uint16_t  COLOR_PURPLE2       = 0x8017;   // M.Sandercock
const uint16_t  COLOR_RED           = 0xF800;   // 255,   0,   0
const uint16_t  COLOR_YELLOW        = 0xFFE0;   // 255, 255,   0
const uint16_t  COLOR_WHITE         = 0xFFFF;   // 255, 255, 255

// Color Definitions
//
// From the Web
//
const uint16_t  COLOR_ALICEBLUE     = 0xF7DF;
const uint16_t  COLOR_ANTIQUEWHITE  = 0xFF5A;
const uint16_t  COLOR_AQUA          = 0x07FF;
const uint16_t  COLOR_AQUAMARINE    = 0x7FFA;
const uint16_t  COLOR_AZURE         = 0xF7FF;
const uint16_t  COLOR_BEIGE         = 0xF7BB;
const uint16_t  COLOR_BISQUE        = 0xFF38;
const uint16_t  COLOR_BLANCHEDALMOND = 0xFF59;
const uint16_t  COLOR_BLUEVIOLET    = 0x895C;
const uint16_t  COLOR_BROWN         = 0xA145;
const uint16_t  COLOR_BURLYWOOD     = 0xDDD0;
const uint16_t  COLOR_CADETBLUE     = 0x5CF4;
const uint16_t  COLOR_CHARTREUSE    = 0x7FE0;
const uint16_t  COLOR_CHOCOLATE     = 0xD343;
const uint16_t  COLOR_CORAL         = 0xFBEA;
const uint16_t  COLOR_CORNFLOWERBLUE = 0x64BD;
const uint16_t  COLOR_CORNSILK      = 0xFFDB;
const uint16_t  COLOR_CRIMSON       = 0xD8A7;
const uint16_t  COLOR_DARKBLUE      = 0x0011;
const uint16_t  COLOR_DARKCYAN2     = 0x0451;
const uint16_t  COLOR_DARKGOLDENROD = 0xBC21;
const uint16_t  COLOR_DARKGREEN2    = 0x0320;
const uint16_t  COLOR_DARKKHAKI     = 0xBDAD;
const uint16_t  COLOR_DARKMAGENTA   = 0x8811;
const uint16_t  COLOR_DARKOLIVEGREEN = 0x5345;
const uint16_t  COLOR_DARKORCHID    = 0x9999;
const uint16_t  COLOR_DARKRED       = 0x8800;
const uint16_t  COLOR_DARKSALMON    = 0xECAF;
const uint16_t  COLOR_DARKSEAGREEN  = 0x8DF1;
const uint16_t  COLOR_DARKSLATEBLUE = 0x49F1;
const uint16_t  COLOR_DARKSLATEGRAY = 0x2A69;
const uint16_t  COLOR_DARKTURQUOISE = 0x067A;
const uint16_t  COLOR_DARKVIOLET    = 0x901A;
const uint16_t  COLOR_DEEPPINK      = 0xF8B2;
const uint16_t  COLOR_DEEPSKYBLUE   = 0x05FF;
const uint16_t  COLOR_DIMGRAY       = 0x6B4D;
const uint16_t  COLOR_DODGERBLUE    = 0x1C9F;
const uint16_t  COLOR_FIREBRICK     = 0xB104;
const uint16_t  COLOR_FLORALWHITE   = 0xFFDE;
const uint16_t  COLOR_FORESTGREEN   = 0x2444;
const uint16_t  COLOR_FUCHSIA       = 0xF81F;
const uint16_t  COLOR_GAINSBORO     = 0xDEFB;
const uint16_t  COLOR_GHOSTWHITE    = 0xFFDF;
const uint16_t  COLOR_GOLD          = 0xFEA0;
const uint16_t  COLOR_GOLDENROD     = 0xDD24;
const uint16_t  COLOR_GRAY          = 0x8410;
const uint16_t  COLOR_GREEN2        = 0x0400;
const uint16_t  COLOR_HONEYDEW      = 0xF7FE;
const uint16_t  COLOR_HOTPINK       = 0xFB56;
const uint16_t  COLOR_INDIANRED     = 0xCAEB;
const uint16_t  COLOR_INDIGO        = 0x4810;
const uint16_t  COLOR_IVORY         = 0xFFFE;
const uint16_t  COLOR_KHAKI         = 0xF731;
const uint16_t  COLOR_LAVENDER      = 0xE73F;
const uint16_t  COLOR_LAVENDERBLUSH = 0xFF9E;
const uint16_t  COLOR_LAWNGREEN     = 0x7FE0;
const uint16_t  COLOR_LEMONCHIFFON  = 0xFFD9;
const uint16_t  COLOR_LIGHTBLUE     = 0xAEDC;
const uint16_t  COLOR_LIGHTCORAL    = 0xF410;
const uint16_t  COLOR_LIGHTCYAN     = 0xE7FF;
const uint16_t  COLOR_LIGHTGOLDENRODYELLOW = 0xFFDA;
const uint16_t  COLOR_LIGHTGREEN    = 0x9772;
const uint16_t  COLOR_LIGHTGREY     = 0xD69A;
const uint16_t  COLOR_LIGHTPINK     = 0xFDB8;
const uint16_t  COLOR_LIGHTSALMON   = 0xFD0F;
const uint16_t  COLOR_LIGHTSEAGREEN = 0x2595;
const uint16_t  COLOR_LIGHTSKYBLUE  = 0x867F;
const uint16_t  COLOR_LIGHTSLATEGRAY = 0x7453;
const uint16_t  COLOR_LIGHTSTEELBLUE = 0xB63B;
const uint16_t  COLOR_LIGHTYELLOW   = 0xFFFC;
const uint16_t  COLOR_LIME          = 0x07E0;
const uint16_t  COLOR_LIMEGREEN     = 0x3666;
const uint16_t  COLOR_LINEN         = 0xFF9C;
const uint16_t  COLOR_MAROON2       = 0x8000;
const uint16_t  COLOR_MEDIUMAQUAMARINE = 0x6675;
const uint16_t  COLOR_MEDIUMBLUE    = 0x0019;
const uint16_t  COLOR_MEDIUMORCHID  = 0xBABA;
const uint16_t  COLOR_MEDIUMPURPLE  = 0x939B;
const uint16_t  COLOR_MEDIUMSEAGREEN = 0x3D8E;
const uint16_t  COLOR_MEDIUMSLATEBLUE = 0x7B5D;
const uint16_t  COLOR_MEDIUMSPRINGGREEN = 0x07D3;
const uint16_t  COLOR_MEDIUMTURQUOISE = 0x4E99;
const uint16_t  COLOR_MEDIUMVIOLETRED = 0xC0B0;
const uint16_t  COLOR_MIDNIGHTBLUE  = 0x18CE;
const uint16_t  COLOR_MINTCREAM     = 0xF7FF;
const uint16_t  COLOR_MISTYROSE     = 0xFF3C;
const uint16_t  COLOR_MOCCASIN      = 0xFF36;
const uint16_t  COLOR_NAVAJOWHITE   = 0xFEF5;
const uint16_t  COLOR_NAVY2          = 0x0010;
const uint16_t  COLOR_OLDLACE       = 0xFFBC;
const uint16_t  COLOR_OLIVE2        = 0x8400;
const uint16_t  COLOR_OLIVEDRAB     = 0x6C64;
const uint16_t  COLOR_ORANGERED     = 0xFA20;
const uint16_t  COLOR_ORCHID        = 0xDB9A;
const uint16_t  COLOR_PALEGOLDENROD = 0xEF55;
const uint16_t  COLOR_PALEGREEN     = 0x9FD3;
const uint16_t  COLOR_PALETURQUOISE = 0xAF7D;
const uint16_t  COLOR_PALEVIOLETRED = 0xDB92;
const uint16_t  COLOR_PAPAYAWHIP    = 0xFF7A;
const uint16_t  COLOR_PEACHPUFF     = 0xFED7;
const uint16_t  COLOR_PERU          = 0xCC27;
const uint16_t  COLOR_PINK          = 0xFE19;
const uint16_t  COLOR_PLUM          = 0xDD1B;
const uint16_t  COLOR_POWDERBLUE    = 0xB71C;
const uint16_t  COLOR_PURPLE        = 0x8010;
const uint16_t  COLOR_ROSYBROWN     = 0xBC71;
const uint16_t  COLOR_ROYALBLUE     = 0x435C;
const uint16_t  COLOR_SADDLEBROWN   = 0x8A22;
const uint16_t  COLOR_SALMON        = 0xFC0E;
const uint16_t  COLOR_SANDYBROWN    = 0xF52C;
const uint16_t  COLOR_SEAGREEN      = 0x2C4A;
const uint16_t  COLOR_SEASHELL      = 0xFFBD;
const uint16_t  COLOR_SIENNA        = 0xA285;
const uint16_t  COLOR_SILVER        = 0xC618;
const uint16_t  COLOR_SKYBLUE       = 0x867D;
const uint16_t  COLOR_SLATEBLUE     = 0x6AD9;
const uint16_t  COLOR_SLATEGRAY     = 0x7412;
const uint16_t  COLOR_SNOW          = 0xFFDF;
const uint16_t  COLOR_SPRINGGREEN   = 0x07EF;
const uint16_t  COLOR_STEELBLUE     = 0x4416;
const uint16_t  COLOR_TAN           = 0xD5B1;
const uint16_t  COLOR_TEAL          = 0x0410;
const uint16_t  COLOR_THISTLE       = 0xDDFB;
const uint16_t  COLOR_TOMATO        = 0xFB08;
const uint16_t  COLOR_TURQUOISE     = 0x471A;
const uint16_t  COLOR_VIOLET        = 0xEC1D;
const uint16_t  COLOR_WHEAT         = 0xF6F6;
const uint16_t  COLOR_WHITESMOKE    = 0xF7BE;
const uint16_t  COLOR_YELLOWGREEN   = 0x9E66;

/*****************************
* Display/TouchScreen Layout *
******************************
*/

// RA8875 Screen/Text Orientations on AudioPlatform
//
//        Normal
//      0 Degrees           90 Degrees       180 Degrees    270 Degrees
//         Top                 Left            Bottom          Right
//          ^                   ^                ^               ^
//   Left < 0 > Right  Bottom < 1 > Top  Right < 2 > Left  Top < 3 > Bottom
//          v                   v                v               v
//        Bottom              Right             Top             Left
//
#define RA8875_ROTATION_0       0   // (top ^) 800 wid x 480 hgt (Normal)
#define RA8875_ROTATION_90      1   // (top >) 480 wid x 840 hgt
#define RA8875_ROTATION_180     2   // (top v) 800 wid x 480 hgt
#define RA8875_ROTATION_270     3   // (top <) 480 wid x 840 hgt

// ILI9341 Screen/ Text Orientations wrt Baseboard on ProtoSupplies Prototyping System
//
//        Normal
//      0 Degrees       90 Degrees           180 Degrees       270 Degrees
//         Top             Right                Bottom              Left
//          ^                ^                    ^                  ^
//   Left < 3 > Right  Top < 2 > Bottom   Right < 1 > Left  Bottom < 0 > Top
//          v                v                    v                  v
//        Bottom            Left                 Top               Right
//
#define ILI9341_ROTATION_0      3   // ^ 320 wid x 240 hgt (Normal)
#define ILI9341_ROTATION_90     2   // < 240 wid x 320 hgt
#define ILI9341_ROTATION_180    1   // v 320 wid x 240 hgt
#define ILI9341_ROTATION_270    0   // > 240 wid x 320 hgt

// LCD Display
//
#define DISPLAY_ROTATION            RA8875_ROTATION_0
#define DISPLAY_WIDTH               800
#define DISPLAY_HEIGHT              480
#define DISPLAY_BGD_COLOR_DEFAULT   COLOR_BLACK
#define HDR_FONT_DEFAULT            Arial_20
#define HDR_TEXT_COLOR_DEFAULT      COLOR_WHITE
#define SUBHDR_FONT_DEFAULT         Arial_12_Bold
#define SUBHDR_TEXT_COLOR_DEFAULT   COLOR_WHITE

// Pins used for LCD and touch.  Using SPI1 bus.
//
const int RA8875_MISO =             39;
const int RA8875_MOSI =             26;
const int RA8875_SCLK =             27;
const int RA8875_CS =               5;
const int RA8875_RESET =            9;
const int RA8875_INT =              2;

// Touch Detection
//
const int MAXTOUCHLIMIT             = 1;        // (1 to 5) number of simultaneous touches allowed
const int TOUCH_INTERVAL_DEFAULT    = 25;       // ms
const int TOUCHPOINT_MAX_X_DIST     = 5;        // Max X,Y distance from current touched point beyond
const int TOUCHPOINT_MAX_Y_DIST     = 5;        // which a new touched point will be reported.

// TFT/LCD Display and Touchscreen Pins
//
// NOTE: Thes pins are specific to the hardware on which the ILI9341 TFT and the
// XPT2046 Touchscreen are installed. The settings used here are for the ProtoSupplies
// Teensy 4.1 Development Board.
//
#define TFT_CS_PIN                  40
#define TFT_DC                      9
#define TS_CS_PIN                   41

// TouchScreen
//
#define TS_MINX                     400
#define TS_MINY                     400
#define TS_MAXX                     3879
#define TS_MAXY                     3843

// Default Screen Margins
//
#define DEFAULT_SCRN_MARGIN_TOP     90
#define DEFAULT_SCRN_MARGIN_BOTTOM  90
#define DEFAULT_SCRN_MARGIN_LEFT    10
#define DEFAULT_SCRN_MARGIN_RIGHT   10

/************************************************************************************
*                     D A T A / T Y P E   D E F I N I T I O N S                     *
*************************************************************************************
*/

/********
* Types *
*********
*/

// Text Alignment
//
typedef enum {
    TextAlignLeft,
    TextAlignCenter,
    TextAlignRight
} TextAlign;

// Text Position (used in relation to object)
//
typedef enum {
    TextLeft,
    TextRight,
    TextAbove,
    TextBelow,
    TextInFront
} TextPosition;

// Text Display Area
//
typedef struct {
    int16_t                 posX;
    int16_t                 posY;
    uint16_t                wid;
    uint16_t                hgt;
    TextAlign               align;
} TextArea;

// Base Screen Information
typedef struct {
    uint16_t                displayBgdColor;
    uint16_t                marginTop;
    uint16_t                marginBottom;
    uint16_t                marginLeft;
    uint16_t                marginRight;
    ILI9341_t3_font_t       headerFont;
    uint16_t                headerFontColor;
    ILI9341_t3_font_t       subHeaderFont;
    uint16_t                subHeaderFontColor;
    const char*             screenHeader;
    const char*             screenSubHeader;
    TextAlign               textAlign;
    bool                    showPrevBtn;
    const char*             prevBtnText;
} DisplayScreenInfo;

// Display Button Profile
//
typedef struct {
    int16_t     btnX;
    int16_t     btnY;
    uint16_t    btnWid;
    uint16_t    btnHgt;
    uint16_t    btnColor;
    uint16_t    btnOutlineColor;
} DisplayButtonStruct;       // Replace name of current usages

// Circle Geometry Parameters
//
typedef struct {
    int16_t         xPos;
    int16_t         yPos;
    uint16_t        radius;
} CircleParams;

// General Purpose ID
//
typedef uint16_t ObjectId;

// ButtonId Type
//
// ButtonIDs identify fixed command buttons and objects, as well as generic buttons
// unique to each screen and/or class. That is, different screens/classes may use the
// same ButtonId for different purposes as needed.
//
// The enumeration CommandButtonId, reserves a section of the numeric range of the
// ButtonIDs as fixed IDs with the same meaning wherever they are applied, leaving
// leave room below their defined range, for arbitrary ID definitions and usage in
// appropriate to the settings in which they are used (classes, objects, etc.)
//
// The value CommandButtonBaseId defines the maximum ID available for such free usage
// as [0, CommandButtonBaseId-1]. Note that the definition of CommandButtonBaseId and
// the other CommandButtonIds can be moved or modified as needed without affecting the
// application compilation and execution.
//
// FIXME: This has to become an enum (perhaps of uint16_t) to keep from mixing up
// and/or duplicating entries. 10/8/24: Testing enum approach below with uint16_t IDs.
//
typedef ObjectId ButtonId;

typedef enum : ButtonId {

    CommandButtonBaseId = 1000,
    BtnPrev,
    BtnNext,

    BtnLoad,
    BtnSave,
    BtnImport,
    BtnEdit,
    BtnRename,
    BtnSelect,
    BtnSelectActive,
    BtnSelectImport,
    BtnCancel,
    BtnProceed,
    BtnExit,

    BtnBkSpc,
    BtnClear,

    BtnUpper,
    BtnLower,

    BtnUp,
    BtnDown,

    BtnDefaultBank,
    BtnSdCardBank,
    BtnActiveBank,
    BtnImportBank,

    // Screen->Presets::presetsCallback() commands. Passed to PresetsScreen
    // and PresetsLiveScreen in their class declarations in ScreenDefs folder
    CmdLoadPreset,
    CmdImportPreset,
    CmdSavePreset,
    CmdClearPreset,
    CmdGetPresetNumber,
    CmdGetPresetName,
    CmdGetImportPresetName,
    CmdSetPresetName,
    CmdCopyDeviceSettings,
    CmdSetPresetModified,
    CmdGetPresetModified,
    CmdSetPresetDeviceModified,
    CmdGetPresetDeviceModified,
    CmdSetBankModified,
    CmdGetBankModified,
    CmdLoadDefaultBank,
    CmdSaveDefaultBank,
    CmdLoadSdBank,
    CmdSaveSdBank,
    CmdGetBankFilename,
    CmdIgnoreNoteOff,
    CmdAllNotesOff,

    BtnNone = 9999

} CommandButtonId;

/************************************************************************************
*                         C L A S S   D E F I N I T I O N S                         *
*************************************************************************************
*/

class DisplayScreenBase {

 // Constructor/Destructor
//
public:

    DisplayScreenBase();
    DisplayScreenBase(DisplayScreenInfo& screenInfo);
    virtual ~DisplayScreenBase();

// Class Variables
//
public:


private:

    // TFT/LCD Display / Touchscreen
    static RA8875           tftDisplay;

    static bool             baseInitialized;

    static uint16_t         lastTouchedY;
    static uint16_t         lastTouchedX;
    static uint16_t         touchInterval;

protected:

    // Buffer for snprintf() in class and subclasses
    EXTMEM static char      stringDispBuf[StringDispBufLength];

    uint16_t                displayBgdColor;
    uint16_t                marginTop;
    uint16_t                marginBottom;
    uint16_t                marginLeft;
    uint16_t                marginRight;
    ILI9341_t3_font_t       headerFont;
    uint16_t                headerFontColor;
    ILI9341_t3_font_t       subHeaderFont;
    uint16_t                subHeaderFontColor;
    const char*             screenHeader;
    const char*             screenSubHeader;
    TextAlign               textAlign;
    static bool             reportTouchRelease;
    static elapsedMillis    touchTimer;

    DisplayButtonStruct     prevButton;
    bool                    showPrevBtn;
    const char*             prevBtnText;

// Class Methods
//
public:

    static void             setupDisplay();
    void                    setScreenInfo(DisplayScreenInfo* displayInfo);
    void                    clearScreen();
    void                    setDisplayFonts(ILI9341_t3_font_t hdrFont, uint16_t hdrFontColor, ILI9341_t3_font_t subHdrFont, uint16_t subHdrFontColor);

    virtual void            showScreenHeading();
    virtual void            activateScreen() {};
    virtual void            deActivateScreen() {};
    virtual void            updateScreen(StatusUpdatePacketType pktType, void* updateInfoPkt) {};
    virtual void            setDisplayMargins(uint16_t top, uint16_t bottom, uint16_t left, uint16_t right);

    // NOTE (TODO): pollScreen() is available on all screens because of the need to
    // detect a touch on the Prev/Next buttons. However, there is also a case to be
    // made for a virtual pollScreen() used in cases where that is the only button,
    // and the screen needs regular "polling" to carry out real-time operations (such
    // as the Activity Monitor screen), which at present piggy-backs on the pollScreen()
    // function call.
    virtual ButtonId        pollScreen() {return BtnNone;};

    // NOTE (TODO): Originally created to allow ParamButtonScreen buttons to be updated
    // in real time by external controls, for example to assign MIDI controls to effects
    // parameters when a MIDI controller is moved. Used here simply as a way to set the
    // current preset, called from Screens.ino. As of v0.3.4.4, only being implemented in
    // PresetsScreen and ParamButtonScreen. Need to find a more direct and less confusing
    // means of implementing these two uses. (T240910-1)
    //
    // setBtnProperties() is defined as virtual in the display base class so that it can be
    // accessed by a generic DisplayScreenBase variable (which may be assigned to an instance
    // of any of its derivative classes) without the need for a dynamic cast, which is not
    // available in the Arduino IDE compiler. As in the following example:
    //
    //    ParamButtonScreen   sdWavPlayerScreen;
    //    DisplayScreenBase&  screenInstance;
    //
    //    screenInstance = sdWavPlayerScreen;
    //    screenInstance->setBtnProperties(...);
    //
    virtual void            setBtnProperties(uint16_t paramId, float btnValue, bool screenActive) {};
    virtual void            setBtnProperties(ButtonId btnId, bool screenActive) {};

    // Indirect Access Functions
    //
    // Following static functions provide a layer of indirection between application,
    // subclasses and the physical display in use (either ILI9341 or RA8875 in initial
    // release)
    //
    // TFT Display
    static void             setRotation(uint8_t rotation);
    static int16_t          width(void);
    static int16_t          height(void);
    static void             fillScreen(uint16_t color);
    static void             drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
    static void             fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
    static void             drawRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color);
    static void             fillRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color);
    static void             writeRect(int16_t x, int16_t y, int16_t w, int16_t h, const uint16_t *pcolors);
    static void             drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
    static void             fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
    static void             drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
    static void             setFont(const ILI9341_t3_font_t &f);
    static void             setTextColor(uint16_t c);
    static void             setTextColor(uint16_t c, uint16_t bg);
    static void             measureText(const char* text, uint16_t& textWidth, uint16_t& textHeight);
    static void             setCursor(int16_t x, int16_t y);
    static size_t           print(const char s[]);
    static size_t           printInArea(const char s[], TextArea& area);
    // TouchScreen
    static bool             touched(uint16_t touchWait = TOUCH_INTERVAL_DEFAULT, bool repeat = false);
    static void             getTouchedPoint(uint16_t& x, uint16_t& y);
    static uint8_t          getTScoordinates(uint16_t (*touch_coordinates)[2]);
    static bool             rectTouched(int16_t touchX, int16_t touchY, int16_t rectX, int16_t rectY, uint16_t rectWid, uint16_t rectHgt);
    static bool             circleTouched(uint16_t touchX, uint16_t touchY, uint16_t circleX, uint16_t circleY, uint16_t circleRadius);

protected:

    bool                    prevBtnTouched(int16_t x, int16_t y);

private:

    void                    drawPrevBtn();
};

#endif // DISPLAYSCREENBASE_H_
