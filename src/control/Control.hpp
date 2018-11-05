/**
 * Interface to generalize all classes that can provide a way to control a Car
 */
#pragma once

namespace smartcarlib
{
namespace constants
{
namespace control
{
const int kMinControlAngle  = -90;
const int kIdleControlAngle = 0;
const int kMaxControlAngle  = 90;
} // namespace control
} // namespace constants
} // namespace smartcarlib

class Control
{
public:
    virtual ~Control() = default;

    /**
     * Sets the driving angle in degrees [-90, 90]
     * @param angle The driving angle
     */
    virtual void setAngle(int angle) = 0;

    /**
     * Sets the driving speed as percentage of the total motor speed where
     * the sign represents the direction of movement [-100, 100]
     * @param speed The driving speed
     */
    virtual void setSpeed(int speed) = 0;

    /**
     * Set the motor speed individually as a percentage of the motors` total
     * power. Use this with caution.
     * @param firstMotorSpeed  The first motor speed [-100, 100]
     * @param secondMotorSpeed The second motor speed [-100, 100]
     */
    virtual void overrideMotorSpeed(int firstMotorSpeed, int secondMotorSpeed) = 0;
};
