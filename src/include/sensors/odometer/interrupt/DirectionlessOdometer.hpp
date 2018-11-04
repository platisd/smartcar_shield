/**
 * A class to represent the common - directionless - Odometers (speed encoders)
 * that can be used to primarily measure the travelled distance of the car
 * as well as its speed.
 */
#pragma once

#include <stdint.h>

#include "../../../runtime/Runtime.hpp"

#ifndef PLATFORM_AGNOSTIC_BUILD
#include "../../../runtime/arduino_runtime/ArduinoRuntime.hpp"
extern ArduinoRuntime arduinoRuntime;
#endif

namespace smartcarlib
{
namespace constants
{
namespace odometer
{
const unsigned long kDefaultPulsesPerMeter = 100; // 1:1 cm to pulses ratio
} // namespace odometer
} // namespace constants
} // namespace smartcarlib

class DirectionlessOdometer
{
public:
#ifndef PLATFORM_AGNOSTIC_BUILD
    DirectionlessOdometer(unsigned long pulsesPerMeter
                          = smartcarlib::constants::odometer::kDefaultPulsesPerMeter,
                          Runtime& runtime = arduinoRuntime);
#else
    DirectionlessOdometer(unsigned long pulsesPerMeter,
                          Runtime& runtime);
#endif

    ~DirectionlessOdometer() = default;

    /**
     * Initializes the Odometer to receive pulses from the specified pin
     * @param  pin      The pin to receive pulses from which must support external hardware
     *                  interrupts
     * @param  callback The function to be run on each incoming pulse. Please use the following as
     *                  an argument: `[](){yourOdometerInstance.update();}`
     * @return          `false` if an error occurred, i.e. invalid pin
     */
    virtual bool attach(uint8_t pin, void (*callback)());

    /**
     * Returns the total travelled distance in centimeters
     * @return The total travelled distance in centimeters
     */
    unsigned long getDistance();

    /**
     * Returns the current absolute speed in meters/sec
     * @return The absolute speed in meters/sec
     */
    float getSpeed();

    /**
     * Resets the total travelled distance and speed to `0`
     */
    virtual void reset();

    /**
     * Conducts the distance and speed measurements.
     * Updates the current dt with the time difference between the last two pulses
     * and increases the pulse counter.
     * **Do not** call it directly in your sketch!
     * Instead, pass it inside a lambda as an argument to `attach`. Example:
     * ```
     * const int ODOMETER_PIN = 2;
     * Odometer odometer;
     * odometer.attach(ODOMETER_PIN, [](){odometer.update();});
     * ```
     */
    virtual void update();

protected:
    const float mPulsesPerMeterRatio;

private:
    const unsigned long mMillimetersPerPulse;
    Runtime& mRuntime;
    uint8_t mPin;
    bool mSensorAttached;
    volatile unsigned long mPulsesCounter;
    volatile unsigned long mPreviousPulse;
    volatile unsigned long mDt;
};
