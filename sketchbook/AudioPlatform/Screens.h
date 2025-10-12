/************************************************************************************
*                                     Screens.h                                     *
*                                                                                   *
*                       Copyright (c) 2025, Donald Swearingen                       *
*                                All Rights Reserved                                *
*************************************************************************************
*
* Description
*
*   Screens.h is a component of the Arduino application AudioPlatform, which defines
*   the structure and appearance of all display screens in that application.
*
*   AudioPlatform display screens are largely data-driven instances of a collection
*   of display classes defined in the user library "DisplayClasses", the appearance
*   and behavior of which are determined by the data structures and elements defined here.
*
*   The associated module "Screens.ino" contains (in the initial implementation) a single
*   function "setActiveScreen()", called from the main AudioPlatform.ino application
*   to display and remove screens based on a user's touch interactions with the screen
*   currently on display.
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

#ifndef SCREENS_H_
#define SCREENS_H_

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
*
* Include files listed below represent dependencies that are referenced in the screen
* definition files (ex.: ScreenDefs/BasicSynthScreenDefs.h) that will be specified in
* Screens.ino includes immediately following the #include "Screens.h" line.
*
* Other modules not listed here are generally referenced in #include statements of
* specific modules that are dynamically displayed during the course of application
* execution.
*
*/

// Application Components
//
#include "AudioPlatform.h"

// User Arduino Libraries
//
// Support Classes
#include "AudioPlatformGlobal.h"
#include "OnOffDisplay.h"
#include "NumberNameDisplay.h"
#include "BitMap.h"
#include "ScreenButton.h"
#include "BitMapData.h"
#include "TestTextColumns.h"
#include "TestListSelect.h"
#include "TestTextEdit.h"
// Screen Classes
#include "DisplayScreenBase.h"
#include "ParamButtonScreen.h"
#include "TextColumnScreen.h"
#include "MainMenuScreen.h"
#include "UtilMenuScreen.h"
#include "FmBanksScreen.h"
#include "FmSynthScreen.h"
#include "MonitorScreen.h"
#include "PresetsScreen.h"
#include "PresetsLiveScreen.h"
#include "SystemProfileScreen.h"
#include "ColorsScreen.h"
#include "ViewMediaLists.h"
#include "SetTimeScreen.h"
#include "SerialFlashUtilScreen.h"

// Teensyduino Classes
//

/************************************************************************************
*                         M A C R O S  /  C O N S T A N T S                         *
*************************************************************************************
*/

/************************************************************************************
*                     D A T A / T Y P E   D E F I N I T I O N S                     *
*************************************************************************************
*/

/************************************************************************************
*            E X P O R T E D   D A T A / T Y P E   D E F I N I T I O N S            *
*************************************************************************************
*
*/

/**********
* General *
***********
*
*/

// Stand-in parameters for undefined structure elements
ParamButtonInfo     dummyBtnParams = (ParamButtonInfo) {0, BtnParam_Type_Int, "Unassigned", "", 0.0, 0.0};
RotaryParams        dummyRotaryParams = {0, 0, 0, {1, 5, 10}, 3, 0, NULL};

// Screen Types (based on types defined in DisplayScreen User Library)
// NOTE: Definition as uint8_t restricts maximum number of screen types to <= 256
typedef enum : uint8_t {
    ScreenTypeDisplay = 0,
    ScreenTypeMenu,
    ScreenTypeParamButton,
    ScreenTypeTextColumn,
    ScreenTypeCustom,
    ScreenTypePopup,
    ScreenTypeNone              // Always Last Entry
} ScreenType;

// Screen Profile for each screen in screenInfo Array (see below)
typedef struct {
    ScreenID            scrnId;
    ScreenType          scrnType;
    DisplayScreenBase&  scrnInstance;

    ScreenID            prevScrnId;
    ScreenID            nextScrnId;

    bool                transientScreen;
} ScreenProfile;

/******************
* Begin Namespace *
*******************
*
* Exported definitions referenced with namespace prefix "Screens::"
*
*/
namespace Screens {

/*******************
* Screen Variables *
********************
*
*/

// Currently active screen
//
ScreenID        activeScreen = ScreenNone;

/************************************************************************************
*              E X P O R T E D   F U N C T I O N   P R O T O T Y P E S              *
*************************************************************************************
*
* Exported functions referenced with namespace prefix "Screens::"
*
*/

void            setActiveScreen(ScreenID screenId);
void            pollActiveScreen();
ScreenProfile*  getActiveScreenProfile();
ScreenProfile*  getScreenProfile(ScreenID screenId);

// Parameter change handlers for AudioPLatform devices called from Presets Manager (Presets.ino).
// Presets Manager calls these functions in all AudioPlatform core components in two cases:
//
//  1. When new presets are loaded, to apply preset settings in other core components.
//  2. When processing applyFn() callbacks from ParamButton screens when preset
//     parameters managed by rotary encoders are modified, again to apply the modified
//     settings in other core components
//
void            setWavPlayerParam(uint16_t paramId, float paramVal);
void            setRawPlayerParam(uint16_t paramId, float paramVal);
void            setBasicSynthParam(uint16_t paramId, float paramVal);
void            setFmSynthParam(uint16_t paramId, float paramVal);
void            setMidiParam(uint16_t paramId, float paramVal);
void            setRvbParam(uint16_t paramId, float paramVal);
void            setEfxParam(uint16_t paramId, float paramVal);
void            setStereoDelayParam(uint16_t paramId, float paramVal);
void            setInputOutputParam(uint16_t paramId, float paramVal);

void            updateStatusDisplay();
void            updateDateTimeDisplay();
void            updateMidiRx();

/****************
* End Namespace *
*****************
*/
}  // namespace Screens


#endif // SCREENS_H_
