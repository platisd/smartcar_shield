#include "SmartCar.hpp"

SmartCar::SmartCar(Control& control,
                   HeadingSensor& headingSensor,
                   Odometer& odometer,
                   Runtime& runtime)
    : SimpleCar::SimpleCar(control)
    , DistanceCar::DistanceCar(control, odometer, runtime)
    , HeadingCar::HeadingCar(control, headingSensor)
{
}

SmartCar::SmartCar(Control& control,
                   HeadingSensor& headingSensor,
                   Odometer& odometerleft,
                   Odometer& odometerRight,
                   Runtime& runtime)
    : SimpleCar::SimpleCar(control)
    , DistanceCar::DistanceCar(control, odometerleft, odometerRight, runtime)
    , HeadingCar::HeadingCar(control, headingSensor)
{
}

void SmartCar::update()
{
    DistanceCar::update();
    HeadingCar::update();
}
