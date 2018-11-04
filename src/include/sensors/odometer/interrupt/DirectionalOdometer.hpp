/**
 * A class to represent directional Odometers. These sensors typically expose three
 * signals lines, as opposed to two in the directionless counterparts. Depending
 * on the state of this third signal, the direction of movement can be inferred.
 */
#pragma once

#include <stdint.h>

#include "../../runtime/Runtime.hpp"
#include "Odometer.hpp"

namespace smartcarlib
{
namespace constants
{
namespace odometer
{
const int8_t kForward  = 1;
const int8_t kBackward = -1;
} // namespace odometer
} // namespace constants
} // namespace smartcarlib

class DirectionalOdometer : public Odometer
{
public:
#ifndef PLATFORM_AGNOSTIC_BUILD
    DirectionalOdometer(uint8_t directionPin,
                        uint8_t pinStateWhenForward,
                        Runtime& runtime = arduinoRuntime,
                        unsigned long pulsesPerMeter
                        = smartcarlib::constants::odometer::kDefaultPulsesPerMeter);
#else
    DirectionalOdometer(uint8_t directionPin,
                        uint8_t pinStateWhenForward,
                        Runtime& runtime,
                        unsigned long pulsesPerMeter
                        = smartcarlib::constants::odometer::kDefaultPulsesPerMeter);
#endif

    /* Refer to parent class for documentation */
    bool attach(uint8_t pin, void (*callback)()) override;

    /* Refer to parent class for documentation */
    void reset() override;

    /* Refer to parent class for documentation */
    void update() override;

    /**
     * Get the relative travelled distance since the sensor was attached or resetted
     * @return The travelled distance in centimeters. A positive value if movement
     *         was mostly forward and negative if it was mostly backward.
     */
    long getRelativeDistance();

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
