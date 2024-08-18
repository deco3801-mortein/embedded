#pragma once
#include <Arduino.h>

class LightSensor
{
public:
    LightSensor(uint8_t address);
    /**
     * Initialise the device to power on and begin measuring.
     * 
     * Assumes that the Wire (I2C) interface is already set up.
     * @return
     * 0: success.
     * 
     * 1: data too long to fit in transmit buffer.
     * 
     * 2: received NACK on transmit of address.
     * 
     * 3: received NACK on transmit of data.
     * 
     * 4: other error.
     * 
     * 5: timeout
     */
    uint8_t init();
    /**
     * Read brightness value in lux.
     * 
     * @return Brightness value in lux if successful.
     * 
     * Return (uint16_t) -1 on failure.
     */
    uint16_t read();

private:
    const uint8_t address;
};