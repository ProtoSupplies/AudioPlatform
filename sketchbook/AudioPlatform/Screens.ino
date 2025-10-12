/************************************************************************************
*                                    Screens.ino                                    *
*                                                                                   *
*                       Copyright (c) 2025, Donald Swearingen                       *
*                                All Rights Reserved                                *
*************************************************************************************
*
* Description
*
*   Screens.ino contains the Arduino C-language functions associated with the Screens
*   components of the main Arduino application AudioPlatform (again a .ino C-language
*   module).
*
*   Even though the code herein is standard C code, when placed in the same directory
*   as the main Arduino application (the .ino containing functions seup() and loop())
*   it must have the suffix ".ino" rather than ".c" in order to compile correctly in
*   the Arduino IDE, in conformance with the standards required by that environment
*   (a quirk/deficiency of the original Arduino scheme for user-friendliness).
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
*/

/************************************************************************************
*                       E X T E R N A L   R E F E R E N C E S                       *
*************************************************************************************
*/

// Application Screen Definitions
//
// NOTE: All ScreenDefs depend on definitions in Screens.h. and therefore must be
// included _after_ Screens.h
//
#include "Screens.h"
#include "ScreenDefs/MainMenuScreenDefs.h"
#include "ScreenDefs/SdWavPlayerScreenDefs.h"
#include "ScreenDefs/FlashRawPlayerScreenDefs.h"
#include "ScreenDefs/BasicSynthScreenDefs.h"
#include "ScreenDefs/FmSynthScreenDefs.h"
#include "ScreenDefs/MidiSettingsScreenDefs.h"
#include "ScreenDefs/StereoDelayScreenDefs.h"
#include "ScreenDefs/ReverbSettingsScreenDefs.h"
#include "ScreenDefs/EfxSettingsScreenDefs.h"
#include "ScreenDefs/AudioSettingsScreenDefs.h"
#include "ScreenDefs/ActivityMonitorScreenDefs.h"
#include "ScreenDefs/PresetsScreenDefs.h"
#include "ScreenDefs/PresetsLiveScreenDefs.h"
#include "ScreenDefs/SystemProfileScreenDefs.h"
#include "ScreenDefs/FmBanksScreenDefs.h"
#include "ScreenDefs/ColorsScreenDefs.h"
#include "ScreenDefs/TestTextColumnsScreenDefs.h"
#include "ScreenDefs/TestListSelectScreenDefs.h"
#include "ScreenDefs/ViewMediaListsScreenDefs.h"
#include "ScreenDefs/SerialFlashUtilScreenDefs.h"
#include "ScreenDefs/SetTimeScreenDefs.h"
#include "ScreenDefs/TestTextEditScreenDefs.h"
#include "ScreenDefs/UtilMenuScreenDefs.h"

/******************
* Begin Namespace *
*******************
*/
namespace Screens {

/************************************************************************************
*                   L O C A L   M A C R O S  /  C O N S T A N T S                   *
*************************************************************************************
*/

/************************************************************************************
*                 L O C A L   F U N C T I O N   P R O T O T Y P E S                 *
*************************************************************************************
*
* Local prototype to avoid forward references (in namespace, but not exported
* in .h file).
*
*/

/************************************************************************************
*               L O C A L   D A T A / T Y P E   D E F I N I T I O N S               *
*************************************************************************************
*
* Data types and variables visible only within this module
*
*/

/************************************************************************************
*                     D A T A / T Y P E   D E F I N I T I O N S                     *
*************************************************************************************
*/

/**********
* General *
***********
*
*/

/************************************
* Array of Profiles for All Screens *
*************************************
*
*  !NOTE! Entries must appear in same order as ScreenID enum (see AudioPlatform.h)
*  as the array will be indexed by those same values.
*
*/

ScreenProfile screenProfiles[ScreenCount] = {

//   scrnId                 scrnType                scrnInstance            prevScrnId      nextScrnId      transientScreen
//   ------                 --------                ------------            ----------      ----------      ---------------
    {ScreenMainMenu,        ScreenTypeMenu,         mainMenuScreen,         ScreenNone,     ScreenNone,     false},
    {ScreenUtilMenu,        ScreenTypeMenu,         utilMenuScreen,         ScreenMainMenu, ScreenNone,     false},
    {ScreenWavPlayer,       ScreenTypeParamButton,  sdWavPlayerScreen,      ScreenMainMenu, ScreenNone,     false},
    {ScreenRawPlayer,       ScreenTypeParamButton,  flashRawPlayerScreen,   ScreenMainMenu, ScreenNone,     false},
    {ScreenBasicSynth,      ScreenTypeParamButton,  basicSynthScreen,       ScreenMainMenu, ScreenNone,     false},
    {ScreenFmSynth,         ScreenTypeCustom,       fmSynthScreen,          ScreenMainMenu, ScreenFmBanks,  false},
    {ScreenFmBanks,         ScreenTypePopup,        fmBanksScreen,          ScreenFmSynth,  ScreenNone,     false},
    {ScreenMidi,            ScreenTypeParamButton,  midiSettingsScreen,     ScreenUtilMenu, ScreenNone,     false},
    {ScreenStereoDelay,     ScreenTypeParamButton,  sDlySettingsScreen,     ScreenMainMenu, ScreenNone,     false},
    {ScreenReverb,          ScreenTypeParamButton,  rvbSettingsScreen,      ScreenMainMenu, ScreenNone,     false},
    {ScreenEfx,             ScreenTypeParamButton,  efxSettingsScreen,      ScreenMainMenu, ScreenNone,     false},
    {ScreenInputOutput,     ScreenTypeParamButton,  audioIoScreen,          ScreenMainMenu, ScreenNone,     false},
    {ScreenPresets,         ScreenTypeCustom,       presetsScreen,          ScreenMainMenu, ScreenNone,     false},
    {ScreenMonitor,         ScreenTypeCustom,       monitorScreen,          ScreenMainMenu, ScreenNone,     false},
    {ScreenSystemInfo,      ScreenTypeTextColumn,   profileScreen,          ScreenUtilMenu, ScreenNone,     false},
    {ScreenColors,          ScreenTypeCustom,       colorsScreen,           ScreenUtilMenu, ScreenNone,     false},
    {ScreenTestTextColumns, ScreenTypeTextColumn,   testTextColumnsScreen,  ScreenUtilMenu, ScreenNone,     false},
    {ScreenTestTextEdit,    ScreenTypeCustom,       testTextEditScreen,     ScreenUtilMenu, ScreenNone,     false},
    {ScreenTestListSelect,  ScreenTypeCustom,       testListSelectScreen,   ScreenUtilMenu, ScreenNone,     false},
    {ScreenViewMedia,       ScreenTypeCustom,       viewMediaListsScreen,   ScreenUtilMenu, ScreenNone,     false},
    {ScreenSetTime,         ScreenTypeCustom,       setTimeScreen,          ScreenUtilMenu, ScreenNone,     false},
    {ScreenSerialFlashUtil, ScreenTypeCustom,       serialFlashUtilScreen,  ScreenUtilMenu, ScreenNone,     false},
    {ScreenLive,            ScreenTypeCustom,       presetsLiveScreen,      ScreenMainMenu, ScreenNone,     false}

};

// Packet that conveys regular status information to displayed screen (fixed at MainMenuScreen)
// in initial releases. Defined as static instead of on stack so that individual fields can
// be updated, while other, unchanged fields, can retain their previous values when sent
// to the target screen.
//
static StatusUpdatePacket statusUpdatePacket;

/************************************************************************************
*                        E X P O R T E D   F U N C T I O N S                        *
*************************************************************************************
*
*  Functions callable from external modules
*
*/

// setActiveScreen
//
// Set and display the specified screen
//
void setActiveScreen(ScreenID screenId) {

//    Serial.printf("setActiveScreen activeScreen %d: screen ID: %d\n", activeScreen, screenId);

    if (activeScreen != ScreenNone) {

        // Deactivate current screen
        screenProfiles[activeScreen].scrnInstance.deActivateScreen();
    }

    if (screenId != ScreenNone) {

        // Activate new screen
        activeScreen = screenId;
        screenProfiles[activeScreen].scrnInstance.activateScreen();

        // Main Menu Time and Footer Displays
        // Send update to any active screen. All screens are derived from DisplayScreenBase which defines
        // updateScreen() as virtual. So, only screens that override updateScreen will receive the message,
        // and only when they are the active screen.
        updateStatusDisplay();
    }
}

// pollActiveScreen
//
// Regularly poll for user screen touches that involve screen navigation. Note also that this
// polling is required on a repeated basis to ensure that user interactions are processed on
// a timely basis.
//
// Note that, except for menu screens, pollScreen() calls only return values >= CommandButtonBaseId.
// In the case of menu screens, return values < CommandButtonBaseId represent menu panel selections
// in the range [0, n-1] where n is the number of buttons in the menu. These numbers represent indices
// into the ScreenID enum and are used to identify the next screen to display.
//
void pollActiveScreen() {

    ButtonId pollResp;
    ScreenProfile *activeScrnProfile;
    ScreenID newScrnId = ScreenNone;

    // Poll active screen
    activeScrnProfile = getActiveScreenProfile();
    pollResp = activeScrnProfile->scrnInstance.pollScreen();

    // Handle screen poll response
    if (activeScreen != ScreenNone) {

        if (activeScrnProfile->scrnType == ScreenTypeMenu) {
            if (pollResp == BtnPrev) {
                if (activeScrnProfile->prevScrnId != ScreenNone) {
                    newScrnId = activeScrnProfile->prevScrnId;
                }
            } else if ((ScreenID)pollResp != ScreenNone && pollResp != BtnNone) {
                newScrnId = (ScreenID)pollResp;
            }

        } else {
            if (pollResp == BtnPrev) {
                if (activeScrnProfile->prevScrnId != ScreenNone) {
                    newScrnId = activeScrnProfile->prevScrnId;
                }

            } else if (pollResp == BtnNext) {
                if (activeScrnProfile->nextScrnId != ScreenNone) {
                    newScrnId = activeScrnProfile->nextScrnId;
                }
            }
        }
    }

    // Change to new screen if valid ID
    if (newScrnId != ScreenNone) {
        ScreenProfile *nextScrnProfile = getScreenProfile(newScrnId);
        if (nextScrnProfile->transientScreen) {
            nextScrnProfile->prevScrnId = activeScrnProfile->scrnId;
        }
        Screens::setActiveScreen(newScrnId);
    }
}

// getActiveScreenProfile
//
// Return a pointer to the currrently active screen
//
ScreenProfile *getActiveScreenProfile() {

    return &screenProfiles[activeScreen];

}

// getScreenProfile
//
// Return a pointer to the currrently active screen
//
ScreenProfile *getScreenProfile(ScreenID screenId) {

    return &screenProfiles[screenId];
}

// setWavPlayerParam
//
void setWavPlayerParam(uint16_t paramId, float paramVal) {

//    Serial.printf("Screens::setWavPlayerParam paramId: %d paramVal: %f\n", paramId, paramVal);

    sdWavPlayerScreen.setBtnProperties(paramId, paramVal, (activeScreen == ScreenWavPlayer));

}

// setRawPlayerParam
//
void setRawPlayerParam(uint16_t paramId, float paramVal) {

//    Serial.printf("Screens::setRawPlayerParam paramId: %d paramVal: %f\n", paramId, paramVal);

    flashRawPlayerScreen.setBtnProperties(paramId, paramVal, (activeScreen == ScreenRawPlayer));

}

// setBasicSynthParam
//
void setBasicSynthParam(uint16_t paramId, float paramVal) {

//    Serial.printf("Screens::setBasicSynthParam paramId: %d paramVal: %f\n", paramId, paramVal);

    basicSynthScreen.setBtnProperties(paramId, paramVal, (activeScreen == ScreenBasicSynth));

}

// setFmSynthParam
//
void setFmSynthParam(uint16_t paramId, float paramVal) {

//    Serial.printf("Screens::setFmSynthParam paramId: %d paramVal: %f\n", paramId, paramVal);

    fmSynthScreen.setBtnProperties(paramId, paramVal, (activeScreen == ScreenFmSynth));

}

// setMidiParam
//
void setMidiParam(uint16_t paramId, float paramVal) {

//    Serial.printf("Screens::setMidiParam paramId: %d paramVal: %f\n", paramId, paramVal);

    midiSettingsScreen.setBtnProperties(paramId, paramVal, (activeScreen == ScreenBasicSynth));

}

// setRvbParam
//
void setRvbParam(uint16_t paramId, float paramVal) {

//    Serial.printf("Screens::setRvbParam paramId: %d paramVal: %f\n", paramId, paramVal);

    rvbSettingsScreen.setBtnProperties(paramId, paramVal, (activeScreen == ScreenReverb));

}

// setEfxParam()
//
void setEfxParam(uint16_t paramId, float paramVal) {

//    Serial.printf("AudioDevices::setEfxParam paramId: %d paramVal: %f\n", paramId, paramVal);

    efxSettingsScreen.setBtnProperties(paramId, paramVal, (activeScreen == ScreenEfx));
}

// setStereoDelayParam
//
void setStereoDelayParam(uint16_t paramId, float paramVal) {

//    Serial.printf("Screens::setStereoDelayParam paramId: %d paramVal: %f\n", paramId, paramVal);

    sDlySettingsScreen.setBtnProperties(paramId, paramVal, (activeScreen == ScreenStereoDelay));

}

// setInputOutputParam
//
void setInputOutputParam(uint16_t paramId, float paramVal) {

//    Serial.printf("Screens::setInputOutputParam paramId: %d paramVal: %f\n", paramId, paramVal);

    audioIoScreen.setBtnProperties(paramId, paramVal, (activeScreen == ScreenInputOutput));

}

/************************************************************************************
*                           L O C A L   F U N C T I O N S                           *
*************************************************************************************
*
*/

// updateStatusDisplay
//
void updateStatusDisplay() {

    if (Presets::presetsInitialized()) {

//        Serial.printf("Screens::updateStatusDisplay: activeScreen %d\n", activeScreen);

        statusUpdatePacket.presetNumber = Presets::getPresetNumber() + 1;   // Convert [0,n-1] to user-friendly [1,n]
        statusUpdatePacket.presetName = Presets::getPresetName(Presets::getPresetNumber());
        statusUpdatePacket.wavPlayerEnabled = (bool)Presets::getWavPlayerParam(MediaPlayWav_Status);
        statusUpdatePacket.rawPlayerEnabled = (bool)Presets::getRawPlayerParam(MediaPlayRaw_Status);
        statusUpdatePacket.basicSynthEnabled = (bool)Presets::getBasicSynthParam(BasicSynth_Status);
        statusUpdatePacket.fmSynthEnabled = (bool)Presets::getFmSynthParam(FmSynth_Status);
        statusUpdatePacket.lineInEnabled =(bool)Presets::getInputOutputParam(AudioIo_LineIn_Status);
        statusUpdatePacket.timeStr = "";

        // Send to any active screen. All screens are derived from DisplayScreenBase which defines
        // updateScreen() as virtual. So, only screens that override updateScreen will receive the message,
        // and only when they are the active screen.
        screenProfiles[activeScreen].scrnInstance.updateScreen(StatusPacket, (void*) &statusUpdatePacket);
    }
}

// updateDateTimeDisplay
//
void updateDateTimeDisplay() {

//    Serial.printf("Screens::updateDateTimeDisplay:\n");

    // Update screens that display current time
    if (activeScreen == ScreenMainMenu) {
        // Pass string to main screen display
        statusUpdatePacket.timeStr = Teensy41_Util::parseRtcDateTime();
        screenProfiles[ScreenMainMenu].scrnInstance.updateScreen(DateTimePacket, (void*) &statusUpdatePacket);

    } else if (activeScreen == ScreenSetTime) {
        DateTimeFields dt;
        Teensy41_Util::parseRtcDateTime(dt);
        screenProfiles[ScreenSetTime].scrnInstance.updateScreen(DateTimePacket, (void*) &dt);
    }
}

// updateMidiRx
//
// Called from InputDevices when new MIDI input data received
//
void updateMidiRx() {

//    Serial.printf("Screens::updateMidiRx\n");

    if (activeScreen == ScreenMonitor) {
        // Inform monitor screen of new MIDI input
        screenProfiles[ScreenMonitor].scrnInstance.updateScreen(MidiRxPacket, (void*) 0);
    }
}

/****************
* End Namespace *
*****************
*/
}  // namespace Screens
