#include "DirectionalOdometer.hpp"

namespace
{
const uint8_t kInput = 0;
}

DirectionalOdometer::DirectionalOdometer(uint8_t directionPin,
                                         uint8_t pinStateWhenForward,
                                         Runtime& runtime,
                                         unsigned long pulsesPerMeter)
    : Odometer(runtime, pulsesPerMeter)
    , mDirectionPin{ directionPin }
    , mPinStateWhenForward{ pinStateWhenForward }
    , mRuntime{ runtime }
    , mNegativePulsesCounter{ 0 }
{
}

bool DirectionalOdometer::attach(uint8_t pin, void (*callback)(void))
{
    Odometer::attach(pin, callback);
    mRuntime.setPinDirection(mDirectionPin, kInput);
}

void DirectionalOdometer::reset()
{
    Odometer::reset();
    mNegativePulsesCounter = 0;
}

void DirectionalOdometer::update()
{
    Odometer::update();
    if (mRuntime.getPinState(mDirectionPin) != mPinStateWhenForward)
    {
        mNegativePulsesCounter++;
    }
}

long DirectionalOdometer::getRelativeDistance()
{
    // Calculate the relative distance by subtracting twice the backward distance
    // from the absolute distance. Subtracting with double the backward distance
    // since it was included when calculating the absolute distance.
    return getDistance() - 2 * mNegativePulsesCounter / mPulsesPerMeterRatio;
}

int8_t DirectionalOdometer::getDirection()
{
    return mRuntime.getPinState(mDirectionPin) == mPinStateWhenForward
               ? smartcarlib::constants::odometer::kForward
               : smartcarlib::constants::odometer::kBackward;
}
