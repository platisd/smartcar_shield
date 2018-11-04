/**
 * A brushed motor is controlled via two wires where the flow of current determines
 * the direction of rotation and the duty cycle the speed.
 *
 * For their control, we will assume the existince of a half-bridge such as the
 * L293D chip where three signals are needed, two for determining the direction
 * and another for determining the speed.
 */
#pragma once

#include <stdint.h>

#include "../../../runtime/Runtime.hpp"
#include "../../Motor.hpp"

#ifndef PLATFORM_AGNOSTIC_BUILD
#include "../../../runtime/arduino_runtime/ArduinoRuntime.hpp"
extern ArduinoRuntime arduinoRuntime;
#endif

class BrushedMotor : public Motor
{
public:
#ifndef PLATFORM_AGNOSTIC_BUILD
    /**
     * Constructs a brushed DC motor instance
     * @param forwardPin  The direction pin that when set to HIGH makes the motor spin forward
     * @param backwardPin The direction pin that when set to HIGH makes the motor spin forward
     * @param enablePin   The pin  that controls the motor's speed
     */
    BrushedMotor(uint8_t forwardPin,
                 uint8_t backwardPin,
                 uint8_t enablePin,
                 Runtime& runtime = arduinoRuntime);
#else
    BrushedMotor(uint8_t forwardPin, uint8_t backwardPin, uint8_t enablePin, Runtime& runtime);
#endif

/* Check `Motor` interface for documentation */
void setSpeed(int speed) override;

private:
    void attach();
    const uint8_t kForwardPin;
    const uint8_t kBackwardPin;
    const uint8_t kEnablePin;
    Runtime& mRuntime;
    bool mAttached;
};
