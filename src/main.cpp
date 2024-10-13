#include <Arduino.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <WiFi.h>
#include "defines.hpp"
#include "MoistureSensor.hpp"
#include "LightSensor.hpp"
#include "TempHumidSensor.hpp"
#include "LinearResonator.hpp"
#include "SoilTempSensor.hpp"
#include "UVSensor.hpp"
#include "AdaLRA.hpp"
#include "LEDController.hpp"
#include "nano_esp_mqtt.h"
#include "secrets.h"
#include "time.h"
#include "SetupMode.hpp"
#include "util.hpp"

// Period of time between updates in milliseconds
#define UPDATE_PERIOD_MS 5000

void receivedMessage(const char *message);


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
    bool success = false;

    Serial.begin(115200);
    // while (!Serial) delay(100);
    delay(2000);
    Serial.println("Initialising device.");

#if BOARD_TYPE == 0
    leds.init();
    leds.set_rgb(true, false, false);
#endif

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
#else
    uvSensor.init();
    soilTempSensor.init();
#endif

    linearResonator.set_frequency(300.0);
    linearResonator.set_intensity(0);

    init_setup_mode();
    
    leds.set_rgb(true, true, false);
    if (persistent_variables.wifi_ssid.isEmpty() || persistent_variables.device_name.isEmpty()) {
        Serial.println("WiFi SSID or device name not set - please enter setup mode.");
    } else {
        Serial.println("Attempting connection to WIFI & MQTT.");
        const char* thing_id = WiFi.macAddress().c_str();
        AWS_Credentials credentials{AWS_ROOT_CA1, AWS_CERT, 
        AWS_PRIVATE_KEY, AWS_IOT_ENDPOINT, AWS_IOT_MQTT_PORT, thing_id};
        success = connectToWiFi(persistent_variables.wifi_ssid.c_str(), persistent_variables.wifi_pass.c_str())
                && setTime() && connectToMQTT_Broker(credentials);
    }

    if (success) {
        leds.set_rgb(false, true, false);
        check_device_name_change();
        subscribe(persistent_variables.device_id.c_str(), receivedMessage);
    } else {
        leds.set_rgb(true, false, false);
    }

    Serial.println("Device initialisation complete.");
}

void loop() {
    update_setup_mode();

    checkIncoming();
    if (millis() % UPDATE_PERIOD_MS == 0) {
        char log[150];

        float moisture;
        float temperature;

#if BOARD_TYPE == 0
        float humidity;
        uint16_t light;
#else
        float uv;
#endif
        
        moisture = moistureSensor.read();

#if BOARD_TYPE == 0
        light = lightSensor.read();
        temperature = tempHumidSensor.read_temp();
        humidity = tempHumidSensor.read_humid();

        sprintf(log, "Moisture: %.2f%%,\tTemperature: %.2f°C,\tLight: %d,\tHumidity: %.2f", 
            moisture, temperature, light, humidity);
        uint8_t light_level = scale_light_level(light);
        leds.set_bar(0, light_level);
        uint8_t moisture_level = scale_moisture_level(moisture);
        leds.set_bar(1, moisture_level);
#else
        uv = uvSensor.read_UV();
        temperature = soilTempSensor.read_temp();

        sprintf(log, "Moisture: %.2f%%,\tTemperature: %.2f°C,\tUV: %d", moisture, temperature, uv);
#endif

        Serial.println(log);
        

        char message[500];
        // Serial.println("sending test message");
        JsonDocument doc;
        doc["DeviceId"] = persistent_variables.device_id;
        doc["Timestamp"] = getTime();
        doc["Moisture"] = moisture;
#if BOARD_TYPE == 0
        doc["Sunlight"] = light;
#else
        doc["Sunlight"] = uv;
#endif
        doc["Temperature"] = temperature;
        doc["IsVibrating"] = linearResonator.get_intensity() != 0;
        serializeJson(doc, message);
        sendMessage("iot/test", message);
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