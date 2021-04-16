/**
 * \interface DistanceSensor
 * A parent class for all sensors that conduct distance measurements.
 */
#pragma once

#include <stdint.h> // NOLINT(modernize-deprecated-headers)

namespace smartcarlib
{
namespace constants
{
namespace distanceSensor
{
const int kMaxMedianMeasurements = 100;
}
} // namespace constants
} // namespace smartcarlib

class DistanceSensor
{
public:
    virtual ~DistanceSensor() = default;

    /**
     * @brief Gets the distance measured by the sensor in centimeters. Calling this might trigger a
     * new measurement by the sensor.
     *
     * @return `0` if an error has occured, otherwise the measured distance in centimeters
     *
     * **Example:**
     * \code
     * unsigned int distance = sensor.getDistance();
     * \endcode
     */
    virtual unsigned int getDistance() = 0;

    /**
     * @brief Gets the median distance from the specified number of measurements.
     *
     * @param iterations Number of measurements to conduct (at most `kMaxMedianMeasurements`)
     * @return The median of the conducted measurements or an error value if the number of
     * iterations is `0` or larger than `kMaxMedianMeasurements`
     *
     * **Example:**
     * \code
     * // Get the median of `10` measurements
     * unsigned int distance = sensor.getMedianDistance(10);
     * \code
     */
    virtual unsigned int getMedianDistance(uint8_t iterations) = 0;
};
