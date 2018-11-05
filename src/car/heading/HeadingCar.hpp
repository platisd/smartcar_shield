/**
 * An class to programmatically represent a vehicle equipped with a heading sensor
 */
#pragma once

#include "../../sensors/odometer/Odometer.hpp"
#include "../simple/SimpleCar.hpp"

class HeadingCar : public SimpleCar
{
public:
    /**
     * Constructs a car equipped with a heading sensor
     * @param control The car's control
     * @param heading The heading sensor
     */
    HeadingCar(Control& control, HeadingSensor& heading);

    /**
     * Returns the car's current heading in degrees [0, 360)
     * @return The car's current heading in degrees
     */
    int getHeading();

    /**
     * Updates the readings from the heading sensor. You must have this being
     * executed as often as possible to get accurate heading readings.
     */
    virtual void update();

    /**
     * Rotates the car by the specified degrees where sign indicates direction
     * @param  degrees Degrees to rotate where positive angle for clockwise and
     *                 negative angle for counter clockwise.
     */
    void rotate(int degrees);
};
