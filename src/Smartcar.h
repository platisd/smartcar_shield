/**
* The entrypoint for the Smartcar platform on boards that are compatible with
* the Arduino API. Everything needed to control the platform (i.e. the vehicle
* as well as its sensors) are included through this header file.
*/
#pragma once

// We use this macro to specify that we are building for a board with an
// Arduino-compatible API. This is done so the constructors of various classes
// include the ArduinoRuntime instance that is defined below
#define SMARTCAR_BUILD_FOR_ARDUINO
#include "runtime/arduino_runtime/ArduinoRuntime.hpp"
// Create an ArduinoRuntime instance to be used as a default argument in
// many of the class constructors, so the users don't have to bother passing
// it themselves.
// In the future there might be built-in support for different runtime
// environments but they will probably reside in separate header folders.
ArduinoRuntime arduinoRuntime;

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
