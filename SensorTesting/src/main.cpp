#include <Arduino.h>
#include <Wire.h>
#include "defines.hpp"
#include "MoistureSensor.hpp"
#include "LightSensor.hpp"
#include "TempHumidSensor.hpp"
#include "LinearResonator.hpp"
#include "SoilTempSensor.hpp"
#include "UVSensor.hpp"
#include "AdaLRA.hpp"


MoistureSensor moistureSensor{MOISTURE_SENSOR_PIN, MOISTURE_MIN_INPUT, MOISTURE_MAX_INPUT};
#if BOARD_TYPE == 0
LightSensor lightSensor{LIGHT_SENSOR_ADDR};
TempHumidSensor tempHumidSensor{DHTPIN};
LinearResonator linearResonator;
#elif BOARD_TYPE == 1
UVSensor uvSensor{UV_PIN};
SoilTempSensor soilTempSensor{THERMISTOR_PIN, SOIL_TEMP_FREEZE_INPUT, SOIL_TEMP_ROOM_INPUT};
AdaLRA linearResonator{ADALRA_PWM_PIN};
#endif

void setup() {
    uint8_t err;

    Serial.begin(115200);
    while (!Serial) delay(100);
    Serial.println("Initialising device.");

    Wire.begin();
    Serial.println("I2C interface intialised.");

    moistureSensor.init();
    linearResonator.init();

    #if BOARD_TYPE == 0
    err = lightSensor.init();
    if (err) {
        Serial.println("Failed to initialise light sensor device.");
        return;
    }
    tempHumidSensor.init();
    tempHumidSensor.print_sensor_info();
    #elif BOARD_TYPE == 1
    uvSensor.init();
    soilTempSensor.init();
    #endif
    

    Serial.println("Device initialisation complete.");
}

void loop() {
    char log[150];
    static int frequency = 100;
    static uint8_t intensity = 0;

    float moisture;
    float temperature;
    #if BOARD_TYPE == 0
    float humidity;
    uint16_t light;
    #elif BOARD_TYPE == 1
    float uv;
    #endif
    
    moisture = moistureSensor.read();

    #if BOARD_TYPE == 0
    light = lightSensor.read();
    temperature = tempHumidSensor.read_temp();
    humidity = tempHumidSensor.read_humid();
    #elif BOARD_TYPE == 1
    uv = uvSensor.read_UV();
    temperature = soilTempSensor.read_temp();
    #endif

    #if BOARD_TYPE == 0
    sprintf(log, "Moisture: %.2f%%,\tTemperature: %.2f°C,\tLight: %d,\tHumidity: %.2f", 
        moisture, temperature, light, humidity);
    #elif BOARD_TYPE == 1
    sprintf(log, "Moisture: %.2f%%,\tTemperature: %.2f°C,\tUV: %d", moisture, temperature, uv);
    #endif
    Serial.println(log);

    linearResonator.set_frequency((float) frequency);
    linearResonator.set_intensity(intensity);
    
    frequency = frequency % 300 + 100;
    if (frequency == 100) {
        intensity = (intensity + 25) % 150;
    }


    delay(500);
}