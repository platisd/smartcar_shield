#include "HeadingCar.hpp"

HeadingCar::HeadingCar(Control& control, HeadingSensor& headingSensor)
    : SimpleCar(control)
    , mHeadingSensor{ headingSensor }
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
