#pragma once

// Moisture sensor
#define MOISTURE_SENSOR_PIN A0
#define MOISTURE_MIN_INPUT 3180.F
#define MOISTURE_MAX_INPUT 1270.F


// Light sensor
#if BOARD_TYPE == 0
#define LIGHT_SENSOR_ADDR 0x23
#endif

// UV sensor
#if BOARD_TYPE == 1
#define UV_PIN A1
#endif

// Temperature/humidity sensor
#if BOARD_TYPE == 0
#define DHTPIN 2 // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11 // Uncomment the type of sensor in use
// #define DHTTYPE DHT22
// #define DHTTYPE DHT21
#endif

// Thermistor sensor
#if BOARD_TYPE == 1
#define ADALRA_PWM_PIN A6
#define THERMISTOR_PIN A2
#define SOIL_TEMP_FREEZE_INPUT 1000.F
#define SOIL_TEMP_ROOM_INPUT 1933.F
#endif