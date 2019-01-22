#include "DifferentialControl.hpp"

#include "../../utilities/Utilities.hpp"

using namespace smartcarlib::utils;
using namespace smartcarlib::constants::control;
using namespace smartcarlib::constants::motor;

DifferentialControl::DifferentialControl(Motor& leftMotor, Motor& rightMotor)
    : mLeftMotor(leftMotor)
    , mRightMotor(rightMotor)
    , mAngle{ 0 }
    , mSpeed{ 0 }
{
}

void DifferentialControl::setAngle(int angle)
{
    mAngle = getConstrain(angle, kMinControlAngle, kMaxControlAngle);
    setMotors();
}

void DifferentialControl::setSpeed(int speed)
{
    mSpeed = getConstrain(speed, kMinMotorSpeed, kMaxMotorSpeed);
    setMotors();
}

void DifferentialControl::setMotors()
{
    // With differential control, angle represents the ratio of speed between the two motors
    float ratio = (kMaxControlAngle - getAbsolute(mAngle)) / static_cast<float>(kMaxControlAngle);

    if (mAngle < kIdleControlAngle)
    {
        // Turning to the left (counter clockwise) by setting lower speed to the left motor
        mLeftMotor.setSpeed(static_cast<int>(mSpeed * ratio));
        mRightMotor.setSpeed(mSpeed);
    }
    else
    {
        // Turning to the right (clockwise) by setting lower speed to the right motor
        mLeftMotor.setSpeed(mSpeed);
        mRightMotor.setSpeed(static_cast<int>(mSpeed * ratio));
    }
}

void DifferentialControl::overrideMotorSpeed(int firstMotorSpeed, int secondMotorSpeed)
{
    firstMotorSpeed  = getConstrain(firstMotorSpeed, kMinMotorSpeed, kMaxMotorSpeed);
    secondMotorSpeed = getConstrain(secondMotorSpeed, kMinMotorSpeed, kMaxMotorSpeed);

    mLeftMotor.setSpeed(firstMotorSpeed);
    mRightMotor.setSpeed(secondMotorSpeed);
}
