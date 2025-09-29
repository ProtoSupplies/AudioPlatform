/************************************************************************************
*                              AudioEffectFreezeScan.h                              *
*                                                                                   *
*                       Copyright (c) 2025, Donald Swearingen                       *
*                                All Rights Reserved                                *
*************************************************************************************
*
* Description
*
*   AudioEffectFreezeScan is based on the original Teensy Audio Library AudioEffectGranular
*   class (see original copyright declaration below), modified, extended, and reformatted.
*
*   For additional details, see AudioEffectFreezeScan.cpp.
*
* Copyright Notices
*
*   AudioEffectFreezeScan
*   Copyright (c) 2025, Donald Swearingen
*
*   AudioEffectGranular
*   Copyright (c) 2018 John-Michael Reed, bleeplabs.com
*
*   Audio Library for Teensy
*   Copyright (c) 2014, Paul Stoffregen, paul@pjrc.com
*   Development of this audio library was funded by PJRC.COM, LLC by sales of
*   Teensy and Audio Adaptor boards.  Please support PJRC's efforts to develop
*   open source software by purchasing Teensy or other PJRC products.
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

#ifndef AUDIOEFFECTFREEZESCAN_H_
#define AUDIOEFFECTFREEZESCAN_H_

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

#include <Arduino.h>
#include <AudioStream.h>

/************************************************************************************
*                         M A C R O S  /  C O N S T A N T S                         *
*************************************************************************************
*/

/************************************************************************************
*                     D A T A / T Y P E   D E F I N I T I O N S                     *
*************************************************************************************
*/

typedef enum {

    PassThru,
    Freeze,
    FreezeScan,
    GlitchShift

} GrainMode;

/************************************************************************************
*                         C L A S S   D E F I N I T I O N S                         *
*************************************************************************************
*/

class AudioEffectFreezeScan : public AudioStream {

// Constructor/Destructor
//
public:

    AudioEffectFreezeScan(void);

// Class Variables
//
public:


private:

    audio_block_t*  inputQueueArray[1];
    int16_t*        sample_buf;
    uint32_t        sample_buf_len;     // samples

    GrainMode       grain_mode;

    uint32_t        write_index;        // sample array index
    uint32_t        read_index;         // sample array index
    uint32_t        read_index_incr;    // sample array index increment (pitch shift)
    uint32_t        read_index_accum;   // sample array index accumulator (pitch shift)
    uint32_t        prev_input;

    uint32_t        glitch_len_samps;   // samples
    uint32_t        freeze_len_samps;   // samples

    uint32_t        scan_begin_index;   // sample array index
    uint32_t        scan_end_index;     // sample array index
    uint32_t        scan_len_samps;     // samples

    bool            allow_len_change;
    bool            sample_loaded;
    bool            write_en;
    bool            sample_req;

protected:

// Class Methods
//
public:

    void begin(int16_t *sample_buf_def, uint32_t max_len_def);
    void setSpeed(float ratio);
    void setScanRange(uint32_t scanBeginMs, uint32_t scanLenMs);
    void beginFreeze(float grain_length);
    void beginFreezeScan();
    void beginPitchShift(float grain_length);
    void stop();
    virtual void update(void);

private:

    void beginFreeze_int(uint32_t grain_samples);
    void beginFreezeScan_int();
    void beginPitchShift_int(uint32_t grain_samples);

protected:


};

#endif /* AUDIOEFFECTFREEZESCAN_H_ */
