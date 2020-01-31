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
    , mDirectionPinState{ pinStateWhenForward }
{
}

bool DirectionalOdometer::attach(uint8_t pin, InterruptCallback callback)
{
    auto success = DirectionlessOdometer::attach(pin, callback);
    if (success)
    {
        mRuntime.setPinDirection(mDirectionPin, mRuntime.getInputState());
    }

    return success;
}

void DirectionalOdometer::reset()
{
    DirectionlessOdometer::reset();
    mNegativePulsesCounter = 0;
}

void STORED_IN_RAM DirectionalOdometer::update()
{
    mDirectionPinState = mRuntime.getPinState(mDirectionPin);
    DirectionlessOdometer::update();
    if (mDirectionPinState != mPinStateWhenForward)
    {
        mNegativePulsesCounter++;
    }
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
    return mDirectionPinState == mPinStateWhenForward ? smartcarlib::constants::odometer::kForward
                                                      : smartcarlib::constants::odometer::kBackward;
}

bool DirectionalOdometer::providesDirection()
{
    return true;
}
