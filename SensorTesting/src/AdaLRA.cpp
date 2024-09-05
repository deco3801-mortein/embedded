#include "AdaLRA.hpp"
#include "Wire.h"

const int8_t CONT_PWM = A6;
const int8_t CONT_ADDRESS = 0x5A;
const int8_t CONT_MODE_REG = 0x01;
const int8_t CONT_MODE = 3;

AdaLRA::AdaLRA(uint8_t pin)
    : pin{pin} {}

void AdaLRA::init()
{
    frequency = 0.F;
    intensity = 0;

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
}

void AdaLRA::set_frequency(float frequency)
{
    this->frequency = frequency;

}

void AdaLRA::set_intensity(uint8_t intensity)
{
    Wire.beginTransmission(CONT_ADDRESS);
    Wire.write(CONT_MODE_REG);
    Wire.write(CONT_MODE);
    Wire.endTransmission();
    analogWrite(CONT_PWM, frequency);
    this->intensity = intensity;

}

float AdaLRA::get_frequency()
{
    return frequency;
}

uint8_t AdaLRA::get_intensity()
{
    return intensity;
}