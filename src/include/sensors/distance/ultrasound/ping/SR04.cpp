#include "SR04.hpp"
#include "../../Median.hpp"

namespace
{
const auto kTimeToTravelOneCmAndBack = 29.15 * 2; // In microseconds
const uint8_t kInput                 = 0;
const uint8_t kOutput                = 1;
const uint8_t kLow                   = 0;
const uint8_t kHigh                  = 1;
// We should wait long enough between two consecutive measurements
// so to avoid getting parasitic readings from old returning waves.
const unsigned long kMedianMeasurementDelay = 15; // In milliseconds
} // namespace

using namespace smartcarlib::constants::sr04;

SR04::SR04(Runtime& runtime, unsigned int maxDistance)
    : mRuntime{ runtime }
    , mTriggerPin{ 0 }
    , mEchoPin{ 0 }
    , mSensorAttached{ false }
    , kTimeout{ static_cast<unsigned long>((maxDistance > 0 ? maxDistance : kDefaultMaxDistance)
                                           * kTimeToTravelOneCmAndBack) }
{
}

void SR04::attach(uint8_t triggerPin, uint8_t echoPin)
{
    mSensorAttached = true;
    mTriggerPin     = triggerPin;
    mEchoPin        = echoPin;
    mRuntime.setPinDirection(mTriggerPin, kOutput);
    mRuntime.setPinDirection(mEchoPin, kInput);
}

unsigned int SR04::getDistance()
{
    if (!mSensorAttached)
    {
        return -1; // Return a large value
    }
    // Generate the pulse
    mRuntime.setPinState(mTriggerPin, kLow);
    mRuntime.delayMicros(5); // Set pin to known state
    mRuntime.setPinState(mTriggerPin, kHigh);
    mRuntime.delayMicros(10); // 10us pulse
    mRuntime.setPinState(mTriggerPin, kLow);
    // Wait for the pulse to arrive and measure its duration
    auto duration = mRuntime.getPulseDuration(mEchoPin, kHigh, kTimeout);
    // Calculate how much far out the object is
    return duration / kTimeToTravelOneCmAndBack;
}

unsigned int SR04::getMedianDistance(const uint8_t iterations)
{
    if (iterations == 0)
    {
        return -1; // Return a large number to indicate error
    }

    unsigned int measurements[iterations] = { 0 };
    for (auto i = 0; i < iterations; i++)
    {
        measurements[i] = getDistance();
        mRuntime.delayMillis(kMedianMeasurementDelay);
    }

    return Median::getMedian(measurements, iterations);
}
