#pragma once

// Moisture sensor
#define MOISTURE_SENSOR_PIN A0
#define MOISTURE_MIN_INPUT 3180.F
#define MOISTURE_MAX_INPUT 1270.F

// Light sensor
#define LIGHT_SENSOR_ADDR 0x23

// Temperature/humidity sensor
#define DHTPIN 2 // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11 // Uncomment the type of sensor in use
// #define DHTTYPE DHT22
// #define DHTTYPE DHT21
