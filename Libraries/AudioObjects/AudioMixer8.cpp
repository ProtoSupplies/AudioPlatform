/************************************************************************************
*                                  AudioMixer8.cpp                                  *
*                                                                                   *
*                       Copyright (c) 2025, Donald Swearingen                       *
*                                All Rights Reserved                                *
*************************************************************************************
*
* Description
*
*   AudioMixer8 is based on the original Teensy Audio Library AudioMixer4 class
*   (see PJRC copyright declaration below), modified, extended, and reformatted to
*   support an additional 4 input channels, for a total of 8 inputs.
*
* Copyright Notices
*
*   AudioMixer8
*   Copyright (c) 2025, Donald Swearingen
*
*   Audio Library for Teensy 3.X
*   Copyright (c) 2014, Paul Stoffregen, paul@pjrc.com
*
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

/************************************************************************************
*                       E X T E R N A L   R E F E R E N C E S                       *
*************************************************************************************
*/

#include "AudioMixer8.h"
#include <Arduino.h>
#include "utility/dspinst.h"

/************************************************************************************
*                         M A C R O S  /  C O N S T A N T S                         *
*************************************************************************************
*/

#if defined(__ARM_ARCH_7EM__)

#define MULTI_UNITYGAIN     65536.0f
#define MIN_GAIN            -32767.0f
#define MAX_GAIN            32767.0f

#elif defined(KINETISL)

#define MIN_GAIN            -127.0f
#define MAX_GAIN            127.0f
#define MULTI_UNITYGAIN     256.0f

#endif // defined(__ARM_ARCH_7EM__) / defined(KINETISL)


/************************************************************************************
*                    P U B L I C   C L A S S   F U N C T I O N S                    *
*************************************************************************************
*/

// Constructor
//
AudioMixer8::AudioMixer8(void) : AudioStream(8, inputQueueArray) {

    for (int i = 0; i < 8; i++) {
        multiplier[i] = (int32_t)MULTI_UNITYGAIN;
    }
}

// gain
//
void AudioMixer8::gain(unsigned int channel, float gain) {

    if (channel >= 8) return;

    if (gain > MAX_GAIN) {
        gain = MAX_GAIN;
    } else if (gain < MIN_GAIN) {
        gain = MIN_GAIN;
    }

    multiplier[channel] = gain * MULTI_UNITYGAIN; // TODO: proper roundoff?
}

// update
//
void AudioMixer8::update(void) {

    audio_block_t *in, *out=NULL;
    unsigned int channel;

    for (channel=0; channel < 8; channel++) {
        if (!out) {
            out = receiveWritable(channel);
            if (out) {
                int32_t mult = multiplier[channel];
                if (mult != MULTI_UNITYGAIN) applyGain(out->data, mult);
            }
        } else {
            in = receiveReadOnly(channel);
            if (in) {
                applyGainThenAdd(out->data, in->data, multiplier[channel]);
                release(in);
            }
        }
    }
    if (out) {
        transmit(out);
        release(out);
    }
}

/************************************************************************************
*                   P R I V A T E   C L A S S   F U N C T I O N S                   *
*************************************************************************************
*/

// applyGain
//
void AudioMixer8::applyGain(int16_t *data, int32_t mult) {

#if defined(__ARM_ARCH_7EM__)

    uint32_t *p = (uint32_t *)data;
    const uint32_t *end = (uint32_t *)(data + AUDIO_BLOCK_SAMPLES);

    do {
        uint32_t tmp32 = *p; // read 2 samples from *data
        int32_t val1 = signed_multiply_32x16b(mult, tmp32);
        int32_t val2 = signed_multiply_32x16t(mult, tmp32);
        val1 = signed_saturate_rshift(val1, 16, 0);
        val2 = signed_saturate_rshift(val2, 16, 0);
        *p++ = pack_16b_16b(val2, val1);
    } while (p < end);

#elif defined(KINETISL)

    const int16_t *end = data + AUDIO_BLOCK_SAMPLES;

    do {
        int32_t val = *data * mult;
        *data++ = signed_saturate_rshift(val, 16, 0);
    } while (data < end);

#endif // defined(__ARM_ARCH_7EM__) / defined(KINETISL)
}

// applyGainThenAdd
//
void AudioMixer8::applyGainThenAdd(int16_t *data, const int16_t *in, int32_t mult) {

#if defined(__ARM_ARCH_7EM__)

    uint32_t *dst = (uint32_t *)data;
    const uint32_t *src = (uint32_t *)in;
    const uint32_t *end = (uint32_t *)(data + AUDIO_BLOCK_SAMPLES);

    if (mult == (int32_t)MULTI_UNITYGAIN) {
        do {
            uint32_t tmp32 = *dst;
            *dst++ = signed_add_16_and_16(tmp32, *src++);
            tmp32 = *dst;
            *dst++ = signed_add_16_and_16(tmp32, *src++);
        } while (dst < end);
    } else {
        do {
            uint32_t tmp32 = *src++; // read 2 samples from *data
            int32_t val1 = signed_multiply_32x16b(mult, tmp32);
            int32_t val2 = signed_multiply_32x16t(mult, tmp32);
            val1 = signed_saturate_rshift(val1, 16, 0);
            val2 = signed_saturate_rshift(val2, 16, 0);
            tmp32 = pack_16b_16b(val2, val1);
            uint32_t tmp32b = *dst;
            *dst++ = signed_add_16_and_16(tmp32, tmp32b);
        } while (dst < end);
    }

#elif defined(KINETISL)

    const int16_t *end = data + AUDIO_BLOCK_SAMPLES;

    if (mult == (int32_t)MULTI_UNITYGAIN) {
        do {
            int32_t val = *data + *in++;
            *data++ = signed_saturate_rshift(val, 16, 0);
        } while (data < end);
    } else {
        do {
            int32_t val = *data + ((*in++ * mult) >> 8); // overflow possible??
            *data++ = signed_saturate_rshift(val, 16, 0);
        } while (data < end);
    }

#endif // defined(__ARM_ARCH_7EM__) / defined(KINETISL)
}
