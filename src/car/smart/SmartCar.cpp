#include "SmartCar.hpp"

SmartCar::SmartCar(Runtime& runtime,
                   Control& control,
                   HeadingSensor& headingSensor,
                   Odometer& odometer)
    : SimpleCar::SimpleCar(control)
    , DistanceCar::DistanceCar(runtime, control, odometer)
    , HeadingCar::HeadingCar(control, headingSensor)
{
}

SmartCar::SmartCar(Runtime& runtime,
                   Control& control,
                   HeadingSensor& headingSensor,
                   Odometer& odometerleft,
                   Odometer& odometerRight)
    : SimpleCar::SimpleCar(control)
    , DistanceCar::DistanceCar(runtime, control, odometerleft, odometerRight)
    , HeadingCar::HeadingCar(control, headingSensor)
{
}

void SmartCar::update()
{
    DistanceCar::update();
    HeadingCar::update();
}
