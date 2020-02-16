#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <memory>

#include "MockRuntime.hpp"
#include "SR04.hpp"

using namespace ::testing;
using namespace smartcarlib::constants::sr04;

namespace
{
const uint8_t kInput                        = 0;
const uint8_t kOutput                       = 1;
const uint8_t kLow                          = 0;
const uint8_t kHigh                         = 1;
const unsigned int kMaxDistance             = 100;
const auto kTimeToTravelOneCmAndBack        = 29.15 * 2;
const unsigned long kTimeToMeasureOneCm     = 120;
const unsigned long kMedianMeasurementDelay = 15;
const uint8_t kTriggerPin                   = 5;
const uint8_t kEchoPin                      = 6;
} // namespace

class SR04Test : public Test
{
public:
    SR04Test(unsigned int maxDistance = kMaxDistance)
    {
        EXPECT_CALL(mRuntime, getOutputState()).WillOnce(Return(kOutput));
        EXPECT_CALL(mRuntime, getInputState()).WillOnce(Return(kInput));
        EXPECT_CALL(mRuntime, getLowState()).WillOnce(Return(kLow));
        EXPECT_CALL(mRuntime, getHighState()).WillOnce(Return(kHigh));

        mSR04 = std::make_unique<SR04>(kTriggerPin, kEchoPin, maxDistance, mRuntime);
    }

    NiceMock<MockRuntime> mRuntime;
    std::unique_ptr<SR04> mSR04;
};

class SR04BadMaxDistanceTest : public SR04Test
{
public:
    SR04BadMaxDistanceTest()
        : SR04Test{ 0 }
    {
    }
};

TEST_F(SR04Test, getDistance_WhenSensorNotAttached_WillSetPinDirectionsOnce)
{
    EXPECT_CALL(mRuntime, setPinDirection(kTriggerPin, kOutput));
    EXPECT_CALL(mRuntime, setPinDirection(kEchoPin, kInput));

    mSR04->getDistance();
    mSR04->getDistance();
}

TEST_F(SR04Test, getDistance_WhenCalled_WillMeasureCorrectly)
{
    unsigned long pulseLength     = 200;
    unsigned int expectedDistance = pulseLength / kTimeToTravelOneCmAndBack;
    unsigned long expectedTimeout = kMaxDistance * kTimeToMeasureOneCm;

    {
        InSequence measurementSequence;
        EXPECT_CALL(mRuntime, setPinState(kTriggerPin, kLow));
        EXPECT_CALL(mRuntime, delayMicros(5));
        EXPECT_CALL(mRuntime, setPinState(kTriggerPin, kHigh));
        EXPECT_CALL(mRuntime, delayMicros(10));
        EXPECT_CALL(mRuntime, setPinState(kTriggerPin, kLow));
        EXPECT_CALL(mRuntime, getPulseDuration(kEchoPin, kHigh, expectedTimeout))
            .WillOnce(Return(pulseLength));
    }

    EXPECT_EQ(mSR04->getDistance(), expectedDistance);
}

TEST_F(SR04Test, getDistance_WhenCalculatedDistanceEqualToMaxDistance_WillReturnMaxDistance)
{
    unsigned long pulseLength     = kMaxDistance * kTimeToTravelOneCmAndBack;
    unsigned int expectedDistance = kMaxDistance;
    unsigned long expectedTimeout = kMaxDistance * kTimeToMeasureOneCm;

    {
        InSequence measurementSequence;
        EXPECT_CALL(mRuntime, setPinState(kTriggerPin, kLow));
        EXPECT_CALL(mRuntime, delayMicros(5));
        EXPECT_CALL(mRuntime, setPinState(kTriggerPin, kHigh));
        EXPECT_CALL(mRuntime, delayMicros(10));
        EXPECT_CALL(mRuntime, setPinState(kTriggerPin, kLow));
        EXPECT_CALL(mRuntime, getPulseDuration(kEchoPin, kHigh, expectedTimeout))
            .WillOnce(Return(pulseLength));
    }

    EXPECT_EQ(mSR04->getDistance(), expectedDistance);
}

TEST_F(SR04Test, getDistance_WhenCalculatedDistanceMoreThanMaxDistance_WillReturnError)
{
    // kMaxDistance + 1 not enough due to rounding down
    unsigned long pulseLength     = (kMaxDistance + 2) * kTimeToTravelOneCmAndBack;
    unsigned int expectedDistance = kError;
    unsigned long expectedTimeout = kMaxDistance * kTimeToMeasureOneCm;

    {
        InSequence measurementSequence;
        EXPECT_CALL(mRuntime, setPinState(kTriggerPin, kLow));
        EXPECT_CALL(mRuntime, delayMicros(5));
        EXPECT_CALL(mRuntime, setPinState(kTriggerPin, kHigh));
        EXPECT_CALL(mRuntime, delayMicros(10));
        EXPECT_CALL(mRuntime, setPinState(kTriggerPin, kLow));
        EXPECT_CALL(mRuntime, getPulseDuration(kEchoPin, kHigh, expectedTimeout))
            .WillOnce(Return(pulseLength));
    }

    EXPECT_EQ(mSR04->getDistance(), expectedDistance);
}

TEST_F(SR04BadMaxDistanceTest, getDistance_WhenBadMaxDistanceSupplied_WillUseDefaultMaxDistance)
{
    unsigned long pulseLength     = 200;
    unsigned int expectedDistance = pulseLength / kTimeToTravelOneCmAndBack;
    unsigned long expectedTimeout = kDefaultMaxDistance * kTimeToMeasureOneCm;

    EXPECT_CALL(mRuntime, getPulseDuration(kEchoPin, kHigh, expectedTimeout))
        .WillOnce(Return(pulseLength));

    EXPECT_EQ(mSR04->getDistance(), expectedDistance);
}

TEST_F(SR04Test, getMedianDistance_WhenNoIterations_WillReturnError)
{
    uint8_t expectedMeasurements = 0;

    EXPECT_CALL(mRuntime, getPulseDuration(_, _, _)).Times(expectedMeasurements);

    EXPECT_EQ(mSR04->getMedianDistance(expectedMeasurements), kError);
}

TEST_F(SR04Test, getMedianDistance_WhenCalled_WillMakeCorrectNumberOfMeasurements)
{
    uint8_t expectedMeasurements = 10;

    EXPECT_CALL(mRuntime, getPulseDuration(_, _, _)).Times(expectedMeasurements);
    EXPECT_CALL(mRuntime, delayMillis(kMedianMeasurementDelay)).Times(expectedMeasurements);

    mSR04->getMedianDistance(expectedMeasurements);
}
