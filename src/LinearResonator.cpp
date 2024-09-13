#include "LinearResonator.hpp"

void LinearResonator::init()
{
    frequency = 0.F;
    intensity = 0;

    driver.begin();
    driver.defaultMotor();
    driver.setOperationMode(DRO_MODE);
    driver.enableFreqTrack(false);
    driver.enableAcceleration(false);
}

void LinearResonator::set_frequency(float frequency)
{
    driver.setActuatorLRAfreq(frequency);
    this->frequency = frequency;
}

void LinearResonator::set_intensity(uint8_t intensity)
{
    driver.setVibrate(intensity);
    this->intensity = intensity;
}

float LinearResonator::get_frequency()
{
    return frequency;
}

uint8_t LinearResonator::get_intensity()
{
    return intensity;
}