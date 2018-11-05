/**
 * An interface to programmatically represent all odometers
 */
#pragma once

namespace smartcarlib
{
namespace constants
{
namespace odometer
{
const int8_t kForward  = 1;
const int8_t kBackward = -1;
const int kNotAttachedError = -1000;
const unsigned long kDefaultPulsesPerMeter = 100; // 1:1 cm to pulses ratio
} // namespace odometer
} // namespace constants
} // namespace smartcarlib

class Odometer
{
    /**
     * Returns the travelled distance in centimeters where sign can indicate
     * direction if there is hardware support
     * @return The travelled distance in centimeters
     */
    virtual long getDistance() = 0;

    /**
     * Returns the current speed in meters/sec where sign can indicate
     * direction if there is hardware support
     * @return The speed in meters/sec
     */
    virtual float getSpeed() = 0;

    /**
     * Returns whether the sensor has been properly attached
     * @return True if the sensor has been attached successfully false otherwise
     */
    virtual bool isAttached() = 0;
};
