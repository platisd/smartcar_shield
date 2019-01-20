/**
 * A class to programmatically represent a vehicle equipped with odometers
 */
#pragma once

#include "../../runtime/Runtime.hpp"
#include "../../sensors/odometer/Odometer.hpp"
#include "../simple/SimpleCar.hpp"

#ifndef PLATFORM_AGNOSTIC_BUILD
#include "../../runtime/arduino_runtime/ArduinoRuntime.hpp"
extern ArduinoRuntime arduinoRuntime;
#endif

namespace smartcarlib
{
namespace constants
{
namespace car
{
const unsigned long kDefaultPidFrequency = 80;
const float kDefaultProportional         = 5.0f;
const float kDefaultIntegral             = 0.0f;
const float kDefaultDerivative           = 10.0f;
const int kOdometersNotAttachedError     = -1000;
} // namespace car
} // namespace constants
} // namespace smartcarlib

class DistanceCar : virtual public SimpleCar
{
public:
#ifndef PLATFORM_AGNOSTIC_BUILD
    /**
     * Constructs a car equipped with a distance sensor
     * @param control  The car's control
     * @param odometer The odometer
     */
    DistanceCar(Control& control, Odometer& odometer, Runtime& runtime = arduinoRuntime);

    /**
     * Constructs a car equipped with a distance sensor
     * @param control        The car's control
     * @param odometerLeft  The left odometer
     * @param odometerRight The right odometer
     */
    DistanceCar(Control& control,
                Odometer& odometerLeft,
                Odometer& odometerRight,
                Runtime& runtime = arduinoRuntime);
#else
    DistanceCar(Control& control, Odometer& odometer, Runtime& runtime);
    DistanceCar(Control& control,
                Odometer& odometerLeft,
                Odometer& odometerRight,
                Runtime& runtime);
#endif

    /**
     * Gets the car's travelled distance
     * @return The car's travelled distance
     */
    long getDistance();

    /**
     * Sets the car's speed in meters per second if cruise control is enabled
     * otherwise as a percentage of the motor speed. Sign in both cases determines
     * direction.
     * @param speed The car's speed
     */
    virtual void setSpeed(float speed) override;

    /**
     * Gets the car's current speed in meters per second
     * @return The car's speed
     */
    float getSpeed();

    /**
     * Enables the car to move with a stable speed using the odometers
     * @param proportional The proportional value of the PID controller
     * @param integral     The integral value of the PID controller
     * @param derivative   The derivative value of the PID controller
     * @param frequency    How often to adjust the speed using `update()`
     */
    void enableCruiseControl(float proportional = smartcarlib::constants::car::kDefaultProportional,
                             float integral     = smartcarlib::constants::car::kDefaultIntegral,
                             float derivative   = smartcarlib::constants::car::kDefaultDerivative,
                             unsigned long frequency
                             = smartcarlib::constants::car::kDefaultPidFrequency);

    /**
     * Adjusts the cruise control speed. You must have this being executed as often
     * as possible when having cruise control enabled. When cruise control is not
     * enabled this has no effect.
     */
    virtual void update();

    /**
     * Disable cruise control.
     */
    void disableCruiseControl();

    /**
     * Sets the motor speed individually as a percentage of the motors` total
     * power unless cruise control is enabled in which case has no effect.
     * Use with caution.
     * @param firstMotorSpeed  The speed of the motor passed as first argument
     *                         argument to the car's control class [-100, 100]
     * @param secondMotorSpeed The speed of the motor passed as second argument
     *                         argument to the car's control class [-100, 100]
     */
    virtual void overrideMotorSpeed(int firstMotorSpeed, int secondMotorSpeed) override;

private:
    Control& mControl;
    Odometer& mOdometerLeft;
    Odometer& mOdometerRight;
    Runtime& mRuntime;
    bool mCruiseControlEnabled;
    float mProportional;
    float mIntegral;
    float mDerivative;
    unsigned long mFrequency;
    unsigned long mPreviousUpdate;
    float mTargetSpeed;
    float mPreviousControlledSpeed;
    float mIntegratedError;
    float mPreviousError;

    bool areOdometersAttached();
    bool areOdometersDirectional();
    float controlMotorSpeed(const float& previousSpeed,
                            const float& targetSpeed,
                            const float& currentSpeed);

    /**
     * Used internally making an attempt to stop the car based on the current speed.
     */
    void brake();
};

/**
 * \example DistanceCar.ino
 * A basic example on how to use the core functionality of the DistanceCar class.
 *
 * \example manualWithCruiseControl.ino
 * An example on how to manually control a DistanceCar via Serial when having
 * cruise control enabled.
 *
 * \example PidControllerMonitor.ino
 * An example on how to observe the behavior of the PID controller used by
 * cruise control of a DistanceCar, via the Arduino IDE's Serial Plotter
 * feature. The sketch can be used during the cruise control's tuning process.
 */
