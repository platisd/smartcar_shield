#include "SRF08.hpp"
#include "../../../../utilities/Utilities.hpp"

namespace
{
// For documentation on addressing, please refer to:
// http://www.robot-electronics.co.uk/htm/srf08tech.html
const uint8_t kFirstAddress           = 112;
const uint8_t kLastAddress            = 127;
const uint8_t kRangingCommandRegister = 0x00;
const uint8_t kRangingInCm            = 0x51;
} // namespace

using namespace smartcarlib::constants::srf08;
using namespace smartcarlib::constants::distanceSensor;
using namespace smartcarlib::utils;

SRF08::SRF08(uint8_t address, Runtime& runtime)
    : mAddress{ address }
    , mRuntime(runtime)
    , mPingDelay{ kDefaultPingDelay }
    , mAttached{ false }
{
}

void SRF08::attach()
{
    if (!mAttached)
    {
        mRuntime.i2cInit();
        mAttached = true;
    }
}

unsigned int SRF08::getDistance()
{
    attach();

    static const uint8_t kFirstEchoHighByte      = 0x02;
    static const uint8_t kNumberOfBytesToRequest = 2;

    measureDistance();

    mRuntime.i2cBeginTransmission(mAddress);
    mRuntime.i2cWrite(kFirstEchoHighByte);
    mRuntime.i2cEndTransmission();

    mRuntime.i2cRequestFrom(mAddress, kNumberOfBytesToRequest);
    if (!mRuntime.i2cAvailable())
    {
        return static_cast<unsigned int>(-1); // Return a large error-like value
    }
    auto high = mRuntime.i2cRead();
    auto low  = mRuntime.i2cRead();

    return static_cast<uint16_t>((high << 8) + low);
}

unsigned int SRF08::getMedianDistance(uint8_t iterations)
{
    if (iterations == 0 || iterations > kMaxMedianMeasurements)
    {
        return static_cast<unsigned int>(-1); // Return a large number to indicate error
    }

    unsigned int measurements[kMaxMedianMeasurements];
    for (auto i = 0; i < iterations; i++)
    {
        measurements[i] = getDistance();
        mRuntime.delayMillis(mPingDelay);
    }

    return getMedian(measurements, iterations);
}

void SRF08::setGain(uint8_t gainValue)
{
    attach();

    static const uint8_t kGainRegister = 0x01;
    static const uint8_t kMinGain      = 0;
    static const uint8_t kMaxGain      = 31;

    mRuntime.i2cBeginTransmission(mAddress);
    mRuntime.i2cWrite(kGainRegister);
    mRuntime.i2cWrite(getConstrain(gainValue, kMinGain, kMaxGain));
    mRuntime.i2cEndTransmission();
}

void SRF08::setRange(uint8_t range)
{
    attach();

    static const uint8_t kRangeRegister = 0x02;

    mRuntime.i2cBeginTransmission(mAddress);
    mRuntime.i2cWrite(kRangeRegister);
    // Max_Range = (range * 3.4) + 3.4 in centimeters
    mRuntime.i2cWrite(range);
    mRuntime.i2cEndTransmission();
}

unsigned long SRF08::setPingDelay(unsigned long milliseconds)
{
    mPingDelay = milliseconds;
    return mPingDelay;
}

uint8_t SRF08::getLightReading()
{
    attach();

    static const uint8_t kLightSensorByte        = 0x01;
    static const uint8_t kNumberOfBytesToRequest = 1;

    // Start a ranging
    measureDistance();

    // Get only the light reading byte
    mRuntime.i2cBeginTransmission(mAddress);
    mRuntime.i2cWrite(kLightSensorByte);
    mRuntime.i2cEndTransmission();
    mRuntime.i2cRequestFrom(mAddress, kNumberOfBytesToRequest);
    mRuntime.delayMillis(mPingDelay);

    return mRuntime.i2cAvailable() ? static_cast<uint8_t>(mRuntime.i2cRead())
                                   : static_cast<uint8_t>(-1);
}

uint8_t SRF08::changeAddress(uint8_t newAddress)
{
    attach();

    static const uint8_t kFirstInChangeAddressSequence  = 0xA0;
    static const uint8_t kSecondInChangeAddressSequence = 0xAA;
    static const uint8_t kThirdInChangeAddressSequence  = 0xA5;

    newAddress = getConstrain(newAddress, kFirstAddress, kLastAddress);

    mRuntime.i2cBeginTransmission(mAddress);
    mRuntime.i2cWrite(kRangingCommandRegister);
    mRuntime.i2cWrite(kFirstInChangeAddressSequence);
    mRuntime.i2cEndTransmission();

    mRuntime.i2cBeginTransmission(mAddress);
    mRuntime.i2cWrite(kRangingCommandRegister);
    mRuntime.i2cWrite(kSecondInChangeAddressSequence);
    mRuntime.i2cEndTransmission();

    mRuntime.i2cBeginTransmission(mAddress);
    mRuntime.i2cWrite(kRangingCommandRegister);
    mRuntime.i2cWrite(kThirdInChangeAddressSequence);
    mRuntime.i2cEndTransmission();

    mRuntime.i2cBeginTransmission(mAddress);
    mRuntime.i2cWrite(kRangingCommandRegister);
    mRuntime.i2cWrite(static_cast<uint8_t>(newAddress << 0x01));
    mRuntime.i2cEndTransmission();

    mAddress = newAddress;

    return mAddress;
}

void SRF08::measureDistance()
{
    mRuntime.i2cBeginTransmission(mAddress);
    mRuntime.i2cWrite(kRangingCommandRegister);
    mRuntime.i2cWrite(kRangingInCm);
    mRuntime.i2cEndTransmission();
    mRuntime.delayMillis(mPingDelay);
}
