/**
 * A class to programmatically represent a vehicle equipped with a heading sensor
 */
#pragma once

#include "../../sensors/heading/HeadingSensor.hpp"
#include "../simple/SimpleCar.hpp"

class HeadingCar : virtual public SimpleCar
{
public:
    /**
     * Constructs a car equipped with a heading sensor
     * @param control       The car's control
     * @param headingSensor The heading sensor
     */
    HeadingCar(Control& control, HeadingSensor& headingSensor);

    /**
     * Returns the car's current heading in degrees [0, 360)
     * @return The car's current heading in degrees
     */
    unsigned int getHeading();

    /**
     * Updates the readings from the heading sensor. You must have this being
     * executed as often as possible to get accurate heading readings.
     */
    virtual void update();

private:
    HeadingSensor& mHeadingSensor;
};

/**
 * \example HeadingCar.ino
 * A basic example on how to use the core functionality of the HeadingCar class
 */
