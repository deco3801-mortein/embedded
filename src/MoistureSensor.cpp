#include "MoistureSensor.hpp"
#include "defines.hpp"

#define MOISTURE_MIN_OUTPUT 0.F
#define MOISTURE_MAX_OUTPUT 100.F

MoistureSensor::MoistureSensor(uint8_t pin, float min_input, float max_input)
    : pin{pin}, min_input{min_input}, max_input{max_input} { }

void MoistureSensor::init()
{
    pinMode(pin, INPUT);
}

float MoistureSensor::read()
{
    int value = analogRead(pin);
    // Serial.print("Raw value is: ");
    // Serial.println(value);
    return ((float) value - min_input) * ((MOISTURE_MAX_OUTPUT - MOISTURE_MIN_OUTPUT)
            / (max_input - min_input)) + MOISTURE_MIN_OUTPUT;
}