#include <math.h>

#include "DirectionlessOdometer.hpp"

namespace
{
const int8_t kNotAnInterrupt         = -1;
const unsigned long kMinimumPulseGap = 700;
const float kMillisecondsInSecond    = 1000.0;
const float kMillimetersInMeter      = 1000.0;
} // namespace

using namespace smartcarlib::constants::odometer;

DirectionlessOdometer::DirectionlessOdometer(unsigned long pulsesPerMeter, Runtime& runtime)
    : mPulsesPerMeterRatio{ pulsesPerMeter > 0 ? pulsesPerMeter / 100.0f
                                               : kDefaultPulsesPerMeter / 100.0f }
    , mMillimetersPerPulse{ pulsesPerMeter > 0 ? static_cast<unsigned long>(
                                lroundf(kMillimetersInMeter / pulsesPerMeter))
                                               : static_cast<unsigned long>(lroundf(
                                                   kMillimetersInMeter / kDefaultPulsesPerMeter)) }
    , mRuntime(runtime)
    , kInput{ mRuntime.getInputState() }
    , kRisingEdge{ mRuntime.getRisingEdgeMode() }
    , mPin{ 0 }
    , mSensorAttached{ false }
    , mPulsesCounter{ 0 }
    , mPreviousPulse{ 0 }
    , mDt{ 0 }
{
}

bool DirectionlessOdometer::attach(uint8_t pin, void (*callback)(void))
{
    auto interruptPin = mRuntime.pinToInterrupt(pin);
    if (interruptPin == kNotAnInterrupt)
    {
        return false;
    }
    mRuntime.setPinDirection(pin, kInput);
    mPin            = interruptPin;
    mSensorAttached = true;

    mRuntime.setInterrupt(mPin, callback, kRisingEdge);

    return true;
}

long DirectionlessOdometer::getDistance()
{
    if (!isAttached())
    {
        return kNotAttachedError;
    }
    return mPulsesCounter / mPulsesPerMeterRatio;
}

float DirectionlessOdometer::getSpeed()
{
    if (!isAttached())
    {
        return kNotAttachedError;
    }
    // To get the current speed in m/sec, divide the meters per pulse (dx) with
    // the length between the last two pulses (dt)
    return mDt > 0 ? kMillisecondsInSecond * mMillimetersPerPulse / mDt : 0;
}

bool DirectionlessOdometer::isAttached()
{
    return mSensorAttached;
}

void DirectionlessOdometer::reset()
{
    mPulsesCounter = 0;
    mPreviousPulse = 0;
    mDt            = 0;
}

void DirectionlessOdometer::update()
{
    if (!isAttached())
    {
        return;
    }
    // Calculate the difference in time between the last two pulses (in microseconds)
    auto currentPulse = mRuntime.currentTimeMicros();
    auto dt           = currentPulse - mPreviousPulse;
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

bool DirectionlessOdometer::providesDirection()
{
    return false;
}
