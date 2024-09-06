#pragma once
#include <Arduino.h>

class AdaLRA
{
public:
    AdaLRA(uint8_t pin);
    void init();
    void set_frequency(float frequency);
    void set_intensity(uint8_t intensity);
    float get_frequency();
    uint8_t get_intensity();

private:
    uint8_t pin;
    float frequency;
    uint8_t intensity;
};