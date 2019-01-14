/**
 * A class to programmatically represent a vehicle equipped with odometers and
 * a heading sensor
 */
#pragma once

#include "../distance/DistanceCar.hpp"
#include "../heading/HeadingCar.hpp"

#ifndef PLATFORM_AGNOSTIC_BUILD
#include "../../runtime/arduino_runtime/ArduinoRuntime.hpp"
extern ArduinoRuntime arduinoRuntime;
#endif

class SmartCar : public DistanceCar, public HeadingCar
{
public:
#ifndef PLATFORM_AGNOSTIC_BUILD
    /**
     * Constructs a car equipped with a heading sensor and an odometer
     * @param control        The car's control
     * @param headingSensor  The heading sensor
     * @param odometer       The odometer
     */
    SmartCar(Control& control,
             HeadingSensor& headingSensor,
             Odometer& odometer,
             Runtime& runtime = arduinoRuntime);

    /**
     * Constructs a car equipped with a heading sensor and two odometers
     * @param control       The car's control
     * @param headingSensor The heading sensor
     * @param odometerLeft  The left odometer
     * @param odometerRight The right odometer
     */
    SmartCar(Control& control,
             HeadingSensor& headingSensor,
             Odometer& odometerLeft,
             Odometer& odometerRight,
             Runtime& runtime = arduinoRuntime);
#else
    SmartCar(Control& control, HeadingSensor& headingSensor, Odometer& odometer, Runtime& runtime);
    SmartCar(Control& control,
             HeadingSensor& headingSensor,
             Odometer& odometerLeft,
             Odometer& odometerRight,
             Runtime& runtime);
#endif

    /**
     * Adjusts the speed when cruise control is enabled and calculates the current
     * heading. You must have this being executed as often as possible for highest
     * accuracy of heading calculations and cruise control.
     */
    virtual void update() override;

    /* Use the overriden functions from DistanceCar */
    using DistanceCar::overrideMotorSpeed;
    using DistanceCar::setSpeed;
};
