#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "MockControl.hpp"
#include "SimpleCar.hpp"

using namespace ::testing;
using namespace smartcarlib::constants::control;

class SimpleCarTest : public Test
{
public:
    SimpleCarTest()
        : mSimpleCar{ mControl }
    {
    }

    NiceMock<MockControl> mControl;
    SimpleCar mSimpleCar;
};

TEST_F(SimpleCarTest, setSpeed_WhenTooHighSpeed_WillSetMaxSpeed)
{
    float speed       = kMaxControlSpeed + 1;
    int expectedSpeed = kMaxControlSpeed;
    EXPECT_CALL(mControl, setSpeed(expectedSpeed));

    mSimpleCar.setSpeed(speed);
}

TEST_F(SimpleCarTest, setSpeed_WhenTooLowSpeed_WillSetMinSpeed)
{
    float speed       = kMinControlSpeed - 1;
    int expectedSpeed = kMinControlSpeed;
    EXPECT_CALL(mControl, setSpeed(expectedSpeed));

    mSimpleCar.setSpeed(speed);
}

TEST_F(SimpleCarTest, setSpeed_WhenValidSpeed_WillSetCorrectSpeed)
{
    float speed       = 45.2f;
    int expectedSpeed = static_cast<int>(speed);
    EXPECT_CALL(mControl, setSpeed(expectedSpeed));

    mSimpleCar.setSpeed(speed);
}

TEST_F(SimpleCarTest, setAngle_WhenTooLargeAngle_WillSetMaxAngle)
{
    int angle = kMaxControlAngle + 1;
    EXPECT_CALL(mControl, setAngle(kMaxControlAngle));

    mSimpleCar.setAngle(angle);
}

TEST_F(SimpleCarTest, setAngle_WhenTooLow_Angle_WillSetMinAngle)
{
    int angle = kMinControlAngle - 1;
    EXPECT_CALL(mControl, setAngle(kMinControlAngle));

    mSimpleCar.setAngle(angle);
}

TEST_F(SimpleCarTest, setAngle_WhenValidAngle_WillSetCorrectAngle)
{
    int angle = -23;
    EXPECT_CALL(mControl, setAngle(angle));

    mSimpleCar.setAngle(angle);
}

TEST_F(SimpleCarTest, overrideMotorSpeed_WhenOutOfBounds_WillSetCorrectSpeed)
{
    int firstMotorSpeed  = kMaxControlSpeed + 1;
    int secondMotorSpeed = kMinControlSpeed - 1;
    EXPECT_CALL(mControl, overrideMotorSpeed(kMaxControlSpeed, kMinControlSpeed));

    mSimpleCar.overrideMotorSpeed(firstMotorSpeed, secondMotorSpeed);
}

TEST_F(SimpleCarTest, overrideMotorSpeed_WhenCalled_WillSetCorrectSpeed)
{
    int firstMotorSpeed  = 50;
    int secondMotorSpeed = -20;
    EXPECT_CALL(mControl, overrideMotorSpeed(firstMotorSpeed, secondMotorSpeed));

    mSimpleCar.overrideMotorSpeed(firstMotorSpeed, secondMotorSpeed);
}
