#ifndef AUDIO_EFFECT_FREEZE_H
#define AUDIO_EFFECT_FREEZE_H

#include <Audio.h>

// Define the buffer size (3 seconds of audio at 44.1kHz, 16-bit samples)
constexpr uint32_t BUFFER_SIZE = 44100 * 3;

class AudioEffectFreeze : public AudioStream {
public:
    AudioEffectFreeze(void);

    virtual void update(void);

    void setFreeze(bool val);
    void setFreezeStart(uint32_t pos);
    void setFreezeLength(uint32_t len);

private:
    int16_t buffer[BUFFER_SIZE];
    volatile uint32_t head;
    volatile uint32_t tail;
    volatile uint32_t freezeStart;
    volatile uint32_t freezeLength;
    volatile bool freezeActive;

    audio_block_t *inputQueueArray[1];
};

#endif
