/**
 * The SRF08 sensor is an ultrasonic sensor that measures distances and
 * communicates over the I2C bus. You can find more information on the module
 * [here](https://www.robot-electronics.co.uk/htm/srf08tech.html).
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
namespace srf08
{
const uint8_t kDefaultIterations = 5;
const uint8_t kDefaultPingDelay  = 70;
} // namespace srf08
} // namespace constants
} // namespace smartcarlib

class SRF08 : public DistanceSensor
{
public:
#ifndef PLATFORM_AGNOSTIC_BUILD
    /**
     * Constructs an SRF08 sensor that communicates over I2C
     * @param address I2C address which should be within the range of [112, 127]
     */
    SRF08(uint8_t address, Runtime& runtime = arduinoRuntime);
#else
    SRF08(uint8_t address, Runtime& runtime);
#endif

    /* Check `DistanceSensor` interface for documentation */
    unsigned int getDistance() override;

    /* Check `DistanceSensor` interface for documentation */
    unsigned int getMedianDistance(uint8_t iterations
                                   = smartcarlib::constants::srf08::kDefaultIterations) override;

    /**
     * Combined with the range setting affects how much time each measurement will last
     * as well as the maximum detectable distance.
     * @param gain Gain Value [0, 31]
     */
    void setGain(uint8_t gain);

    /**
     * Combined with the gain setting affects how much time each measurement will last
     * as well as the maximum detectable distance.
     * The maximum range can be calculated as `Max_Range = (range * 3.4) + 3.4 in cm`
     * @param range Range Value
     */
    void setRange(uint8_t range);

    /**
     * Delay between ranging measurements. If set too low without gain and range
     * being properly configured will cause errors during measurements.
     * @param  milliseconds Delay between ranging measurements
     * @return              The set delay
     */
    unsigned long setPingDelay(unsigned long milliseconds
                               = smartcarlib::constants::srf08::kDefaultPingDelay);

    /**
     * Conducts a measurement from the on-board light sensor
     * @return The light measurement [0, 255]
     */
    uint8_t getLightReading();

    /**
     * Changes the address of the specific SRF08 sensor.
     * @param  newAddress New address to use [112, 127]
     * @return            The in-bounds address set for the sensor
     */
    uint8_t changeAddress(uint8_t newAddress);

private:
    uint8_t mAddress;
    Runtime& mRuntime;
    unsigned long mPingDelay;
    bool mInitializedI2C;

    void initializeI2C();
};
