#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "MockRuntime.hpp"
#include "Odometer.hpp"

using namespace ::testing;

namespace
{
const auto kErrorReading                   = static_cast<unsigned long>(-1);
const unsigned long kDefaultPulsesPerMeter = 100;
const int8_t kNotAnInterrupt               = -1;
const int8_t kAnInterrupt                  = 1;
const uint8_t kRisingEdge                  = 1;
const uint8_t kInput                       = 0;
const unsigned long kMinimumPulseGap       = 700;
} // namespace

class OdometerBasicTest : public Test
{
public:
    OdometerBasicTest(unsigned long pulsesPerMeter = kDefaultPulsesPerMeter)
        : mOdometer(mRuntime, pulsesPerMeter)
    {
    }

    NiceMock<MockRuntime> mRuntime;
    Odometer mOdometer;
};

class OdometerNotAttachedTest : public OdometerBasicTest
{
public:
    OdometerNotAttachedTest()
        : OdometerBasicTest(kDefaultPulsesPerMeter)
    {
    }

    virtual void SetUp()
    {
        ON_CALL(mRuntime, pinToInterrupt(_)).WillByDefault(Return(kNotAnInterrupt));
        mOdometer.attach(1, []() {});
    }
};

class OdometerAttachedTest : public OdometerBasicTest
{
public:
    OdometerAttachedTest()
        : OdometerBasicTest(kDefaultPulsesPerMeter)
    {
    }

    virtual void SetUp()
    {
        ON_CALL(mRuntime, pinToInterrupt(_)).WillByDefault(Return(kAnInterrupt));
        mOdometer.attach(1, []() {});
    }
};

TEST_F(OdometerBasicTest, attach_WhenInvalidPin_WillReturnFalse)
{
    EXPECT_CALL(mRuntime, pinToInterrupt(_)).WillOnce(Return(kNotAnInterrupt));
    EXPECT_CALL(mRuntime, setInterrupt(_, _, _)).Times(0);

    EXPECT_FALSE(mOdometer.attach(0, []() {}));
}

TEST_F(OdometerBasicTest, attach_WhenValidInterruptPin_WillSetUpPinAndInterrupt)
{
    static auto counter          = 0; // Declared as `static` for captureless lambda
    auto pin                     = 1;
    auto interruptServiceRoutine = []() { counter++; };
    void (*expectedIsr)();

    EXPECT_CALL(mRuntime, setPinDirection(pin, kInput));
    EXPECT_CALL(mRuntime, pinToInterrupt(_)).WillOnce(Return(kAnInterrupt));
    EXPECT_CALL(mRuntime, setInterrupt(pin, _, kRisingEdge)).WillOnce(SaveArg<1>(&expectedIsr));

    EXPECT_TRUE(mOdometer.attach(pin, interruptServiceRoutine));

    // Check if the correct callback was passed
    auto expectedCounterValue = counter + 1;
    expectedIsr();
    EXPECT_EQ(expectedCounterValue, counter);
}

TEST_F(OdometerNotAttachedTest, getDistance_WhenNotAttached_WillReturnError)
{
    EXPECT_CALL(mRuntime, currentTimeMicros()).Times(0);
    EXPECT_EQ(mOdometer.getDistance(), kErrorReading);
}

TEST_F(OdometerAttachedTest, getDistance_WhenCalled_WillReturnCorrectDistance)
{
    // Simulate the triggering of the callback for as many pulses constitute a meter
    unsigned long oneMillisecondInterval = 1000;
    for (auto i = 0; i < kDefaultPulsesPerMeter; i++)
    {
        // Simulate pulses at 1 millisecond interval
        EXPECT_CALL(mRuntime, currentTimeMicros())
            .WillOnce(Return((i + 1) * oneMillisecondInterval));
        mOdometer.update();
    }

    // The expected distance in cm is (pulses/pulsesPerMeter) * centimetersPerMeter
    unsigned long expectedDistance = 100;
    EXPECT_EQ(mOdometer.getDistance(), expectedDistance);
}

TEST_F(OdometerNotAttachedTest, getSpeed_WhenNotAttached_WillReturnError)
{
    EXPECT_CALL(mRuntime, currentTimeMicros()).Times(0);
    EXPECT_FLOAT_EQ(mOdometer.getSpeed(), 0);
}

TEST_F(OdometerAttachedTest, getSpeed_WhenNoPulses_WillNotCrashAndReturnZero)
{
    // Verify we did not divide by the current dT which is 0
    EXPECT_FLOAT_EQ(mOdometer.getSpeed(), 0);
}

TEST_F(OdometerAttachedTest, getSpeed_WhenCalled_WillReturnCorrectSpeed)
{
    unsigned long firstPulse  = 1000;
    unsigned long secondPulse = 21000;
    {
        InSequence seq;
        EXPECT_CALL(mRuntime, currentTimeMicros()).WillOnce(Return(firstPulse));
        EXPECT_CALL(mRuntime, currentTimeMicros()).WillOnce(Return(secondPulse));
    }
    mOdometer.update();
    mOdometer.update();

    // The expected speed in m/s is millisecondsInSecond * (millimetersPerPulse) / dt
    // where millimetersPerPulse is millimetersInMeter / pulsesPerMeter
    float expectedSpeed = 0.5;
    EXPECT_FLOAT_EQ(mOdometer.getSpeed(), expectedSpeed);
}

TEST_F(OdometerAttachedTest, reset_WhenCalled_WillSetSpeedAndDistanceToZero)
{
    unsigned long firstPulse  = 1000;
    unsigned long secondPulse = 21000;
    {
        InSequence seq;
        EXPECT_CALL(mRuntime, currentTimeMicros()).WillOnce(Return(firstPulse));
        EXPECT_CALL(mRuntime, currentTimeMicros()).WillOnce(Return(secondPulse));
    }
    mOdometer.update();
    mOdometer.update();
    mOdometer.reset();

    EXPECT_FLOAT_EQ(mOdometer.getSpeed(), 0);
    EXPECT_EQ(mOdometer.getDistance(), 0);
}

TEST_F(OdometerNotAttachedTest, update_WhenNotAttached_WillDoNothing)
{
    EXPECT_CALL(mRuntime, currentTimeMicros()).Times(0);
    mOdometer.update();
}

TEST_F(OdometerAttachedTest, update_WhenPulsesTooClose_WillIgnorePulses)
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
    mOdometer.update();
    mOdometer.update();
    auto initialSpeed    = mOdometer.getSpeed();
    auto initialDistance = mOdometer.getDistance();
    // Third pulse arrives too fast and thus should be ignored
    mOdometer.update();

    EXPECT_FLOAT_EQ(mOdometer.getSpeed(), initialSpeed);
    EXPECT_EQ(mOdometer.getDistance(), initialDistance);
}

TEST_F(OdometerAttachedTest, update_WhenFirstPulseArrives_WillCalculateDistanceButNotSpeed)
{
    unsigned long firstPulse = 2000;
    EXPECT_CALL(mRuntime, currentTimeMicros()).WillOnce(Return(firstPulse));
    mOdometer.update();

    auto expectedDistance = 1;
    EXPECT_EQ(mOdometer.getDistance(), expectedDistance);
    EXPECT_FLOAT_EQ(mOdometer.getSpeed(), 0);
}

TEST_F(OdometerAttachedTest, update_WhenFirstPulseArrivesFast_WillCalculateDistanceButNotSpeed)
{
    unsigned long firstPulse = kMinimumPulseGap - 1;
    EXPECT_CALL(mRuntime, currentTimeMicros()).WillOnce(Return(firstPulse));
    mOdometer.update();

    auto expectedDistance = 1;
    EXPECT_EQ(mOdometer.getDistance(), expectedDistance);
    EXPECT_FLOAT_EQ(mOdometer.getSpeed(), 0);
}
