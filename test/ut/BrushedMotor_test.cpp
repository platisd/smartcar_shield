#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <memory>

#include "BrushedMotor.hpp"
#include "MockRuntime.hpp"

using namespace ::testing;
using namespace smartcarlib::constants::motor;

namespace
{
const uint8_t kForwardPin  = 3;
const uint8_t kBackwardPin = 4;
const uint8_t kEnablePin   = 5;
const uint8_t kLow         = 0;
const uint8_t kHigh        = 1;
const uint8_t kMaxPwm      = 255;
const uint8_t kOutput      = 1;
} // namespace

class BrushedMotorTest : public Test
{
public:
    BrushedMotorTest()
    {
        EXPECT_CALL(mRuntime, getOutputState()).WillOnce(Return(kOutput));
        EXPECT_CALL(mRuntime, getLowState()).WillOnce(Return(kLow));
        EXPECT_CALL(mRuntime, getHighState()).WillOnce(Return(kHigh));

        mBrushedMotor
            = std::make_unique<BrushedMotor>(kForwardPin, kBackwardPin, kEnablePin, mRuntime);
    }

    NiceMock<MockRuntime> mRuntime;
    std::unique_ptr<BrushedMotor> mBrushedMotor;
};

TEST(BrushedMotorHelperConstructorTest,
     BrushedMotorPinsConstructor_WhenCalled_WillCallPrimaryConstructor)
{
    NiceMock<MockRuntime> runtime;
    EXPECT_CALL(runtime, getOutputState()).WillOnce(Return(kOutput));
    EXPECT_CALL(runtime, getLowState()).WillOnce(Return(kLow));
    EXPECT_CALL(runtime, getHighState()).WillOnce(Return(kHigh));

    BrushedMotorPins pins{ kForwardPin, kBackwardPin, kEnablePin };
    BrushedMotor motor{ pins, runtime };
}

TEST_F(BrushedMotorTest, setSpeed_WhenNotAttached_WillAttach)
{
    EXPECT_CALL(mRuntime, setPinDirection(kForwardPin, kOutput));
    EXPECT_CALL(mRuntime, setPinDirection(kBackwardPin, kOutput));
    EXPECT_CALL(mRuntime, setPinDirection(kEnablePin, kOutput));

    mBrushedMotor->setSpeed(-100);
}

TEST_F(BrushedMotorTest, setSpeed_WhenNegativeSpeed_WillSetDirectionBackward)
{
    EXPECT_CALL(mRuntime, setPinState(kForwardPin, kLow));
    EXPECT_CALL(mRuntime, setPinState(kBackwardPin, kHigh));

    mBrushedMotor->setSpeed(-50);
}

TEST_F(BrushedMotorTest, setSpeed_WhenPositiveSpeed_WillSetDirectionForward)
{
    EXPECT_CALL(mRuntime, setPinState(kForwardPin, kHigh));
    EXPECT_CALL(mRuntime, setPinState(kBackwardPin, kLow));

    mBrushedMotor->setSpeed(50);
}

TEST_F(BrushedMotorTest, setSpeed_WhenSpeedOverBounds_WillSetMaxSpeedForward)
{
    EXPECT_CALL(mRuntime, setPinState(kForwardPin, kHigh));
    EXPECT_CALL(mRuntime, setPinState(kBackwardPin, kLow));
    EXPECT_CALL(mRuntime, setPWM(kEnablePin, kMaxPwm));

    mBrushedMotor->setSpeed(kMaxMotorSpeed + 1);
}

TEST_F(BrushedMotorTest, setSpeed_WhenSpeedUnderBounds_WillSetMaxSpeedBackward)
{
    EXPECT_CALL(mRuntime, setPinState(kForwardPin, kLow));
    EXPECT_CALL(mRuntime, setPinState(kBackwardPin, kHigh));
    EXPECT_CALL(mRuntime, setPWM(kEnablePin, kMaxPwm));

    mBrushedMotor->setSpeed(kMinMotorSpeed - 1);
}

TEST_F(BrushedMotorTest, setSpeed_WhenSpeedIdle_WillSetZeroSpeed)
{
    EXPECT_CALL(mRuntime, setPWM(kEnablePin, 0));

    mBrushedMotor->setSpeed(kIdleMotorSpeed);
}

TEST_F(BrushedMotorTest, setSpeed_WhenSpeedWithinRange_WillSetSuppliedSpeed)
{
    EXPECT_CALL(mRuntime, setPWM(kEnablePin, kMaxPwm / 2));

    mBrushedMotor->setSpeed(kMaxMotorSpeed / 2);
}
