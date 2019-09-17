#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "DirectionlessOdometer.hpp"
#include "MockRuntime.hpp"

using namespace ::testing;
using namespace smartcarlib::constants::odometer;

namespace
{
const int8_t kNotAnInterrupt         = -1;
const int8_t kAnInterrupt            = 1;
const uint8_t kRisingEdge            = 3;
const uint8_t kInput                 = 0;
const unsigned long kMinimumPulseGap = 700;
} // namespace

class DirectionlessOdometerBasicTest : public Test
{
public:
    DirectionlessOdometerBasicTest(unsigned long pulsesPerMeter = kDefaultPulsesPerMeter)
        : mDirectionlessOdometer(pulsesPerMeter, mRuntime)
    {
    }

    NiceMock<MockRuntime> mRuntime;
    DirectionlessOdometer mDirectionlessOdometer;
};

class DirectionlessOdometerNotAttachedTest : public DirectionlessOdometerBasicTest
{
public:
    DirectionlessOdometerNotAttachedTest()
        : DirectionlessOdometerBasicTest(kDefaultPulsesPerMeter)
    {
    }

    virtual void SetUp()
    {
        ON_CALL(mRuntime, pinToInterrupt(_)).WillByDefault(Return(kNotAnInterrupt));
        mDirectionlessOdometer.attach(1, []() {});
    }
};

class DirectionlessOdometerAttachedTest : public DirectionlessOdometerBasicTest
{
public:
    virtual void SetUp()
    {
        ON_CALL(mRuntime, pinToInterrupt(_)).WillByDefault(Return(kAnInterrupt));
        mDirectionlessOdometer.attach(1, []() {});
    }
};

class DirectionlessOdometerBadPulsesPerMeter : public DirectionlessOdometerBasicTest
{
public:
    DirectionlessOdometerBadPulsesPerMeter()
        : DirectionlessOdometerBasicTest(0)
    {
    }
};

TEST_F(DirectionlessOdometerBadPulsesPerMeter,
       constructor_WhenCalledWithZeroPulsesPerMeter_WillNotDivideByZero)
{
    // Providing `0` as the argument to pulses per meter should not cause the constructor
    // to crash due to a division by zero
}

TEST_F(DirectionlessOdometerBasicTest, attach_WhenInvalidPin_WillReturnFalse)
{
    EXPECT_CALL(mRuntime, pinToInterrupt(_)).WillOnce(Return(kNotAnInterrupt));
    EXPECT_CALL(mRuntime, setInterrupt(_, _, _)).Times(0);

    EXPECT_FALSE(mDirectionlessOdometer.attach(0, []() {}));
}

TEST_F(DirectionlessOdometerBasicTest, attach_WhenValidInterruptPin_WillSetUpPinAndInterrupt)
{
    static auto counter          = 0; // Declared as `static` for captureless lambda
    auto pin                     = 1;
    auto interruptServiceRoutine = []() { counter++; };
    void (*expectedIsr)();

    EXPECT_CALL(mRuntime, setPinDirection(pin, kInput));
    EXPECT_CALL(mRuntime, pinToInterrupt(_)).WillOnce(Return(kAnInterrupt));
    EXPECT_CALL(mRuntime, setInterrupt(pin, _, kRisingEdge)).WillOnce(SaveArg<1>(&expectedIsr));

    EXPECT_TRUE(mDirectionlessOdometer.attach(pin, interruptServiceRoutine));

    // Check if the correct callback was passed
    auto expectedCounterValue = counter + 1;
    expectedIsr();
    EXPECT_EQ(expectedCounterValue, counter);
}

TEST_F(DirectionlessOdometerNotAttachedTest, getDistance_WhenNotAttached_WillReturnError)
{
    EXPECT_CALL(mRuntime, currentTimeMicros()).Times(0);
    EXPECT_EQ(mDirectionlessOdometer.getDistance(), kNotAttachedError);
}

TEST_F(DirectionlessOdometerAttachedTest, getDistance_WhenCalled_WillReturnCorrectDistance)
{
    // Simulate the triggering of the callback for as many pulses constitute a meter
    unsigned long oneMillisecondInterval = 1000;
    for (unsigned long i = 0; i < kDefaultPulsesPerMeter; i++)
    {
        // Simulate pulses at 1 millisecond interval
        EXPECT_CALL(mRuntime, currentTimeMicros())
            .WillOnce(Return((i + 1) * oneMillisecondInterval));
        mDirectionlessOdometer.update();
    }

    // The expected distance in cm is (pulses/pulsesPerMeter) * centimetersPerMeter
    unsigned long expectedDistance = 100;
    EXPECT_EQ(mDirectionlessOdometer.getDistance(), expectedDistance);
}

TEST_F(DirectionlessOdometerNotAttachedTest, getSpeed_WhenNotAttached_WillReturnError)
{
    EXPECT_CALL(mRuntime, currentTimeMicros()).Times(0);
    EXPECT_FLOAT_EQ(mDirectionlessOdometer.getSpeed(), kNotAttachedError);
}

TEST_F(DirectionlessOdometerAttachedTest, getSpeed_WhenNoPulses_WillNotCrashAndReturnZero)
{
    // Verify we did not divide by the current dT which is 0
    EXPECT_FLOAT_EQ(mDirectionlessOdometer.getSpeed(), 0);
}

TEST_F(DirectionlessOdometerAttachedTest, getSpeed_WhenCalled_WillReturnCorrectSpeed)
{
    unsigned long firstPulse  = 1000;
    unsigned long secondPulse = 21000;
    {
        InSequence seq;
        EXPECT_CALL(mRuntime, currentTimeMicros()).WillOnce(Return(firstPulse));
        EXPECT_CALL(mRuntime, currentTimeMicros()).WillOnce(Return(secondPulse));
    }
    mDirectionlessOdometer.update();
    mDirectionlessOdometer.update();

    // The expected speed in m/s is millisecondsInSecond * (millimetersPerPulse) / dt
    // where millimetersPerPulse is millimetersInMeter / pulsesPerMeter
    float expectedSpeed = 0.5;
    EXPECT_FLOAT_EQ(mDirectionlessOdometer.getSpeed(), expectedSpeed);
}

TEST_F(DirectionlessOdometerAttachedTest, reset_WhenCalled_WillSetSpeedAndDistanceToZero)
{
    unsigned long firstPulse  = 1000;
    unsigned long secondPulse = 21000;
    {
        InSequence seq;
        EXPECT_CALL(mRuntime, currentTimeMicros()).WillOnce(Return(firstPulse));
        EXPECT_CALL(mRuntime, currentTimeMicros()).WillOnce(Return(secondPulse));
    }
    mDirectionlessOdometer.update();
    mDirectionlessOdometer.update();
    mDirectionlessOdometer.reset();

    EXPECT_FLOAT_EQ(mDirectionlessOdometer.getSpeed(), 0);
    EXPECT_EQ(mDirectionlessOdometer.getDistance(), 0);
}

TEST_F(DirectionlessOdometerNotAttachedTest, update_WhenNotAttached_WillDoNothing)
{
    EXPECT_CALL(mRuntime, currentTimeMicros()).Times(0);
    mDirectionlessOdometer.update();
}

TEST_F(DirectionlessOdometerAttachedTest, update_WhenPulsesTooClose_WillIgnorePulses)
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
    mDirectionlessOdometer.update();
    mDirectionlessOdometer.update();
    auto initialSpeed    = mDirectionlessOdometer.getSpeed();
    auto initialDistance = mDirectionlessOdometer.getDistance();
    // Third pulse arrives too fast and thus should be ignored
    mDirectionlessOdometer.update();

    EXPECT_FLOAT_EQ(mDirectionlessOdometer.getSpeed(), initialSpeed);
    EXPECT_EQ(mDirectionlessOdometer.getDistance(), initialDistance);
}

TEST_F(DirectionlessOdometerAttachedTest,
       update_WhenFirstPulseArrives_WillCalculateDistanceButNotSpeed)
{
    unsigned long firstPulse = 2000;
    EXPECT_CALL(mRuntime, currentTimeMicros()).WillOnce(Return(firstPulse));
    mDirectionlessOdometer.update();

    auto expectedDistance = 1;
    EXPECT_EQ(mDirectionlessOdometer.getDistance(), expectedDistance);
    EXPECT_FLOAT_EQ(mDirectionlessOdometer.getSpeed(), 0);
}

TEST_F(DirectionlessOdometerAttachedTest,
       update_WhenFirstPulseArrivesFast_WillCalculateDistanceButNotSpeed)
{
    unsigned long firstPulse = kMinimumPulseGap - 1;
    EXPECT_CALL(mRuntime, currentTimeMicros()).WillOnce(Return(firstPulse));
    mDirectionlessOdometer.update();

    auto expectedDistance = 1;
    EXPECT_EQ(mDirectionlessOdometer.getDistance(), expectedDistance);
    EXPECT_FLOAT_EQ(mDirectionlessOdometer.getSpeed(), 0);
}

TEST_F(DirectionlessOdometerNotAttachedTest, isAttached_WhenNotAttached_WillReturnFalse)
{
    EXPECT_FALSE(mDirectionlessOdometer.isAttached());
}

TEST_F(DirectionlessOdometerAttachedTest, isAttached_WhenAttached_WillReturnTrue)
{
    EXPECT_TRUE(mDirectionlessOdometer.isAttached());
}

TEST_F(DirectionlessOdometerBasicTest, providesDirection_WhenCalled_WillReturnFalse)
{
    EXPECT_FALSE(mDirectionlessOdometer.providesDirection());
}
