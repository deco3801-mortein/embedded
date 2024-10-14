#pragma once

#if BOARD_TYPE == 0
#   include "LinearResonator.hpp"
#else
#   include "AdaLRA.hpp"
#endif

class VibrationController
{
public:
    VibrationController() = default;
    /**
     * Initialise the vibration controller.
     */
    void init();
    /**
     * Update internal timings for the vibration controller (call once per loop).
     */
    void update();
    /**
     * Start running the pest control vibration pattern.
     */
    void start_pattern();
    /**
     * Stop running the pest control vibration pattern.
     */
    void stop_pattern();
    /**
     * Return true if the pattern is running, false otherwise.
     */
    bool is_pattern_running();
    /**
     * Toggle whether the vibration pattern should be overriden.
     * 
     * When overriden, a constant vibration is played. When not overriden,
     * the controller returns back to whatever state it was in before (either
     * with the pattern started or stopped).
     */
    void toggle_override();

private:
    void vibrate_on();
    void vibrate_off();

    uint32_t pattern_start;
    uint32_t override_start;
    bool pattern_running;
    bool override;
    bool vibrating;
#if BOARD_TYPE == 0
    LinearResonator linear_resonator;
#else
    AdaLRA linear_resonator;
#endif
};
