#include "SimpleCar.hpp"

SimpleCar::SimpleCar(Control& control) : mControl{control}
{
}

void SimpleCar::setSpeed(float speed)
{
    mControl.setSpeed(static_cast<int>(speed));
}

void SimpleCar::setAngle(int angle)
{
    mControl.setAngle(angle);
}

void SimpleCar::overrideMotorSpeed(int firstMotorSpeed, int secondMotorSpeed)
{
    mControl.overrideMotorSpeed(firstMotorSpeed, secondMotorSpeed);
}
