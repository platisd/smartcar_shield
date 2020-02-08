#include "DirectionalOdometer.hpp"

using namespace smartcarlib::constants::odometer;

DirectionalOdometer::DirectionalOdometer(uint8_t pin,
                                         InterruptCallback callback,
                                         uint8_t directionPin,
                                         int pinStateWhenForward,
                                         unsigned long pulsesPerMeter,
                                         Runtime& runtime)
    : DirectionlessOdometer(pin, callback, pulsesPerMeter, runtime)
    , mDirectionPin{ directionPin }
    , mPinStateWhenForward{ pinStateWhenForward }
    , mRuntime(runtime)
    , mDirectionPinState{ mRuntime.getPinState(mDirectionPin) }
    , mPreviousDirectionPinState{ mDirectionPinState }
{
    mRuntime.setPinDirection(mDirectionPin, mRuntime.getInputState());
}

void DirectionalOdometer::reset()
{
    DirectionlessOdometer::reset();
    mNegativePulsesCounter = 0;
}

void STORED_IN_RAM DirectionalOdometer::update()
{
    const auto currentDirectionPinState = mRuntime.getPinState(mDirectionPin);
    DirectionlessOdometer::update();
    if (currentDirectionPinState != mPreviousDirectionPinState)
    {
        if (currentDirectionPinState == mPinStateWhenForward)
        {
            mNegativePulsesCounter++;
        }
    }
    else
    {
        mDirectionPinState = currentDirectionPinState;
        if (currentDirectionPinState != mPinStateWhenForward)
        {
            mNegativePulsesCounter++;
        }
    }
    mPreviousDirectionPinState = currentDirectionPinState;
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

bool DirectionalOdometer::providesDirection() const
{
    return true;
}
