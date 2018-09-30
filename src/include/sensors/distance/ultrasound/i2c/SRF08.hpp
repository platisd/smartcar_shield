/**
 * The SRF08 sensor is an ultrasonic sensor that measures distances and
 * communicates over the I2C bus. You can find more information on the module
 * [here](https://www.robot-electronics.co.uk/htm/srf08tech.html).
 */
#pragma once

#include "../../../../runtime/Runtime.hpp"
#include "../../DistanceSensor.hpp"
#include "../../Median.hpp"
#include <stdint.h>

#ifndef PLATFORM_AGNOSTIC_BUILD
#include "../../../../runtime/arduino_runtime/ArduinoRuntime.hpp"
extern ArduinoRuntime arduinoRuntime;
#endif

namespace
{
const uint8_t kDefaultIterations = 5;
const uint8_t kDefaultPingDelay  = 70;
const uint8_t kDefaultAddress    = 112;
} // namespace

class SRF08 : public DistanceSensor, public Median
{
public:
#ifndef PLATFORM_AGNOSTIC_BUILD
    SRF08(Runtime& runtime = arduinoRuntime);
#else
    SRF08(Runtime& runtime);
#endif
    unsigned int getDistance() override;
    unsigned int getMedianDistance(const uint8_t iterations = kDefaultIterations) override;
    uint8_t attach(uint8_t address = kDefaultAddress);
    void setGain(uint8_t gain);
    void setRange(uint8_t range);
    unsigned long setPingDelay(unsigned long milliseconds = kDefaultPingDelay);
    uint8_t getLightReading();
    uint8_t changeAddress(uint8_t newAddress);

private:
    Runtime& mRuntime;
    uint8_t mAddress;
    unsigned long mPingDelay;
};
