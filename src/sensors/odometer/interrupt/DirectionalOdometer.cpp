#include "DirectionalOdometer.hpp"

using namespace smartcarlib::constants::odometer;

DirectionalOdometer::DirectionalOdometer(uint8_t directionPin,
                                         int pinStateWhenForward,
                                         unsigned long pulsesPerMeter,
                                         Runtime& runtime)
    : DirectionlessOdometer(pulsesPerMeter, runtime)
    , mDirectionPin{ directionPin }
    , mPinStateWhenForward{ pinStateWhenForward }
    , mRuntime(runtime)
    , kInput{ mRuntime.getInputState() }
{
}

bool DirectionalOdometer::attach(uint8_t pin, void (*callback)(void))
{
    auto success = DirectionlessOdometer::attach(pin, callback);
    if (success)
    {
        mRuntime.setPinDirection(mDirectionPin, kInput);
    }

    return success;
}

void DirectionalOdometer::reset()
{
    DirectionlessOdometer::reset();
    mNegativePulsesCounter = 0;
}

void DirectionalOdometer::update()
{
    if (!isAttached())
    {
        return;
    }

    DirectionlessOdometer::update();
    const auto directionPinState = mRuntime.getPinState(mDirectionPin);
    if (directionPinState != mPinStateWhenForward)
    {
        mNegativePulsesCounter++;
    }
    mDirection = directionPinState;
}

long DirectionalOdometer::getDistance()
{
    if (!isAttached())
    {
        return kNotAttachedError;
    }
    // Calculate the relative distance by subtracting twice the backward distance
    // from the absolute distance. Subtracting with double the backward distance
    // since it was included when calculating the absolute distance.
    return DirectionlessOdometer::getDistance() - 2 * mNegativePulsesCounter / mPulsesPerMeterRatio;
}

float DirectionalOdometer::getSpeed()
{
    if (!isAttached())
    {
        return kNotAttachedError;
    }

    return DirectionlessOdometer::getSpeed() * getDirection();
}

int8_t DirectionalOdometer::getDirection() const
{
    return mDirection == mPinStateWhenForward ? smartcarlib::constants::odometer::kForward
                                              : smartcarlib::constants::odometer::kBackward;
}

bool DirectionalOdometer::providesDirection()
{
    return true;
}
