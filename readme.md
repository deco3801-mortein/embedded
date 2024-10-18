# Embedded System
This repository contains the firmware for the physical device.

## Build
This folder must be opened in Visual Studio Code with the PlatformIO IDE extension installed.
Wait for the extension to initialise, and open the PlatformIO Side Bar from the Activity Bar
on the far left-hand side. You may need to reload the window on the first attempt.

For security purposes, the `/include/secrets.h` file is not included in this repository.
This contains the necessary authentication the connection to the MQTT broker running on
AWS servers. Please create this file, and fill in the required keys and certificates.

The template for this `/include/secrets.h` file is demonstrated below:
```c
#ifndef SECRETS_H
#define SECRETS_H

#include <pgmspace.h>

// URL for AWS MQTT broker
static const char AWS_IOT_ENDPOINT[] = "www.fill-in-mqtt-broker-url-here.com";
static const unsigned short AWS_IOT_MQTT_PORT = 8883;
 
// Amazon Root CA 1
static const char AWS_ROOT_CA1[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF
ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6
b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL
MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv
b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj
ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM
9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw
IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6
VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L
93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm
jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC
AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA
A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI
U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs
N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv
o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU
5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy
rqXRfboQnoZsG4q5WTP468SQvvG5
-----END CERTIFICATE-----
)EOF";

// Device Certificate
static const char AWS_CERT[] PROGMEM = R"KEY(
-----BEGIN CERTIFICATE-----
Copy device certificate here.
-----END CERTIFICATE-----
)KEY";
 
// Device Private Key
static const char AWS_PRIVATE_KEY[] PROGMEM = R"KEY(
-----BEGIN RSA PRIVATE KEY-----
Copy device private key here.
-----END RSA PRIVATE KEY-----
)KEY";

#endif
```

The firmware should now be ready. In the PlatformIO Side Bar, select
`final_prototype > General > Build` to build.

## Upload

Connect the Arduino Nano ESP32 to your computer via USB. Then, select
`final_prototype > General > Upload` from the PlatformIO Side Bar to flash the firmware to the
device.

To see basic logging information of the device's operation, select
`final_prototype > General > Monitor` from the PlatformIO Side Bar while the device is still
connected over USB. This will print all Serial logging messages to the console.


