#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "DifferentialControl.hpp"
#include "MockMotor.hpp"

using namespace ::testing;
using namespace smartcarlib::constants::control;
using namespace smartcarlib::constants::motor;

class DifferentialControlBasicTest : public Test
{
public:
    DifferentialControlBasicTest()
        : mDifferentialControl{ mLeftMotor, mRightMotor }
    {
    }

    NiceMock<MockMotor> mLeftMotor;
    NiceMock<MockMotor> mRightMotor;
    DifferentialControl mDifferentialControl;
};

class DifferentialControlSpeedTest : public DifferentialControlBasicTest
{
public:
    DifferentialControlSpeedTest()
        : kSpeed{ kMaxMotorSpeed }
    {
    }

    virtual void SetUp()
    {
        mDifferentialControl.setSpeed(kSpeed);
    }

    const int kSpeed;
};

TEST_F(DifferentialControlBasicTest, setAngle_WhenSpeedNotSet_WillNotTakeEffectImmediately)
{
    EXPECT_CALL(mLeftMotor, setSpeed(kIdleMotorSpeed));
    EXPECT_CALL(mRightMotor, setSpeed(kIdleMotorSpeed));

    mDifferentialControl.setAngle(kMaxControlAngle);
}

TEST_F(DifferentialControlSpeedTest, setAngle_WhenTooLowAngle_WillSetSpeedOnlyToRightMotor)
{
    EXPECT_CALL(mLeftMotor, setSpeed(kIdleMotorSpeed));
    EXPECT_CALL(mRightMotor, setSpeed(kSpeed));

    mDifferentialControl.setAngle(kMinControlAngle - 1);
}

TEST_F(DifferentialControlSpeedTest, setAngle_WhenTooHighAngle_WillSetSpeedOnlyToLeftMotor)
{
    EXPECT_CALL(mLeftMotor, setSpeed(kSpeed));
    EXPECT_CALL(mRightMotor, setSpeed(kIdleMotorSpeed));

    mDifferentialControl.setAngle(kMaxControlAngle + 1);
}

TEST_F(DifferentialControlSpeedTest, setAngle_WhenIdleAngle_WillSetSameSpeedToMotors)
{
    EXPECT_CALL(mLeftMotor, setSpeed(kSpeed));
    EXPECT_CALL(mRightMotor, setSpeed(kSpeed));

    mDifferentialControl.setAngle(kIdleControlAngle);
}

TEST_F(DifferentialControlSpeedTest, setAngle_WhenValidAngle_WillSetCorrectSpeedToMotors)
{
    EXPECT_CALL(mLeftMotor, setSpeed(kSpeed));
    EXPECT_CALL(mRightMotor, setSpeed(kSpeed * 2 / 3.0f));

    mDifferentialControl.setAngle(kMaxControlAngle / 3);
}

TEST_F(DifferentialControlBasicTest, setSpeed_WhenTooHighSpeed_WillSetMaxSpeedToMotors)
{
    EXPECT_CALL(mLeftMotor, setSpeed(kMaxMotorSpeed));
    EXPECT_CALL(mRightMotor, setSpeed(kMaxMotorSpeed));

    mDifferentialControl.setSpeed(kMaxMotorSpeed + 1);
}

TEST_F(DifferentialControlBasicTest, setSpeed_WhenTooLowSpeed_WillSetMinSpeedToMotors)
{
    EXPECT_CALL(mLeftMotor, setSpeed(kMinMotorSpeed));
    EXPECT_CALL(mRightMotor, setSpeed(kMinMotorSpeed));

    mDifferentialControl.setSpeed(kMinMotorSpeed - 1);
}

TEST_F(DifferentialControlBasicTest, setSpeed_WhenIdleSpeed_WillSetIdleSpeedToMotors)
{
    EXPECT_CALL(mLeftMotor, setSpeed(kIdleMotorSpeed));
    EXPECT_CALL(mRightMotor, setSpeed(kIdleMotorSpeed));

    mDifferentialControl.setSpeed(kIdleMotorSpeed);
}

TEST_F(DifferentialControlBasicTest, setSpeed_WhenValidSpeed_WillSetCorrectSpeedToMotors)
{
    EXPECT_CALL(mLeftMotor, setSpeed(kMinMotorSpeed / 2));
    EXPECT_CALL(mRightMotor, setSpeed(kMinMotorSpeed / 2));

    mDifferentialControl.setSpeed(kMinMotorSpeed / 2);
}

TEST_F(DifferentialControlBasicTest, setSpeed_WhenAngleSet_WillSetCorrectSpeedToMotors)
{
    EXPECT_CALL(mLeftMotor, setSpeed(kIdleMotorSpeed));
    EXPECT_CALL(mRightMotor, setSpeed(kIdleMotorSpeed));

    mDifferentialControl.setAngle(kMaxControlAngle / 2);

    EXPECT_CALL(mLeftMotor, setSpeed(kMaxMotorSpeed));
    EXPECT_CALL(mRightMotor, setSpeed(kMaxMotorSpeed / 2));

    mDifferentialControl.setSpeed(kMaxMotorSpeed);
}
