#include <Wire.h>
#include "LightSensor.hpp"

#define LIGHT_SENSOR_PWR_ON 0x01
#define LIGHT_SENSOR_CONTINUOUS_H_RESOLUTION_MODE 0x10
#define LIGHT_SENSOR_READ_MEASUREMENT 0x0

LightSensor::LightSensor(uint8_t address)
    : address{address} { }

uint8_t LightSensor::init()
{
    uint8_t err;

    Wire.beginTransmission(address);
    Wire.write(LIGHT_SENSOR_PWR_ON);
    err = Wire.endTransmission();
    if (err != 0) {
        return err;
    }

    Wire.beginTransmission(address);
    Wire.write(LIGHT_SENSOR_CONTINUOUS_H_RESOLUTION_MODE);
    err = Wire.endTransmission();
    return err;
}

uint16_t LightSensor::read()
{
    uint8_t ret;
    uint16_t result;

    // Returns number of bytes received
    ret = Wire.requestFrom(address, (uint8_t) 2);
    if (ret != 2) {
        return -1;
    }

    result = Wire.read() << 8;
    result |= Wire.read();

    return result;
}