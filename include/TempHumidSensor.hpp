#pragma once
#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

/**
 * DHT11 Temperature and Humidity Sensor.
 */
class TempHumidSensor
{
public:
    TempHumidSensor(uint8_t pin);
    void init();
    void print_sensor_info();
    float read_temp();
    float read_humid();

private:
    DHT_Unified dht;
    sensor_t temp_sensor;
    sensor_t humid_sensor;
};