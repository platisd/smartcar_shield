#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "MockRuntime.hpp"
#include "SR04.hpp"

using namespace ::testing;

namespace
{
const auto kErrorReading                    = static_cast<unsigned int>(-1);
const uint8_t kInput                        = 0;
const uint8_t kOutput                       = 1;
const uint8_t kLow                          = 0;
const uint8_t kHigh                         = 1;
const unsigned int kMaxDistance             = 100;
const auto kTimeToTravelOneCmAndBack        = 29.15 * 2;
const unsigned long kMedianMeasurementDelay = 15;
} // namespace

class SR04Test : public Test
{
public:
    SR04Test(unsigned int maxDistance = kMaxDistance)
        : mSR04{ mRuntime, maxDistance }
    {
    }

    NiceMock<MockRuntime> mRuntime;
    SR04 mSR04;
};

class SR04BadMaxDistanceTest : public SR04Test
{
public:
    SR04BadMaxDistanceTest()
        : SR04Test{ 0 }
    {
    }
};

TEST_F(SR04Test, attach_WhenCalled_WillSetPinDirections)
{
    uint8_t triggerPin = 5;
    uint8_t echoPin    = 6;
    EXPECT_CALL(mRuntime, setPinDirection(triggerPin, kOutput));
    EXPECT_CALL(mRuntime, setPinDirection(echoPin, kInput));

    mSR04.attach(triggerPin, echoPin);
}

TEST_F(SR04Test, getDistance_WhenSensorNotAttached_WillReturnError)
{
    EXPECT_CALL(mRuntime, getPulseDuration(_, _, _)).Times(0);

    EXPECT_EQ(mSR04.getDistance(), kErrorReading);
}

TEST_F(SR04Test, getDistance_WhenSensorAttached_WillMeasureCorrectly)
{
    uint8_t triggerPin            = 5;
    uint8_t echoPin               = 6;
    unsigned long pulseLength     = 200;
    unsigned int expectedDistance = pulseLength / kTimeToTravelOneCmAndBack;
    unsigned long expectedTimeout = kMaxDistance * kTimeToTravelOneCmAndBack;

    mSR04.attach(triggerPin, echoPin);
    {
        InSequence measurementSequence;
        EXPECT_CALL(mRuntime, setPinState(triggerPin, kLow));
        EXPECT_CALL(mRuntime, delayMicros(5));
        EXPECT_CALL(mRuntime, setPinState(triggerPin, kHigh));
        EXPECT_CALL(mRuntime, delayMicros(10));
        EXPECT_CALL(mRuntime, setPinState(triggerPin, kLow));
        EXPECT_CALL(mRuntime, getPulseDuration(echoPin, kHigh, expectedTimeout))
            .WillOnce(Return(pulseLength));
    }

    EXPECT_EQ(mSR04.getDistance(), expectedDistance);
}

TEST_F(SR04BadMaxDistanceTest, getDistance_WhenBadMaxDistanceSupplied_WillUseDefaultMaxDistance)
{
    using namespace smartcarlib::constants::sr04;
    uint8_t triggerPin            = 5;
    uint8_t echoPin               = 6;
    unsigned long pulseLength     = 200;
    unsigned int expectedDistance = pulseLength / kTimeToTravelOneCmAndBack;
    unsigned long expectedTimeout = kDefaultMaxDistance * kTimeToTravelOneCmAndBack;

    mSR04.attach(triggerPin, echoPin);
    EXPECT_CALL(mRuntime, getPulseDuration(echoPin, kHigh, expectedTimeout))
        .WillOnce(Return(pulseLength));

    EXPECT_EQ(mSR04.getDistance(), expectedDistance);
}

TEST_F(SR04Test, getMedianDistance_WhenNoIterations_WillReturnError)
{
    uint8_t triggerPin = 5;
    uint8_t echoPin    = 6;
    uint8_t expectedMeasurements = 0;
    mSR04.attach(triggerPin, echoPin);
    EXPECT_CALL(mRuntime, getPulseDuration(_, _, _)).Times(expectedMeasurements);

    EXPECT_EQ(mSR04.getMedianDistance(expectedMeasurements), kErrorReading);
}

TEST_F(SR04Test, getMedianDistance_WhenCalled_WillMakeCorrectNumberOfMeasurements)
{
    uint8_t triggerPin           = 5;
    uint8_t echoPin              = 6;
    uint8_t expectedMeasurements = 10;
    mSR04.attach(triggerPin, echoPin);

    EXPECT_CALL(mRuntime, getPulseDuration(_, _, _)).Times(expectedMeasurements);
    EXPECT_CALL(mRuntime, delayMillis(kMedianMeasurementDelay)).Times(expectedMeasurements);

    mSR04.getMedianDistance(expectedMeasurements);
}
