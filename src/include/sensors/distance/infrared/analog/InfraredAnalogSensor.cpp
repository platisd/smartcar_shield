#include "InfraredAnalogSensor.hpp"
#include "../../Median.hpp"

namespace
{
const unsigned long kMedianMeasurementDelay = 15;
}

InfraredAnalogSensor::InfraredAnalogSensor(Runtime& runtime)
    : mPin{ 0 }
    , mSensorAttached{ false }
    , mRuntime {runtime}
{
}

void InfraredAnalogSensor::attach(uint8_t pin)
{
    mSensorAttached = true;
    mPin            = pin;
}

unsigned int InfraredAnalogSensor::getMedianDistance(uint8_t iterations)
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
