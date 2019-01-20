/**
 * Programmatically represents a SHARP GP2Y0A02 infrared sensor.
 */
#pragma once

#include "../../../../../runtime/Runtime.hpp"
#include "../InfraredAnalogSensor.hpp"

#ifndef PLATFORM_AGNOSTIC_BUILD
#include "../../../../../runtime/arduino_runtime/ArduinoRuntime.hpp"
extern ArduinoRuntime arduinoRuntime;
#endif

class GP2Y0A02 : public InfraredAnalogSensor
{
public:
#ifndef PLATFORM_AGNOSTIC_BUILD
    /**
     * Constructs a GP2Y0A02 sensor
     * @param pin   The analog pin receiving sensor signals
     */
    GP2Y0A02(uint8_t pin, Runtime& runtime = arduinoRuntime);
#else
    GP2Y0A02(uint8_t pin, Runtime& runtime);
#endif

    /* Check `DistanceSensor` interface for documentation */
    unsigned int getDistance() override;

private:
    const uint8_t kPin;
    Runtime& mRuntime;
};

/**
* \example GP2Y0A02.ino
* A basic example on how to get a distance measurement from a GP2Y0A02 infrared sensor.
*/
