/************************************************************************************
*                               ParamButtonScreen.cpp                               *
*                                                                                   *
*                       Copyright (c) 2025, Donald Swearingen                       *
*                                All Rights Reserved                                *
*************************************************************************************
*
* Description
*
*   ParamButtonScreen is a sub-class of class ButtonScreen, which itself is a subclass
*   of the DisplayScreenBase class, inheriting variables and methods from both
*   classes, in particular inheriting DisplayScreenBase's static variables "tftDisplay"
*   and "touchScreen" (common to all display screens).
*
*   ParamButtonScreen creates a display screen containing a an array of screen "buttons"
*   (BtlPanel, filled rectangles arranged in rows and columns), each of which represents
*   a specific parameter associated with the application in which it resides (for example
*   the volume level of an audio device), displaying the name of the parameter and its
*   current value.
*
* Rotary Encoders
*
*   At the bottom of the screen, there is a row of circles, one for each column in
*   the button array. The circles represent rotary encoders that are used to modify
*   the parameter values of a "selected" button in the column above (a single button
*   in each column is selected for editing by touching the button).
*
*   Inside each rotary circle is a number representing the increment by which the
*   selected button's parameter value will change +/- as the encoder is rotated,
*   respectively, clockwise and counter-clockwise. The displayed increment value is
*   specific to each selected button, and will change to the current increment value
*   for whatever button is selected.
*
*   When a rotary encoder is pressed down in the vertical direction, the increment
*   value cycles through a range of values specific to the selected button in the
*   column above.
*
* Rotary Encoder Callbacks
*
*   The initial implementation of the RotaryEncoder class included a single means of
*   establishing callbacks into Arduino C language space when the value or increment
*   settings of an encoder change. In this situation, it was then necessary for the C code
*   (owner of the ParamButtonScreen instance) to in turn make calls into ParamButtonScreen
*   methods in order to complete the update process for the display objects.
*
*   This scheme has now been augmented by the addition of a second mechanism whereby the
*   encoder can call directly into C++ space of ParamButtonScreen without the intervening
*   step of first calling into C space. In this case, ParamButtonScreen uses the derived
*   class RotaryEncoderPB instead of the RotaryEncoder base class.
*
*   Direct callback into the ParamButtonScreen instances is enabled in ParamButtonScreen's
*   activate() function by setting a reference pointer ("pbScreen") in the RotaryEncoderPB
*   class so that the screen update functions in ParamButtonScreen can be called directly via the
*   overriding reportValueChange() and reportIncrChange() methods in RotaryEncoderPB.
*
* Modifications List
*
*   •
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

#define SHOW_MARGINS    0

/************************************************************************************
*                       E X T E R N A L   R E F E R E N C E S                       *
*************************************************************************************
*/

#include "ParamButtonScreen.h"
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

ScreenButtonInfo ParamButtonScreen::saveChangesBtnInfo = (ScreenButtonInfo) {
     296,                           // btnX
     30,                            // btnY
     36,                            // btnWid
     36,                            // btnHgt
     COLOR_BLACK,                   // btnColor
     SCREENBUTTON_OUTLINE_COLOR,    // btnOutlineColor
     0,                             // btnOutlineRadius
     "Save Changes",                // btnLabel (set at runtime)
     SCREENBUTTON_TEXT_COLOR,       // btnLabelColor
     SCREENBUTTON_TEXT_FONT,        // btnLabelFont (note: must set again in constructor B241001-1)
     TextAlignLeft,                 // btnLabelAlign
     TextRight,                     // btnLabelPosition
     SaveIcon_35x35,                // bitMapInfo
};

ScreenButtonInfo ParamButtonScreen::copySettingsBtnInfo = (ScreenButtonInfo) {
     470,                           // btnX
     30,                            // btnY
     36,                            // btnWid
     36,                            // btnHgt
     COLOR_BLACK,                   // btnColor
     SCREENBUTTON_OUTLINE_COLOR,    // btnOutlineColor
     0,                             // btnOutlineRadius
     "Copy Settings",               // btnLabel (set at runtime)
     SCREENBUTTON_TEXT_COLOR,       // btnLabelColor
     SCREENBUTTON_TEXT_FONT,        // btnLabelFont (note: must set again in constructor B241001-1)
     TextAlignLeft,                 // btnLabelAlign
     TextRight,                     // btnLabelPosition
     ImportSettingsIcon_35x35,      // bitMapInfo
};

// !!NOTA BENE!! The order of the following definitions must synchronize with that of the
// entries in the enumeration PresetDeviceId in AudioPlatform.h. However, the names below,
// which (as of 3/15/25) cannot be defined in AudioPlatform.h, because library modules
// do not have access to terms defined in application-specific components.
//
PROGMEM char ParamButtonScreen::presetDeviceNames[PresetDeviceCount][PresetNameSize] = {

    // Device Name      Device ID (AudioPlatform.h)
    // -----------      ---------
    "PresetCommon",     // devicePresetCommon  = 0
    "Wav Player",       // deviceWavPlayer     = 1
    "Basic Synth",      // deviceBasicSynth    = 2
    "Midi",             // deviceMidi          = 3
    "Reverb / Efx",     // deviceEfx           = 4
    "Audio IO",         // deviceAudioIo       = 5
    "Raw Player",       // deviceRawPlayer     = 6
    "FM Synth",         // deviceFmSynth       = 7
    "Stereo Delay"      // deviceStereoDelay   = 8
};

/*****************
* Duplicate Code *
******************
*
* TODO: Following duplicates mechanism used in PresetsScreen.
* Need to find a way to refactor and combine the functionality.
*/
EXTMEM static char  presetBtnLabels[PresetDeviceCount][PresetNameSize];
EXTMEM static char* presetPanelBtnLabels[PresetDeviceCount];

/************************************************************************************
*                    P U B L I C   C L A S S   F U N C T I O N S                    *
*************************************************************************************
*/

// Static Member Initializations
//

// Constructor/Destructor
//
ParamButtonScreen::ParamButtonScreen(DisplayScreenInfo& displayInfo, ButtonPanelInfo& panelInfo, ParamButtonInfo *paramBtnParams,
        RotaryParams *paramBtnRotaryParams, RotaryEncoderPB **encoders, uint16_t presetDeviceId,
        const char* (*_presetsCallbackFn)(CommandButtonId cmdId, uint16_t& numberParam,  void* textParam)) :
        DisplayScreenBase(displayInfo),
        presetsCallbackFn(_presetsCallbackFn),
        btnPanel(panelInfo),
        saveChangesBtn(saveChangesBtnInfo), copySettingsBtn(copySettingsBtnInfo),
        btnParams(paramBtnParams), btnRotaryParams(paramBtnRotaryParams), rotaries(encoders),
        statusDisplay(),
        selectPresetToCopy(), presetDevice(presetDeviceId) {

    numColumns = btnPanel.btnColumnCount;

    // Workaround for B241001-1
    saveChangesBtnInfo.btnLabelFont = Arial_9_Bold;
    copySettingsBtnInfo.btnLabelFont = Arial_9_Bold;
}

ParamButtonScreen::~ParamButtonScreen() {

}

// activateScreen
//
void ParamButtonScreen::activateScreen() {

//    Serial.printf("ParamButtonScreen::activateScreen Begin: Device %d\n", presetDevice);

    // Clear and display screen
    clearScreen();
#if SHOW_MARGINS // Temporary fill in of margins for reference
    DisplayScreenBase::fillRect(0, 0, DISPLAY_WIDTH, marginTop, COLOR_ORANGERED);
    DisplayScreenBase::fillRect(0, DISPLAY_HEIGHT - marginBottom, DISPLAY_WIDTH, marginBottom, COLOR_ORANGERED);
    DisplayScreenBase::fillRect(0, 0, marginLeft, DISPLAY_HEIGHT, COLOR_ORANGERED);
    DisplayScreenBase::fillRect(DISPLAY_WIDTH - marginRight, 0, marginRight, DISPLAY_HEIGHT, COLOR_ORANGERED);
#endif // SHOW_MARGINS
    showScreenHeading();

    // Place panel below top margin, and centered between left and right margins
    int16_t displayAreaX1 = marginLeft;
    int16_t displayAreaX2 = DISPLAY_WIDTH - marginRight;
    int16_t displayAreaWidth = displayAreaX2 - displayAreaX1;
    int16_t panelX = displayAreaX1 + ((displayAreaWidth - btnPanel.panelWidth) / 2);
    btnPanel.setPanelPosition(panelX, marginTop);
    btnPanel.clearPanel();

#if SHOW_MARGINS // Temporary fill in of margins for reference
    DisplayScreenBase::fillRect(0, marginTop, panelX, DISPLAY_HEIGHT - (marginTop + marginBottom), COLOR_DARKKHAKI);
    DisplayScreenBase::fillRect(DISPLAY_WIDTH - panelX, marginTop, panelX, DISPLAY_HEIGHT - (marginTop + marginBottom), COLOR_DARKKHAKI);
#endif // SHOW_MARGINS

    // Draw buttons, labels, and values
    for (ButtonId btn = 0; btn < btnPanel.btnCount; btn++) {
        btnPanel.drawButton(btn, btnIsActive(btn));
        showBtnName(btn);
        showBtnValue(btn);
    }

    // Initialize rotary increment button geometric info
    initRotaryIncrDisplay();

    // Setup path in encoders to allow direct callbacks into our code space.
    for (uint8_t col = 0; col < numColumns && col < ROTARY_COUNT; col++) {
        rotaries[col]->pbScreen = this;
    }

    // Setup rotary encoders values for active button in each column
    for (uint8_t col = 0; col < numColumns && col < ROTARY_COUNT; col++) {
        ButtonId paramBtn = activeParamBtnInCol[col];
        rotaries[col]->setParams(btnRotaryParams[paramBtn]);
        btnRotaryParams[paramBtn].incrIndex = rotaries[col]->getIncrementValuesIndex();
        rotaryIncrChange(col, btnRotaryParams[paramBtn].incrs[btnRotaryParams[paramBtn].incrIndex]);
    }

    // Recall and Redisplay button parameter values
    restoreBtnParamValues();

    // Display Preset Save and Settings Import buttons
    saveChangesBtn.drawButton();
    copySettingsBtn.drawButton();
    bool presetDeviceModified = false;
    (*presetsCallbackFn)(CmdGetPresetDeviceModified, presetDevice, (void *)&presetDeviceModified);
    if (presetDeviceModified) {
        saveChangesBtn.setState(ScrnBtnOn);
//        saveChangesBtn.drawLabel(COLOR_RED);
    } else {
        saveChangesBtn.setState(ScrnBtnOff);
//        saveChangesBtn.drawLabel(saveChangesBtn.btnLabelColor);
    }

    // Display status bar (preset, player states) at bottom of screen
    statusDisplay.initialize();
}

// deActivateScreen
//
void ParamButtonScreen::deActivateScreen() {

    // Save values from active buttons that may not have been updated
    for (uint8_t col = 0; col < numColumns  && col < ROTARY_COUNT; col++) {
        ButtonId paramBtn = activeParamBtnInCol[col];
        btnRotaryParams[paramBtn].value = rotaries[col]->getVal();
        btnRotaryParams[paramBtn].incrIndex = rotaries[col]->getIncrementValuesIndex();
    }

    // Remove direct callbacks bridge into our code space.
    for (uint8_t col = 0; col < numColumns && col < ROTARY_COUNT; col++) {
        rotaries[col]->pbScreen = 0;
    }

    saveChangesBtn.removeButton();
    copySettingsBtn.removeButton();
}

// pollScreen
//
ButtonId ParamButtonScreen::pollScreen() {

    ButtonId touchedBtn = BtnNone;
    uint16_t pointX, pointY;

    if (selectPresetToCopy.pollEnabled) {
        pollSelectPreset();

    // Check for touched buttons
    } else if (DisplayScreenBase::touched()) {
        DisplayScreenBase::getTouchedPoint(pointX, pointY);

        // First check for Home/Back button. If pressed, we'll return that value to main application.
        if (DisplayScreenBase::prevBtnTouched(pointX, pointY)) {
            touchedBtn = BtnPrev;

        // Check for parameter increment display button
        } else if (incrButtonTouched(pointX, pointY, touchedBtn)) {
            rotaries[touchedBtn]->nextIncrementValue();
            touchedBtn = BtnNone;   // handled locally

        // Check for touch on command selection button
        } else if (btnPanel.isPanelButton(pointX, pointY, touchedBtn)) {
            handleParamBtnTouch(touchedBtn);
            touchedBtn = BtnNone;   // handled locally

        } else if (saveChangesBtn.buttonTouched(pointX, pointY)) {
            touchedBtn = BtnNone;
//            Serial.printf("ParamButtonScreen::pollScreen: Save Button Touched\n");

            uint16_t presetNumber;
            (*presetsCallbackFn)(CmdGetPresetNumber, presetNumber, 0);
            (*presetsCallbackFn)(CmdSavePreset, presetNumber, 0);
            bool presetModified = true;
            (*presetsCallbackFn)(CmdSetPresetModified, presetNumber, (void*)&presetModified);
            saveChangesBtn.setState(ScrnBtnOff);
//            saveChangesBtn.drawLabel(saveChangesBtn.btnLabelColor);

        } else if (copySettingsBtn.buttonTouched(pointX, pointY)) {
//            Serial.printf("ParamButtonScreen::pollScreen: Copy Button Touched\n");
            showSelectPresetPopup();
            touchedBtn = BtnNone;
        }
    }

    return touchedBtn;
}

// updateScreen
//
void ParamButtonScreen::updateScreen(StatusUpdatePacketType pktType, void* pkt) {

    if (pktType == StatusPacket && statusDisplay.displayInitialized) {
//        Serial.printf("ParamButtonScreen::updateScreen: Preset %02d: %s\n", fPkt->presetNumber, fPkt->presetName);
        statusDisplay.updateDisplay(pktType, pkt);
    }
}

// setBtnProperties
//
// Used chiefly to update rotary parameters when a new preset is loaded from the Presets screen.
// In this case, we can assume that the targeted parameter is NOT on display, and can therefore
// take no actions to update the screen button.
//
// TODO/FFS: Originally created to allow screen buttons to be updated in real time by external
// controls, for example to assign MIDI controls to effects parameters when a MIDI controller
// is moved. However, in this case, must be careful to update the display ONLY when the
// targeted screen is active. (T240910-1)
//
void ParamButtonScreen::setBtnProperties(uint16_t paramId, float paramValue, bool screenActive) {

//    Serial.printf("ParamButtonScreen::setBtnProperties paramId %d float paramValue %f screenActive %d\n", paramId, paramValue, screenActive);

    // Find button with paramId
    for (ButtonId btnNum = 0; btnNum < btnPanel.btnCount; btnNum++) {
        if (btnParams[btnNum].paramId == paramId) {
//            Serial.printf("ParamButtonScreen::setBtnProperties setting btn  %d properties\n", btnNum);

            // - If next statement is commented-out, dB values on display are correct to 0.5 dB when rotary changed, but screen does not
            //    update when preset is recalled, whether on display or not
            //  - If not commented-out, screen updates when preset is recalled whether on display or not, dB +/- 0.5dB values display correctly
            //    when encoder moved, but when encoder moved, displayed value jumps to last setting rather than starting with displayed value.
            //    This issue (B240701-1) is still to be solved as of 7/24/24.
            //
            btnRotaryParams[btnNum].value = paramValToBtnRotaryVal(btnNum, paramValue);

            // Set display value and update rotary state
            if (screenActive) {
                showBtnValue(btnNum);
            }
        }
    }

}

// rotaryValueChange
//
void ParamButtonScreen::rotaryValueChange(uint8_t rotaryId, int16_t rotaryVal) {

//    Serial.printf("ParamButtonScreen::rotaryValueChange: Rotary %d value %d\n", rotaryId, rotaryVal);

    ButtonId paramBtn;
    RotaryParams *rotaryParams;

    paramBtn = activeParamBtnInCol[rotaryId];
    rotaryParams = &btnRotaryParams[paramBtn];
    rotaryParams->value = rotaryVal;

// TODO: Fix encoder<>param conversions
//    call btnRotaryValToParamVal(paramBtn) before showBtnValue() and applyFn
    showBtnValue(paramBtn);
    if (rotaryParams->applyFn != NULL) {
//        Serial.printf("ParamButtonScreen::rotaryValueChange: Calling applyFn paramId %d paramValue %f\n", getBtnParamId(paramBtn), btnRotaryValToParamVal(paramBtn));
        rotaryParams->applyFn(getBtnParamId(paramBtn), btnRotaryValToParamVal(paramBtn));
    }
    bool presetDeviceModified = true;
    (*presetsCallbackFn)(CmdSetPresetDeviceModified, presetDevice, (void *)&presetDeviceModified);
    saveChangesBtn.setState(ScrnBtnOn);
//    saveChangesBtn.drawLabel(COLOR_RED);
}

// rotaryIncrChange
//
void ParamButtonScreen::rotaryIncrChange(uint8_t rotaryId, uint16_t incrVal) {

//    Serial.printf("ParamButtonScreen::rotaryIncrChange: Rotary %d increment %d\n", rotaryId, incrVal);

    ButtonId activeParamBtn = activeParamBtnInCol[rotaryId];

//    Serial.printf("ParamButtonScreen::rotaryIncrChange: activeParamBtn %d\n", activeParamBtn);

    const char* paramUnits = btnParams[activeParamBtn].paramUnits;

    // Special handling for specific unit types
    // TODO: How to eliminate special case for dB?
    if (strcmp(paramUnits, "dB") == 0) {
        if (incrVal == 1) {
            snprintf(stringDispBuf, sizeof(stringDispBuf)-1, "0.5");
        } else {
            snprintf(stringDispBuf, sizeof(stringDispBuf)-1, "%d", incrVal / 2);
        }
    } else {
        snprintf(stringDispBuf, sizeof(stringDispBuf)-1, "%d", incrVal);
    }

    showRotaryIncr(rotaryId, stringDispBuf);
}

/************************************************************************************
*                 P R O T E C T E D   C L A S S   F U N C T I O N S                 *
*************************************************************************************
*/

// handleParamBtnTouch()
//
// Process a touch on the param buttons panel
//
void ParamButtonScreen::handleParamBtnTouch(ButtonId touchedBtn) {

    uint8_t btnColumn = touchedBtn % numColumns;
    ButtonId activeParamBtn = activeParamBtnInCol[btnColumn];
//        Serial.printf("ParamButtonScreen::handleParamBtnTouch: btnColumn %d activeParamBtn %d\n", btnColumn, activeParamBtn);

    // A new btn has been touched in the column.
    if (touchedBtn != activeParamBtn) {

        // Make touched btn active in column
        activeParamBtnInCol[btnColumn] = touchedBtn;

        // Redisplay and retrieve attributes of currently active btn
        if (activeParamBtn != BtnNone) {

            btnPanel.drawButton(activeParamBtn, btnIsActive(activeParamBtn));
            showBtnName(activeParamBtn);
            showBtnValue(activeParamBtn);

            if (btnColumn < numColumns && btnColumn < ROTARY_COUNT) {
                btnRotaryParams[activeParamBtn].incrIndex = rotaries[btnColumn]->getIncrementValuesIndex();
//                    Serial.printf("ParamButtonScreen::handleParamBtnTouch: activeParamBtn %d incrIndex %d saved\n", activeParamBtn, btnRotaryParams[activeParamBtn].incrIndex);
            }
        }

        // Display touched btn as active
        btnPanel.drawButton(touchedBtn, btnIsActive(touchedBtn));
        showBtnName(touchedBtn);
        showBtnValue(touchedBtn);

        // Assign rotary attributes for newly active btn
        if (btnColumn < numColumns && btnColumn < ROTARY_COUNT) {
            rotaries[btnColumn]->setParams(btnRotaryParams[touchedBtn]);
            rotaryIncrChange(btnColumn, btnRotaryParams[touchedBtn].incrs[btnRotaryParams[touchedBtn].incrIndex]);
        }
    }
}

// restoreBtnParamValues()
//
// When the screen is displayed or re-displayed, restore the parameter
// values to the settings specified in the RotaryParams "value" field.
//
void ParamButtonScreen::restoreBtnParamValues() {

    // Establish default parameter settings
    for (int btn = 0; btn < btnPanel.btnCount; btn++) {
        RotaryParams *rotaryParams = &btnRotaryParams[btn];
        if (rotaryParams->applyFn != NULL) {
            showBtnValue(btn);
            rotaryParams->applyFn(getBtnParamId(btn), btnRotaryValToParamVal(btn));
        }
    }
}

// btnIsActive()
//
// Return true if parameter button is currently selected.
//
bool ParamButtonScreen::btnIsActive(ButtonId btnNum) {

    bool retVal = false;

    for (int i = 0; i < numColumns; i++) {
        if (activeParamBtnInCol[i] == btnNum) {
            retVal = true;
            break;
        }
    }

    return retVal;
}

// initRotaryIncrDisplay()
//
// Initialize rotary increment button geometric info
//
void ParamButtonScreen::initRotaryIncrDisplay() {

    uint16_t columnX, columnY;

    for (int i = 0; i < ROTARY_COUNT_MAX; i++) {
        btnPanel.getButtonXY(i, columnX, columnY);
        incrCircleData[i].xPos = columnX + btnPanel.btnWidth / 2;;
        incrCircleData[i].yPos = btnPanel.panelY + btnPanel.panelHeight + ROTARY_CIRCLE_DEF_GAP_ABOVE + ROTARY_CIRCLE_RADIUS;
        incrCircleData[i].radius = ROTARY_CIRCLE_RADIUS;
    }
}

// showRotaryIncr()
//
void ParamButtonScreen::showRotaryIncr(uint8_t column, char* valueStr) {

    uint16_t textWidth, textHeight;
    DisplayScreenBase::measureText(valueStr, textWidth, textHeight);

    uint16_t textX = incrCircleData[column].xPos - ((float)textWidth / 2.0);
    uint16_t textY = incrCircleData[column].yPos - ((float)textHeight / 2.0);

    DisplayScreenBase::setFont(btnPanel.btnFont);
    DisplayScreenBase::setTextColor(btnPanel.btnActiveTextColor);
    DisplayScreenBase::fillCircle(incrCircleData[column].xPos, incrCircleData[column].yPos, incrCircleData[column].radius, btnPanel.btnActiveColor);
    DisplayScreenBase::drawCircle(incrCircleData[column].xPos, incrCircleData[column].yPos, incrCircleData[column].radius, btnPanel.btnActiveTextColor);
    DisplayScreenBase::setCursor(textX, textY);
    DisplayScreenBase::print(valueStr);
}

// incrButtonTouched()
//
bool ParamButtonScreen::incrButtonTouched(uint16_t pointX, uint16_t pointY, ButtonId& touchedBtn) {

    bool touched = false;

    for (uint8_t col = 0; col < numColumns && col < ROTARY_COUNT; col++) {
        if (DisplayScreenBase::circleTouched(pointX, pointY, incrCircleData[col].xPos, incrCircleData[col].yPos, incrCircleData[col].radius+5)) {
            touchedBtn = col;
            touched = true;
            break;
        }
    }

    return touched;
}

// showBtnName
//
void ParamButtonScreen::showBtnName(ButtonId btnNum) {

    btnPanel.showButtonName(btnNum, btnParams[btnNum].paramName, btnIsActive(btnNum));
}

// showBtnValue
//
void ParamButtonScreen::showBtnValue(ButtonId btnNum) {

//    Serial.printf("ParamButtonScreen::showBtnValue btn %d\n", btnNum);

    switch (btnParams[btnNum].paramType) {

        case BtnParam_Type_Int:
//            Serial.printf("Button %d Type is INT\n", btnNum);
            snprintf(stringDispBuf, sizeof(stringDispBuf)-1, "%d %s", btnRotaryParams[btnNum].value, btnParams[btnNum].paramUnits);
            break;

        case BtnParam_Type_Float: {
//            Serial.printf("Button %d Type is FLOAT\n", btnNum);

            float paramVal = btnRotaryValToParamVal(btnNum);

//            Serial.printf("ParamButtonScreen::showBtnValue btnNum %d paramVal %f\n", btnNum, paramVal);

            // Special handling for specific unit types
            // TODO: How to eliminate special case for dB?
            if ((strcmp(btnParams[btnNum].paramUnits, "dB") == 0) && (paramVal > 0.01)) {
                snprintf(stringDispBuf, sizeof(stringDispBuf)-1, "+%.1f %s", paramVal, btnParams[btnNum].paramUnits);
            } else {
                snprintf(stringDispBuf, sizeof(stringDispBuf)-1, "%.1f %s", paramVal, btnParams[btnNum].paramUnits);
            }

            }
            break;

        case BtnParam_Type_Text:
//            Serial.printf("Button %d Type is TEXT\n", btnNum);
//            Serial.printf(" Labels: %s %s\n", paramButtonInfo->paramLabels[0], paramButtonInfo->paramLabels[1]);
//            Serial.printf(" indexMin: %d indexMax: %d\n", (int32_t)paramButtonInfo->paramMin, (int32_t)paramButtonInfo->paramMax);
            snprintf(stringDispBuf, sizeof(stringDispBuf)-1, "%s %s", btnParams[btnNum].paramLabels[btnRotaryParams[btnNum].value], btnParams[btnNum].paramUnits);
            break;

        default:
            snprintf(stringDispBuf, sizeof(stringDispBuf)-1, "ERROR");
            break;
    }

    btnPanel.showButtonValue(btnNum, stringDispBuf, btnIsActive(btnNum));
}

// btnRotaryValToParamVal
//
float ParamButtonScreen::btnRotaryValToParamVal(ButtonId btnNum) {

    float rotaryMin = (float) btnRotaryParams[btnNum].valueMin;
    float rotaryMax = (float) btnRotaryParams[btnNum].valueMax;
    float rotaryVal = (float) btnRotaryParams[btnNum].value;

    float paramMin = btnParams[btnNum].paramMin;
    float paramMax = btnParams[btnNum].paramMax;
    float paramVal = UtilFns::scale(rotaryVal, rotaryMin, rotaryMax, paramMin, paramMax);

    return paramVal;
}

// paramValToBtnRotaryVal
//
int16_t ParamButtonScreen::paramValToBtnRotaryVal(ButtonId btnNum, float paramValue) {

    float paramMin = btnParams[btnNum].paramMin;
    float paramMax = btnParams[btnNum].paramMax;

    float rotaryMin = (float) btnRotaryParams[btnNum].valueMin;
    float rotaryMax = (float) btnRotaryParams[btnNum].valueMax;
    float rotaryVal = UtilFns::scale(paramValue, paramMin, paramMax, rotaryMin, rotaryMax);
//    Serial.printf("ParamButtonScreen::paramValToBtnRotaryVal paramValue %f paramMin %f paramMax %f rotaryMin %f rotaryMax %f paramVal %f\n",
//            paramValue, paramMin, paramMax, rotaryMin, rotaryMax, rotaryVal);

    // Round to 2 digits. corrects +/- 0.5dB display errors. (B240716-1)
    return (int16_t)(round(rotaryVal * 100.0) / 100.0);
}

// getBtnParamId
//
uint16_t ParamButtonScreen::getBtnParamId(ButtonId btnNum) {

    return btnParams[btnNum].paramId;
}

// pollSelectPreset
//
ButtonId ParamButtonScreen::pollSelectPreset() {

    ButtonId touchedBtn = selectPresetToCopy.pollPopup();

    if (touchedBtn != BtnNone) {
        selectPresetToCopy.pollEnabled = false;
        if (touchedBtn == BtnSelect) {
//            Serial.printf("ParamButtonScreen::pollSelectPreset: Select Pressed. Preset %d\n", selectPresetToCopy.selectedPreset);
            (*presetsCallbackFn)(CmdCopyDeviceSettings, selectPresetToCopy.selectedPreset, (void *)&presetDevice);

            bool presetDeviceModified = true;
            (*presetsCallbackFn)(CmdSetPresetDeviceModified, presetDevice, (void *)&presetDeviceModified);
            saveChangesBtn.setState(ScrnBtnOn);
//            saveChangesBtn.drawLabel(COLOR_RED);
        }

        touchedBtn = BtnNone;
        this->activateScreen();
    }

    return touchedBtn;
}

// showSelectPresetPopup
//
void ParamButtonScreen::showSelectPresetPopup() {

    updatePresetNames();

//    Serial.printf("ParamButtonScreen::showSelectPresetPopup: presetDevice # %d\n", presetDevice);

    snprintf(stringDispBuf, sizeof(stringDispBuf)-1, "Copy %s Device Settings from Import Preset", presetDeviceNames[presetDevice]);
    selectPresetToCopy.setPopupHeading(stringDispBuf);

//    selectPresetToCopy.setPopupHeading("Select Preset to Copy Settings From");
    selectPresetToCopy.setBtnLabels((const char**)presetPanelBtnLabels);
    selectPresetToCopy.showPopup();
    selectPresetToCopy.pollEnabled = true;
}

/*****************
* Duplicate Code *
******************
*
* TODO: Following duplicates mechanism used in PresetsScreen.
* Need to find a way to refactor and combine the functionality.
*/

// updatePresetNames
//
// Get current preset names from application and draw presets panel
//
void ParamButtonScreen::updatePresetNames() {

//    Serial.printf("ParamButtonScreen::activateScreen: updatePresetNames\n");

    for (uint16_t presetNum = 0; presetNum < PresetCount; presetNum++) {
        const char *presetName = (*presetsCallbackFn)(CmdGetImportPresetName, presetNum, 0);
//        const char *presetName = (*presetsCallbackFn)(CmdGetPresetName, presetNum, 0);
        setPresetBtnLabel(presetNum, (char *)presetName, false);
    }
}
// setPresetBtnLabel
//
// malloc()-free version fixes memory leak bug B241217-1
//
void ParamButtonScreen::setPresetBtnLabel(ButtonId btnNum, char* btnLabel, bool reDraw) {

//    Serial.printf("ParamButtonScreen::setBtnLabel: %d, %s %d\n", btnNum, btnLabel, reDraw);
//    Serial.printf("ParamButtonScreen::setBtnLabel: len %d\n", strlen(btnLabel));

    // Clear button label
    memset(presetBtnLabels[btnNum], 0, PresetNameSize);

    // Check whether to abbreviate name
    if (strlen(btnLabel) <= MaxPresetBtnNameLen) {
        // Just copy label to labels array
        strncpy(presetBtnLabels[btnNum], btnLabel, PresetNameSize-1);
        presetBtnLabels[btnNum][PresetNameSize-1] = '\0';
        presetPanelBtnLabels[btnNum] = presetBtnLabels[btnNum];

    } else {
        // Abbreviate Name
        // Use first 8 letters + ellipsis as button name.
        strncpy(presetBtnLabels[btnNum], btnLabel, 8);
        strcat(presetBtnLabels[btnNum], "...");
        presetPanelBtnLabels[btnNum] = presetBtnLabels[btnNum];
    }
}


