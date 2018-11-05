/**
 * An class to programmatically represent a vehicle equipped with odometers
 */
#pragma once

#include "../../sensors/odometer/Odometer.hpp"
#include "../simple/SimpleCar.hpp"

namespace smartcarlib
{
namespace constants
{
namespace car
{
const unsigned long kDefaultPidFrequency = 80;
const float kDefaultProportional         = 5.0f;
const float kDefaultIntegral             = 0.0f;
const float kDefaultDerivative           = 10.0f;
} // namespace car
} // namespace constants
} // namespace smartcarlib

class DistanceCar : public SimpleCar
{
public:
    /**
     * Constructs a car equipped with a distance sensor
     * @param control  The car's control
     * @param odometer The odometer
     */
    DistanceCar(Control& control, Odometer& odometer);

    /**
     * Constructs a car equipped with a distance sensor
     * @param control        The car's control
     * @param odometerLeft  The left odometer
     * @param odometerRight The right odometer
     */
    DistanceCar(Control& control, Odometer& odometerLeft, Odometer& odometerRight);

    /**
     * Gets the car's travelled distance
     * @return The car's travelled distance
     */
    long getDistance();

    /**
     * Sets the travelled distance to `0`
     */
    void resetDistance();

    /**
     * Sets the car's speed in meters per second if cruise control is enabled
     * otherwise as a percentage of the motor speed. Sign in both cases determines
     * direction.
     * @param speed The car's speed
     */
    virtual void setSpeed(float speed) override;

    /**
     * Gets the car's current speed in meters per second
     * @return The car's speed
     */
    float getSpeed();

    /**
     * Enables the car to move with a stable speed using the odometers
     * @param proportional The proportional value of the PID controller
     * @param integral     The integral value of the PID controller
     * @param derivative   The derivative value of the PID controller
     * @param frequency    How often to adjust the speed using `update()`
     */
    void enableCruiseControl(float proportional = smartcarlib::constants::car::kDefaultProportional,
                             float integral     = smartcarlib::constants::car::kDefaultIntegral,
                             float derivative   = smartcarlib::constants::car::kDefaultDerivative,
                             unsigned long frequency
                             = smartcarlib::constants::car::kDefaultPidFrequency);

    /**
     * Adjusts the cruise control speed. You must have this being executed as often
     * as possible when having cruise control enabled. When cruise control is not
     * enabled this has no effect.
     */
    virtual void update();

    /**
     * Disable cruise control.
     */
    void disableCruiseControl();

    /**
     * Make the car travel a specific distance in centimeters where the sign
     * indicates direction.
     * @param distance The distance to travel in centimeters
     */
    void go(int distance);
};
