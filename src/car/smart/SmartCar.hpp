/**
 * \class SmartCar
 * A class to programmatically represent a vehicle equipped with odometers and a heading
 * sensor
 */
#pragma once

#include "../distance/DistanceCar.hpp"
#include "../heading/HeadingCar.hpp"

#ifdef SMARTCAR_BUILD_FOR_ARDUINO
#include "../../runtime/arduino_runtime/ArduinoRuntime.hpp"
extern ArduinoRuntime arduinoRuntime;
#endif

class SmartCar : public DistanceCar, public HeadingCar
{
public:
#ifdef SMARTCAR_BUILD_FOR_ARDUINO
    /**
     * Constructs a car equipped with a heading sensor and an odometer
     * @param control        The car's control
     * @param headingSensor  The heading sensor
     * @param odometer       The odometer
     *
     * **Example:**
     * \code
     * BrushedMotor leftMotor(8, 10, 9);
     * BrushedMotor rightMotor(12, 13, 11);
     * DifferentialControl control(leftMotor, rightMotor);
     *
     * GY50 gyroscope(37);
     * DirectionlessOdometer odometer(100);

     * SmartCar car(control, gyroscope, odometer);
     * \endcode
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
     *
     * **Example:**
     * \code
     * BrushedMotor leftMotor(8, 10, 9);
     * BrushedMotor rightMotor(12, 13, 11);
     * DifferentialControl control(leftMotor, rightMotor);
     *
     * GY50 gyroscope(37);
     * DirectionlessOdometer leftOdometer(100);
     * DirectionlessOdometer rightOdometer(100);

     * SmartCar car(control, gyroscope, leftOdometer, rightOdometer);
     * \endcode
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
     * Adjusts the speed when cruise control is enabled and calculates the current heading.
     * You must have this being executed as often as possible for highest
     * accuracy of heading calculations and cruise control.
     *
     * **Example:**
     * \code
     * void loop() {
     *   // Update the car readings as often as possible
     *   car.update();
     *   // Other functionality
     * }
     * \endcode
     */
    virtual void update() override;

    /* Use the overriden functions from DistanceCar */
    using DistanceCar::overrideMotorSpeed;
    using DistanceCar::setSpeed;
};

/**
 * \example SmartCar.ino
 * A basic example on how to use the core functionality of the SmartCar class.
 *
 * \example automatedMovements.ino
 * An example of how to use the SmartCar functionality in order to perform a series
 * of automated movements using the vehicle's HeadingSensor and Odometer capabilities.
 *
 * \example rotateOnSpot.ino
 * An example on how to make a SmartCar rotate on spot by using the HeadingSensor
 * and the SmartCar::overrideMotorSpeed functionality.
 */
