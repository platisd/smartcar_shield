/**
 * \class GY50
 * The class representing a GY-50 gyroscope module based on the L3G4200D sensor.
 * This sensor allows you to get the current angular displacement, in degrees,
 * since the beginning of the measurement **in the range of [0,360)**. By the
 * default setup, clockwise movement increases the degree count and counter-clockwise
 * decreases them. If you have different set up (orientation of gyroscope) you will
 * have to adapt accordingly. This method will measure the angular displacement
 * (rotation) since the first GY50::update was called.
 *
 * **Always** remember to use GY50::update in your main loop along with this method.
 */
#pragma once

#include <stdint.h> // NOLINT(modernize-deprecated-headers)

#include "../../../runtime/Runtime.hpp"
#include "../HeadingSensor.hpp"

namespace smartcarlib
{
namespace constants
{
namespace gy50
{
const uint8_t kDefaultSamplingInterval    = 100;
const int kError                          = -32767;
const int kDefaultCalibrationMeasurements = 100;
} // namespace gy50
} // namespace constants
} // namespace smartcarlib

class GY50 : public HeadingSensor
{
public:
    /**
     * Constructs a GY50 gyroscope
     * @param runtime          The runtime environment you want to run the class for
     * @param offset           The sensor-specific measurement value when idle.
     *                         Find the value for your sensing with GY50::getOffset.
     * @param samplingInterval How often to upate the heading
     *
     * **Example:**
     * \code
     * ArduinoRuntime arduinoRuntime;
     * int offset = 37; // The offset we have acquired via the getOffset method
     * GY50 gyro(arduinoRuntime, offset);
     * \endcode
     */
    GY50(Runtime& runtime,
         int offset,
         unsigned long samplingInterval = smartcarlib::constants::gy50::kDefaultSamplingInterval);

    /* Check `HeadingSensor` interface for documentation */
    int getHeading() override;

    /* Check `HeadingSensor` interface for documentation */
    void update() override;

    /**
     * Get the sensor's offset which is the value the sensor returns when still
     * @param  measurements The amount of measurements to conduct to determine the offset
     * @return              The sensor's offset
     *
     * **Example:**
     * \code
     * // Initialize it with any value, it does not play a role during calibration
     * GY50 gyro(0);
     * // Discover the offset and use it in the constructor next time
     * int offset = gyro.getOffset();
     * \endcode
     */
    int getOffset(int measurements = smartcarlib::constants::gy50::kDefaultCalibrationMeasurements);

private:
    const int kOffset;
    const unsigned long kSamplingInterval;
    Runtime& mRuntime;
    unsigned long mPreviousSample;
    bool mAttached;
    float mAngularDisplacement;

    void attach();
    int getAngularVelocity();
    int readL3G4200DRegister(uint8_t registerAddress);
    void writeL3G4200DRegister(uint8_t registerAddress, uint8_t value);
};

/**
 * \example gyroscopeCalibration.ino
 * An example on how to calibrate your GY50 gyroscope and determine its offset.
 *
 * \example gyroscopeHeading.ino
 * A basic example on how to get the current heading from your GY50 gyroscope
 * and print it out over Serial.
 */
