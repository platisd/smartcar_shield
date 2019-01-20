/*
* The entrypoint for the Smartcar platform. Everything needed to control
* the platform (i.e. the vehicle as well as its sensors) are included through
* this header file.
*/
#pragma once

#include "sensors/distance/ultrasound/i2c/SRF08.hpp"
#include "sensors/distance/ultrasound/ping/SR04.hpp"
#include "sensors/distance/infrared/analog/sharp/GP2D120.hpp"
#include "sensors/distance/infrared/analog/sharp/GP2Y0A02.hpp"
#include "sensors/distance/infrared/analog/sharp/GP2Y0A21.hpp"
#include "sensors/odometer/interrupt/DirectionlessOdometer.hpp"
#include "sensors/odometer/interrupt/DirectionalOdometer.hpp"
#include "sensors/heading/gyroscope/GY50.hpp"
#include "motor/analog/pwm/BrushedMotor.hpp"
#include "motor/digital/servo/ServoMotor.hpp"
#include "control/ackerman/AckermanControl.hpp"
#include "control/differential/DifferentialControl.hpp"
#include "car/simple/SimpleCar.hpp"
#include "car/distance/DistanceCar.hpp"
#include "car/heading/HeadingCar.hpp"
#include "car/smart/SmartCar.hpp"
#include "utilities/Utilities.hpp"

/** \example BareMinimum.ino
 * A example on the boiler-plate code needed to start using the library.
 */
