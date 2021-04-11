/**
 * \class SmartCar
 * A class to programmatically represent a vehicle equipped with odometers and a heading
 * sensor
 */
#pragma once

#include "../distance/DistanceCar.hpp"
#include "../heading/HeadingCar.hpp"

class SmartCar : public DistanceCar, public HeadingCar
{
public:
    /**
     * Constructs a car equipped with a heading sensor and an odometer
     * @param runtime        The runtime environment you want to run the class for
     * @param control        The car's control
     * @param headingSensor  The heading sensor
     * @param odometer       The odometer
     *
     * **Example:**
     * \code
     * ArduinoRuntime arduinoRuntime;
     * BrushedMotor leftMotor(arduinoRuntime, smartcarlib::pins::v2::leftMotorPins);
     * BrushedMotor rightMotor(arduinoRuntime, smartcarlib::pins::v2::rightMotorPins);
     * DifferentialControl control(leftMotor, rightMotor);
     *
     * GY50 gyroscope(arduinoRuntime, 37);
     *
     * const auto pulsesPerMeter = 600;
     *
     * DirectionlessOdometer odometer(
     *     arduinoRuntime, smartcarlib::pins::v2::leftOdometerPin, []() { leftOdometer.update(); },
     *     pulsesPerMeter);
     *
     * SmartCar car(arduinoRuntime, control, gyroscope, odometer);
     * \endcode
     */
    SmartCar(Runtime& runtime, Control& control, HeadingSensor& headingSensor, Odometer& odometer);

    /**
     * Constructs a car equipped with a heading sensor and two odometers
     * @param runtime       The runtime environment you want to run the class for
     * @param control       The car's control
     * @param headingSensor The heading sensor
     * @param odometerLeft  The left odometer
     * @param odometerRight The right odometer
     *
     * **Example:**
     * \code
     * ArduinoRuntime arduinoRuntime;
     * BrushedMotor leftMotor(arduinoRuntime, smartcarlib::pins::v2::leftMotorPins);
     * BrushedMotor rightMotor(arduinoRuntime, smartcarlib::pins::v2::rightMotorPins);
     * DifferentialControl control(leftMotor, rightMotor);
     *
     * GY50 gyroscope(arduinoRuntime, 37);
     *
     * const auto pulsesPerMeter = 600;
     *
     * DirectionlessOdometer leftOdometer(
     *     arduinoRuntime, smartcarlib::pins::v2::leftOdometerPin, []() { leftOdometer.update(); },
     *     pulsesPerMeter);
     * DirectionlessOdometer rightOdometer(
     *     arduinoRuntime, smartcarlib::pins::v2::rightOdometerPin, []() { rightOdometer.update();
     * }, pulsesPerMeter);
     *
     * SmartCar car(arduinoRuntime, control, gyroscope, leftOdometer, rightOdometer);
     * \endcode
     */
    SmartCar(Runtime& runtime,
             Control& control,
             HeadingSensor& headingSensor,
             Odometer& odometerLeft,
             Odometer& odometerRight);

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
