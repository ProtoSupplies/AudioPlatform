/************************************************************************************
*                             AudioEffectFreezeScan.cpp                             *
*                                                                                   *
*                       Copyright (c) 2025, Donald Swearingen                       *
*                                All Rights Reserved                                *
*************************************************************************************
*
* Description
*
*   AudioEffectFreezeScan is based on the original Teensy Audio Library AudioEffectGranular
*   class (see original copyright declaration below), modified, extended, and reformatted
*   to support
*
*       • Longer audio buffers of sizes greater than the original upper limit of 32767
*         bytes imposed by the definition of the buffer as:
*
*           int16_t sample_buf_len;
*
*       • When freeze mode is triggered and a variable freeze time length is set,
*         for example, to 2.5 seconds, the freeze buffer is defined as the LAST 2.5
*         seconds currently present in the buffer, rather than the NEXT 2.5 seconds to
*         be recorded.
*       • During playback of the freeze buffer, the position of the playback start
*         and end points can be dynamically varied, subject to the current freeze length.
*       • (Likely in later version, a "shuffling" mechanism where the playback segment
*         of freeze length is taken from random positions within the overall "frozen"
*         sample.
*
*   The implementation is targeted specifically to the Teensy 4.1 processor, which
*   provides 1MB of internal RAM, and 8MB of internal flash storage, running at 600MHz,
*   and, desirably, with external 8-16MB PSRAM accessible via its built-in QSPI support.
*
* Modifications in Support of Longer Buffers
*
*   As mentioned above, the original AudioEffectGranular class defined the sample buffer
*   argument in its begin() function as int16_t max_len_def, a signed value:
*
*       int16_t sample_buf_len;
*
*       void AudioEffectGranular::begin(int16_t *sample_buf_def, int16_t max_len_def) {
*           .
*           .
*           .
*           sample_buf_len = max_len_def;
*
*   can in fact be specified as having a negative value or value > 32767. For example
*   if max_len_def == 32768, the C++ compiler interprets this as -32768. And, when
*   -3276i is used as an index into an int16_t array, a memory protection violation
*   will occur (though this is not detected/reported when running the Teensy Audio
*   Library.
*
*   Therefore, all values defined and employed in this version of the effect are defined
*   as unsigned 32-bit quantities, not only to avoid these errors, but to provide for
*   the use of buffers of greater length, and thus capable of storing multi-second segments
*   of recorded 16-bit, 44.1kHz samples.
*
*   Finally, buffer space is allocated in applications using this module in extended memory,
*   when available, in order to conserve space in the internal 1M byte space (of which 512k
*   bytes are available for variable allocations) of the Teensy 4.1 memory space.
*
* Pitch Shift Support
*
*   Presumably to avoid using floating point operations in support of pitch shifting by
*   manipulation of the increment for reading the next sample during playback, the original
*   version of the effect employed an integer-arithmetic based approach whereby the initial
*   value of the increment was established first as a floating point value based on the
*   selected pitch shift ratio (.125 to 8.0 for a +/- 4 octave range), and then stored in the
*   upper 16-bits (via multiplication by 2^16 = 65536) of an unsigned 32-bit integer "playback_rate":
*
*       void setSpeed(float ratio) {
*           if (ratio < 0.125f) ratio = 0.125f;
*           else if (ratio > 8.0f) ratio = 8.0f;
*           read_index_incr = ratio * 65536.0f + 0.499f;
*       }
*
*   During playback, read_index_incr is added to the 32-bit unsigned "accumulator" for each sample,
*   and the index of the next sample to be read is formed by shifting the accumulator right by 16
*   bits, (equivalent to dividing the accumulator value by 65536). This effectively implements
*   a pitch shift by delaying the advancement of the read_index when the ratio is < 1.0 and
*   increasing the advance when the ratio is > 1.0.
*
*       if (sample_loaded) {
*           if (read_index_incr >= 0) {
*               accumulator += read_index_incr;
*               read_index = accumulator >> 16;
*           }
*           if (read_index >= freeze_len) {
*               accumulator = 0;
*               read_index = 0;
*           }
*           block->data[j] = sample_buf[read_index];
*       }
*
*   Several observations are in order here:
*
*       • The use of the "magic numbers" of 65536.0 used in setSpeed(), and the shift value >> 16,
*         based on the assumed 65536 impose an inflexible constraint on the size of the sample buffer.
*         A better design would define constant values for the maximum buffer size, with the proper
*         power of 2 and shift values determined based on the definition of the buffer size.
*
*         In this implementation, a shift value of 13-bits is employed (providing for 2^13 = 8192
*         individual pitch shift values in a +/- 2 octave range (.25 to 4.0) and 2^(32-13) = 2^19 = 524,288,
*         or app. 11.8 seconds of audio at 16-bits 44.1kHz.
*
*           const uint32_t  PitchShiftBitRes    = 13;
*
*         and all related values are set in reference to this value:
*
*           void setSpeed(float ratio) {
*               if (ratio < 0.25f) ratio = 0.25f;
*               else if (ratio > 4.0f) ratio = 4.0f;
*               read_index_incr = ratio * (pow(2.0, PitchShiftBitRes)) + 0.499f;
*           }
*           .
*           .
*           .
*           if (sample_loaded) {
*               if (read_index_incr >= 0) {
*                   accumulator += read_index_incr;
*                   read_index = accumulator >> PitchShiftBitRes;
*               }
*               if (read_index >= freeze_len) {
*                   accumulator = 0;
*                   read_index = 0;
*               }
*               block->data[j] = sample_buf[read_index];
*           }
*
*       • All the variables associated with the sample buffer, the buffer length, and
*         buffer pointers should be defined as 32-bit unsigned integers rather than
*         16-bit signed integers in order to 1) allocate buffers of length > 32767
*         and 2) avoid any possible code errors occasioned by the use of signed values
*         when accessing a buffer whose size is 32 but unsigned (and thus of length
*         far greater than 32767.
*
*       • The names of several variables should be changed to more accurately reflect
*         their function and usage.
*
*               read_index_incr   ---> playback_increment (NOTE: "playpack" in the original code
*                                    was obviously intended to be "playback").
*               accumulator     --->
*
* Buffer Length vs Freeze Length
*
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


/****************
* "Pre-Defines" *
*****************
*
* Definitions that affect subsequent external references
*
*/

#include <Math.h>
#include "AudioEffectFreezeScan.h"
#include "ArduinoEclipseDefs.h"

/************************************************************************************
*                       E X T E R N A L   R E F E R E N C E S                       *
*************************************************************************************
*/

/************************************************************************************
*                         M A C R O S  /  C O N S T A N T S                         *
*************************************************************************************
*/

const uint32_t  PitchShiftBitRes    = 13;

/************************************************************************************
*                    P U B L I C   C L A S S   F U N C T I O N S                    *
*************************************************************************************
*/

// Constructor
//
AudioEffectFreezeScan::AudioEffectFreezeScan() : AudioStream(1, inputQueueArray) {

}

// begin
//
void AudioEffectFreezeScan::begin(int16_t *sample_buf_def, uint32_t max_len_def) {

    grain_mode = PassThru;

    sample_buf = sample_buf_def;
    sample_buf_len = max_len_def;

    read_index = 0;
    write_index = 0;
    prev_input = 0;

    read_index_accum = 0;
    allow_len_change = true;
    sample_loaded = false;

    setScanRange(500, 3000);
    setSpeed(1.0);
}

// setSpeed
//
void AudioEffectFreezeScan::setSpeed(float ratio) {

    if (grain_mode == FreezeScan) {
        ratio = 1.0;
    } else {
        if (ratio < 0.125f) {
            ratio = 0.125f;
        } else if (ratio > 8.0f) {
            ratio = 8.0f;

        }
    }

    read_index_incr = ratio * (pow((double)2.0, (double)PitchShiftBitRes)) + 0.499f;
    Serial.printf("ratio =  %f read_index_incr = %d\n", ratio, read_index_incr);
}

// setScanRange
//
// Set scan begin and length
//
void AudioEffectFreezeScan::setScanRange(uint32_t scanBeginMs, uint32_t scanLenMs) {

    Serial.printf("setScanRange: scanBeginMs = %u scanLenMs = %u\n", scanBeginMs, scanLenMs);

    scan_begin_index = scanBeginMs * (AUDIO_SAMPLE_RATE_EXACT * 0.001f);
    if (scan_begin_index >= sample_buf_len) {
        scan_begin_index = 0;
    }

    scan_len_samps = scanLenMs * (AUDIO_SAMPLE_RATE_EXACT * 0.001f);
    if (scan_len_samps > sample_buf_len) {
        scan_len_samps = scan_len_samps - sample_buf_len;
    }

    scan_end_index = scan_begin_index + scan_len_samps;
    if (scan_end_index > sample_buf_len) {
        scan_end_index = sample_buf_len;
        scan_len_samps = scan_end_index - scan_begin_index;
    }

    Serial.printf("setScanRange: scan_begin_index = %u scan_end_index = %u scan_len_samps = %u\n", scan_begin_index, scan_end_index, scan_len_samps);
}

// beginFreeze
//
void AudioEffectFreezeScan::beginFreeze(float grain_length) {

    if (grain_length <= 0.0f) return;
    beginFreeze_int(grain_length * (AUDIO_SAMPLE_RATE_EXACT * 0.001f) + 0.5f);
}

// beginFreezeScan
//
void AudioEffectFreezeScan::beginFreezeScan() {

    beginFreezeScan_int();
}

// beginPitchShift
//
void AudioEffectFreezeScan::beginPitchShift(float grain_length) {

    if (grain_length > 0.0f) {
        beginPitchShift_int(grain_length * (AUDIO_SAMPLE_RATE_EXACT * 0.001f) + 0.5f);
    }
}

// stop
//
void AudioEffectFreezeScan::stop() {

    grain_mode = PassThru;
    allow_len_change = true;
}

// update
//
void AudioEffectFreezeScan::update(void) {

    audio_block_t *block;

    if (sample_buf == NULL) {
        block = receiveReadOnly(0);
        if (block) release(block);
        return;
    }

    block = receiveWritable(0);
    if (!block) return;

    if (grain_mode == PassThru) {
        // passthrough, no granular effect
        prev_input = block->data[AUDIO_BLOCK_SAMPLES-1];

        // recording continuously
        for (uint16_t j = 0; j < AUDIO_BLOCK_SAMPLES; j++) {
            sample_buf[write_index++] = block->data[j];
            if (write_index >= sample_buf_len) {
                write_index = 0;;
            }
        }
    }
    else if (grain_mode == FreezeScan) {
        // Freeze - sample 1 grain, then repeatedly play it back
        for (uint16_t j = 0; j < AUDIO_BLOCK_SAMPLES; j++) {
            if (read_index_incr >= 0) {
                read_index_accum += read_index_incr;
                read_index = read_index_accum >> PitchShiftBitRes;
            }
            if (read_index >= scan_end_index) {
                read_index_accum = scan_begin_index << PitchShiftBitRes;
            }
            block->data[j] = sample_buf[read_index];
        }
    }
    else if (grain_mode == Freeze) {
        // Freeze - sample 1 grain, then repeatedly play it back
        for (uint16_t j = 0; j < AUDIO_BLOCK_SAMPLES; j++) {
            if (sample_req) {
                // only begin capture on zero cross
                int16_t current_input = block->data[j];
                if ((current_input < 0 && prev_input >= 0) ||
                  (current_input >= 0 && prev_input < 0)) {
                    write_en = true;
                    write_index = 0;
                    read_index = 0;
                    sample_req = false;
                } else {
                    prev_input = current_input;
                }
            }
            if (write_en) {
                sample_buf[write_index++] = block->data[j];
                if (write_index >= freeze_len_samps) {
                    sample_loaded = true;
                    write_en = false;
                }
                if (write_index >= sample_buf_len) {
                    write_en = false;
                }
            }
            if (sample_loaded) {
                if (read_index_incr >= 0) {
                    read_index_accum += read_index_incr;
                    read_index = read_index_accum >> PitchShiftBitRes;
                }
                if (read_index >= freeze_len_samps) {
                    read_index_accum = 0;
                    read_index = 0;
                }
                block->data[j] = sample_buf[read_index];
            }
        }
    }
    else if (grain_mode == GlitchShift) {
        //GLITCH SHIFT
        //basic granular synth thingy
        // the shorter the sample the sample_buf_len the more tonal it is.
        // Longer it has more definition.  It's a bit roboty either way which
        // is obv great and good enough for noise music.

        for (int k = 0; k < AUDIO_BLOCK_SAMPLES; k++) {
            // only start recording when the audio is crossing zero to minimize pops
            if (sample_req) {
                int16_t current_input = block->data[k];
                if ((current_input < 0 && prev_input >= 0) ||
                  (current_input >= 0 && prev_input < 0)) {
                    write_en = true;
                } else {
                    prev_input = current_input;
                }
            }

            if (write_en) {
                sample_req = false;
                allow_len_change = true; // Reduces noise by not allowing the
                        // length to change after the sample has been
                        // recored.  Kind of not too much though
                if (write_index >= glitch_len_samps) {
                    write_index = 0;
                    sample_loaded = true;
                    write_en = false;
                    allow_len_change = false;
                }
                sample_buf[write_index] = block->data[k];
                write_index++;
            }

            if (sample_loaded) {
                //move it to the middle third of the bank.
                //3 "separate" banks are used
                float fade_len = 20.00;
                uint32_t m2 = fade_len;

                for (uint32_t m = 0; m < 2; m++) {
                    // I'm off by one somewhere? why is there a tick at the
                    // beginning of this only when it's combined with the
                    // fade out???? ooor am i osbserving that incorrectly
                    // either wait it works enough
                    sample_buf[m + glitch_len_samps] = 0;
                }

                for (uint32_t m = 2; m < glitch_len_samps-m2; m++) {
                    sample_buf[m + glitch_len_samps] = sample_buf[m];
                }

                for (uint32_t m = glitch_len_samps-m2; m < glitch_len_samps; m++) {
                    // fade out the end. You can just make fadet=0
                    // but it's a little too daleky
                    float fadet = sample_buf[m] * (m2 / fade_len);
                    sample_buf[m + glitch_len_samps] = (int16_t)fadet;
                    m2--;
                }
                sample_loaded = false;
                prev_input = block->data[k];
                sample_req = true;
            }

            read_index_accum += read_index_incr;
            read_index = (read_index_accum >> PitchShiftBitRes);

            if (read_index >= glitch_len_samps) {
                read_index -= glitch_len_samps;
                read_index_accum = 0;

                for (uint32_t m = 0; m < glitch_len_samps; m++) {
                    sample_buf[m + (glitch_len_samps*2)] = sample_buf[m+glitch_len_samps];
                    //  sample_buf[m + (glitch_len_samps*2)] = (m%20)*1000;
                }
            }
            block->data[k] = sample_buf[read_index + (glitch_len_samps*2)];
        }
    }
    transmit(block);
    release(block);
}

/************************************************************************************
*                   P R I V A T E   C L A S S   F U N C T I O N S                   *
*************************************************************************************
*/

// beginFreeze_int
//
void AudioEffectFreezeScan::beginFreeze_int(uint32_t grain_samples) {

    __disable_irq();
    grain_mode = Freeze;
    if (grain_samples < sample_buf_len) {
        freeze_len_samps = grain_samples;
    } else {
        freeze_len_samps = sample_buf_len;
    }
    sample_loaded = false;
    write_en = false;
    sample_req = true;
    __enable_irq();
}

// beginFreezeScan_int
//
void AudioEffectFreezeScan::beginFreezeScan_int() {

    __disable_irq();
    grain_mode = FreezeScan;
    read_index_accum = 0;
    read_index_accum = scan_begin_index << PitchShiftBitRes;
    __enable_irq();
}

// beginPitchShift_int
//
void AudioEffectFreezeScan::beginPitchShift_int(uint32_t grain_samples) {

    __disable_irq();
    grain_mode = GlitchShift;
    if (allow_len_change) {
        if (grain_samples < 100) grain_samples = 100;
        uint32_t maximum = (sample_buf_len - 1) / 3;
        if (grain_samples > maximum) grain_samples = maximum;
        glitch_len_samps = grain_samples;
    }
    sample_loaded = false;
    write_en = false;
    sample_req = true;
    __enable_irq();
}

