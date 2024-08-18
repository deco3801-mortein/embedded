#include <Arduino.h>
#include <Wire.h>
#include "defines.hpp"
#include "MoistureSensor.hpp"
#include "LightSensor.hpp"
#include "TempHumidSensor.hpp"
#include "LinearResonator.hpp"

MoistureSensor moistureSensor{MOISTURE_SENSOR_PIN, MOISTURE_MIN_INPUT, MOISTURE_MAX_INPUT};
LightSensor lightSensor{LIGHT_SENSOR_ADDR};
TempHumidSensor tempHumidSensor{DHTPIN};
LinearResonator linearResonator;

void setup() {
    uint8_t err;

    Serial.begin(115200);
    while (!Serial) delay(100);
    Serial.println("Initialising device.");

    Wire.begin();
    Serial.println("I2C interface intialised.");

    moistureSensor.init();
    err = lightSensor.init();
    if (err) {
        Serial.println("Failed to initialise light sensor device.");
        return;
    }
    tempHumidSensor.init();
    tempHumidSensor.print_sensor_info();
    linearResonator.init();

    Serial.println("Device initialisation complete.");
}

void loop() {
    static int frequency = 100;
    static uint8_t intensity = 0;

    float moisture;
    uint16_t light;
    float temperature;
    float humidity;
    
    moisture = moistureSensor.read();
    light = lightSensor.read();
    temperature = tempHumidSensor.read_temp();
    humidity = tempHumidSensor.read_humid();

    Serial.print("Moisture: ");
    Serial.print(moisture);
    Serial.print("%,\tLight: ");
    Serial.print(light);
    Serial.print("lx,\tTemperature: ");
    Serial.print(temperature);
    Serial.print("°C,\tHumidity: ");
    Serial.print(humidity);
    Serial.println("%.");

    linearResonator.set_frequency((float) frequency);
    linearResonator.set_intensity(intensity);
    
    frequency = frequency % 300 + 100;
    if (frequency == 100) {
        intensity = (intensity + 25) % 150;
    }

    delay(500);
}