#include <Arduino.h>
#include "SoilTempSensor.hpp"
#include "defines.hpp"

#define SOIL_TEMP_FREEZE_OUTPUT 0.F
#define SOIL_TEMP_ROOM_OUTPUT 23.F

#if BOARD_TYPE == 1
SoilTempSensor::SoilTempSensor(uint8_t pin, float freeze_input, float room_input)
//    : thermistorPin(pin) { }
    : pin{pin}, freeze_input{freeze_input}, room_input{room_input} {}

void SoilTempSensor::init()
{
    pinMode(pin, INPUT);
}

float SoilTempSensor::read_temp()
{
    int value = analogRead(pin);
    return ((float) value - freeze_input) * ((SOIL_TEMP_ROOM_OUTPUT - SOIL_TEMP_FREEZE_OUTPUT)
            / (room_input - freeze_input)) + SOIL_TEMP_FREEZE_OUTPUT;
}
#endif