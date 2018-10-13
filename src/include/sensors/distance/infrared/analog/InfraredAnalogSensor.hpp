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

    unsigned int
    getMedianDistance(uint8_t iterations
                      = smartcarlib::constants::analogInfraredSensor::kDefaultIterations) override;

    /**
     * Initializes the sensor to the specified pin
     * @param pin The pin to read the sensor's analog signal from
     */
    void attach(uint8_t pin);

protected:
    uint8_t mPin;
    bool mSensorAttached;

private:
    Runtime& mRuntime;
};

// SHARP_IR::SHARP_IR(){
//     //_sensorMedianDelay = 15; //median delay for all the sharp infrared sensors
//     _pin = 99; //give it an initial (likely invalid) value
//     _maxDistance = 0;
//     _minDistance = 0;
// }
//
// void SHARP_IR::attach(unsigned short pin){
//     _pin = pin;
//     pinMode(_pin, INPUT);
// }
//
// unsigned int SHARP_IR::getDistance(){
//     unsigned int distance = voltsToCentimeters(readAnalogInput());
//     if ((distance < _minDistance) || (distance > _maxDistance)){
//         return 0;
//     }
//     return distance;
// }
//
// unsigned int SHARP_IR::readAnalogInput(){
//     return analogRead(_pin);
// }
