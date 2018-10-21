#include "DirectionalOdometer.hpp"
#include "MockRuntime.hpp"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace ::testing;

namespace
{
const uint8_t kInput                       = 0;
const unsigned long kDefaultPulsesPerMeter = 100;
const int8_t kAnInterrupt                  = 1;
const uint8_t kDirectionPin                = 5;
const uint8_t kPinStateWhenForward         = 1;
} // namespace

class DirectionalOdometerBasicTest : public Test
{
public:
    DirectionalOdometerBasicTest()
        : mDirectionalOdometer(
              kDirectionPin, kPinStateWhenForward, mRuntime, kDefaultPulsesPerMeter)
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

TEST_F(DirectionalOdometerBasicTest, attach_WhenCalled_WillSetDirectionPin)
{
    uint8_t pulsePin = 1;
    EXPECT_CALL(mRuntime, pinToInterrupt(_)).WillOnce(Return(kAnInterrupt));
    EXPECT_CALL(mRuntime, setPinDirection(pulsePin, kInput));
    EXPECT_CALL(mRuntime, setPinDirection(kDirectionPin, kInput));

    mDirectionalOdometer.attach(pulsePin, []() {});
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
    EXPECT_EQ(mDirectionalOdometer.getDistance(), -mDirectionalOdometer.getRelativeDistance());
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
    EXPECT_EQ(mDirectionalOdometer.getDistance(), mDirectionalOdometer.getRelativeDistance());
}

TEST_F(DirectionalOdometerAttachedTest, getRelativeDistance_WhenCalled_WillReturnCorrectDistance)
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
    EXPECT_EQ(mDirectionalOdometer.getRelativeDistance(), 0);
}

TEST_F(DirectionalOdometerBasicTest, getDirection_WhenPinStateForward_WillReturnForward)
{
    EXPECT_CALL(mRuntime, getPinState(kDirectionPin)).WillOnce(Return(kPinStateWhenForward));

    EXPECT_EQ(mDirectionalOdometer.getDirection(), smartcarlib::constants::odometry::kForward);
}

TEST_F(DirectionalOdometerBasicTest, getDirection_WhenPinStateBackward_WillReturnBackward)
{
    EXPECT_CALL(mRuntime, getPinState(kDirectionPin)).WillOnce(Return(!kPinStateWhenForward));

    EXPECT_EQ(mDirectionalOdometer.getDirection(), smartcarlib::constants::odometry::kBackward);
}
