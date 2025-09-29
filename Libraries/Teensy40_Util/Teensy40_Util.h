/************************************************************************************
*                                 Teensy40_Util.h                                   *
*                      Copyright (c) 2025, Donald Swearingen                        *
*                               All Rights Reserved                                 *
*************************************************************************************
*
* Description
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

#ifndef TEENSY4UTIL_H_
#define TEENSY4UTIL_H_

/************************************************************************************
*                       E X T E R N A L   R E F E R E N C E S                       *
*************************************************************************************
*/

#include <Arduino.h>

/************************************************************************************
*                         M A C R O S  /  C O N S T A N T S                         *
*************************************************************************************
*/

#define TEENSYUTIL          Teensy40_Util

#define TEENSYVERSION       "4.0"

#define NO_OUTPUT           -1

#define RX1_PIN             0
#define TX1_PIN             1
#define RX2_PIN             7
#define TX2_PIN             8
#define RX3_PIN             15
#define TX3_PIN             14
#define RX4_PIN             16
#define TX4_PIN             17

#define SCL0_PIN            19
#define SDA0_PIN            18
#define SCL1_PIN            16
#define SDA1_PIN            17

#define SPDIF_IN_PIN        15
#define SPDIF_OUT_PIN       14

#define SPI_CS_PIN          10
#define SPI_MOSI_PIN        11
#define SPI_MISO_PIN        12
#define SPI_SCK_PIN         13

#define ARDUINO_LED_PIN     13

#define DIGITAL_IO_0_PIN    0
#define DIGITAL_IO_1_PIN    1
#define DIGITAL_IO_2_PIN    2
#define DIGITAL_IO_3_PIN    3
#define DIGITAL_IO_4_PIN    4
#define DIGITAL_IO_5_PIN    5
#define DIGITAL_IO_6_PIN    6
#define DIGITAL_IO_7_PIN    7
#define DIGITAL_IO_8_PIN    8
#define DIGITAL_IO_9_PIN    9
#define DIGITAL_IO_10_PIN   10
#define DIGITAL_IO_11_PIN   11
#define DIGITAL_IO_12_PIN   12
#define DIGITAL_IO_13_PIN   13
#define DIGITAL_IO_14_PIN   14
#define DIGITAL_IO_15_PIN   15
#define DIGITAL_IO_16_PIN   16
#define DIGITAL_IO_17_PIN   17
#define DIGITAL_IO_18_PIN   18
#define DIGITAL_IO_19_PIN   19
#define DIGITAL_IO_20_PIN   20
#define DIGITAL_IO_21_PIN   21
#define DIGITAL_IO_22_PIN   22
#define DIGITAL_IO_23_PIN   23

#define ANALOG_IN_0_PIN     14
#define ANALOG_IN_1_PIN     15
#define ANALOG_IN_2_PIN     16
#define ANALOG_IN_3_PIN     17
#define ANALOG_IN_4_PIN     18
#define ANALOG_IN_5_PIN     19
#define ANALOG_IN_6_PIN     20
#define ANALOG_IN_7_PIN     21
#define ANALOG_IN_8_PIN     22
#define ANALOG_IN_9_PIN     23

#define ANALOG_IN_MIN_VAL   0
#define ANALOG_IN_MAX_VAL   1023


/************************************************************************************
*                     D A T A / T Y P E   D E F I N I T I O N S                     *
*************************************************************************************
*/

/************************************************************************************
*                         C L A S S   D E F I N I T I O N S                         *
*************************************************************************************
*/

class Teensy40_Util {

// Constructor/Destructor
//
public:
    Teensy40_Util(void);
    virtual ~Teensy40_Util();

// Class Variables
//
public:

private:

protected:

// Class Methods
//
public:

    static void getTeensyMac(unsigned char *buf);

private:

protected:

};

#endif // TEENSY4UTIL_H_
