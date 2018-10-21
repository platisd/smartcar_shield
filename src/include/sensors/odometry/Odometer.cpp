#include <math.h>

#include "Odometer.hpp"

namespace
{
const int8_t kNotAnInterrupt         = -1;
const uint8_t kRisingEdge            = 1;
const uint8_t kInput                 = 0;
const unsigned long kMinimumPulseGap = 700;
const float kMillisecondsInSecond    = 1000.0;
const float kMillimetersInMeter      = 1000.0;
} // namespace

using namespace smartcarlib::constants::odometry;

Odometer::Odometer(Runtime& runtime, unsigned long pulsesPerMeter)
    : mPulsesPerMeterRatio{ pulsesPerMeter > 0 ? pulsesPerMeter / 100.0f
                                               : kDefaultPulsesPerMeter / 100.0f }
    , mMillimetersPerPulse{ pulsesPerMeter > 0
                                ? static_cast<unsigned long>(
                                      lroundf(kMillimetersInMeter / pulsesPerMeter))
                                : static_cast<unsigned long>(
                                      lroundf(kMillimetersInMeter / kDefaultPulsesPerMeter)) }
    , mRuntime{ runtime }
    , mPin{ 0 }
    , mSensorAttached{ false }
    , mPulsesCounter{ 0 }
    , mPreviousPulse{ 0 }
    , mDt{ 0 }
{
}

bool Odometer::attach(uint8_t pin, void (*callback)(void))
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

unsigned long Odometer::getDistance()
{
    if (!mSensorAttached)
    {
        return -1;
    }
    return mPulsesCounter / mPulsesPerMeterRatio;
}

float Odometer::getSpeed()
{
    if (!mSensorAttached)
    {
        return 0;
    }
    // To get the current speed in m/sec, divide the meters per pulse (dx) with
    // the length between the last two pulses (dt)
    return mDt > 0 ? kMillisecondsInSecond * mMillimetersPerPulse / mDt : 0;
}

void Odometer::reset()
{
    mPulsesCounter = 0;
    mPreviousPulse = 0;
    mDt            = 0;
}

void Odometer::update()
{
    if (!mSensorAttached)
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
