#pragma once
#include <Arduino.h>
#include "Haptic_Driver.h"

class LinearResonator
{
public:
    LinearResonator() = default;
    void init();
    void set_frequency(float frequency);
    void set_intensity(uint8_t intensity);
    float get_frequency();
    uint8_t get_intensity();

private:
    Haptic_Driver driver;
    float frequency;
    uint8_t intensity;
};