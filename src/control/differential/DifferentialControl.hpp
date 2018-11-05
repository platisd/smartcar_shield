/**
 * Class to represent controling a vehicle by applying different speed on each
 * side, like a tank.
 */
#pragma once

#include "../../motor/Motor.hpp"
#include "../Control.hpp"

class DifferentialControl : public Control
{
public:
    /**
     * Constructs an Ackerman way of controlling the vehicle
     * @param steering   The motor that controls the steering
     * @param throttling The motor that controls the throttling
     */
    DifferentialControl(Motor& leftMotor, Motor& rightMotor);

    /* Check `Control` interface for documentation */
    void setAngle(int angle) override;

    /* Check `Control` interface for documentation */
    void setSpeed(int speed) override;

    /* Check `Control` interface for documentation */
    virtual void overrideMotorSpeed(int firstMotorSpeed, int secondMotorSpeed) override;

private:
    void setMotors();
    Motor& mLeftMotor;
    Motor& mRightMotor;
    int mAngle;
    int mSpeed;
};
