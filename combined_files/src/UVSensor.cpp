#include <Arduino.h>
#include "UVSensor.hpp"
#include "defines.hpp"
#if BOARD_TYPE == 1
UVSensor::UVSensor(uint8_t pin)
    : pin{pin} {}

void UVSensor::init()
{
    pinMode(pin, INPUT);
}

float UVSensor::read_UV()
{
    int value = analogRead(pin);
    return value;
}
#endif