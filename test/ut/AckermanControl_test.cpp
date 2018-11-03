#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "AckermanControl.hpp"
#include "MockMotor.hpp"

using namespace ::testing;
using namespace smartcarlib::constants::control;
using namespace smartcarlib::constants::motor;

class AckerManControlTest : public Test
{
public:
    AckerManControlTest()
        : mAckermanControl{ mSteering, mThrottling }
    {
    }

    StrictMock<MockMotor> mSteering;
    StrictMock<MockMotor> mThrottling;
    AckermanControl mAckermanControl;
};

TEST_F(AckerManControlTest, setAngle_WhenTooHighAngle_WillSetMaxSpeedToSteeringMotor)
{
    EXPECT_CALL(mSteering, setSpeed(kMaxMotorSpeed));

    mAckermanControl.setAngle(kMaxControlAngle + 1);
}

TEST_F(AckerManControlTest, setAngle_WhenTooLowAngle_WillSetMinSpeedToSteeringMotor)
{
    EXPECT_CALL(mSteering, setSpeed(kMinMotorSpeed));

    mAckermanControl.setAngle(kMinMotorSpeed - 1);
}

TEST_F(AckerManControlTest, setAngle_WhenIdleAngle_WillSetIdleSpeedToSteeringMotor)
{
    EXPECT_CALL(mSteering, setSpeed(kIdleMotorSpeed));

    mAckermanControl.setAngle(kIdleControlAngle);
}

TEST_F(AckerManControlTest, setAngle_WhenAngleInRange_WillSetCorrectSpeedToSteeringMotor)
{
    EXPECT_CALL(mSteering, setSpeed(kMaxMotorSpeed / 2));

    mAckermanControl.setAngle(kMaxControlAngle / 2);
}

TEST_F(AckerManControlTest, setSpeed_WhenTooHighSpeed_WillSetMaxSpeedToThrottlingMotor)
{
    EXPECT_CALL(mThrottling, setSpeed(kMaxMotorSpeed));

    mAckermanControl.setSpeed(kMaxMotorSpeed + 1);
}

TEST_F(AckerManControlTest, setSpeed_WhenTooLowSpeed_WillSetMinSpeedToThrottlingMotor)
{
    EXPECT_CALL(mThrottling, setSpeed(kMinMotorSpeed));

    mAckermanControl.setSpeed(kMinMotorSpeed - 1);
}

TEST_F(AckerManControlTest, setSpeed_WhenIdleSpeed_WillSetIdleSpeedToThrottlingMotor)
{
    EXPECT_CALL(mThrottling, setSpeed(kIdleMotorSpeed));

    mAckermanControl.setSpeed(kIdleMotorSpeed);
}

TEST_F(AckerManControlTest, setSpeed_WhenSpeedInRange_WillSetCorrectSpeedToThrottlingMotor)
{
    EXPECT_CALL(mThrottling, setSpeed(kMaxMotorSpeed / 2));

    mAckermanControl.setSpeed(kMaxMotorSpeed / 2);
}
