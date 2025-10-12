 /************************************************************************************
*                                ArduinoEclipseDefs.h                               *
*                       Copyright (c) 2025, Donald Swearingen                       *
*                                All Rights Reserved                                *
*************************************************************************************
*
* Description
*
*   When editing Arduino code in the Eclipse IDE, references to certain Arduino and
*   installed library classes are generally flagged as errors in the IDE because the
*   include files for those classes are not defined in the settings for the Eclipse
*   project. In most cases, this can be remedied by determining the location of the
*   include file, and adding it to the
*
*       Project Properties / C/C++ General / Paths and Symbols
*
*   list in the Eclipse settings. Even better, if the folder where the definition is
*   located is added to the project itself, by right-clicking the project, then selecting
*
*       Import / File System / From Directory
*
*   and browsing to the directory where the folder is located to select the folder.
*   The next action is to select the folder, then be sure to click the "Advanced"
*   checkbox on the lower left, then "Create links in Workspace." This ensures that
*   the imported folder will point to the actual library folder rather than making
*   a copy in the project's workspace. The great advantage of this approach is that
*   it makes all of the files in the imported directory, along with their contents,
*   visible in Eclipse, so that the contents can be opened and examined to better
*   understand how the imported objects work and are used. But care must be taken
*   when actually editing the contents that the changes do not sabotage the correct
*   operation of the imported resource.
*
*   At times, it is not entirely possible to locate or modify terms defined in Arduino
*   and/or installed libraries in order to eliminate error being flagged in Eclipse.
*   In these cases, it is possible to add the referenced term to the preprocessor
*   definitions for the project in
*
*       Project Settings / C/C++ Build / GCC C++ Compiler / Preprocessor / Defined Symbols
*
*   In this case, references in Eclipse will be defined and not flagged as errors, while
*   the Arduino compiler will not be affected because the Eclipse defines will not be
*   visible in the Arduino IDE.
*
*   In certain cases, the above may not be enough since it may also be necessary to
*   define "fake" or null versions of libraries and their internal functions. This is
*   true of the Arduino "Serial" class and is discussed in the next comments section.
*
* DUMMY_SERIAL Flag
*
*   When editing Arduino code in the Eclipse IDE, references to Arduino Serial
*   class functions (ex.: Serial.print()) are generally flagged as errors in the IDE
*   because the include files in the Arduino installation do not explicitly define
*   the Serial class. However, this is not the case when the code files are opened
*   and compiled in the Arduino IDE (which is the case here, where the Eclipse IDE
*   is used only for code editing, not for code compilation.
*
*   If the pre-processor flag DUMMY_SERIAL is defined in the Eclipse project's
*   C/C++/Build/Settings/Preprocessor settings, the Serial class references are defined
*   here as dummy functions so that the statements are not flagged as errors in the
*   Eclipse IDE. And since DUMMY_SERIAL is NOT defined in the Arduino IDE, the Serial
*   class functions will compile correctly as normal.
*
* TODO
*
*   • It also seems that the TeensyDuino usbMIDI class functions are also not found
*   among the Teensy include files in the installed TeensyDuino library, though, as
*   with the Serial class, they are known in the Arduino IDE when TeensyDuino is
*   installed. Dummy functions for usbMIDI could be added here to make the class
*   functions "visible" to the Eclipse IDE. In this case, this file could more suitably
*   be named something such as "TeensyMacros.h".
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

#ifndef ARDUINOECLIPSEDEFS_H_
#define ARDUINOECLIPSEDEFS_H_

/************************************************************************************
*                       E X T E R N A L   R E F E R E N C E S                       *
*************************************************************************************
*/

/************************************************************************************
*                         M A C R O S  /  C O N S T A N T S                         *
*************************************************************************************
*/

// Define DUMMY_SERIAL in Eclipse Preprocessor for project to
// disable error flags on serial IO function calls. Will
// not apply in Arduino IDE, so code will compile normally there.
//
#ifdef DUMMY_SERIAL

#include <stdint.h>
#include <Stream.h>

#define String
#define __stringVal

class serial {
public:
    bool    available() {return true;}
    void    begin(int baud) {}
    void    begin(int baud, int bits, int rx, int tx) {}
    bool    find(const char *target) {return false;}
    long    parseInt(LookaheadMode lookahead = SKIP_ALL, char ignore = '\x01') {}
    void    print(int val) {}
    void    print(int8_t val) {}
    void    print(int val, unsigned int fmt) {}
    void    print(char val, unsigned int fmt) {}
    void    print(const char* str) {}
    void    print(const char* fmt, uint8_t val) {}
    void    print(String __stringVal) {}
    void    println(const char* str) {}
    void    println(int val) {}
    void    println(int val, int fmt) {}
    void    println() {}
    void    printf(char *fmt, ...) {}
    char    read();
    size_t  readBytesUntil(char, char*, size_t);
    void    write(char c);
};

serial Serial;
serial Serial1;
serial Serial2;
#endif // DUMMY_SERIAL

#endif // ARDUINOECLIPSEDEFS_H_
