#include "HeadingCar.hpp"

HeadingCar::HeadingCar(Control& control, HeadingSensor& headingSensor)
    : mHeadingSensor{ headingSensor }
    , SimpleCar(control)
{
}

unsigned int HeadingCar::getHeading()
{
    return mHeadingSensor.getHeading();
}

void HeadingCar::update()
{
    mHeadingSensor.update();
}
