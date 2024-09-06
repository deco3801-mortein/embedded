#pragma once
#include <Arduino.h>

/**
 * UV sensor
 */
class UVSensor
{
public:
    /**
     * Construct a new UV Sensor object
     * 
     * @param pin The analog input pin
     */
    UVSensor(uint8_t pin);
    void init();
    float read_UV();

private:
    const uint8_t pin;
};