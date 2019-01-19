#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "GY50.hpp"
#include "MockRuntime.hpp"

using namespace ::testing;

namespace
{
const uint8_t kGyroAddress = 105;
const int kOffset          = 8;
} // namespace

class GY50BasicTest : public Test
{
public:
    GY50BasicTest(int offset             = kOffset,
                  unsigned long interval = smartcarlib::constants::gy50::kDefaultSamplingInterval)
        : mGyro{ offset, interval, mRuntime }
        , kInterval{ interval }
    {
    }

    NiceMock<MockRuntime> mRuntime;
    GY50 mGyro;
    const unsigned long kInterval;
};

class GY50AttachedTest : public GY50BasicTest
{
public:
    virtual void SetUp()
    {
        // Make sure the sensor is attached so it does not interfer with the
        // rest of our test logic
        mGyro.getOffset(1);
    }
};

class GY50ZeroIntervalTest : public GY50BasicTest
{
public:
    GY50ZeroIntervalTest()
        : GY50BasicTest{ kOffset, 0 }
    {
    }
};

TEST_F(GY50BasicTest, update_WhenCalled_WillSetupSensorViaI2COnce)
{
    uint8_t controlRegister1 = 0x20;
    uint8_t controlRegister2 = 0x21;
    uint8_t controlRegister3 = 0x22;
    uint8_t controlRegister4 = 0x23;
    uint8_t controlRegister5 = 0x24;

    EXPECT_CALL(mRuntime, currentTimeMillis()).Times(2).WillRepeatedly(Return(kInterval + 1));
    EXPECT_CALL(mRuntime, i2cInit());
    // More i2c calls due to `update()`
    EXPECT_CALL(mRuntime, i2cBeginTransmission(kGyroAddress)).Times(AtLeast(1));
    EXPECT_CALL(mRuntime, i2cEndTransmission()).Times(AtLeast(1));
    EXPECT_CALL(mRuntime, i2cWrite(_)).Times(AtLeast(1));
    EXPECT_CALL(mRuntime, i2cWrite(controlRegister1));
    EXPECT_CALL(mRuntime, i2cWrite(controlRegister2));
    EXPECT_CALL(mRuntime, i2cWrite(controlRegister3));
    EXPECT_CALL(mRuntime, i2cWrite(controlRegister4));
    EXPECT_CALL(mRuntime, i2cWrite(controlRegister5));

    mGyro.update();
    mGyro.update();
}

TEST_F(GY50BasicTest, update_WhenNotTimeToRun_WillDoNothing)
{
    EXPECT_CALL(mRuntime, i2cWrite(_)).Times(0);
    EXPECT_CALL(mRuntime, currentTimeMillis()).WillOnce(Return(kInterval));

    mGyro.update();
}

TEST_F(GY50ZeroIntervalTest, update_WhenCalledWithZeroInterval_WillNotDivideByZero)
{
    EXPECT_CALL(mRuntime, currentTimeMillis()).WillOnce(Return(0));
    // As long as this does not crash then this test is good enough
    mGyro.update();
}

TEST_F(GY50AttachedTest, update_WhenI2CNotAvailable_WillNotReadViaI2C)
{
    EXPECT_CALL(mRuntime, currentTimeMillis()).WillOnce(Return(kInterval + 1));
    // Should read 2 registers for MSB and LSB
    EXPECT_CALL(mRuntime, i2cBeginTransmission(kGyroAddress)).Times(2);
    EXPECT_CALL(mRuntime, i2cWrite(_)).Times(2);
    EXPECT_CALL(mRuntime, i2cEndTransmission()).Times(2);
    EXPECT_CALL(mRuntime, i2cRequestFrom(kGyroAddress, 1)).Times(2);
    EXPECT_CALL(mRuntime, i2cAvailable()).Times(2).WillRepeatedly(Return(0));
    EXPECT_CALL(mRuntime, i2cRead()).Times(0);

    mGyro.update();
}

TEST_F(GY50AttachedTest, update_WhenTimeToRun_WillGetAngularVelocityViaI2C)
{
    EXPECT_CALL(mRuntime, currentTimeMillis()).WillOnce(Return(kInterval + 1));
    // Should read 2 registers for MSB and LSB
    EXPECT_CALL(mRuntime, i2cBeginTransmission(kGyroAddress)).Times(2);
    EXPECT_CALL(mRuntime, i2cWrite(_)).Times(2);
    EXPECT_CALL(mRuntime, i2cEndTransmission()).Times(2);
    EXPECT_CALL(mRuntime, i2cRequestFrom(kGyroAddress, 1)).Times(2);
    EXPECT_CALL(mRuntime, i2cAvailable()).Times(2).WillRepeatedly(Return(1));
    EXPECT_CALL(mRuntime, i2cRead()).Times(2);

    mGyro.update();
}

TEST_F(GY50AttachedTest, getHeading_WhenI2CNotAvailable_WillReturnZeroDegrees)
{
    ON_CALL(mRuntime, currentTimeMillis()).WillByDefault(Return(kInterval + 1));
    ON_CALL(mRuntime, i2cAvailable()).WillByDefault(Return(0));
    EXPECT_CALL(mRuntime, i2cRead()).Times(0);

    mGyro.update();
    EXPECT_EQ(0, mGyro.getHeading());
}

TEST_F(GY50AttachedTest, getHeading_WhenAngularVelocityRead_WillReturnCorrectDistance)
{
    ON_CALL(mRuntime, currentTimeMillis()).WillByDefault(Return(kInterval + 1));
    ON_CALL(mRuntime, i2cAvailable()).WillByDefault(Return(1));
    EXPECT_CALL(mRuntime, i2cRead()).Times(2).WillOnce(Return(0)).WillOnce(Return(-200));

    mGyro.update();
    EXPECT_EQ(1, mGyro.getHeading());
}

TEST_F(GY50AttachedTest, getHeading_WhenAngularVelocityReadOverTime_WillReturnCorrectDistance)
{
    const int step       = kInterval + 1;
    int currentIteration = 0;
    auto timer           = [&step, &currentIteration]() { return ++currentIteration * step; };
    ON_CALL(mRuntime, currentTimeMillis()).WillByDefault(InvokeWithoutArgs(timer));
    ON_CALL(mRuntime, i2cAvailable()).WillByDefault(Return(1));
    EXPECT_CALL(mRuntime, i2cRead())
        .Times(4)
        .WillOnce(Return(0))
        .WillOnce(Return(-100))
        .WillOnce(Return(0))
        .WillOnce(Return(-100));

    mGyro.update();
    EXPECT_EQ(0, mGyro.getHeading());

    mGyro.update();
    EXPECT_EQ(1, mGyro.getHeading());
}

TEST_F(GY50AttachedTest, getHeading_WhenTooLowAngularVelocityRead_WillNotUpdateHeading)
{
    const int gyroThreshold            = 12;
    const unsigned int expectedHeading = 1;
    const int step                     = kInterval + 1;
    int currentIteration               = 0;
    auto timer = [&step, &currentIteration]() { return ++currentIteration * step; };
    ON_CALL(mRuntime, currentTimeMillis()).WillByDefault(InvokeWithoutArgs(timer));
    ON_CALL(mRuntime, i2cAvailable()).WillByDefault(Return(1));
    EXPECT_CALL(mRuntime, i2cRead()).Times(2).WillOnce(Return(0)).WillOnce(Return(-200));

    mGyro.update();
    EXPECT_EQ(expectedHeading, mGyro.getHeading());

    // No matter how many measurements are under the threshold the reading shouldn't change
    for (auto i = 0; i < 200; i++)
    {
        EXPECT_CALL(mRuntime, i2cRead())
            .WillOnce(Return(0))
            .WillOnce(Return(gyroThreshold + kOffset));
        mGyro.update();
    }
    EXPECT_EQ(expectedHeading, mGyro.getHeading());
}

TEST_F(GY50AttachedTest, getHeading_WhenNegativeAngularVelocity_WillReturnComplimentaryDegrees)
{
    ON_CALL(mRuntime, currentTimeMillis()).WillByDefault(Return(kInterval + 1));
    ON_CALL(mRuntime, i2cAvailable()).WillByDefault(Return(1));
    EXPECT_CALL(mRuntime, i2cRead()).Times(2).WillOnce(Return(0)).WillOnce(Return(200));

    mGyro.update();
    EXPECT_EQ(359, mGyro.getHeading()); // -1 + 360
}

TEST_F(GY50AttachedTest, getOffset_WhenInvalidArgument_WillReturnError)
{
    EXPECT_CALL(mRuntime, i2cWrite(_)).Times(0);

    EXPECT_EQ(mGyro.getOffset(0), smartcarlib::constants::gy50::kError);
}

TEST_F(GY50BasicTest, getOffset_WhenCalled_WillSetupSensorViaI2COnce)
{
    EXPECT_CALL(mRuntime, i2cInit()).Times(1);

    mGyro.getOffset(1);
    mGyro.getOffset(1);
}

TEST_F(GY50BasicTest, getOffset_WhenCalled_WillReturnCorrectAverageOfMeasurements)
{
    unsigned int measurements = 100;
    int msb                   = 0;
    int highMeasurement       = 560;
    int lowMeasurement        = -120;
    int expectedOffset        = (lowMeasurement + highMeasurement) / 2;
    int step                  = 0;
    int token                 = 0;
    auto mockReader           = [&step, &token, highMeasurement, lowMeasurement, msb]() {
        auto measurement
            = step++ % 2 == 0 ? msb : (token++ % 2 == 0 ? highMeasurement : lowMeasurement);
        return measurement;
    };
    ON_CALL(mRuntime, i2cAvailable()).WillByDefault(Return(1));

    EXPECT_CALL(mRuntime, i2cRead())
        .Times(measurements * 2) // Two i2c calls per run
        .WillRepeatedly(InvokeWithoutArgs(mockReader));

    EXPECT_EQ(mGyro.getOffset(measurements), expectedOffset);
}
