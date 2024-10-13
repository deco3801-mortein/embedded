#include <WiFi.h>
#include <WiFiAP.h>
#include <WebServer.h>
#include <Preferences.h>
#include <HTTPClient.h>
#include <UrlEncode.h>
#include <ArduinoJson.h>
#include "SetupMode.hpp"
#include "defines.hpp"
#include "LEDController.hpp"

PersistentVariables persistent_variables;
static WebServer server(80);
static Preferences prefs;

uint32_t button_start;
bool button_pressed;

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

static void handleRoot()
{
    server.send(200, "text/html", page);
}

static void handleForm()
{
    persistent_variables.wifi_ssid = server.arg("wifissid");
    persistent_variables.wifi_pass = server.arg("wifipass");
    persistent_variables.device_name = server.arg("devname");
    persistent_variables.device_name_changed = true;

    prefs.putString("wifissid", persistent_variables.wifi_ssid);
    prefs.putString("wifipass", persistent_variables.wifi_pass);
    prefs.putString("devname", persistent_variables.device_name);
    prefs.putBool("devchanged", persistent_variables.device_name_changed);

    Serial.print("WiFi SSID: ");
    Serial.println(persistent_variables.wifi_ssid);
    Serial.print("WiFi pwd: ");
    Serial.println(persistent_variables.wifi_pass);
    Serial.print("Device name: ");
    Serial.println(persistent_variables.device_name);
    // server.send(200, "text/html", page);
    server.sendHeader("Location", "/");
    server.send(303, "text/plain", "");
}

void init_setup_mode()
{
    prefs.begin("vibegrow", false);
    persistent_variables.wifi_ssid = prefs.getString("wifissid");
    persistent_variables.wifi_pass = prefs.getString("wifipass");
    persistent_variables.device_name = prefs.getString("devname");
    persistent_variables.device_id = prefs.getString("devid");
    persistent_variables.device_name_changed = prefs.getBool("devchanged", false);

    button_start = millis();
    button_pressed = false;
    pinMode(SETUP_BTN_PIN, INPUT_PULLUP);
}

void check_device_name_change()
{
    // If the device name has changed, we need to create a new device
    if (persistent_variables.device_name_changed) {
        HTTPClient http;
        String url = "https://api.vibegrow.pro/Device?name=" + urlEncode(persistent_variables.device_name);
        http.begin(url);
        // http.addHeader("Content-Type", "text/plain");

        int response_code = http.POST("");
        if (response_code > 0) {
            if (response_code == HTTP_CODE_CREATED) {
                String payload = http.getString();
                Serial.println("[HTTP] POST... successfully received response.");
                JsonDocument doc;
                deserializeJson(doc, payload);
                const char *did = doc["id"];
                persistent_variables.device_id = did;
                prefs.putString("devid", persistent_variables.device_id);
                Serial.print("New Device ID: ");
                Serial.println(persistent_variables.device_id);
                persistent_variables.device_name_changed = false;
                prefs.putBool("devchanged", persistent_variables.device_name_changed);
            } else {
                Serial.print("[HTTP] POST... failed, received response code: ");
                Serial.println(response_code);
            }
        } else {
            Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(response_code).c_str());
        }
    }
}

bool update_setup_mode()
{
    bool ret = false;

    if (button_pressed) {
        if (digitalRead(SETUP_BTN_PIN) != 0) {
            button_pressed = false;
            uint32_t duration = millis() - button_start;
            if (duration > SETUP_BTN_DEBOUNCE_MIN && duration < SETUP_BTN_DEBOUNCE_MAX) {
                ret = true;
            }
        } else if (millis() - button_start > SETUP_BTN_DURATION) {
            Serial.println("Going into setup mode.");
            leds.set_rgb(false, true, true);
            if (!WiFi.disconnect()) {
                Serial.println("Failed to disconnect from the WiFi");
            }
            if (!WiFi.softAP("VibeGrowSetup", "vibegrow1234")) {
                Serial.println("Soft AP creation failed.");
                return ret;
            }

            IPAddress myIP = WiFi.softAPIP();
            Serial.print("AP IP address: ");
            Serial.println(myIP);

            server.on("/", handleRoot);
            server.on("/settings", HTTP_POST, handleForm);
            server.begin();
            
            while (!(millis() - button_start > SETUP_BTN_PAUSE && digitalRead(SETUP_BTN_PIN) == 0)) {
                server.handleClient();
                delay(2);
            }

            server.close();
            WiFi.softAPdisconnect();

            ESP.restart(); // Restart the device from scratch
        }
    } else {
        if (digitalRead(SETUP_BTN_PIN) == 0) {
            button_pressed = true;
            button_start = millis();
        }
    }

    return ret;
}
