#include "InfraredAnalogSensor.hpp"
#include "../../../../utilities/Utilities.hpp"

namespace
{
const unsigned long kMedianMeasurementDelay = 15;
}

using namespace smartcarlib::constants::distanceSensor;
using namespace smartcarlib::utils;

InfraredAnalogSensor::InfraredAnalogSensor(Runtime& runtime)
    : mRuntime(runtime)
{
}

unsigned int InfraredAnalogSensor::getMedianDistance(uint8_t iterations)
{
    if (iterations == 0 || iterations > kMaxMedianMeasurements)
    {
        return static_cast<unsigned int>(-1); // Return a large number to indicate error
    }

    unsigned int measurements[kMaxMedianMeasurements];
    for (auto i = 0; i < iterations; i++)
    {
        measurements[i] = getDistance();
        mRuntime.delayMillis(kMedianMeasurementDelay);
    }

    return getMedian(measurements, iterations);
}
