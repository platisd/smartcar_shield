/**
 * \class AckermanControl
 * Class to represent controling a vehicle by changing the angle of the front
 * wheels, like a common car.
 */
#pragma once

#include "../Control.hpp"
#include "../../motor/Motor.hpp"

class AckermanControl : public Control
{
public:
    /**
     * Constructs an Ackerman way of controlling the vehicle
     * @param steering   The motor that controls the steering
     * @param throttling The motor that controls the throttling
     *
     * **Example:**
     * \code
     * BrushedMotor motor(8, 10, 9);
     * ServoMotor steeringWheel(7);
     * AckermanControl control(steeringWheel, motor);
     * \endcode
     */
    AckermanControl(Motor& steering, Motor& throttling);

    /* Check `Control` interface for documentation */
    void setAngle(int angle) override;

    /* Check `Control` interface for documentation */
    void setSpeed(int speed) override;

    /* Check `Control` interface for documentation */
    virtual void overrideMotorSpeed(int firstMotorSpeed, int secondMotorSpeed) override;

private:
    Motor& mSteering;
    Motor& mThrottling;
};
