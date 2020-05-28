#include "DirectionalOdometer.hpp"
#include <limits.h>

using namespace smartcarlib::constants::odometer;

namespace
{
const auto kInvalidPinState = INT_MIN;
}

DirectionalOdometer::DirectionalOdometer(uint8_t pulsePin,
                                         uint8_t forwardWhenLowPin,
                                         InterruptCallback callback,
                                         unsigned long pulsesPerMeter,
                                         Runtime& runtime)
    : DirectionlessOdometer(pulsePin, callback, pulsesPerMeter, runtime)
    , mDirectionPin{ forwardWhenLowPin }
    , mRuntime(runtime)
    , kPinStateWhenForward{ mRuntime.getLowState() }
    , mDirectionPinState{ kInvalidPinState }
{
    mRuntime.setPinDirection(mDirectionPin, mRuntime.getInputState());
}

DirectionalOdometer::DirectionalOdometer(DirectionalOdometerPins pins,
                                         InterruptCallback callback,
                                         unsigned long pulsesPerMeter,
                                         Runtime& runtime)
    : DirectionalOdometer(pins.pulse, pins.direction, callback, pulsesPerMeter, runtime)
{
}

void DirectionalOdometer::reset()
{
    DirectionlessOdometer::reset();
    mNegativePulsesCounter = 0;
}

void STORED_IN_RAM DirectionalOdometer::update()
{
    // Calculate the difference in time between the last two pulses (in microseconds)
    const auto currentPulse = mRuntime.currentTimeMicros();
    const auto dt           = currentPulse - mPreviousPulse;
    // Unless this is the first time we are called, if two pulses are too close
    // then the signal is noisy and they should be ignored
    if (mPreviousPulse != 0 && dt < kMinimumPulseGap)
    {
        return;
    }

    mDirectionPinState = mRuntime.getPinState(mDirectionPin);

    // Unless this is the first time we are called then calculate the dT since
    // on the first time we cannot determine the speed yet
    if (mPreviousPulse != 0)
    {
        mDt = dt;
    }
    mPreviousPulse = currentPulse;

    if (mDirectionPinState != kPinStateWhenForward)
    {
        mNegativePulsesCounter++;
    }
    else
    {
        mPulsesCounter++;
    }
}

long DirectionalOdometer::getDistance()
{
    return DirectionlessOdometer::getDistance()
           - static_cast<long>(static_cast<float>(mNegativePulsesCounter) / mPulsesPerMeterRatio);
}

float DirectionalOdometer::getSpeed()
{
    return DirectionlessOdometer::getSpeed() * getDirection();
}

int8_t DirectionalOdometer::getDirection() const
{
    return mDirectionPinState == kPinStateWhenForward ? smartcarlib::constants::odometer::kForward
                                                      : smartcarlib::constants::odometer::kBackward;
}

bool DirectionalOdometer::providesDirection() const
{
    return true;
}
