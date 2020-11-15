/**
 * \class DirectionalOdometer
 * A class to represent directional Odometers. These sensors typically expose three
 * signals lines, as opposed to two in the directionless counterparts. Depending
 * on the state of this third signal, the direction of movement can be inferred.
 */
#pragma once

#include "DirectionlessOdometer.hpp"

/**
 * @brief Helper class to represent directional odometer pins
 */
struct DirectionalOdometerPins
{
    /**
     * @brief Construct a DirectionalOdometerPins object
     *
     * @param pulsePin          The pin that receives the pulses
     * @param forwardWhenLowPin The pin that is set to LOW when moving forward
     */
    DirectionalOdometerPins(uint8_t pulsePin, uint8_t forwardWhenLowPin)
        : pulse{ pulsePin }
        , direction{ forwardWhenLowPin }
    {
    }

    const uint8_t pulse;     // NOLINT: It's OK for these member variables to be non-private
    const uint8_t direction; // NOLINT
};

class DirectionalOdometer : public DirectionlessOdometer
{
public:
    /**
     * Constructs an odometer that can measure distance, speed and direction
     * @param runtime           The runtime environment you want to run the class for
     * @param pulsePin          The pin that receives the pulses
     * @param forwardWhenLowPin The pin that is set to LOW when moving forward
     * @param callback          The callback to be invoked when a pulse is received (see example)
     * @param pulsesPerMeter    The amount of odometer pulses that constitute a meter
     *
     * **Example:**
     * \code
     * unsigned short ODOMETER_PIN = 32;
     * unsigned short DIRECTION_PIN = 8;
     * unsigned long PULSES_PER_METER = 40;
     * ArduinoRuntime arduinoRuntime;
     *
     * DirectionalOdometer odometer(arduinoRuntime,
     *                              ODOMETER_PIN,
     *                              DIRECTION_PIN,
     *                              []() { odometer.update(); },
     *                              PULSES_PER_METER);
     * \endcode
     */
    DirectionalOdometer(Runtime& runtime,
                        uint8_t pulsePin,
                        uint8_t forwardWhenLowPin,
                        InterruptCallback callback,
                        unsigned long pulsesPerMeter);

    /**
     * Constructs an odometer that can measure distance, speed and direction
     * @param pins              The `DirectionalOdometerPins` object with the pins of the odometer
     * @param callback          The callback to be invoked when a pulse is received (see example)
     * @param pulsesPerMeter    The amount of odometer pulses that constitute a meter
     *
     * **Example:**
     * \code
     * unsigned long PULSES_PER_METER = 40;
     * ArduinoRuntime arduinoRuntime;
     *
     * DirectionalOdometer leftOdometer(arduinoRuntime,
     *                                  smartcarlib::pins::v2::leftOdometerPins,
     *                                  []() { odometer.update(); },
     *                                  PULSES_PER_METER);
     * \endcode
     */
    DirectionalOdometer(Runtime& runtime,
                        DirectionalOdometerPins pins,
                        InterruptCallback callback,
                        unsigned long pulsesPerMeter);

    /* Check `DirectionlessOdometer` for documentation */
    void reset() override;

    /* Check `DirectionlessOdometer` for documentation */
    void update() override;

    /* Check `Odometer` interface for documentation */
    long getDistance() override;

    /* Check `Odometer` interface for documentation */
    float getSpeed() override;

    /* Check `Odometer` interface for documentation */
    bool providesDirection() const override;

    /**
     * Get the direction of movement
     * @return `1` if travelling forward
     *         `-1` if travelling backward
     *
     * **Example:**
     * \code
     * unsigned short direction = odometer.getDirection();
     * \endcode
     */
    int8_t getDirection() const;

private:
    const uint8_t mDirectionPin;
    Runtime& mRuntime;
    const int kPinStateWhenForward;
    volatile unsigned long mNegativePulsesCounter{ 0 };
    volatile int mDirectionPinState;
};

/**
 * \example directionalOdometers.ino
 * An example on how to get the travelled distance from two DirectionalOdometer.
 */
