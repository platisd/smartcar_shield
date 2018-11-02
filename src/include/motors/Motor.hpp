/**
 * Interface programmatically representing any kind of motor.
 * Fundamentally, a motor is something you can control the speed and direction
 * of its rotation.
 */
#pragma once

namespace smartcarlib
{
namespace constants
{
namespace motor
{
const int kMinMotorSpeed = -100;
const int kIdleMotorSpeed = 0;
const int kMaxMotorSpeed = 100;
} // namespace motor
} // namespace constants
} // namespace smartcarlib

class Motor
{
public:
    virtual ~Motor() = default;

    /**
     * Sets the motor speed and direction as the percentage of the maximum
     * possible speed, where the sign of the argument represents the direction.
     *
     * @param speed The speed and direction within the range of [-100, 100].
     *              `-100` means that the motor is spinning at full speed
     *              towards the direction we have defined as backward.
     *              `100` interpreted as full speed forward.
     *              `0` means that the motor is at its idle/neutral state.
     */
    virtual void setSpeed(int speed) = 0;
};
