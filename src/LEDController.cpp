#include <SPI.h>
#include "LEDController.hpp"
#include "defines.hpp"

static const SPISettings settings(100000, MSBFIRST, SPI_MODE0);

#define MAX7219_DECODE_MODE_REG 0x09
#define MAX7219_LIGHT_INTENS_REG 0x0A
#define MAX7219_SCAN_LIMIT_REG 0x0B
#define MAX7219_SHUTDOWN_MODE_REG 0x0C
#define MAX7219_DISPLAY_TEST_REG 0x0F
#define MAX7219_DIGIT_0_REG 0x01
#define MAX7219_DIGIT_1_REG 0x02
#define MAX7219_DIGIT_2_REG 0x03
#define MAX7219_DIGIT_3_REG 0x04
#define MAX7219_DIGIT_4_REG 0x05
#define MAX7219_DIGIT_5_REG 0x06
#define MAX7219_DIGIT_6_REG 0x07
#define MAX7219_DIGIT_7_REG 0x08

LEDController leds{};

LEDController::LEDController() : digit_values{}
{ }

void LEDController::init()
{
    pinMode(CS_PIN, OUTPUT);
    pinMode(MOSI_PIN, OUTPUT);
    pinMode(MISO_PIN, INPUT);
    pinMode(SCK_PIN, OUTPUT);

    digitalWrite(CS_PIN, HIGH);

    SPI.begin();
    SPI.beginTransaction(settings);
    send_command(MAX7219_DECODE_MODE_REG, 0x00, false); // no decode
    send_command(MAX7219_LIGHT_INTENS_REG, 0x0F, false); // maximum intensity
    send_command(MAX7219_SCAN_LIMIT_REG, 0x03, false); // display digits 0-3
    send_command(MAX7219_DISPLAY_TEST_REG, 0x00, false); // normal mode
    send_command(MAX7219_SHUTDOWN_MODE_REG, 0x01, false); // normal operation
    for (uint8_t i = MAX7219_DIGIT_0_REG; i <= MAX7219_DIGIT_7_REG; ++i) {
        send_command(i, 0x00, false);
    }
    SPI.endTransaction();
}

void LEDController::set_bar(uint8_t bar, uint8_t level)
{
    if (bar > 1 || level > 10) {
        return;
    }
    if (bar == 0) {
        if (level > 8) {
            digit_values[0] = 0xFF;
            digit_values[2] &= ~(0x03);
            digit_values[2] |= ((1 << (level - 8)) - 1) & 0x03;
            send_command(MAX7219_DIGIT_0_REG, digit_values[0]);
            send_command(MAX7219_DIGIT_2_REG, digit_values[2]);
        } else {
            digit_values[0] = (1 << level) - 1;
            digit_values[2] &= ~(0x03);
            send_command(MAX7219_DIGIT_0_REG, digit_values[0]);
            send_command(MAX7219_DIGIT_2_REG, digit_values[2]);
        }
    } else {
        if (level > 8) {
            digit_values[1] = 0xFF;
            digit_values[2] &= ~(0x0C);
            digit_values[2] |= (((1 << (level - 8)) - 1) << 2) & 0x0C;
            send_command(MAX7219_DIGIT_1_REG, digit_values[1]);
            send_command(MAX7219_DIGIT_2_REG, digit_values[2]);
        } else {
            digit_values[1] = (1 << level) - 1;
            digit_values[2] &= ~(0x0C);
            send_command(MAX7219_DIGIT_1_REG, digit_values[1]);
            send_command(MAX7219_DIGIT_2_REG, digit_values[2]);
        }
    }
}

void LEDController::set_rgb(bool red, bool green, bool blue)
{
    digit_values[3] = (red << 0) | (green << 1) | (blue << 2);
    send_command(MAX7219_DIGIT_3_REG, digit_values[3]);
}

void LEDController::toggle_rgb(bool red, bool green, bool blue)
{
    digit_values[3] ^= (red << 0) | (green << 1) | (blue << 2);
    send_command(MAX7219_DIGIT_3_REG, digit_values[3]);
}

void LEDController::test_loop()
{
    for (int i = 0; i <= 10; ++i) {
        set_bar(0, i);
        delay(200);
    }
    for (int i = 0; i <= 10; ++i) {
        set_bar(1, i);
        delay(200);
    }
    set_rgb(true, false, false);
    delay(200);
    set_rgb(true, true, false);
    delay(200);
    set_rgb(true, true, true);
    delay(2000);

    set_bar(0, 0);
    set_bar(1, 0);
    set_rgb(false, false, false);
    delay(2000);
}

void LEDController::send_command(uint8_t reg, uint8_t data, bool new_transaction)
{
    uint8_t buffer[2];

    if (new_transaction) {
        SPI.beginTransaction(settings);
    }

    digitalWrite(CS_PIN, LOW);
    buffer[0] = reg;
    buffer[1] = data;
    SPI.transfer(buffer, 2);
    digitalWrite(CS_PIN, HIGH);

    if (new_transaction) {
        SPI.endTransaction();
    }
}
