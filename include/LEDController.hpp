#pragma once
#include <Arduino.h>

class LEDController
{
public:
    LEDController();
    void init();
    /**
     * Set the level (0-10) of an LED bar (either bar 0 or bar 1).
     * 
     * @param bar Which LED bar
     * @param level How many LEDs to illuminate (0-10)
     */
    void set_bar(uint8_t bar, uint8_t level);
    void set_rgb(bool red, bool green, bool blue);
    void test_loop();

private:
    void send_command(uint8_t reg, uint8_t data, bool new_transaction = true);
    uint8_t digit_values[4];
};
