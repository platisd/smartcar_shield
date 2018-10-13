/**
 * The SR04 (or SRF05) is an inexpensive ultrasonic sensor controllable over two
 * digital pins.
 *
 * For a more advanced solution (faster readings, not using `pulseIn`, non-blocking measurements)
 * please use the [NewPing library](https://bitbucket.org/teckel12/arduino-new-ping/wiki/Home).
 */
#pragma once

#include <stdint.h>

#include "../../../../runtime/Runtime.hpp"
#include "../../DistanceSensor.hpp"

#ifndef PLATFORM_AGNOSTIC_BUILD
#include "../../../../runtime/arduino_runtime/ArduinoRuntime.hpp"
extern ArduinoRuntime arduinoRuntime;
#endif

namespace smartcarlib
{
namespace constants
{
namespace sr04
{
const uint8_t kDefaultIterations       = 5;
const unsigned int kDefaultMaxDistance = 70;
} // namespace sr04
} // namespace constants
} // namespace smartcarlib

class SR04 : public DistanceSensor
{
public:
#ifndef PLATFORM_AGNOSTIC_BUILD
    SR04(Runtime& runtime         = arduinoRuntime,
         unsigned int maxDistance = smartcarlib::constants::sr04::kDefaultMaxDistance);
#else
    SR04(Runtime& runtime,
         unsigned int maxDistance = smartcarlib::constants::sr04::kDefaultMaxDistance);
#endif
    unsigned int getDistance() override;
    unsigned int getMedianDistance(uint8_t iterations
                                   = smartcarlib::constants::sr04::kDefaultIterations) override;

    /**
     * Initializes the SR04 sensor to the specified pins
     * @param triggerPin The pin to send the ultrasound pulse
     * @param echoPin    The pin to receive the ultrasound pulse deflection
     */
    void attach(uint8_t triggerPin, uint8_t echoPin);

private:
    Runtime& mRuntime;
    uint8_t mTriggerPin;
    uint8_t mEchoPin;
    bool mSensorAttached;
    const unsigned long kTimeout;
};
