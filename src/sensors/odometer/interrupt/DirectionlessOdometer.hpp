/**
 * \class DirectionlessOdometer
 * A class to represent the common - directionless - Odometers (speed encoders)
 * that can be used to primarily measure the travelled distance of the car
 * as well as its speed.
 */
#pragma once

#include <stdint.h>

#include "../../../runtime/Runtime.hpp"
#include "../Odometer.hpp"

class DirectionlessOdometer : public Odometer
{
public:
    /**
     * Constructs an odometer that can measure distance, speed but not direction
     * @param runtime           The runtime environment you want to run the class for
     * @param pulsePin          The pin that receives the pulses
     * @param callback          The callback to be invoked when a pulse is received (see example)
     * @param pulsesPerMeter    The amount of odometer pulses that constitute a meter
     *
     * **Example:**
     * \code
     * unsigned short ODOMETER_PIN = 32;
     * unsigned long PULSES_PER_METER = 110;
     * ArduinoRuntime arduinoRuntime;
     *
     * DirectionlessOdometer odometer(arduinoRuntime,
     *                                ODOMETER_PIN,
     *                                []() { odometer.update(); },
     *                                PULSES_PER_METER);
     * \endcode
     */
    DirectionlessOdometer(Runtime& runtime,
                          uint8_t pulsePin,
                          InterruptCallback callback,
                          unsigned long pulsesPerMeter);

    virtual ~DirectionlessOdometer() = default;

    /* Check `Odometer` interface for documentation */
    long getDistance() override;

    /* Check `Odometer` interface for documentation */
    float getSpeed() override;

    /* Check `Odometer` interface for documentation */
    bool isAttached() const override;

    /* Check `Odometer` interface for documentation */
    bool providesDirection() const override;

    /**
     * Resets the total travelled distance and speed to `0`
     */
    virtual void reset();

    /**
     * Conducts the distance and speed measurements.
     * Updates the current dt with the time difference between the last two pulses
     * and increases the pulse counter.
     * **Do not** call it directly in your sketch!
     * Instead, pass it in a lambda to the constructor.
     */
    virtual void update();

protected:
    const float mPulsesPerMeterRatio;
    volatile unsigned long mPulsesCounter{ 0 };
    volatile unsigned long mPreviousPulse{ 0 };
    volatile unsigned long mDt{ 0 };

private:
    const float mMillimetersPerPulse;
    Runtime& mRuntime;
    const bool kSensorAttached;
};

/**
 * \example findPulsesPerMeter.ino
 * An example on how to determine how many pulses are emitted from your Odometer
 * for every meter travelled. The output should be supplied to the constructor.
 *
 * \example singleOdometer.ino
 * An example on how to get the travelled distance from a single DirectionlessOdometer.
 *
 * \example twoOdometers.ino
 * An example on how to get the travelled distance from two DirectionlessOdometer.
 */
