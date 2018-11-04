/**
 * An abstract class to represent the various infrared sensors that are able to
 * conduct measurements by emiting an analog signal that can be read by a
 * microcontroller through the ADC.
 */
#pragma once

#include <stdint.h>

#include "../../../../runtime/Runtime.hpp"
#include "../../DistanceSensor.hpp"

namespace smartcarlib
{
namespace constants
{
namespace analogInfraredSensor
{
const uint8_t kDefaultIterations = 5;
} // namespace analogInfraredSensor
} // namespace constants
} // namespace smartcarlib

class InfraredAnalogSensor : public DistanceSensor
{
public:
    InfraredAnalogSensor(Runtime& runtime);

    /* Check `DistanceSensor` interface for documentation */
    unsigned int
    getMedianDistance(uint8_t iterations
                      = smartcarlib::constants::analogInfraredSensor::kDefaultIterations) override;

private:
    Runtime& mRuntime;
};
