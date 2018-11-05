/**
 * An class to programmatically represent a vehicle equipped with odometers and
 * a heading sensor
 */
#pragma once

#include "../Car.hpp"

class SmartCar : public DistanceCar, public HeadingCar
{
public:
    /**
     * Constructs a car equipped with a heading sensor and an odometer
     * @param control  The car's control
     * @param heading  The heading sensor
     * @param odometer The odometer
     */
    SmartCar(Control& control, HeadingSensor& heading, Odometer& odometer);

    /**
     * Constructs a car equipped with a heading sensor and two odometers
     * @param control       The car's control
     * @param heading       The heading sensor
     * @param odometerLeft  The left odometer
     * @param odometerRight The right odometer
     */
    SmartCar(Control& control,
             HeadingSensor& heading,
             Odometer& odometerLeft,
             Odometer& odometerRight);

    /**
     * Adjusts the speed when cruise control is enabled and calculates the current
     * heading. You must have this being executed as often as possible for highest
     * accuracy.
     */
    virtual void update() override;
};
