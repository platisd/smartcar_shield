#include "DirectionalOdometer.hpp"
#include "MockRuntime.hpp"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace ::testing;
using namespace smartcarlib::constants::odometer;

namespace
{
const uint8_t kInput               = 0;
const int8_t kAnInterrupt          = 1;
const uint8_t kDirectionPin        = 5;
const uint8_t kPinStateWhenForward = 1;
const int8_t kNotAnInterrupt       = -1;
} // namespace

class DirectionalOdometerBasicTest : public Test
{
public:
    DirectionalOdometerBasicTest()
        : mDirectionalOdometer(
            kDirectionPin, kPinStateWhenForward, kDefaultPulsesPerMeter, mRuntime)
    {
    }

    NiceMock<MockRuntime> mRuntime;
    DirectionalOdometer mDirectionalOdometer;
};

class DirectionalOdometerAttachedTest : public DirectionalOdometerBasicTest
{
public:
    DirectionalOdometerAttachedTest()
        : mCounter{ 0 }
    {
    }

    virtual void SetUp()
    {
        // Attach the sensor
        uint8_t pulsePin = 13;
        ON_CALL(mRuntime, pinToInterrupt(_)).WillByDefault(Return(kAnInterrupt));
        mDirectionalOdometer.attach(pulsePin, []() {});

        // Calls to microseconds timer return values at 1 millisecond intervals
        auto timer = [this]() {
            mCounter += 1000;
            return mCounter;
        };
        ON_CALL(mRuntime, currentTimeMicros()).WillByDefault(InvokeWithoutArgs(timer));
    }

    unsigned long mCounter;
};

TEST_F(DirectionalOdometerBasicTest, attach_WhenInvalidPin_WillNotSetAnyPinDirections)
{
    EXPECT_CALL(mRuntime, pinToInterrupt(_)).WillOnce(Return(kNotAnInterrupt));
    EXPECT_CALL(mRuntime, setPinDirection(_, _)).Times(0);

    EXPECT_FALSE(mDirectionalOdometer.attach(0, []() {}));
}

TEST_F(DirectionalOdometerBasicTest, attach_WhenCalled_WillSetDirectionPin)
{
    uint8_t pulsePin = 1;
    EXPECT_CALL(mRuntime, pinToInterrupt(_)).WillOnce(Return(kAnInterrupt));
    EXPECT_CALL(mRuntime, setPinDirection(pulsePin, kInput));
    EXPECT_CALL(mRuntime, setPinDirection(kDirectionPin, kInput));

    EXPECT_TRUE(mDirectionalOdometer.attach(pulsePin, []() {}));
}

TEST_F(DirectionalOdometerAttachedTest, update_WhenPinStateNotForward_WillRegisterNegativeDistance)
{
    auto numberOfPulses = 1000;
    EXPECT_CALL(mRuntime, getPinState(kDirectionPin))
        .Times(numberOfPulses)
        .WillRepeatedly(Return(!kPinStateWhenForward)); // Return backward direction
    for (auto i = 0; i < numberOfPulses; i++)
    {
        mDirectionalOdometer.update();
    }

    // The absolute distance has the same value but opposite sign to the relative distance
    // since we have only been moving backward
    EXPECT_LT(mDirectionalOdometer.getDistance(), 0);
}

TEST_F(DirectionalOdometerAttachedTest, update_WhenPinStateForward_WillNotRegisterNegativeDistance)
{
    auto numberOfPulses = 400;
    EXPECT_CALL(mRuntime, getPinState(kDirectionPin))
        .Times(numberOfPulses)
        .WillRepeatedly(Return(kPinStateWhenForward)); // Return forward direction
    for (auto i = 0; i < numberOfPulses; i++)
    {
        mDirectionalOdometer.update();
    }

    // The absolute distance should be the same as the relative distance as we have
    // only been moving forward
    EXPECT_GT(mDirectionalOdometer.getDistance(), 0);
}

TEST_F(DirectionalOdometerBasicTest, getDistance_WhenNotAttached_WillReturnError)
{
    EXPECT_CALL(mRuntime, currentTimeMicros()).Times(0);
    EXPECT_EQ(mDirectionalOdometer.getDistance(), kNotAttachedError);
}

TEST_F(DirectionalOdometerAttachedTest, getDistance_WhenCalled_WillReturnCorrectDistance)
{
    auto numberOfPulses                = 400;
    int currentPulse                   = 0;
    auto equalForwardAndBackwardPulses = [&currentPulse, numberOfPulses]() {
        return currentPulse < numberOfPulses / 2 ? kPinStateWhenForward : !kPinStateWhenForward;
    };
    EXPECT_CALL(mRuntime, getPinState(kDirectionPin))
        .Times(numberOfPulses)
        .WillRepeatedly(InvokeWithoutArgs(equalForwardAndBackwardPulses));

    for (currentPulse = 0; currentPulse < numberOfPulses; currentPulse++)
    {
        mDirectionalOdometer.update();
    }

    // The relative distance should be `0` as we have been moving forward as much as backward
    EXPECT_EQ(mDirectionalOdometer.getDistance(), 0);
}

TEST_F(DirectionalOdometerAttachedTest, getDirection_WhenPinStateForward_WillReturnForward)
{
    EXPECT_CALL(mRuntime, getPinState(kDirectionPin)).WillOnce(Return(kPinStateWhenForward));

    mDirectionalOdometer.update();

    EXPECT_EQ(mDirectionalOdometer.getDirection(), smartcarlib::constants::odometer::kForward);
}

TEST_F(DirectionalOdometerAttachedTest, getDirection_WhenPinStateBackward_WillReturnBackward)
{
    EXPECT_CALL(mRuntime, getPinState(kDirectionPin)).WillOnce(Return(!kPinStateWhenForward));

    mDirectionalOdometer.update();

    EXPECT_EQ(mDirectionalOdometer.getDirection(), smartcarlib::constants::odometer::kBackward);
}

TEST_F(DirectionalOdometerAttachedTest, reset_WhenCalled_WillSetSpeedAndDistanceToZero)
{
    unsigned long firstPulse  = 1000;
    unsigned long secondPulse = 21000;
    {
        InSequence seq;
        EXPECT_CALL(mRuntime, currentTimeMicros()).WillOnce(Return(firstPulse));
        EXPECT_CALL(mRuntime, currentTimeMicros()).WillOnce(Return(secondPulse));
    }
    mDirectionalOdometer.update();
    mDirectionalOdometer.update();
    mDirectionalOdometer.reset();

    EXPECT_FLOAT_EQ(mDirectionalOdometer.getSpeed(), 0);
    EXPECT_EQ(mDirectionalOdometer.getDistance(), 0);
}

TEST_F(DirectionalOdometerBasicTest, getSpeed_WhenNotAttached_WillReturnError)
{
    EXPECT_CALL(mRuntime, currentTimeMicros()).Times(0);
    EXPECT_FLOAT_EQ(mDirectionalOdometer.getSpeed(), kNotAttachedError);
}

TEST_F(DirectionalOdometerAttachedTest, getSpeed_WhenCalled_WillReturnCorrectSpeed)
{
    unsigned long firstPulse  = 1000;
    unsigned long secondPulse = 21000;
    EXPECT_CALL(mRuntime, getPinState(kDirectionPin))
        .Times(2)
        .WillRepeatedly(Return(!kPinStateWhenForward));
    {
        InSequence seq;
        EXPECT_CALL(mRuntime, currentTimeMicros()).WillOnce(Return(firstPulse));
        EXPECT_CALL(mRuntime, currentTimeMicros()).WillOnce(Return(secondPulse));
    }
    mDirectionalOdometer.update();
    mDirectionalOdometer.update();

    // The expected speed in m/s is millisecondsInSecond * (millimetersPerPulse) / dt
    // where millimetersPerPulse is millimetersInMeter / pulsesPerMeter
    float expectedSpeed = -0.5;
    EXPECT_FLOAT_EQ(mDirectionalOdometer.getSpeed(), expectedSpeed);
}

TEST_F(DirectionalOdometerBasicTest, providesDirection_WhenCalled_WillReturnTrue)
{
    EXPECT_TRUE(mDirectionalOdometer.providesDirection());
}
