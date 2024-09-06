#include "TempHumidSensor.hpp"
#include "defines.hpp"
#if BOARD_TYPE == 0
TempHumidSensor::TempHumidSensor(uint8_t pin)
    : dht{pin, DHTTYPE} { }

void TempHumidSensor::init()
{
    dht.begin();
    dht.temperature().getSensor(&temp_sensor);
    dht.humidity().getSensor(&humid_sensor);
}

void TempHumidSensor::print_sensor_info()
{
    // Print temperature sensor details.
    Serial.println(F("------------------------------------"));
    Serial.println(F("Temperature Sensor"));
    Serial.print  (F("Sensor Type: ")); Serial.println(temp_sensor.name);
    Serial.print  (F("Driver Ver:  ")); Serial.println(temp_sensor.version);
    Serial.print  (F("Unique ID:   ")); Serial.println(temp_sensor.sensor_id);
    Serial.print  (F("Max Value:   ")); Serial.print(temp_sensor.max_value); Serial.println(F("°C"));
    Serial.print  (F("Min Value:   ")); Serial.print(temp_sensor.min_value); Serial.println(F("°C"));
    Serial.print  (F("Resolution:  ")); Serial.print(temp_sensor.resolution); Serial.println(F("°C"));
    Serial.println(F("------------------------------------"));

    // Print humidity sensor details.
    Serial.println(F("Humidity Sensor"));
    Serial.print  (F("Sensor Type: ")); Serial.println(humid_sensor.name);
    Serial.print  (F("Driver Ver:  ")); Serial.println(humid_sensor.version);
    Serial.print  (F("Unique ID:   ")); Serial.println(humid_sensor.sensor_id);
    Serial.print  (F("Max Value:   ")); Serial.print(humid_sensor.max_value); Serial.println(F("%"));
    Serial.print  (F("Min Value:   ")); Serial.print(humid_sensor.min_value); Serial.println(F("%"));
    Serial.print  (F("Resolution:  ")); Serial.print(humid_sensor.resolution); Serial.println(F("%"));
    Serial.println(F("------------------------------------"));
}

float TempHumidSensor::read_temp()
{
    sensors_event_t event;
    dht.temperature().getEvent(&event);
    return event.temperature;
}

float TempHumidSensor::read_humid()
{
    sensors_event_t event;
    dht.humidity().getEvent(&event);
    return event.relative_humidity;
}
#endif