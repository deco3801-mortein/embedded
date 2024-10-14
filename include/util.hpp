#pragma once
#include <Arduino.h>

/**
 * Scale light sensor readings to value 0-10 for LED bar.
 * 
 * @param light Sensor reading in lux (lumen/m^2)
 * @return Light level 0-10
 */
uint8_t scale_light_level(uint16_t light);

/**
 * Scale soil moisture level to value 0-10 for LED bar.
 * 
 * @param moisture Sensor reading (0-100%)
 * @return Moisture level 0-10
 */
uint8_t scale_moisture_level(float moisture);
