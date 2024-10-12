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
#include "LEDController.hpp"
#include "nano_esp_mqtt.h"
#include "secrets.h"
#include "time.h"

#include <WiFi.h>
#include <WiFiAP.h>
#include <WebServer.h>
#include <Preferences.h>
#include <HTTPClient.h>
#include <UrlEncode.h>
#include <cmath>

#define SETUP_BTN_PIN B0

// int delayTime = 1000;
int delayTime = 5000;
bool success = false;

void receivedMessage(const char *message);

// AWS_Credentials credentials{AWS_ROOT_CA1, AWS_CERT, 
// AWS_PRIVATE_KEY, AWS_IOT_ENDPOINT, AWS_IOT_MQTT_PORT, AWS_THING_ID};


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

LEDController leds{};

WebServer server(80);
uint32_t button_start;
bool button_pressed;

Preferences prefs;
String wifi_ssid;
String wifi_pass;
String device_name;
String device_id;
bool device_name_changed = false;

static const String page{R"EOF(<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Setup Device</title>
</head>
<body>
    <h1>Set Up Device</h1>
    <form action="/settings" method="post">
        <p>
            <label for="wifissid">WiFi SSID</label>
            <input type="text" name="wifissid" id="wifissid">
        </p>
        <p>
            <label for="wifipass">WiFi Password</label>
            <input type="text" name="wifipass" id="wifipass">
        </p>
        <p>
            <label for="devname">Device Name</label>
            <input type="text" name="devname" id="devname">
        </p>
        <p>
            <input type="submit" value="Set">
        </p>
    </form>
</body>
</html>)EOF"};

void handleRoot() {
    server.send(200, "text/html", page);
}

void handleForm() {
    wifi_ssid = server.arg("wifissid");
    wifi_pass = server.arg("wifipass");
    device_name = server.arg("devname");
    device_name_changed = true;

    prefs.putString("wifissid", wifi_ssid);
    prefs.putString("wifipass", wifi_pass);
    prefs.putString("devname", device_name);
    prefs.putBool("devchanged", device_name_changed);

    Serial.print("WiFi SSID: ");
    Serial.println(wifi_ssid);
    Serial.print("WiFi pwd: ");
    Serial.println(wifi_pass);
    Serial.print("Device name: ");
    Serial.println(device_name);
    // server.send(200, "text/html", page);
    server.sendHeader("Location", "/");
    server.send(303, "text/plain", "");
}

void setup() {
    uint8_t err;

    Serial.begin(115200);
    // while (!Serial) delay(100);
    delay(2000);
    Serial.println("Initialising device.");

#if BOARD_TYPE == 0
    leds.init();
    leds.set_rgb(true, false, false);

    button_start = millis();
    button_pressed = false;
    pinMode(LED_RED, INPUT_PULLUP);
#endif

    prefs.begin("vibegrow", false);
    wifi_ssid = prefs.getString("wifissid");
    wifi_pass = prefs.getString("wifipass");
    device_name = prefs.getString("devname");
    device_id = prefs.getString("devid");
    device_name_changed = prefs.getBool("devchanged", false);

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

    linearResonator.set_frequency(300.0);
    linearResonator.set_intensity(0);
    

    Serial.println("Device initialisation complete.");
    
    leds.set_rgb(true, true, false);
    if (wifi_ssid.isEmpty() || device_name.isEmpty()) {
        Serial.println("WiFi SSID or device name not set - please enter setup mode.");
    } else {
        Serial.println("Attempting connection to WIFI & MQTT.");
        const char* thing_id = WiFi.macAddress().c_str();
        AWS_Credentials credentials{AWS_ROOT_CA1, AWS_CERT, 
        AWS_PRIVATE_KEY, AWS_IOT_ENDPOINT, AWS_IOT_MQTT_PORT, thing_id};
        success = connectToWiFi(wifi_ssid.c_str(), wifi_pass.c_str()) && setTime() && connectToMQTT_Broker(credentials);
    }

    if (success) {
        leds.set_rgb(false, true, false);
        
        // If the device name has changed, we need to create a new device
        if (device_name_changed) {
            HTTPClient http;
            String url = "https://api.vibegrow.pro/Device?name=" + urlEncode(device_name);
            http.begin(url);
            // http.addHeader("Content-Type", "text/plain");

            int response_code = http.POST("");
            if (response_code > 0) {
                if (response_code == HTTP_CODE_CREATED) {
                    String payload = http.getString();
                    // Serial.println("[HTTP] POST... Received payload:");
                    // Serial.println(payload);
                    Serial.println("[HTTP] POST... successfully received response.");
                    JsonDocument doc;
                    deserializeJson(doc, payload);
                    const char *did = doc["id"];
                    device_id = did;
                    prefs.putString("devid", device_id);
                    Serial.print("New Device ID: ");
                    Serial.println(device_id);
                } else {
                    Serial.print("[HTTP] POST... failed, received response code: ");
                    Serial.println(response_code);
                }
            } else {
                Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(response_code).c_str());
            }
            device_name_changed = false;
            prefs.putBool("devchanged", device_name_changed);
        }

        subscribe(device_id.c_str(), receivedMessage);
    } else {
        leds.set_rgb(true, false, false);
    }
}

void loop() {
    if (button_pressed) {
        if (digitalRead(LED_RED) != 0) {
            button_pressed = false;
        } else if (millis() - button_start > 2000) {
            Serial.println("Going into setup mode.");
            leds.set_rgb(false, true, true);
            if (!WiFi.disconnect()) {
                Serial.println("Failed to disconnect from the WiFi");
            }
            if (!WiFi.softAP("VibeGrowSetup", "vibegrow1234")) {
                Serial.println("Soft AP creation failed.");
                return;
            }

            IPAddress myIP = WiFi.softAPIP();
            Serial.print("AP IP address: ");
            Serial.println(myIP);

            server.on("/", handleRoot);
            server.on("/settings", HTTP_POST, handleForm);
            server.begin();
            
            while (!(millis() - button_start > 5000 && digitalRead(LED_RED) == 0)) {
                server.handleClient();
                delay(2);
            }

            server.close();
            WiFi.softAPdisconnect();

            ESP.restart(); // Restart the device from scratch
        }
    } else {
        if (digitalRead(LED_RED) == 0) {
            button_pressed = true;
            button_start = millis();
        }
    }

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
        uint8_t light_level = light / 1000;
        if (light_level > 10) light_level = 10;
        float scaler = 10.F / log10f(65535.F);
        uint8_t moisture_level = (uint8_t) (scaler * log10f(moisture)); //(moisture / 10.0);
        if (moisture_level > 10) moisture_level = 10;
        leds.set_bar(0, light_level);
        leds.set_bar(1, moisture_level);
        #elif BOARD_TYPE == 1
        sprintf(log, "Moisture: %.2f%%,\tTemperature: %.2f°C,\tUV: %d", moisture, temperature, uv);
        #endif
        Serial.println(log);

        // linearResonator.set_frequency((float) frequency);
        // linearResonator.set_intensity(intensity);
        

        char message[500];
        // Serial.println("sending test message");
        JsonDocument doc;
        doc["DeviceId"] = device_id;
        doc["Timestamp"] = getTime();
        doc["Moisture"] = moisture;
#if BOARD_TYPE == 1
        doc["Sunlight"] = uv;
#else
        doc["Sunlight"] = light;
#endif
        doc["Temperature"] = temperature;
        doc["IsVibrating"] = linearResonator.get_intensity() != 0;
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