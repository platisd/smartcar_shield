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
        GP2Y0A02(Runtime& runtime = arduinoRuntime);
    #else
        GP2Y0A02(Runtime& runtime);
    #endif
    unsigned int getDistance() override;

private:
    Runtime& mRuntime;
};
