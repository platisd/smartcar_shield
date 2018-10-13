/**
 * A parent class for all sensors that conduct distance measurements.
 */
#pragma once

#include <stdint.h>

class DistanceSensor
{
public:
    virtual ~DistanceSensor() = default;

    /**
     * Gets the distance measured by the sensor. Calling this might trigger
     * a new measurement by the sensor.
     * @return `0` if an error has occured, otherwise the measured distance
     */
    virtual unsigned int getDistance() = 0;

    /**
     * Gets the median distance from the specified number of measurements.
     * @param  iterations Number of measurements to conduct
     * @return            The median of the conducted measurements
     */
    virtual unsigned int getMedianDistance(uint8_t iterations) = 0;
};
