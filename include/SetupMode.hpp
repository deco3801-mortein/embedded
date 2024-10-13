#pragma once
#include <Arduino.h>

/**
 * Single source of truth for the persistent variables.
 */
extern struct PersistentVariables
{
    String wifi_ssid;
    String wifi_pass;
    String device_name;
    String device_id;
    bool device_name_changed = false;
} persistent_variables;

/**
 * Initialise setup mode button.
 */
void init_setup_mode();

/**
 * Check if the device name in persistent memory has changed since last time.
 * 
 * If it has, this function will connect to the back-end API and create a new
 * device with the new device name.
 */
void check_device_name_change();

/**
 * Call once per update loop - checks if device enters setup mode.
 * 
 * If device enters setup mode, this will close any existing WiFi connection,
 * create a WiFi access point, host the setup webpage, and reset the device
 * once the setup mode button is pressed again.
 */
void update_setup_mode();
