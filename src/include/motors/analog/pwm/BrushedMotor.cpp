#include "BrushedMotor.hpp"
#include "../../../utilities/Utilities.hpp"

namespace
{
const uint8_t kOutput       = 1;
const uint8_t kLow          = 0;
const uint8_t kHigh         = 1;
const int kMinSpeed         = -100;
const int kMaxSpeed         = 100;
const int kMinAbsoluteSpeed = 0;
const int kMinPwm       = 0;
const int kMaxPwm       = 255;
} // namespace

using namespace smartcarlib::utils;

BrushedMotor::BrushedMotor(uint8_t forwardPin,
                           uint8_t backwardPin,
                           uint8_t enablePin,
                           Runtime& runtime)
    : kForwardPin{ forwardPin }
    , kBackwardPin{ backwardPin }
    , kEnablePin{ enablePin }
    , mRuntime{ runtime }
    , mAttached{ false }
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
    else
    {
        // Set forward direction
        mRuntime.setPinState(kForwardPin, kHigh);
        mRuntime.setPinState(kBackwardPin, kLow);
    }

    // Get the absolute speed within the [0, 100] range
    auto absoluteSpeed = getAbsolute(getConstrain(speed, kMinSpeed, kMaxSpeed));
    // Set the PWM within the [0,255] range
    mRuntime.setPWM(kEnablePin,
                    getMap(absoluteSpeed, kMinAbsoluteSpeed, kMaxSpeed, kMinPwm, kMaxPwm));
}

void BrushedMotor::attach()
{
    mRuntime.setPinDirection(kForwardPin, kOutput);
    mRuntime.setPinDirection(kBackwardPin, kOutput);
    mRuntime.setPinDirection(kEnablePin, kOutput);
    mAttached = true;
}
