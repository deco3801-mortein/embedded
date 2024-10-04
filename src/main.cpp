#include <Arduino.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include "defines.hpp"
#include "MoistureSensor.hpp"
#include "LightSensor.hpp"
#include "TempHumidSensor.hpp"
#include "LinearResonator.hpp"
#include "SoilTempSensor.hpp"
#include "UVSensor.hpp"
#include "AdaLRA.hpp"
#include "nano_esp_mqtt.h"
#include "secrets.h"
#include "time.h"

int delayTime = 1000;
bool success = true;

void receivedMessage(const char *message);

AWS_Credentials credentials{AWS_ROOT_CA1, AWS_CERT, 
AWS_PRIVATE_KEY, AWS_IOT_ENDPOINT, AWS_IOT_MQTT_PORT, AWS_THING_ID};


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
    Serial.println("Attempting connection to WIFI & MQTT.");
    success = connectToWiFi(WIFI_SSID, WIFI_PWD) && setTime() && connectToMQTT_Broker(credentials) 
    && subscribe(AWS_THING_ID, receivedMessage)
    ;

    linearResonator.set_frequency(100.0);
    linearResonator.set_intensity(0);
}

void loop() {
    checkIncoming();
    if (millis() % delayTime == 0) {
        char log[150];
        // static int frequency = 100;
        // static uint8_t intensity = 0;

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

        // linearResonator.set_frequency((float) frequency);
        // linearResonator.set_intensity(intensity);
        

        char message[500];
        // Serial.println("sending test message");
        JsonDocument doc;
        doc["DeviceId"] = AWS_THING_ID;
        doc["Timestamp"] = getTime();
        doc["Moisture"] = moisture;
#if BOARD_TYPE == 1
        doc["Sunlight"] = uv;
#else
        doc["Sunlight"] = light;
#endif
        doc["Temperature"] = temperature;
        doc["IsVibrating"] = true;
        serializeJson(doc, message);
        sendMessage("iot/test", message);
        // frequency = frequency % 300 + 100;
        // if (frequency == 100) {
        //     intensity = (intensity + 25) % 150;
        // }


        // delay(500);
    }
}


void receivedMessage(const char *message) {
    JsonDocument doc;
    deserializeJson(doc, message);
    bool vibrate = doc["Vibrate"];
    if (vibrate) {
        linearResonator.set_intensity(100);
    } else {
        linearResonator.set_intensity(0);
    }

    Serial.println(message);
}