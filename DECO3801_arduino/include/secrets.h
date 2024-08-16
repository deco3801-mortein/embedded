#ifndef SECRETS_H
#define SECRETS_H

#include <pgmspace.h>

static const char WIFI_SSID[] = "NAME OF WIFI GOES HERE";
static const char WIFI_PWD[] = "PASSWORD GOES HERE";


// AWS Thing ID
static const char AWS_THING_ID[] = "DECO3801_mortein_device";

// URL for AWS MQTT broker
static const char AWS_IOT_ENDPOINT[] = "a26tzy9bc6hmgf-ats.iot.ap-southeast-2.amazonaws.com";
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
MIIDWjCCAkKgAwIBAgIVAK+noPBCGRzg8lXAJXRMHo1J87hvMA0GCSqGSIb3DQEB
CwUAME0xSzBJBgNVBAsMQkFtYXpvbiBXZWIgU2VydmljZXMgTz1BbWF6b24uY29t
IEluYy4gTD1TZWF0dGxlIFNUPVdhc2hpbmd0b24gQz1VUzAeFw0yNDA3MzEwMDQw
MzhaFw00OTEyMzEyMzU5NTlaMB4xHDAaBgNVBAMME0FXUyBJb1QgQ2VydGlmaWNh
dGUwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQCyAag0BzYR29sf1jNT
GlLRGNWH/C74oT6imHBuLBXv3Xw/jI1FxhUvRmzxEkEVLLXk7XQB3YitKgnob2U6
py3eOFusIW51eFIoqAxoPitjho+4Hty2XOMM43zJQwMgOswgB9/Rh9b76+D5klEO
ms1KnpU1WDnPuDsLBPDWlBYLKdjA4gJnwHeXAy7NF80Qo+A4l5TBukj81rshb12y
nX+li+TH3DBHuLcoRtmsPit/1AbGWge3PY8bT+DFOn/nlL3D6LJqXsykcxjizWzQ
1+GOc7lZTNegcLOXxE/+3/dMxFQtzcWr1YxYQ3+lBGF4S/WaZDAjueR4qfvyjTQK
BlN3AgMBAAGjYDBeMB8GA1UdIwQYMBaAFFDGPn+SjgMFCnTCOzHR0ZtrmHjgMB0G
A1UdDgQWBBQwz6duEtp6fLDfQHMAaTR/h6P+dTAMBgNVHRMBAf8EAjAAMA4GA1Ud
DwEB/wQEAwIHgDANBgkqhkiG9w0BAQsFAAOCAQEAnQcn8LweEKV87En/92hs+b4Q
qv9eKyXcp4VGdS1aYJ36o5EGkpJxgAdHxIwsCGv1j92eXOhD72YBvmwMk/lks2Vb
29EG1I0+P1SoV2g8g7ioxFwNRtJKF79LeFtkV75MDa/GSi/du6sEjx2G6jl1Ik6F
itZTol8Gt3cJJRWNXHZaDZAj2Tqa2mu5sIlJBZL4ZwLuB7FVcgzQJJsxnJDhOz3P
ZZqTf7vnkqBR0pDjad3SuesSPbrMnhWPaQ0Rlkn4/6F0qru/W7zD6Vposxr1ySJd
my5szs0Wsas38mwveF8JCTQElpaPJsN6jY5+mwz3P5Mla6hjiaNgVo5TOqVymA==
-----END CERTIFICATE-----
)KEY";
 
// Device Private Key
static const char AWS_PRIVATE_KEY[] PROGMEM = R"KEY(
-----BEGIN RSA PRIVATE KEY-----
MIIEogIBAAKCAQEAsgGoNAc2EdvbH9YzUxpS0RjVh/wu+KE+ophwbiwV7918P4yN
RcYVL0Zs8RJBFSy15O10Ad2IrSoJ6G9lOqct3jhbrCFudXhSKKgMaD4rY4aPuB7c
tlzjDON8yUMDIDrMIAff0YfW++vg+ZJRDprNSp6VNVg5z7g7CwTw1pQWCynYwOIC
Z8B3lwMuzRfNEKPgOJeUwbpI/Na7IW9dsp1/pYvkx9wwR7i3KEbZrD4rf9QGxloH
tz2PG0/gxTp/55S9w+iyal7MpHMY4s1s0NfhjnO5WUzXoHCzl8RP/t/3TMRULc3F
q9WMWEN/pQRheEv1mmQwI7nkeKn78o00CgZTdwIDAQABAoIBACZsyyVoBRj3z6Zg
s7WVshjm9nkjS/Rpwj6RD3wI0jd0JTUGnxxXtQz+c9faK+hpPV6Mj4UzkCRpNbhS
C5UMr48PtWJD2TdMYNfb5sm92dnjGzUQnG5VQMHwfWhoqc0URCoJ6Ruu6g55jZfy
KMMxUW1xLB5WzCv1xfEMAP0g/F0uMAJOspYQhnlDn601ToM3j5x8+EV0ogGKIhMN
9M8xVIJkb+9I0dHpwMK4IriCx4o5Rv6bpuUXZvEO3sNtt4k4ZX17anZ35jxu41vb
9iA70cgpWik0MMmsdHbRq5h8h7a6Dtz8k2hkDOwPQtV3whEP8jIFZemgScGqTrDk
hTPVSCkCgYEA4sg0kl+WU9vM1Gq3EahUyr6vfsjOEDKbMFhXZHVFcUWju/ppwgmx
T7ARpY0TPKCD4KM9UR3nAiEqR/Im4QafhpVfEWrRv3U3Ga/Fo9wtKISQdKsNwerq
thj2nqO9dTeaXe4noQxoR/w7gO7wYADpHK2Y4XE0HOP/ATckUn+jZp0CgYEAyPC4
fvu2dkmo9EqVEeU9iL62zCb6YeZ6O48h7b8D6p9elwKrwYYKGzlKeee7Q3g+RDbc
aP26Syw9O7LW6P/5otp8GCR6T0g46AFVc7iuO6wBOpv/5Q1bB0xZp9W2fcDOS+US
cFFV5lms69MAoja63DgPw8IQKy4sMcaNhepkvCMCgYAgVIhiIaxtshUmZzxIESZd
bQfZ3Ei8OGq1XrK+yJUoy9PG1bFED1uuz5wEbPNEHmIOoVb5fHjVbeL4NMZkKCyP
5DWUZxUV5WEAkXVsodPyRcvORiCE97eUVES8Yv18e3vPTkdqBznKoWPeQrIUDa0u
AHWeFiRmxf4xrynHdRjXZQKBgHzdgXh/SHom+dOciRfOwTc5c/RXAF5Xz3s1TZsk
iPWCRVBHu52Ngrg6GLq6MpcRoHfZM4wWp25RNlfjoUK21lTJujVybYLY2WvqcERx
7etsznKyQAkBDKDzY/80gauRZWCqH5QmJt/2t8rvmsQyiFmSiNFXxZ5fmdzwij8+
UYzLAoGASDKuMrGmEqDGNBjOV0wZyQ7suxTpkDRedp3Mz9KM1ii2w4a4zk+zJQFC
yvBbMoux1uzAda+Jqo4Waqv6a5Gd/842VN3DUujpJoj1YGw0l+J/h7NAFJlg8A2D
OfL2BNlKyHJwrtV6hlz3HDexv1ZfO70S6uAF85t8FDPiTi2vOp4=
-----END RSA PRIVATE KEY-----
)KEY";

#endif
