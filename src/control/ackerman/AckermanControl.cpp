#include "AckermanControl.hpp"

#include "../../utilities/Utilities.hpp"

using namespace smartcarlib::utils;
using namespace smartcarlib::constants::control;
using namespace smartcarlib::constants::motor;

AckermanControl::AckermanControl(Motor& steering, Motor& throttling)
    : mSteering{ steering }
    , mThrottling{ throttling }
{
}

void AckermanControl::setAngle(int angle)
{
    auto speed = getMap(getConstrain(angle, kMinControlAngle, kMaxControlAngle),
                        kMinControlAngle,
                        kMaxControlAngle,
                        kMinMotorSpeed,
                        kMaxMotorSpeed);
    mSteering.setSpeed(speed);
}

void AckermanControl::setSpeed(int speed)
{
    speed = getConstrain(speed, kMinMotorSpeed, kMaxMotorSpeed);
    mThrottling.setSpeed(speed);
}

void AckermanControl::overrideMotorSpeed(int firstMotorSpeed, int secondMotorSpeed)
{
    firstMotorSpeed  = getConstrain(firstMotorSpeed, kMinMotorSpeed, kMaxMotorSpeed);
    secondMotorSpeed = getConstrain(secondMotorSpeed, kMinMotorSpeed, kMaxMotorSpeed);

    mSteering.setSpeed(firstMotorSpeed);
    mThrottling.setSpeed(secondMotorSpeed);
}
