/*
    L3G4200D implementation based on code by:
    * http://bildr.org/2011/06/l3g4200d-arduino/
    * http://www.pieter-jan.com/node/7
    * https://github.com/sparkfun/Tri-Axis_Gyro_Breakout-L3G4200D
*/
#include "GY50.hpp"
#include "../../../utilities/Utilities.hpp"

namespace
{
const uint8_t kGyroAddress      = 105;
const auto kMeasurementInterval = 100;
const float kGyroSensitivity    = 0.07f;
const int kGyroThreshold        = 12; // Smaller changes are to be ignored
} // namespace

using namespace smartcarlib::utils;
using namespace smartcarlib::constants::gy50;

GY50::GY50(int offset, unsigned long samplingInterval, Runtime& runtime)
    : kOffset{ offset }
    , kSamplingInterval{ samplingInterval }
    , mRuntime(runtime)
    , mPreviousSample{ 0 }
    , mAttached{ false }
    , mAngularDisplacement{ 0 }
{
}

int GY50::getHeading()
{
    // Get the reading from (-180,180) to [0, 360) scale
    auto normalizedReading = static_cast<int>(mAngularDisplacement) % 360;
    return normalizedReading < 0 ? normalizedReading + 360 : normalizedReading;
}

void GY50::update()
{
    unsigned long currentTime = mRuntime.currentTimeMillis();
    unsigned long interval    = currentTime - mPreviousSample;
    if (interval <= kSamplingInterval)
    {
        return; // Not the time to read yet
    }

    int drift = kOffset - getAngularVelocity();

    if (getAbsolute(drift) > kGyroThreshold)
    {
        float gyroRate = drift * kGyroSensitivity;
        mAngularDisplacement += gyroRate / (1000.0 / interval);
    }
    mPreviousSample = currentTime;
}

void GY50::attach()
{
    if (mAttached)
    {
        return;
    }

    static const uint8_t controlRegister1 = 0x20;
    static const uint8_t controlRegister2 = 0x21;
    static const uint8_t controlRegister3 = 0x22;
    static const uint8_t controlRegister4 = 0x23;
    static const uint8_t controlRegister5 = 0x24;

    mRuntime.i2cInit();
    // Enable z and turn off power down
    writeL3G4200DRegister(controlRegister1, 0b00001100);
    // If you'd like to adjust/use the HPF, you can edit the line below to configure CTRL_REG2
    writeL3G4200DRegister(controlRegister2, 0b00000000);
    // Configure CTRL_REG3 to generate data ready interrupt on INT2
    // No interrupts used on INT1, if you'd like to configure INT1
    // or INT2 otherwise, consult the datasheet
    writeL3G4200DRegister(controlRegister3, 0b00001000);
    // CTRL_REG4 controls the full-scale range, among other things
    writeL3G4200DRegister(controlRegister4, 0b00110000);
    // CTRL_REG5 controls high-pass filtering of outputs, use it if you'd like
    writeL3G4200DRegister(controlRegister5, 0b00000000);

    mAttached = true;
}

int GY50::getOffset(int measurements)
{
    if (measurements <= 0)
    {
        return kError;
    }

    long sum = 0;
    for (auto i = 0; i < measurements; i++)
    {
        sum += getAngularVelocity();
        mRuntime.delayMillis(kMeasurementInterval);
    }

    return sum / measurements;
}

int GY50::getAngularVelocity()
{
    attach();

    static const uint8_t zAxisFirstByteRegister  = 0x2D;
    static const uint8_t zAxisSecondByteRegister = 0x2C;

    auto firstByte = readL3G4200DRegister(zAxisFirstByteRegister);
    // Serial.println("Bytes:");
    // Serial.println(firstByte, BIN);
    auto secondByte = readL3G4200DRegister(zAxisSecondByteRegister);

    return static_cast<int16_t>((firstByte << 8) | secondByte);
}

int GY50::readL3G4200DRegister(uint8_t registerAddress)
{
    mRuntime.i2cBeginTransmission(kGyroAddress);
    mRuntime.i2cWrite(registerAddress);
    mRuntime.i2cEndTransmission();
    mRuntime.i2cRequestFrom(kGyroAddress, 1);

    return mRuntime.i2cAvailable() ? mRuntime.i2cRead() : 0;
}

void GY50::writeL3G4200DRegister(uint8_t registerAddress, uint8_t value)
{
    mRuntime.i2cBeginTransmission(kGyroAddress);
    mRuntime.i2cWrite(registerAddress);
    mRuntime.i2cWrite(value);
    mRuntime.i2cEndTransmission();
}
