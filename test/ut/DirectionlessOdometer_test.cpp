#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <memory>

#include "DirectionlessOdometer.hpp"
#include "MockRuntime.hpp"

using namespace ::testing;
using namespace smartcarlib::constants::odometer;

namespace
{
const int8_t kNotAnInterrupt = -1;
const int8_t kAnInterrupt    = 1;
const uint8_t kRisingEdge    = 3;
const uint8_t kInput         = 0;
const uint8_t kPin           = 23;
const auto kDummyCallback    = []() {};

struct MockOdometerUser
{
    MOCK_METHOD(void, update, ());
};
} // namespace

class DirectionlessOdometerConstructorTest : public Test
{
public:
    NiceMock<MockRuntime> mRuntime;
};

class DirectionlessOdometerBasicTest : public Test
{
public:
    DirectionlessOdometerBasicTest(unsigned long pulsesPerMeter = kDefaultPulsesPerMeter)
        : kPulsesPerMeter{ pulsesPerMeter }
    {
    }

    void SetUp()
    {
        EXPECT_CALL(mRuntime, pinToInterrupt(kPin)).Times(2).WillRepeatedly(Return(kAnInterrupt));
        mDirectionlessOdometer = std::make_unique<DirectionlessOdometer>(
            kPin, kDummyCallback, kDefaultPulsesPerMeter, mRuntime);
    }

    const unsigned long kPulsesPerMeter;
    NiceMock<MockRuntime> mRuntime;
    std::unique_ptr<DirectionlessOdometer> mDirectionlessOdometer;
};

class DirectionlessOdometerNotAttachedTest : public Test
{
public:
    virtual void SetUp()
    {
        EXPECT_CALL(mRuntime, pinToInterrupt(_)).Times(2).WillRepeatedly(Return(kNotAnInterrupt));
        mDirectionlessOdometer = std::make_unique<DirectionlessOdometer>(
            kPin, kDummyCallback, kDefaultPulsesPerMeter, mRuntime);
    }

    NiceMock<MockRuntime> mRuntime;
    std::unique_ptr<DirectionlessOdometer> mDirectionlessOdometer;
};

TEST_F(DirectionlessOdometerConstructorTest, constructor_WhenCalled_WillSetInterrupt)
{
    const auto inputState = 123;
    EXPECT_CALL(mRuntime, getInputState()).WillOnce(Return(inputState));
    const auto interruptPin = 66;
    EXPECT_CALL(mRuntime, pinToInterrupt(kPin)).Times(2).WillRepeatedly(Return(interruptPin));
    const auto risingEdge = 1;
    EXPECT_CALL(mRuntime, getRisingEdgeMode()).WillOnce(Return(risingEdge));
    EXPECT_CALL(mRuntime, setPinDirection(kPin, inputState));
    EXPECT_CALL(mRuntime, setInterrupt(interruptPin, _, risingEdge));
    DirectionlessOdometer odometer(kPin, kDummyCallback, kDefaultPulsesPerMeter, mRuntime);
}

TEST_F(DirectionlessOdometerConstructorTest, constructor_WhenCalled_WillPassCorrectInterrupt)
{
    static MockOdometerUser odometerUser;
    EXPECT_CALL(odometerUser, update());
    EXPECT_CALL(mRuntime, setInterrupt(_, _, _)).WillOnce(InvokeArgument<1>());
    DirectionlessOdometer odometer(
        kPin, []() { odometerUser.update(); }, kDefaultPulsesPerMeter, mRuntime);
}

TEST(DirectionlessOdometerBadPulsesPerMeter,
     constructor_WhenCalledWithZeroPulsesPerMeter_WillNotDivideByZero)
{
    // Providing `0` as the argument to pulses per meter should not cause the constructor
    // to crash due to a division by zero
    NiceMock<MockRuntime> mRuntime;
    const auto badPulsesPerMeter = 0;
    EXPECT_NO_THROW(DirectionlessOdometer(kPin, kDummyCallback, badPulsesPerMeter, mRuntime));
}

TEST_F(DirectionlessOdometerNotAttachedTest, getDistance_WhenNotAttached_WillReturnError)
{
    EXPECT_CALL(mRuntime, currentTimeMicros()).Times(0);
    EXPECT_EQ(mDirectionlessOdometer->getDistance(), kNotAttachedError);
}

TEST_F(DirectionlessOdometerBasicTest, getDistance_WhenCalled_WillReturnCorrectDistance)
{
    // Simulate the triggering of the callback for as many pulses constitute a meter
    unsigned long oneMillisecondInterval = 1000;
    for (unsigned long i = 0; i < kDefaultPulsesPerMeter; i++)
    {
        // Simulate pulses at 1 millisecond interval
        EXPECT_CALL(mRuntime, currentTimeMicros())
            .WillOnce(Return((i + 1) * oneMillisecondInterval));
        mDirectionlessOdometer->update();
    }

    // The expected distance in cm is (pulses/pulsesPerMeter) * centimetersPerMeter
    unsigned long expectedDistance = 100;
    EXPECT_EQ(mDirectionlessOdometer->getDistance(), expectedDistance);
}

TEST_F(DirectionlessOdometerNotAttachedTest, getSpeed_WhenNotAttached_WillReturnError)
{
    EXPECT_CALL(mRuntime, currentTimeMicros()).Times(0);
    EXPECT_FLOAT_EQ(mDirectionlessOdometer->getSpeed(), kNotAttachedError);
}

TEST_F(DirectionlessOdometerBasicTest, getSpeed_WhenNoPulses_WillNotCrashAndReturnZero)
{
    // Verify we did not divide by the current dT which is 0
    EXPECT_FLOAT_EQ(mDirectionlessOdometer->getSpeed(), 0);
}

TEST_F(DirectionlessOdometerBasicTest, getSpeed_WhenCalled_WillReturnCorrectSpeed)
{
    unsigned long firstPulse  = 1000;
    unsigned long secondPulse = 21000;
    {
        InSequence seq;
        EXPECT_CALL(mRuntime, currentTimeMicros()).WillOnce(Return(firstPulse));
        EXPECT_CALL(mRuntime, currentTimeMicros()).WillOnce(Return(secondPulse));
    }
    mDirectionlessOdometer->update();
    mDirectionlessOdometer->update();

    // The expected speed in m/s is millisecondsInSecond * (millimetersPerPulse) / dt
    // where millimetersPerPulse is millimetersInMeter / pulsesPerMeter
    float expectedSpeed = 0.5;
    EXPECT_FLOAT_EQ(mDirectionlessOdometer->getSpeed(), expectedSpeed);
}

TEST_F(DirectionlessOdometerBasicTest, reset_WhenCalled_WillSetSpeedAndDistanceToZero)
{
    unsigned long firstPulse  = 1000;
    unsigned long secondPulse = 21000;
    {
        InSequence seq;
        EXPECT_CALL(mRuntime, currentTimeMicros()).WillOnce(Return(firstPulse));
        EXPECT_CALL(mRuntime, currentTimeMicros()).WillOnce(Return(secondPulse));
    }
    mDirectionlessOdometer->update();
    mDirectionlessOdometer->update();
    mDirectionlessOdometer->reset();

    EXPECT_FLOAT_EQ(mDirectionlessOdometer->getSpeed(), 0);
    EXPECT_EQ(mDirectionlessOdometer->getDistance(), 0);
}

TEST_F(DirectionlessOdometerBasicTest, update_WhenPulsesTooClose_WillIgnorePulses)
{
    unsigned long firstPulse         = 1000;
    unsigned long secondPulse        = 21000;
    unsigned long thirdPulseTooClose = 21100;
    {
        InSequence seq;
        EXPECT_CALL(mRuntime, currentTimeMicros()).WillOnce(Return(firstPulse));
        EXPECT_CALL(mRuntime, currentTimeMicros()).WillOnce(Return(secondPulse));
        EXPECT_CALL(mRuntime, currentTimeMicros()).WillOnce(Return(thirdPulseTooClose));
    }
    // Two pulses arrive well-spaced from each other
    mDirectionlessOdometer->update();
    mDirectionlessOdometer->update();
    auto initialSpeed    = mDirectionlessOdometer->getSpeed();
    auto initialDistance = mDirectionlessOdometer->getDistance();
    // Third pulse arrives too fast and thus should be ignored
    mDirectionlessOdometer->update();

    EXPECT_FLOAT_EQ(mDirectionlessOdometer->getSpeed(), initialSpeed);
    EXPECT_EQ(mDirectionlessOdometer->getDistance(), initialDistance);
}

TEST_F(DirectionlessOdometerBasicTest,
       update_WhenFirstPulseArrives_WillCalculateDistanceButNotSpeed)
{
    unsigned long firstPulse = 2000;
    EXPECT_CALL(mRuntime, currentTimeMicros()).WillOnce(Return(firstPulse));
    mDirectionlessOdometer->update();

    auto expectedDistance = 1;
    EXPECT_EQ(mDirectionlessOdometer->getDistance(), expectedDistance);
    EXPECT_FLOAT_EQ(mDirectionlessOdometer->getSpeed(), 0);
}

TEST_F(DirectionlessOdometerBasicTest,
       update_WhenFirstPulseArrivesFast_WillCalculateDistanceButNotSpeed)
{
    unsigned long firstPulse = kMinimumPulseGap - 1;
    EXPECT_CALL(mRuntime, currentTimeMicros()).WillOnce(Return(firstPulse));
    mDirectionlessOdometer->update();

    auto expectedDistance = 1;
    EXPECT_EQ(mDirectionlessOdometer->getDistance(), expectedDistance);
    EXPECT_FLOAT_EQ(mDirectionlessOdometer->getSpeed(), 0);
}

TEST_F(DirectionlessOdometerNotAttachedTest, isAttached_WhenNotAttached_WillReturnFalse)
{
    EXPECT_FALSE(mDirectionlessOdometer->isAttached());
}

TEST_F(DirectionlessOdometerBasicTest, isAttached_WhenAttached_WillReturnTrue)
{
    EXPECT_TRUE(mDirectionlessOdometer->isAttached());
}

TEST_F(DirectionlessOdometerBasicTest, providesDirection_WhenCalled_WillReturnFalse)
{
    EXPECT_FALSE(mDirectionlessOdometer->providesDirection());
}
