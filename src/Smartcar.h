/**
 * @file Smartcar.h
 * @brief The entrypoint for the Smartcar library
 * The entrypoint for the Smartcar platform on boards that are compatible with
 * the Arduino API. Everything needed to control the platform (i.e. the vehicle
 * as well as its sensors) are included through this header file.
 */
#pragma once

#include "car/distance/DistanceCar.hpp"
#include "car/heading/HeadingCar.hpp"
#include "car/simple/SimpleCar.hpp"
#include "car/smart/SmartCar.hpp"
#include "control/ackerman/AckermanControl.hpp"
#include "control/differential/DifferentialControl.hpp"
#include "motor/analog/pwm/BrushedMotor.hpp"
#include "motor/digital/servo/ServoMotor.hpp"
#include "runtime/arduino_runtime/ArduinoRuntime.hpp"
#include "sensors/distance/infrared/analog/sharp/GP2D120.hpp"
#include "sensors/distance/infrared/analog/sharp/GP2Y0A02.hpp"
#include "sensors/distance/infrared/analog/sharp/GP2Y0A21.hpp"
#include "sensors/distance/ultrasound/i2c/SRF08.hpp"
#include "sensors/distance/ultrasound/ping/SR04.hpp"
#include "sensors/heading/gyroscope/GY50.hpp"
#include "sensors/odometer/interrupt/DirectionalOdometer.hpp"
#include "sensors/odometer/interrupt/DirectionlessOdometer.hpp"
#include "utilities/Utilities.hpp"

namespace smartcarlib
{
namespace pins
{
/**
 * \namespace v1
 * Helper resources for the `v1` platform (i.e. the Smartcar shield)
 */
namespace v1
{
const BrushedMotorPins leftMotorPins{ 8, 10, 9 };
const BrushedMotorPins rightMotorPins{ 12, 13, 11 };
} // namespace v1

/**
 * \namespace v2
 * Helper resources for the `v2` platform (i.e. the ESP32-enabled Smartcar)
 */
namespace v2
{
const BrushedMotorPins leftMotorPins{ 12, 14, 13 };
const BrushedMotorPins rightMotorPins{ 25, 26, 27 };
const uint8_t leftOdometerPin  = 35;
const uint8_t rightOdometerPin = 36;
const DirectionalOdometerPins leftOdometerPins{ leftOdometerPin, 34 };
const DirectionalOdometerPins rightOdometerPins{ rightOdometerPin, 39 };
} // namespace v2
} // namespace pins
} // namespace smartcarlib

/** \example BareMinimum.ino
 * A example on the boiler-plate code needed to start using the library.
 */
