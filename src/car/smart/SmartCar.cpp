#include "SmartCar.hpp"

SmartCar::SmartCar(Control& control,
                   HeadingSensor& headingSensor,
                   Odometer& odometer,
                   Runtime& runtime)
    : HeadingCar::HeadingCar(control, headingSensor)
    , DistanceCar::DistanceCar(control, odometer, runtime)
    , SimpleCar::SimpleCar(control)
{
}

SmartCar::SmartCar(Control& control,
                   HeadingSensor& headingSensor,
                   Odometer& odometerleft,
                   Odometer& odometerRight,
                   Runtime& runtime)
    : HeadingCar::HeadingCar(control, headingSensor)
    , DistanceCar::DistanceCar(control, odometerleft, odometerRight, runtime)
    , SimpleCar::SimpleCar(control)
{
}

void SmartCar::update()
{
    DistanceCar::update();
    HeadingCar::update();
}
