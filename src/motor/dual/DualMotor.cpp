#include "DualMotor.hpp"

DualMotor::DualMotor(Motor& motor1, Motor& motor2)
    : mMotor1{ motor1 }
    , mMotor2{ motor2 }
{
}

void DualMotor::setSpeed(int speed)
{
    mMotor1.setSpeed(speed);
    mMotor2.setSpeed(speed);
}
