#include "util.hpp"

uint8_t scale_light_level(uint16_t light)
{
    if (light < 20) return 0;
    if (light < 250) return 1;
    if (light < 800) return 2;
    if (light < 2500) return 3;
    if (light < 5000) return 4;
    if (light < 10000) return 5;
    if (light < 20000) return 6;
    if (light < 30000) return 7;
    if (light < 45000) return 8;
    if (light < 65535) return 9;
    return 10;
}

uint8_t scale_moisture_level(float moisture)
{
    uint8_t value = moisture / 10.F;
    if (value > 10) value = 10;
    return value;
}