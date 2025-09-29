#include "EncoderTool.h"
using namespace EncoderTool;

Encoder encoder;          // interrupt based encoder

void setup()
{
    encoder.begin(26, 25);
}

void loop()
{
    if (encoder.valueChanged()) // do we have a new value?
    {
        Serial.println(encoder.getValue());
    }
}