#include <math.h>

#include "Odometer.hpp"

namespace
{
const int8_t kNotAnInterrupt         = -1;
const uint8_t kRisingEdge            = 1;
const uint8_t kInput                 = 0;
const unsigned long kMinimumPulseGap = 700;
} // namespace

using namespace smartcarlib::constants::odometry;

Odometer::Odometer(unsigned long pulsesPerMeter, Runtime& runtime)
    : mPulsesPerMeterRatio{ pulsesPerMeter > 0 ? pulsesPerMeter / 100.0
                                               : kDefaultPulsesPerMeter / 100.0 }
    , mMillimetersPerPulse{ pulsesPerMeter > 0 ? lroundf(1000.0 / pulsesPerMeter)
                                               : lroundf(1000.0 / kDefaultPulsesPerMeter) }
    , mRuntime{ runtime }
    , mPin{ 0 }
    , mSensorAttached{ false }
    , mPulsesCounter{ 0 }
    , mPreviousPulse{ 0 }
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

    //    auto isr = [this](){updateDtAndPulses();};
    mRuntime.setInterrupt(mPin, callback, kRisingEdge);

    return true;
}

unsigned long Odometer::getDistance()
{
    if (!mSensorAttached)
    {
        return 0;
    }
    return mPulsesCounter / mPulsesPerMeterRatio;
}

float Odometer::getSpeed()
{
    if (!mSensorAttached)
    {
        return 0;
    }
    // To get the current speed divide the meters per pulse (dx) with the length
    // between the last two pulses (dt)
    return mDt > 0 ? 1000.0 * mMillimetersPerPulse / mDt : 0;
}

void Odometer::reset()
{
    if (!mSensorAttached)
    {
        return;
    }
    mPulsesCounter = 0;
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
    if (dt < kMinimumPulseGap)
    {
        // Less than the minimum acceptable time between two pulses is an unstable signal
        return;
    }
    mDt            = dt;
    mPreviousPulse = currentPulse;
    mPulsesCounter++;
}
