#include "BrushedMotor.hpp"
#include "../../../utilities/Utilities.hpp"

namespace
{
const int kMinPwm = 0;
const int kMaxPwm = 255;
} // namespace

using namespace smartcarlib::utils;
using namespace smartcarlib::constants::motor;

BrushedMotor::BrushedMotor(Runtime& runtime,
                           uint8_t forwardPin,
                           uint8_t backwardPin,
                           uint8_t enablePin)
    : kForwardPin{ forwardPin }
    , kBackwardPin{ backwardPin }
    , kEnablePin{ enablePin }
    , mRuntime(runtime)
    , kOutput{ mRuntime.getOutputState() }
    , kLow{ mRuntime.getLowState() }
    , kHigh{ mRuntime.getHighState() }
{
}

BrushedMotor::BrushedMotor(Runtime& runtime, BrushedMotorPins pins)
    : BrushedMotor{ runtime, pins.forward, pins.backward, pins.enable }
{
}

void BrushedMotor::setSpeed(int speed)
{
    if (!mAttached)
    {
        attach();
    }

    if (speed < 0)
    {
        // Set backward direction
        mRuntime.setPinState(kForwardPin, kLow);
        mRuntime.setPinState(kBackwardPin, kHigh);
    }
    else if (speed > 0)
    {
        // Set forward direction
        mRuntime.setPinState(kForwardPin, kHigh);
        mRuntime.setPinState(kBackwardPin, kLow);
    }
    else
    {
        // Set idle
        mRuntime.setPinState(kForwardPin, kLow);
        mRuntime.setPinState(kBackwardPin, kLow);
    }

    // Get the absolute speed within the [0, 100] range
    auto absoluteSpeed = getAbsolute(getConstrain(speed, kMinMotorSpeed, kMaxMotorSpeed));
    // Set the PWM within the [0,255] range
    mRuntime.setPWM(kEnablePin,
                    getMap(absoluteSpeed, kIdleMotorSpeed, kMaxMotorSpeed, kMinPwm, kMaxPwm));
}

void BrushedMotor::attach()
{
    mRuntime.setPinDirection(kForwardPin, kOutput);
    mRuntime.setPinDirection(kBackwardPin, kOutput);
    mRuntime.setPinDirection(kEnablePin, kOutput);
    mAttached = true;
}
