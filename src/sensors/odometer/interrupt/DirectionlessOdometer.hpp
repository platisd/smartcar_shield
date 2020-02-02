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

#ifdef SMARTCAR_BUILD_FOR_ARDUINO
#include "../../../runtime/arduino_runtime/ArduinoRuntime.hpp"
extern ArduinoRuntime arduinoRuntime;
#endif

class DirectionlessOdometer : public Odometer
{
public:
#ifdef SMARTCAR_BUILD_FOR_ARDUINO
    /**
     * Constructs an odometer that can measure distance, speed but not direction
     * @param pulsesPerMeter The amount of odometer pulses that constitute a meter
     *
     * **Example:**
     * \code
     * unsigned short ODOMETER_PIN = 32;
     * unsigned long PULSES_PER_METER = 110;
     *
     * DirectionlessOdometer odometer(ODOMETER_PIN,
     *                                []() { odometer.update(); },
     *                                PULSES_PER_METER);
     * \endcode
     */
    DirectionlessOdometer(uint8_t pin,
                          InterruptCallback callback,
                          unsigned long pulsesPerMeter,
                          Runtime& runtime = arduinoRuntime);
#else
    DirectionlessOdometer(uint8_t pin,
                          InterruptCallback callback,
                          unsigned long pulsesPerMeter,
                          Runtime& runtime);
#endif

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

    volatile unsigned long mPulsesCounter{ 0 };

protected:
    const float mPulsesPerMeterRatio;

private:
    const unsigned long mMillimetersPerPulse;
    Runtime& mRuntime;
    const bool kSensorAttached;
    // volatile unsigned long mPulsesCounter{ 0 };
    volatile unsigned long mPreviousPulse{ 0 };
    volatile unsigned long mDt{ 0 };
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
