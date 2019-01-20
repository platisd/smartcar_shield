#pragma once

#include <stdint.h>

#include "../../../runtime/Runtime.hpp"
#include "../HeadingSensor.hpp"

#ifndef PLATFORM_AGNOSTIC_BUILD
#include "../../../runtime/arduino_runtime/ArduinoRuntime.hpp"
extern ArduinoRuntime arduinoRuntime;
#endif

namespace smartcarlib
{
namespace constants
{
namespace gy50
{
const uint8_t kDefaultSamplingInterval             = 100;
const int kError                                   = -32767;
const unsigned int kDefaultCalibrationMeasurements = 100;
} // namespace gy50
} // namespace constants
} // namespace smartcarlib

class GY50 : public HeadingSensor
{
public:
#ifndef PLATFORM_AGNOSTIC_BUILD
    /**
     * Constructs a GY50 gyroscope
     * @param offset           The sensor-specific measurement value when idle.
     *                         Find the value for your sensing with `getOffset`.
     * @param samplingInterval How often to upate the heading
     */
    GY50(int offset,
         unsigned long samplingInterval = smartcarlib::constants::gy50::kDefaultSamplingInterval,
         Runtime& runtime               = arduinoRuntime);
#else
    GY50(int offset, unsigned long samplingInterval, Runtime& runtime);
#endif

    /* Check `HeadingSensor` interface for documentation */
    unsigned int getHeading() override;

    /* Check `HeadingSensor` interface for documentation */
    void update() override;

    /**
     * Get the sensor's offset which is the value the sensor returns when still
     * @param  measurements The amount of measurements to conduct to determine the offset
     * @return              The sensor's offset
     */
    int getOffset(unsigned int measurements
                  = smartcarlib::constants::gy50::kDefaultCalibrationMeasurements);

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
