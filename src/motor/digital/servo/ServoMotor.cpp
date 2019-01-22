#include "ServoMotor.hpp"
#include "../../../utilities/Utilities.hpp"

using namespace smartcarlib::utils;
using namespace smartcarlib::constants::motor;

ServoMotor::ServoMotor(uint8_t controlPin,
                       int minPulseLength,
                       int idlePulseLength,
                       int maxPulseLength,
                       Runtime& runtime)
    : kControlPin{ controlPin }
    , kMinPulseLength{ minPulseLength }
    , kIdlePulseLength{ idlePulseLength }
    , kMaxPulseLength{ maxPulseLength }
    , mRuntime(runtime)
    , mAttached{ false }
{
}

void ServoMotor::setSpeed(int speed)
{
    if (!mAttached)
    {
        Servo::attach(kControlPin, kMinPulseLength, kMaxPulseLength);
        mAttached = true;
    }

    speed = getConstrain(speed, kMinMotorSpeed, kMaxMotorSpeed);
    // The pulses duration might not be symmetrically distributed around the
    // idle speed so we need to split the mapping between the positive and the
    // negative values
    if (speed < kIdleMotorSpeed)
    {
        Servo::writeMicroseconds(
            getMap(speed, kMinMotorSpeed, kIdleMotorSpeed, kMinPulseLength, kIdlePulseLength));
    }
    else
    {
        Servo::writeMicroseconds(
            getMap(speed, kIdleMotorSpeed, kMaxMotorSpeed, kIdlePulseLength, kMaxPulseLength));
    }
}
