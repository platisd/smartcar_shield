/**
 * The SR04 (or SRF05) is an inexpensive ultrasonic sensor controllable over two
 * digital pins.
 *
 * This class is essentially a stripped-down version of the NewPing library (v.1.5)
 * by [Tim Eckel](http://code.google.com/p/arduino-new-ping/). By the time of the
 * adoption, the NewPing library was published under the GPL 3.0 library while
 * now it was moved to a prioprietary one. This class is the reason the Smartcar
 * library is not published under a more permissive license and all pull requests
 * to remove this GPL'd code that is no longer supported are more than welcome.
 */
#pragma once

#include <stdint.h>

#include "../../../../runtime/Runtime.hpp"
#include "../../DistanceSensor.hpp"
#include "../../Median.hpp"

#ifndef PLATFORM_AGNOSTIC_BUILD
#include "../../../../runtime/arduino_runtime/ArduinoRuntime.hpp"
extern ArduinoRuntime arduinoRuntime;
#endif

namespace sr04
{
const uint8_t kDefaultIterations = 5;
const auto kMaxDistance          = 70;
} // namespace sr04

class SR04 : public DistanceSensor, public Median
{
public:
#ifndef PLATFORM_AGNOSTIC_BUILD
    SR04(Runtime& runtime = arduinoRuntime, unsigned int maxDistance = sr04::kMaxDistance);
#else
    SR04(Runtime& runtime, unsigned int maxDistance = kMaxDistance);
#endif
    unsigned int getDistance() override;
    unsigned int getMedianDistance(const uint8_t iterations = sr04::kDefaultIterations) override;

    /**
     * Initializes the SR04 sensor to the specified pins
     * @param triggerPin The pin to send the ultrasound pulse
     * @param echoPin    The pin to receive the ultrasound pulse deflection
     */
    void attach(uint8_t triggerPin, uint8_t echoPin);

private:
    unsigned int ping();
    bool ping_trigger();
    Runtime& mRuntime;
    uint8_t mTriggerBit;
    uint8_t mEchoBit;
    volatile uint8_t* mTriggerOutput;
    volatile uint8_t* mTriggerMode;
    volatile uint8_t* mEchoInput;
    unsigned int mMaxEchoTime;
    unsigned long mMaxTime;
    unsigned int mMaxDistance;
};
