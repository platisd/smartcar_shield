/**
 * \class SRF08
 * The SRF08 sensor is an ultrasonic sensor that measures distances and
 * communicates over the I2C bus. You can find more information on the module
 * [here](https://www.robot-electronics.co.uk/htm/srf08tech.html).
 */
#pragma once

#include <stdint.h>

#include "../../../../runtime/Runtime.hpp"
#include "../../DistanceSensor.hpp"

#ifdef SMARTCAR_BUILD_FOR_ARDUINO
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
#ifdef SMARTCAR_BUILD_FOR_ARDUINO
    /**
     * Constructs an SRF08 sensor that communicates over I2C
     * @param address I2C address which should be within the range of [112, 127]
     *
     * **Example:**
     * \code
     * SRF08 srf08(112);
     * \endcode
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
     *
     * By default, the SRF08 sensor measurements last 70 milliseconds, _blocking_
     * the execution of the sketch. If you need to measure distances faster then
     * you need to set a default gain, range and ping delay manually, during
     * `setup()`. You can find more information about this
     * [here](http://www.robot-electronics.co.uk/htm/srf08tech.html).
     *
     * The allowed range for gain values is from 0 to 31. There is no rule about
     * which combinations of values work for every range. Some **empirical** range,
     * gain and ping delay combinations are listed below. Please beware that these
     * might not apply in your setting. Experiment and adapt accordingly.
     *
     * **Settings:** Gain: 31 Range: 7 Delay: 6
     * **Results:** Maximum range: 34 centimeters - Measurements every 6 milliseconds
     *
     * **Settings:** Gain: 0 Range: 23 Delay: 11
     * **Results:** Maximum range: 103 centimeters - Measurements every 11 milliseconds
     *
     * **Settings:** Gain: 10 Range: 35 Delay: 15
     * **Results:** Maximum range: 155 centimeters - Measurements every 15 milliseconds
     *
     * **Example:**
     * \code
     * srf08.setGain(10);
     * \endcode
     */
    void setGain(uint8_t gain);

    /**
     * Combined with the gain setting affects how much time each measurement will last
     * as well as the maximum detectable distance.
     * The maximum range can be calculated as `Max_Range = (range * 3.4) + 3.4 in cm`
     * @param range Range Value
     *
     * By default, the SRF08 sensor measurements last 70 milliseconds, _blocking_
     * the execution of the sketch. If you need to measure distances faster, then
     * you need to set a default gain, range and ping delay manually, during
     * `setup()`. You can find more information about this
     * [here](http://www.robot-electronics.co.uk/htm/srf08tech.html).
     *
     * The maximum range the sensor can measure, is calculated by the following
     * formula, giving a result in centimeters: `(rangeValue + 1) * 4.3`. Remember,
     * that just setting the range, will not change the delay between the measurements
     * or how fast the measurements can be initiated. If you need to do this, then
     * you will have to set the rest of the values as well. There is no rule about
     * which combinations of values work for every range. Some **empirical** range,
     * gain and ping delay combinations are listed in the documentation of
     * SRF08::setGain. Please beware that these might not apply in your setting.
     * Experiment and adapt accordingly.
     *
     * **Example:**
     * \code
     * srf08.setRange(35);
     * \endcode
     */
    void setRange(uint8_t range);

    /**
     * Delay between ranging measurements. If set too low without gain and range
     * being properly configured will cause errors during measurements.
     * @param  milliseconds Delay between ranging measurements
     * @return              The set delay
     *
     * Use this method to set the delay between the measurements. Remember, that
     * just setting this delay, does not by itself mean that the sensor will be
     * able to initiate measurements faster or have a shorter range. If you need
     * to do that, then you will have to set the rest of the values as well.
     * Refer to the documentation of SRF08::setGain for more details.
     *
     * **Example:**
     * \code
     * srf08.setPingDelay(15);
     * \endcode
     */
    unsigned long setPingDelay(unsigned long milliseconds
                               = smartcarlib::constants::srf08::kDefaultPingDelay);

    /**
     * Conducts a measurement from the on-board light sensor
     * @return The light measurement [0, 255]
     *
     * **Example:**
     * \code
     * unsigned short lightIntensity = srf08.getLightReading();
     * \endcode
     */
    uint8_t getLightReading();

    /**
     * Changes the address of the specific SRF08 sensor.
     * @param  newAddress New address to use [112, 127]
     * @return            The in-bounds address set for the sensor
     *
     * **Example:**
     * \code
     * unsigned short badAddress = 130;
     * unsigned short actualAddress = srf08.changeAddress(badAddress);
     * // The actual newly acquired address should now be 127
     * \endcode
     */
    uint8_t changeAddress(uint8_t newAddress);

private:
    uint8_t mAddress;
    Runtime& mRuntime;
    unsigned long mPingDelay;
    bool mAttached;

    void attach();
    void measureDistance();
};

/**
 * \example SRF08.ino
 * An example on how to conduct a single measurement with the SRF08 sensor and
 * print the results over Serial.
 */
