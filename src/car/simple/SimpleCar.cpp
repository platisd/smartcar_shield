#include "SimpleCar.hpp"
#include "../../utilities/Utilities.hpp"

using namespace smartcarlib::constants::control;
using namespace smartcarlib::utils;

SimpleCar::SimpleCar(Control& control)
    : mControl{ control }
{
}

void SimpleCar::setSpeed(float speed)
{
    mControl.setSpeed(getConstrain(static_cast<int>(speed), kMinControlSpeed, kMaxControlSpeed));
}

void SimpleCar::setAngle(int angle)
{
    mControl.setAngle(getConstrain(angle, kMinControlAngle, kMaxControlAngle));
}

void SimpleCar::overrideMotorSpeed(int firstMotorSpeed, int secondMotorSpeed)
{
    mControl.overrideMotorSpeed(getConstrain(firstMotorSpeed, kMinControlSpeed, kMaxControlSpeed),
                                getConstrain(secondMotorSpeed, kMinControlSpeed, kMaxControlSpeed));
}
