/**
 * \class SimpleCar
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
     *
     * **Example:**
     * \code
     * BrushedMotor leftMotor(8, 10, 9);
     * BrushedMotor rightMotor(12, 13, 11);
     * DifferentialControl control(leftMotor, rightMotor);
     *
     * SimpleCar car(control);
     * \endcode
     */
    SimpleCar(Control& control);

    virtual ~SimpleCar() = default;

    /**
     * Sets the car's driving speed as a percentage of the motors total speed where
     * the sign indicates direction.
     * @param speed The car's speed [-100, 100]
     *
     * **Example:**
     * \code
     * car.setSpeed(-100); // Full speed backward
     * car.setSpeed(0); // Car immobilized
     * car.setSpeed(50); // Car with half speed forward
     * \endcode
     */
    virtual void setSpeed(float speed) override;

    /* Check `Car` interface for documentation */
    virtual void setAngle(int angle) override;

    /* Check `Car` interface for documentation */
    virtual void overrideMotorSpeed(int firstMotorSpeed, int secondMotorSpeed) override;

private:
    Control& mControl;
};

/**
 * \example SimpleCar.ino
 * A basic example on how to use the core functionality of the SimpleCar class.
 *
 * \example FullSerialControl.ino
 * An example on how to set via Serial the speed an angle for a SimpleCar. Other
 * Car types (e.g. DistanceCar and SmartCar) can also be controled
 * in the same way.
 *
 * \example manualControl.ino
 * An example on how to manually control a SimpleCar (also applicable for DistanceCar
 * and SmartCar) via Serial using predefined combinations of speeds and angles.
 *
 * \example SerialControlServoESC.ino
 * A basic example on how to create a SimpleCar via Serial when it uses an
 * ESC for throttling and a ServoMotor for steering via the AckermanControl method.
 *
 * \example shieldMotorsTest.ino
 * A sketch used to help you set up the default Smartcar setup using four BrushedMotor
 * via DifferentialControl.
 */
