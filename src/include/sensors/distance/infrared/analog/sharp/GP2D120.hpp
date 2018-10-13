/**
 * Programmatically represents a SHARP GP2D120 infrared sensor.
 */
#pragma once

#include "../../../../../runtime/Runtime.hpp"
#include "../InfraredAnalogSensor.hpp"

#ifndef PLATFORM_AGNOSTIC_BUILD
#include "../../../../../runtime/arduino_runtime/ArduinoRuntime.hpp"
extern ArduinoRuntime arduinoRuntime;
#endif

class GP2D120 : public InfraredAnalogSensor
{
public:
    #ifndef PLATFORM_AGNOSTIC_BUILD
        GP2D120(Runtime& runtime = arduinoRuntime);
    #else
        GP2D120(Runtime& runtime);
    #endif
    unsigned int getDistance() override;

private:
    Runtime& mRuntime;
};
