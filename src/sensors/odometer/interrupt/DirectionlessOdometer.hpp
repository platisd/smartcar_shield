/**
 * A class to represent the common - directionless - Odometers (speed encoders)
 * that can be used to primarily measure the travelled distance of the car
 * as well as its speed.
 */
#pragma once

#include <stdint.h>

#include "../../../runtime/Runtime.hpp"
#include "../Odometer.hpp"

#ifndef PLATFORM_AGNOSTIC_BUILD
#include "../../../runtime/arduino_runtime/ArduinoRuntime.hpp"
extern ArduinoRuntime arduinoRuntime;
#endif

class DirectionlessOdometer : public Odometer
{
public:
#ifndef PLATFORM_AGNOSTIC_BUILD
    /**
     * Constructs an odometer that can measure distance, speed but not direction
     * @param pulsesPerMeter The amount of odometer pulses that constitute a meter
     */
    DirectionlessOdometer(unsigned long pulsesPerMeter,
                          Runtime& runtime = arduinoRuntime);
#else
    DirectionlessOdometer(unsigned long pulsesPerMeter, Runtime& runtime);
#endif

    virtual ~DirectionlessOdometer() = default;

    /**
     * Initializes the Odometer to receive pulses from the specified pin
     * @param  pin      The pin to receive pulses from which must support external hardware
     *                  interrupts
     * @param  callback The function to be run on each incoming pulse. Please use the following as
     *                  an argument: `[](){yourOdometerInstance.update();}`
     * @return          `false` if an error occurred, i.e. invalid pin
     */
    virtual bool attach(uint8_t pin, void (*callback)());

    /* Check `Odometer` interface for documentation */
    long getDistance() override;

    /* Check `Odometer` interface for documentation */
    float getSpeed() override;

    /* Check `Odometer` interface for documentation */
    bool isAttached() override;

    /* Check `Odometer` interface for documentation */
    bool providesDirection() override;

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
