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
     * Initializes the Odometer to receive pulses from the specified pin
     * @param  pin      The pin to receive pulses from, which **must** support
     *                  external hardware interrupts
     * @param  callback The function to be run on each incoming pulse. Please use the following as
     *                  an argument: `[](){yourOdometerInstance.update();}`
     * @return          `false` if an error occurred, i.e. invalid pin
     *
     * You **must** run this function before making any calls to your Odometer
     * or you will not get any usable distance or speed from it.
     *
     * **Example:**
     * \code
     * const unsigned short LEFT_ODOMETER_PIN = 2;
     * const unsigned long PULSES_PER_METER = 80;
     * DirectionlessOdometer leftOdometer(PULSES_PER_METER);
     *
     * void setup() {
     * #ifdef ESP_BOARD
     * leftOdometer.attach(RIGHT_ODOMETER_PIN, std::bind(&DirectionlessOdometer::update,
     * leftOdometer)); #else leftOdometer.attach(odometerPin, []() { leftOdometer.update(); });
     * #endif
     * }
     * \endcode
     */
    virtual bool attach(uint8_t pin, InterruptCallback callback) = 0;

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
    virtual bool isAttached() = 0;

    /**
     * Return whether the sensor is capable of inferring the direction of movement
     * @return `true` if the sensor supports direction readings otherwise `false`
     *
     * **Example:**
     * \code
     * bool directional = odometer.providesDirection();
     * \endcode
     */
    virtual bool providesDirection() = 0;
};
