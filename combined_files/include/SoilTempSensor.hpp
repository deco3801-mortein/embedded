#pragma once
#include <Arduino.h>

/**
 * Soil temperature thermistor sensor
 */
class SoilTempSensor
{
public:
    /**
     * Construct a new Moisture Sensor object
     * 
     * @param pin The analog input pin
     */
    SoilTempSensor(uint8_t pin, float freeze_input, float room_input);
    void init();
    float read_temp();

private:
    const uint8_t pin;
    float freeze_input;
    float room_input;
};