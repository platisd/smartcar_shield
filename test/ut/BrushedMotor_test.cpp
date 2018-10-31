#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "BrushedMotor.hpp"
#include "MockRuntime.hpp"

using namespace ::testing;

namespace
{
const uint8_t kForwardPin   = 3;
const uint8_t kBackwardPin  = 4;
const uint8_t kEnablePin    = 5;
const uint8_t kLow          = 0;
const uint8_t kHigh         = 1;
const int kMinSpeed         = -100;
const int kMaxSpeed         = 100;
const int kMinAbsoluteSpeed = 0;
const uint8_t kMinPwm       = 0;
const uint8_t kMaxPwm       = 255;
} // namespace

class BrushedMotorTest : public Test
{
public:
    BrushedMotorTest()
        : mBrushedMotor{ kForwardPin, kBackwardPin, kEnablePin, mRuntime }
    {
    }

    NiceMock<MockRuntime> mRuntime;
    BrushedMotor mBrushedMotor;
};

TEST_F(BrushedMotorTest, setSpeed_WhenNotAttached_WillAttach)
{
    const uint8_t output = 1;

    EXPECT_CALL(mRuntime, setPinDirection(kForwardPin, output));
    EXPECT_CALL(mRuntime, setPinDirection(kBackwardPin, output));
    EXPECT_CALL(mRuntime, setPinDirection(kEnablePin, output));

    mBrushedMotor.setSpeed(-100);
}

TEST_F(BrushedMotorTest, setSpeed_WhenNegativeSpeed_WillSetDirectionBackward)
{
    EXPECT_CALL(mRuntime, setPinState(kForwardPin, kLow));
    EXPECT_CALL(mRuntime, setPinState(kBackwardPin, kHigh));

    mBrushedMotor.setSpeed(-50);
}

TEST_F(BrushedMotorTest, setSpeed_WhenPositiveSpeed_WillSetDirectionForward)
{
    EXPECT_CALL(mRuntime, setPinState(kForwardPin, kHigh));
    EXPECT_CALL(mRuntime, setPinState(kBackwardPin, kLow));

    mBrushedMotor.setSpeed(50);
}

TEST_F(BrushedMotorTest, setSpeed_WhenCalled_WillSetMappedValueAsPwm)
{
    long expectedPwm = 200;
    EXPECT_CALL(mRuntime, mapValue(_, _, _, _, _)).WillOnce(Return(expectedPwm));
    EXPECT_CALL(mRuntime, setPWM(kEnablePin, expectedPwm));

    mBrushedMotor.setSpeed(12);
}

TEST_F(BrushedMotorTest, setSpeed_WhenSpeedOverBounds_WillSeMaxSpeedForward)
{
    auto expectedSpeed = kMaxSpeed;
    EXPECT_CALL(mRuntime, setPinState(kForwardPin, kHigh));
    EXPECT_CALL(mRuntime, setPinState(kBackwardPin, kLow));
    EXPECT_CALL(mRuntime, mapValue(expectedSpeed, kMinAbsoluteSpeed, kMaxSpeed, kMinPwm, kMaxPwm));

    mBrushedMotor.setSpeed(kMaxSpeed + 1);
}

TEST_F(BrushedMotorTest, setSpeed_WhenSpeedUnderBounds_WillSeMaxSpeedBackward)
{
    auto expectedSpeed = kMaxSpeed;
    EXPECT_CALL(mRuntime, setPinState(kForwardPin, kLow));
    EXPECT_CALL(mRuntime, setPinState(kBackwardPin, kHigh));
    EXPECT_CALL(mRuntime, mapValue(expectedSpeed, kMinAbsoluteSpeed, kMaxSpeed, kMinPwm, kMaxPwm));

    mBrushedMotor.setSpeed(kMinSpeed - 1);
}

TEST_F(BrushedMotorTest, setSpeed_WhenSpeedZero_WillSetZeroSpeed)
{
    auto expectedSpeed = 0;
    EXPECT_CALL(mRuntime, mapValue(expectedSpeed, kMinAbsoluteSpeed, kMaxSpeed, kMinPwm, kMaxPwm));

    mBrushedMotor.setSpeed(0);
}

TEST_F(BrushedMotorTest, setSpeed_WhenSpeedWithinRange_WillSetSuppliedSpeed)
{
    auto expectedSpeed = 55;
    EXPECT_CALL(mRuntime, mapValue(expectedSpeed, kMinAbsoluteSpeed, kMaxSpeed, kMinPwm, kMaxPwm));

    mBrushedMotor.setSpeed(expectedSpeed);
}
