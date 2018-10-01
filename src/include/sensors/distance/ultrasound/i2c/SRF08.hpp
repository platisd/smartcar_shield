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

    /**
     * Initializes the I2C connection to the specific address. If the specified
     * address is out of bounds then the higher of the lower limit will be chosen
     * instead.
     * @param  address I2C address [112, 127]
     * @return         The I2C address the connection was initiated with
     */
    uint8_t attach(uint8_t address = kDefaultAddress);

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
    unsigned long setPingDelay(unsigned long milliseconds = kDefaultPingDelay);

    /**
     * Conducts a measurement from the on-board light sensor
     * @return The light measurement [0, 255]
     */
    uint8_t getLightReading();

    /**
     * Changes the address of the specific SRF08 sensor
     * @param  newAddress New address to use [112, 127]
     * @return            The in-bounds address set for the sensor
     */
    uint8_t changeAddress(uint8_t newAddress);

private:
    Runtime& mRuntime;
    uint8_t mAddress;
    unsigned long mPingDelay;
};
