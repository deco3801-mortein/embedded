#include "VibrationController.hpp"

#define PULSE_INTERVAL_MS       10000  // 10 seconds
#define PULSE_DURATION_MS        1000  // 1 second
#define REPETITION_INTERVAL_MS 900000  // 15 minutes (900 seconds)
#define REPETITION_DURATION_MS  60000  // 1 minute (60 seconds)
#define MAX_OVERRIDE_MS         30000  // 30 seconds

void VibrationController::init()
{
    pattern_start = millis();
    override_start = pattern_start;
    pattern_running = false;
    override = false;
    vibrating = false;
    linear_resonator.init();
    linear_resonator.set_frequency(300.F);
    linear_resonator.set_intensity(0);
}

void VibrationController::update()
{
    if (override) {
        uint32_t duration = millis() - override_start;
        if (duration > MAX_OVERRIDE_MS) {
            vibrate_off();
            override = false;
        }
    } else if (pattern_running) {
        uint32_t duration = millis() - pattern_start;
        if (duration % REPETITION_INTERVAL_MS < REPETITION_DURATION_MS
                && duration % PULSE_INTERVAL_MS < PULSE_DURATION_MS) {
            vibrate_on();
        } else {
            vibrate_off();
        }
    } else {
        vibrate_off();
    }
}

void VibrationController::start_pattern()
{
    pattern_running = true;
    pattern_start = millis();
}

void VibrationController::stop_pattern()
{
    pattern_running = false;
}

bool VibrationController::is_pattern_running()
{
    return pattern_running;
}

void VibrationController::toggle_override()
{
    override = !override;
    override_start = millis();
    if (override) {
        vibrate_on();
    }
}


void VibrationController::vibrate_on()
{
    if (!vibrating) {
        linear_resonator.set_intensity(100);
        vibrating = true;
    }
}

void VibrationController::vibrate_off()
{
    if (vibrating) {
        linear_resonator.set_intensity(0);
        vibrating = false;
    }
}