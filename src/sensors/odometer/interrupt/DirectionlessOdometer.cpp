#include <math.h>

#include "DirectionlessOdometer.hpp"

namespace
{
const int8_t kNotAnInterrupt      = -1;
const float kMillisecondsInSecond = 1000.0;
const float kMillimetersInMeter   = 1000.0;
} // namespace

using namespace smartcarlib::constants::odometer;

DirectionlessOdometer::DirectionlessOdometer(Runtime& runtime,
                                             uint8_t pulsePin,
                                             InterruptCallback callback,
                                             unsigned long pulsesPerMeter)
    : mPulsesPerMeterRatio{ pulsesPerMeter > 0 ? static_cast<float>(pulsesPerMeter) / 100.0f
                                               : kDefaultPulsesPerMeter / 100.0f }
    , mMillimetersPerPulse{ pulsesPerMeter > 0
                                ? kMillimetersInMeter / static_cast<float>(pulsesPerMeter)
                                : kMillimetersInMeter / static_cast<float>(kDefaultPulsesPerMeter) }
    , mRuntime(runtime)
    , kSensorAttached{ mRuntime.pinToInterrupt(pulsePin) != kNotAnInterrupt }
{
    mRuntime.setPinDirection(pulsePin, mRuntime.getInputState());
    mRuntime.setInterrupt(static_cast<uint8_t>(mRuntime.pinToInterrupt(pulsePin)),
                          callback,
                          mRuntime.getRisingEdgeMode());
}

long DirectionlessOdometer::getDistance()
{
    return static_cast<long>(static_cast<float>(mPulsesCounter) / mPulsesPerMeterRatio);
}

float DirectionlessOdometer::getSpeed()
{
    // To get the current speed in m/sec, divide the meters per pulse (dx) with
    // the length between the last two pulses (dt)
    return mDt > 0 ? kMillisecondsInSecond * mMillimetersPerPulse / static_cast<float>(mDt) : 0.0f;
}

bool DirectionlessOdometer::isAttached() const
{
    return kSensorAttached;
}

void DirectionlessOdometer::reset()
{
    mPulsesCounter = 0;
    mPreviousPulse = 0;
    mDt            = 0;
}

void STORED_IN_RAM DirectionlessOdometer::update()
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
    // Unless this is the first time we are called then calculate the dT since
    // on the first time we cannot determine the speed yet
    if (mPreviousPulse != 0)
    {
        mDt = dt;
    }
    mPreviousPulse = currentPulse;
    mPulsesCounter++;
}

bool DirectionlessOdometer::providesDirection() const
{
    return false;
}
