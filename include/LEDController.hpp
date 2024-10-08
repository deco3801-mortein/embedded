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
    /**
     * Set each RGB colour on (true) or off (false)
     * 
     * @param red Red LED state
     * @param green Green LED state
     * @param blue Blue LED state
     */
    void set_rgb(bool red, bool green, bool blue);
    /**
     * Toggle each RGB colour to its opposite state (if true), otherwise keep same.
     * 
     * @param red Toggle Red LED state if true, don't change if false.
     * @param green Toggle Green LED state if true, don't change if false.
     * @param blue Toggle Blue LED state if true, don't change if false.
     */
    void toggle_rgb(bool red, bool green, bool blue);
    void test_loop();

private:
    void send_command(uint8_t reg, uint8_t data, bool new_transaction = true);
    uint8_t digit_values[4];
};

extern LEDController leds;
