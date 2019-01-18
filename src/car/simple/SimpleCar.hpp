/**
 * A class to programmatically represent a very basic vehicle with two motors
 */
#pragma once

#include "../../control/Control.hpp"
#include "../Car.hpp"

class SimpleCar : public Car
{
public:
    /**
     * Constructs a simple car
     * @param control The car's control
     */
    SimpleCar(Control& control);

    virtual ~SimpleCar() = default;

    /**
     * Sets the car's driving speed as a percentage of the motors total speed where
     * the sign indicates direction.
     * @param speed The car's speed [-100, 100]
     */
    virtual void setSpeed(float speed) override;

    /* Check `Car` interface for documentation */
    virtual void setAngle(int angle) override;

    /* Check `Car` interface for documentation */
    virtual void overrideMotorSpeed(int firstMotorSpeed, int secondMotorSpeed) override;

private:
    Control& mControl;
};
