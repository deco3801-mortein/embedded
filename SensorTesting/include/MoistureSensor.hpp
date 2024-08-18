#pragma once
#include <Arduino.h>

/**
 * Basic moisture sensor with analog input pin.
 */
class MoistureSensor
{
public:
    /**
     * Construct a new Moisture Sensor object
     * 
     * @param pin The analog input pin
     * @param min_input The minimum analog reading (for no moisture)
     * @param max_input The maximum analog reading (for pure water)
     */
    MoistureSensor(uint8_t pin, float min_input, float max_input);
    /** Initialise input pin. */
    void init();
    /** Read value, calibrated with 0 as no moisture and 100 as pure water. */
    float read();

private:
    const uint8_t pin;
    const float min_input;
    const float max_input;
};