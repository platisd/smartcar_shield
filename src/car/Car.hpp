/**
 * An interface to programmatically represent any kind of vehicle
 */
#pragma once

class Car
{
public:
    virtual ~Car() = default;

    /**
     * Sets the car's driving speed
     * @param speed The car's speed
     */
    virtual void setSpeed(float speed) = 0;

    /**
     * Set the car's driving angle
     * @param angle The car's angle in degrees [-90, 90]
     */
    virtual void setAngle(int angle) = 0;

    /**
     * Set the motor speed individually as a percentage of the motors` total
     * power. Use this with caution.
     * @param firstMotorSpeed  The speed of the motor passed as first argument
     *                         argument to the car's control class [-100, 100]
     * @param secondMotorSpeed The speed of the motor passed as second argument
     *                         argument to the car's control class [-100, 100]
     */
    virtual void overrideMotorSpeed(int firstMotorSpeed, int secondMotorSpeed) = 0;
};
