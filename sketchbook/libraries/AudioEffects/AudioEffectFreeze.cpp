/*
 * AudioEffectFreeze.cpp
 *
 *  Created on: May 12, 2024
 *      Author: Donald Swearingen
 *
 *
 *      Here's how this implementation works:

1. The `AudioEffectFreeze` class is a subclass of `AudioStream` and follows the requirements for subclasses of `AudioStream`.
2. The class is defined in a header file (`AudioEffectFreeze.h`) and a separate implementation file (`AudioEffectFreeze.cpp`).
3. The class constructor initializes the `AudioStream` base class with 1 channel and an input queue array called `inputQueueArray`.
4. The `update()` function is a virtual function that gets called repeatedly by the audio library when audio samples are received.
5. Inside the `update()` function, it receives an audio data block from the input queue using `receiveReadOnly()`.
6. If a valid audio block is received, it iterates over the samples in the block:
   - If `freezeActive` is `false`, it stores the input sample in the buffer, advances the `head` pointer, and outputs the input sample.
   - If `freezeActive` is `true`, it outputs a sample from the freeze range by reading from the buffer at the `freezeStart` position and
   advancing `freezeStart`. If `freezeStart` reaches the end of the freeze range, it wraps around to the `tail` position.
7. After processing the block, it transmits the processed audio data using `transmit(block)` and releases the block memory using `release(block)`.
8. The `setFreeze()`, `setFreezeStart()`, and `setFreezeLength()` functions are public member functions that allow setting the corresponding
variables (`freezeActive`, `freezeStart`, and `freezeLength`), with necessary bounds checking.

This implementation integrates the `AudioEffectFreeze` class with the Teensy 4.1 Audio Library, inherits from the `AudioStream`
class, and provides the required `update()` function to process incoming audio samples. The class can be used in the Arduino 2.0 IDE
and follows the coding style and conventions of the Teensy Audio Library.

Note that you'll need to include the `AudioEffectFreeze.h` header file in your Arduino sketch and create an instance of the `AudioEffectFreeze`
class to use it with the Teensy Audio Library. You can then set up the audio connections and use the provided public functions to control the freeze effect.

 */

#include "AudioEffectFreeze.h"

AudioEffectFreeze::AudioEffectFreeze(void) :
    AudioStream(1, inputQueueArray),
    head(0),
    tail(0),
    freezeStart(0),
    freezeLength(0),
    freezeActive(false) {
    // Zero out the buffer
    memset(buffer, 0, sizeof(buffer));
}

void AudioEffectFreeze::update(void) {
    // This function gets called repeatedly by the audio library

    // Read data from the input queue
    audio_block_t *block = receiveReadOnly();
    if (block) {
        int16_t *data = block->data;
        for (uint16_t i = 0; i < AUDIO_BLOCK_SAMPLES; i++) {
//        for (uint16_t i = 0; i < block->length; i++) {
            if (!freezeActive) {
                // Store the input sample in the buffer
                buffer[head] = data[i];
                head = (head + 1) % BUFFER_SIZE;

                // Advance the tail pointer if the buffer is full
                if (head == (tail + 1) % BUFFER_SIZE) {
                    tail = (tail + 1) % BUFFER_SIZE;
                }

                // Output the input sample
//                data[i] = data[i];
            } else {
                // Output a sample from the freeze range
                data[i] = buffer[freezeStart];
                freezeStart = (freezeStart + 1) % BUFFER_SIZE;

                // Wrap around the freeze range if necessary
                if (freezeStart == (freezeStart + freezeLength) % BUFFER_SIZE) {
                    freezeStart = tail;
                }
            }
        }

        // Transmit the processed audio data
        transmit(block);
        release(block);
    }
}

void AudioEffectFreeze::setFreeze(bool val) {
    freezeActive = val;
}

void AudioEffectFreeze::setFreezeStart(uint32_t pos) {
    if (pos < BUFFER_SIZE && pos <= head - 1000) {
        freezeStart = pos;
    }
}

void AudioEffectFreeze::setFreezeLength(uint32_t len) {
    uint32_t maxLen = (tail > freezeStart) ? tail - freezeStart : BUFFER_SIZE - freezeStart;
    freezeLength = min(len, maxLen);
}

