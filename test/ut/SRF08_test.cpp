#include <algorithm>
#include <utility>
#include <vector>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "MockRuntime.hpp"
#include "SRF08.hpp"

using namespace ::testing;
using namespace smartcarlib::constants::distanceSensor;

namespace
{
const auto kErrorReading              = static_cast<unsigned int>(-1);
const uint8_t kRangingCommandRegister = 0x00;
const uint8_t kRangingInCm            = 0x51;
const uint8_t kFirstEchoHighByte      = 0x02;
const uint8_t kAddress                = 112;
} // namespace

class SRF08Test : public Test
{
public:
    SRF08Test()
        : mSRF08{ mRuntime, kAddress }
    {
    }

    /**
     * Breaks down a distance reading into a pair of bytes
     * @param reading The distance to break down
     * @return        Pair of bytes making up the distance
     */
    std::pair<uint8_t, uint8_t> readingToBytes(unsigned int reading)
    {
        return std::make_pair(reading >> 8, reading & 0xFF);
    }

    NiceMock<MockRuntime> mRuntime;
    SRF08 mSRF08;
};

TEST_F(SRF08Test, getDistance_WhenBusNotAvailable_WillReturnError)
{
    EXPECT_CALL(mRuntime, i2cAvailable()).WillOnce(Return(0));

    EXPECT_EQ(mSRF08.getDistance(), kErrorReading);
}

TEST_F(SRF08Test, getDistance_WhenI2cNotInitialized_WillInitializeBusOnce)
{
    EXPECT_CALL(mRuntime, i2cInit());

    mSRF08.getDistance();
    mSRF08.getDistance();
}

TEST_F(SRF08Test, getDistance_WhenBusAvailable_WillReturnCorrectDistance)
{
    unsigned int expectedReading = 267;
    auto expectedBytes           = readingToBytes(expectedReading);
    Sequence rangingSequence, readingSequence;

    EXPECT_CALL(mRuntime, i2cAvailable()).WillOnce(Return(1));

    EXPECT_CALL(mRuntime, i2cWrite(kRangingCommandRegister)).InSequence(rangingSequence);
    EXPECT_CALL(mRuntime, i2cWrite(kRangingInCm)).InSequence(rangingSequence);
    EXPECT_CALL(mRuntime, i2cWrite(kFirstEchoHighByte)).InSequence(rangingSequence);

    EXPECT_CALL(mRuntime, i2cRead())
        .InSequence(readingSequence)
        .WillOnce(Return(expectedBytes.first));
    EXPECT_CALL(mRuntime, i2cRead())
        .InSequence(readingSequence)
        .WillOnce(Return(expectedBytes.second));

    EXPECT_EQ(mSRF08.getDistance(), expectedReading);
}

TEST_F(SRF08Test, getMedianDistance_WhenNoIterations_WillReturnError)
{
    uint8_t expectedMeasurements = 0;
    EXPECT_CALL(mRuntime, i2cRead()).Times(expectedMeasurements);

    EXPECT_EQ(mSRF08.getMedianDistance(expectedMeasurements), kErrorReading);
}

TEST_F(SRF08Test, getMedianDistance_WhenTooManyIterations_WillReturnError)
{
    uint8_t expectedMeasurements = kMaxMedianMeasurements + 1;
    EXPECT_CALL(mRuntime, i2cRead()).Times(0);

    EXPECT_EQ(mSRF08.getMedianDistance(expectedMeasurements), kErrorReading);
}

TEST_F(SRF08Test, getMedianDistance_WhenCalled_WillMakeCorrectNumberOfMeasurements)
{
    using namespace smartcarlib::constants::srf08;
    uint8_t expectedMeasurements = 10;
    EXPECT_CALL(mRuntime, i2cAvailable()).WillRepeatedly(Return(1));
    EXPECT_CALL(mRuntime, i2cRead())
        .Times(expectedMeasurements * 2); // Two bytes for each measurement

    mSRF08.getMedianDistance(expectedMeasurements);
}

TEST_F(SRF08Test, setGain_WhenCalled_WillSetGainRegister)
{
    uint8_t gainValue    = 10;
    uint8_t gainRegister = 0x01;

    {
        InSequence seq;
        EXPECT_CALL(mRuntime, i2cWrite(gainRegister));
        EXPECT_CALL(mRuntime, i2cWrite(gainValue));
    }

    mSRF08.setGain(gainValue);
}

TEST_F(SRF08Test, setGain_WhenI2cNotInitialized_WillInitializeBusOnce)
{
    EXPECT_CALL(mRuntime, i2cInit());

    mSRF08.setGain(0);
    mSRF08.setGain(0);
}

TEST_F(SRF08Test, setRange_WhenCalled_WillSetRange)
{
    uint8_t rangeRegister = 0x02;
    uint8_t rangeValue    = 40;

    {
        InSequence seq;
        EXPECT_CALL(mRuntime, i2cWrite(rangeRegister));
        EXPECT_CALL(mRuntime, i2cWrite(rangeValue));
    }

    mSRF08.setRange(rangeValue);
}

TEST_F(SRF08Test, setRange_WhenI2cNotInitialized_WillInitializeBusOnce)
{
    EXPECT_CALL(mRuntime, i2cInit());

    mSRF08.setRange(0);
    mSRF08.setRange(0);
}

TEST_F(SRF08Test, setPingDelay_WhenCalled_WillReturnPingDelay)
{
    unsigned long milliseconds = 100;

    EXPECT_EQ(mSRF08.setPingDelay(milliseconds), milliseconds);
}

TEST_F(SRF08Test, getLightReading_WhenI2cNotInitialized_WillInitializeBusOnce)
{
    EXPECT_CALL(mRuntime, i2cInit());

    mSRF08.getLightReading();
    mSRF08.getLightReading();
}

TEST_F(SRF08Test, getLightReading_WhenBusNotAvailable_WillReturnError)
{
    auto errorLightReading = static_cast<uint8_t>(-1);

    EXPECT_CALL(mRuntime, i2cAvailable()).WillOnce(Return(0));
    EXPECT_CALL(mRuntime, i2cRead()).Times(0);

    EXPECT_EQ(mSRF08.getLightReading(), errorLightReading);
}

TEST_F(SRF08Test, getLightReading_WhenBusAvailable_WillReturnLightReading)
{
    uint8_t lightSensorByte        = 0x01;
    uint8_t numberOfBytesToRequest = 1;
    uint8_t expectedLightReading   = 30;

    EXPECT_CALL(mRuntime, i2cAvailable()).WillOnce(Return(1));
    EXPECT_CALL(mRuntime, i2cRead()).WillOnce(Return(expectedLightReading));

    {
        InSequence seq;
        EXPECT_CALL(mRuntime, i2cWrite(kRangingCommandRegister));
        EXPECT_CALL(mRuntime, i2cWrite(kRangingInCm));
        EXPECT_CALL(mRuntime, i2cWrite(lightSensorByte));
        EXPECT_CALL(mRuntime, i2cRequestFrom(_, numberOfBytesToRequest));
    }

    EXPECT_EQ(mSRF08.getLightReading(), expectedLightReading);
}

TEST_F(SRF08Test, changeAddress_WhenI2cNotInitialized_WillInitializeBusOnce)
{
    EXPECT_CALL(mRuntime, i2cInit());

    mSRF08.changeAddress(113);
    mSRF08.changeAddress(114);
}

TEST_F(SRF08Test, changeAddress_WhenCalled_WillChangeAddress)
{
    uint8_t firstInChangeAddressSequence  = 0xA0;
    uint8_t secondInChangeAddressSequence = 0xAA;
    uint8_t thirdInChangeAddressSequence  = 0xA5;
    uint8_t newAddress                    = 125;

    {
        InSequence seq;
        EXPECT_CALL(mRuntime, i2cWrite(kRangingCommandRegister));
        EXPECT_CALL(mRuntime, i2cWrite(firstInChangeAddressSequence));

        EXPECT_CALL(mRuntime, i2cWrite(kRangingCommandRegister));
        EXPECT_CALL(mRuntime, i2cWrite(secondInChangeAddressSequence));

        EXPECT_CALL(mRuntime, i2cWrite(kRangingCommandRegister));
        EXPECT_CALL(mRuntime, i2cWrite(thirdInChangeAddressSequence));

        EXPECT_CALL(mRuntime, i2cWrite(kRangingCommandRegister));
        EXPECT_CALL(mRuntime, i2cWrite(static_cast<uint8_t>(newAddress << 0x01)));
    }

    EXPECT_EQ(mSRF08.changeAddress(newAddress), newAddress);
}
