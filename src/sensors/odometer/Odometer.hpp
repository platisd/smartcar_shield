/**
 * \interface Odometer
 * An interface to programmatically represent all odometers
 */
#pragma once

#include <stdint.h>

#include "../../runtime/InterruptCallback.hpp"

namespace smartcarlib
{
namespace constants
{
namespace odometer
{
const int8_t kForward                      = 1;
const int8_t kBackward                     = -1;
const int kNotAttachedError                = -1000;
const unsigned long kDefaultPulsesPerMeter = 100; // 1:1 cm to pulses ratio
} // namespace odometer
} // namespace constants
} // namespace smartcarlib

class Odometer
{
public:
    /**
     * Returns the travelled distance in centimeters where sign can indicate
     * direction if there is hardware support
     * @return The travelled distance in centimeters
     *
     * **Example:**
     * \code
     * unsigned long travelledDistance = odometer.getDistance();
     * \endcode
     */
    virtual long getDistance() = 0;

    /**
     * Returns the current speed in meters/sec where sign can indicate
     * direction if there is hardware support
     * @return The speed in meters/sec
     *
     * **Example:**
     * \code
     * float speed = odometer.getSpeed();
     * \endcode
     */
    virtual float getSpeed() = 0;

    /**
     * Returns whether the sensor has been properly attached
     * @return True if Odometer::attach has been successfully run,
     *         false otherwise
     * **Example:**
     * \code
     * bool hasAttachBeenRun = odometer.isAttached();
     * \endcode
     */
    virtual bool isAttached() const = 0;

    /**
     * Return whether the sensor is capable of inferring the direction of movement
     * @return `true` if the sensor supports direction readings otherwise `false`
     *
     * **Example:**
     * \code
     * bool directional = odometer.providesDirection();
     * \endcode
     */
    virtual bool providesDirection() const = 0;
};
