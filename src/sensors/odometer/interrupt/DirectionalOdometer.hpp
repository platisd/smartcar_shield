/**
 * A class to represent directional Odometers. These sensors typically expose three
 * signals lines, as opposed to two in the directionless counterparts. Depending
 * on the state of this third signal, the direction of movement can be inferred.
 */
#pragma once

#include "DirectionlessOdometer.hpp"

class DirectionalOdometer : public DirectionlessOdometer
{
public:
#ifndef PLATFORM_AGNOSTIC_BUILD
    /**
     * Constructs an odometer that can measure distance, speed and direction
     * @param directionPin        The pin that receives the direction signal
     * @param pinStateWhenForward The direction pin state when moving forward
     * @param pulsesPerMeter       The amount of odometer pulses that constitute a meter
     */
    DirectionalOdometer(uint8_t directionPin,
                        uint8_t pinStateWhenForward,
                        unsigned long pulsesPerMeter
                        = smartcarlib::constants::odometer::kDefaultPulsesPerMeter,
                        Runtime& runtime = arduinoRuntime);
#else
    DirectionalOdometer(uint8_t directionPin,
                        uint8_t pinStateWhenForward,
                        unsigned long pulsesPerMeter,
                        Runtime& runtime);
#endif

    /* Check `DirectionlessOdometer` for documentation */
    bool attach(uint8_t pin, void (*callback)()) override;

    /* Check `DirectionlessOdometer` for documentation */
    void reset() override;

    /* Check `DirectionlessOdometer` for documentation */
    void update() override;

    /* Check `Odometer` interface for documentation */
    long getDistance() override;

    /* Check `Odometer` interface for documentation */
    float getSpeed() override;

    /* Check `Odometer` interface for documentation */
    bool providesDirection() override;

    /**
     * Get the direction of movement
     * @return `1` if travelling forward
     *         `-1` if travelling backward
     */
    int8_t getDirection();


private:
    const uint8_t mDirectionPin;
    const uint8_t mPinStateWhenForward;
    Runtime& mRuntime;
    volatile unsigned long mNegativePulsesCounter;
};
