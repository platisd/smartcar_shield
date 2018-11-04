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
const int kDefaultOffset                           = 8;   // Sensor measurement when standing still
const uint8_t kDefaultSamplingInterval             = 100; // In milliseconds
const unsigned int kDefaultCalibrationMeasurements = 100;
} // namespace gy50
} // namespace constants
} // namespace smartcarlib

class GY50 : public HeadingSensor
{
public:
#ifndef PLATFORM_AGNOSTIC_BUILD
    GY50(int offset                     = smartcarlib::constants::gy50::kDefaultOffset,
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
     * Initializes the connection to the sensor
     */
    void attach();

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
    uint8_t mInterval;
    unsigned long mPreviousSample;
    bool mAttached;
    float mAngularDisplacement;

    int getAngularVelocity();
    int readL3G4200DRegister(uint8_t registerAddress);
    void writeL3G4200DRegister(uint8_t registerAddress, uint8_t value);
};
