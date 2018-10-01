#include <algorithm>
#include <utility>
#include <vector>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "MockRuntime.hpp"
#include "SRF08.hpp"

using namespace ::testing;

namespace
{
const auto kErrorReading              = static_cast<unsigned int>(-1);
const uint8_t kRangingCommandRegister = 0x00;
const uint8_t kRangingInCm            = 0x51;
const uint8_t kFirstEchoHighByte      = 0x02;
} // namespace

class SRF08Test : public Test
{
public:
    SRF08Test()
        : mSRF08{ mRuntime }
    {
    }

    std::pair<uint8_t, uint8_t> readingToBytes(unsigned int reading)
    {
        return std::make_pair(reading >> 8, reading & 0xFF);
    }

    /**
     * Helper function to set expectations for the `getMedianDistance` test cases
     * @param readings The expected readings
     */
    void setGetMedianDistanceExpectations(const std::vector<unsigned int>& readings)
    {
        EXPECT_CALL(mRuntime, i2cAvailable()).WillRepeatedly(Return(1));

        {
            InSequence seq;
            for (auto reading : readings)
            {
                auto measurementBytes = readingToBytes(reading);
                EXPECT_CALL(mRuntime, i2cRead()).WillOnce(Return(measurementBytes.first));
                EXPECT_CALL(mRuntime, i2cRead()).WillOnce(Return(measurementBytes.second));
            }
        }
    }

    NiceMock<MockRuntime> mRuntime;
    SRF08 mSRF08;
};

TEST_F(SRF08Test, getDistance_WhenBusNotAvailable_WillReturnError)
{
    EXPECT_CALL(mRuntime, i2cAvailable()).WillOnce(Return(0));

    EXPECT_EQ(mSRF08.getDistance(), kErrorReading);
}

TEST_F(SRF08Test, getDistance_WhenBusAvailable_WillReturnCorrectDistance)
{
    auto expectedReading = 267;
    auto expectedBytes   = readingToBytes(expectedReading);
    Sequence rangingSequence, readingSequence;

    EXPECT_CALL(mRuntime, i2cAvailable()).WillOnce(Return(1));

    EXPECT_CALL(mRuntime, i2cWrite(kRangingCommandRegister)).InSequence(rangingSequence);
    EXPECT_CALL(mRuntime, i2cWrite(kRangingInCm)).InSequence(rangingSequence);
    EXPECT_CALL(mRuntime, i2cWrite(kFirstEchoHighByte)).InSequence(rangingSequence);

    EXPECT_CALL(mRuntime, i2cRead()).InSequence(readingSequence).WillOnce(Return(expectedBytes.first));
    EXPECT_CALL(mRuntime, i2cRead()).InSequence(readingSequence).WillOnce(Return(expectedBytes.second));

    EXPECT_EQ(mSRF08.getDistance(), expectedReading);
}

TEST_F(SRF08Test, getMedianDistance_WhenNoIterations_WillReturnError)
{
    EXPECT_EQ(mSRF08.getMedianDistance(0), kErrorReading);
}

TEST_F(SRF08Test, getMedianDistance_WhenCalled_WillReturnCorrectDistance)
{
    auto iterations             = 5;
    unsigned int expectedMedian = 25;

    // Expect the same median regardless of the order of readings
    std::vector<unsigned int> readings{ 12, expectedMedian, 0, 55, 300 };
    setGetMedianDistanceExpectations(readings);
    EXPECT_EQ(mSRF08.getMedianDistance(iterations), expectedMedian);

    // Sort ascending
    std::sort(readings.begin(), readings.end());
    setGetMedianDistanceExpectations(readings);
    EXPECT_EQ(mSRF08.getMedianDistance(iterations), expectedMedian);

    // Sort descending
    std::reverse(readings.begin(), readings.end());
    setGetMedianDistanceExpectations(readings);
    EXPECT_EQ(mSRF08.getMedianDistance(iterations), expectedMedian);
}

TEST_F(SRF08Test, attach_WhenCalled_WillInitializeBus)
{
    uint8_t address = 115;
    EXPECT_CALL(mRuntime, i2cInit());
    EXPECT_CALL(mRuntime, constrainValue(address, _, _)).WillOnce(Return(address));

    EXPECT_EQ(mSRF08.attach(address), address);
}

TEST_F(SRF08Test, setGain_WhenCalled_WillSetGainRegister)
{
    uint8_t gainValue    = 10;
    uint8_t gainRegister = 0x01;

    {
        InSequence seq;
        EXPECT_CALL(mRuntime, i2cWrite(gainRegister));
        EXPECT_CALL(mRuntime, constrainValue(gainValue, _, _)).WillOnce(Return(gainValue));
        EXPECT_CALL(mRuntime, i2cWrite(gainValue));
    }

    mSRF08.setGain(gainValue);
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

TEST_F(SRF08Test, setPingDelay_WhenCalled_WillReturnPingDelay)
{
    unsigned long milliseconds = 100;

    EXPECT_EQ(mSRF08.setPingDelay(milliseconds), milliseconds);
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

TEST_F(SRF08Test, changeAddress_WhenCalled_WillChangeAddress)
{
    uint8_t firstInChangeAddressSequence  = 0xA0;
    uint8_t secondInChangeAddressSequence = 0xAA;
    uint8_t thirdInChangeAddressSequence  = 0xA5;
    uint8_t newAddress                    = 125;

    {
        InSequence seq;
        EXPECT_CALL(mRuntime, constrainValue(newAddress, _, _)).WillOnce(Return(newAddress));

        EXPECT_CALL(mRuntime, i2cWrite(kRangingCommandRegister));
        EXPECT_CALL(mRuntime, i2cWrite(firstInChangeAddressSequence));

        EXPECT_CALL(mRuntime, i2cWrite(kRangingCommandRegister));
        EXPECT_CALL(mRuntime, i2cWrite(secondInChangeAddressSequence));

        EXPECT_CALL(mRuntime, i2cWrite(kRangingCommandRegister));
        EXPECT_CALL(mRuntime, i2cWrite(thirdInChangeAddressSequence));

        EXPECT_CALL(mRuntime, i2cWrite(kRangingCommandRegister));
        EXPECT_CALL(mRuntime, i2cWrite(newAddress << 1));
    }

    EXPECT_EQ(mSRF08.changeAddress(newAddress), newAddress);
}
