#include <Arduino.h>
#include <ArduinoJson.h>
#include <Wire.h>

#include "nano_esp_mqtt.h"
#include "secrets.h"
#include "time.h"

bool success = true;

// int delayTime = 10000;
int delayTime = 1000;

const int MIN_MOISTURE = 3180;
const int MAX_MOISTURE = 1270;
const int MIN_TEMP = 1600;
const int MAX_TEMP = 1950;
const int8_t MOISTURE_PIN = A0;
const int8_t UV_PIN = A1;
const int8_t TEMP_PIN = A2;
const int8_t CONT_PWM = A6;
const int8_t CONT_ADDRESS = 0x5A;
const int8_t CONT_MODE_REG = 0x01;
const int8_t CONT_MODE = 3;




AWS_Credentials credentials{AWS_ROOT_CA1, AWS_CERT, AWS_PRIVATE_KEY, AWS_IOT_ENDPOINT, AWS_IOT_MQTT_PORT, AWS_THING_ID};

void receivedMessage(const char *message);

void setup() {
    pinMode(MOISTURE_PIN, INPUT);
    pinMode(TEMP_PIN, INPUT); 
    pinMode(UV_PIN, INPUT);
    pinMode(CONT_PWM, OUTPUT);
    pinMode(LED_RED, OUTPUT);
    pinMode(LED_GREEN, OUTPUT);
    pinMode(LED_BLUE, OUTPUT);
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);
    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_GREEN, HIGH);
    digitalWrite(LED_BLUE, LOW);

    // controller setup
    Wire.begin();


    Serial.begin(115200);
    while (!Serial)
        ;
    delay(1000);
    Serial.println("hello");

    // controller test
    Wire.beginTransmission(CONT_ADDRESS);
    Wire.write(0);
    Wire.endTransmission();
    Serial.println("got here");
    Wire.requestFrom(CONT_ADDRESS, 1);

    while (!Wire.available()) {
        ;
    }
    Serial.println(Wire.read(), HEX);
    Wire.beginTransmission(CONT_ADDRESS);
    Wire.write(CONT_MODE_REG);
    Wire.write(1<<6);
    Wire.endTransmission();

    Serial.println("goodbye");


    success = connectToWiFi(WIFI_SSID, WIFI_PWD) && setTime() && connectToMQTT_Broker(credentials) &&
              subscribe("test_sub_topic", receivedMessage);

    if (success) {
        digitalWrite(LED_BUILTIN, LOW);
        digitalWrite(LED_RED, HIGH);
        digitalWrite(LED_GREEN, LOW);
        digitalWrite(LED_BLUE, HIGH);
    } else {
        digitalWrite(LED_BUILTIN, LOW);
        digitalWrite(LED_RED, LOW);
        digitalWrite(LED_GREEN, HIGH);
        digitalWrite(LED_BLUE, HIGH);
    }
    
}

void loop() {
    checkIncoming();
    if (millis() % delayTime == 0) {
        int moisture = analogRead(MOISTURE_PIN);
        moisture = map(moisture, MIN_MOISTURE, MAX_MOISTURE, 0, 100);
#if DEBUG
        Serial.print("moisture: ");
        Serial.println(moisture);
#endif DEBUG
        int sunlight = analogRead(UV_PIN);
        // Serial.print("sunlight: ");
        // Serial.println(sunlight);
        int temp = analogRead(TEMP_PIN);
        temp = map(temp, MIN_TEMP, MAX_TEMP, 20, 35);
        // Serial.print("temp: ");
        // Serial.print(temp);
        // Serial.println(" celcius");
        
        char message[500];
        // Serial.println("sending test message");
        JsonDocument doc;
        doc["sensor_id"] = AWS_THING_ID;
        doc["timestamp"] = getTime();
        doc["moisture"] = moisture;
        doc["sunlight"] = sunlight;
        doc["temp"] = temp;
        serializeJson(doc, message);
        sendMessage("test_topic", message);

        


        
    }
}

void receivedMessage(const char *message) {
    JsonDocument doc;
    deserializeJson(doc, message);
    int LRA_freq = doc["lra_freq"];
    if (LRA_freq == 0) {
       Wire.beginTransmission(CONT_ADDRESS);
        Wire.write(CONT_MODE_REG);
        Wire.write(1<<6);
        Wire.endTransmission(); 
    }
    if (LRA_freq != 0) {
       Wire.beginTransmission(CONT_ADDRESS);
        Wire.write(CONT_MODE_REG);
        Wire.write(CONT_MODE);
        Wire.endTransmission();
        analogWrite(CONT_PWM, LRA_freq); 
    }
    Serial.println((const char *)doc["message"]);
}


