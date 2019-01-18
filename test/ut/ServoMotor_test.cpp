#include <memory>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "MockRuntime.hpp"
#include "ServoMotor.hpp"

using namespace ::testing;
using namespace smartcarlib::constants::motor;

namespace
{
const uint8_t kControlPin  = 12;
const int kMinPulseLength  = 800;
const int kIdlePulseLength = 1500;
const int kMaxPulseLength  = 2500;
} // namespace

class ServoMotorTest : public Test
{
public:
    ServoMotorTest()
        : mRuntime{}
        , mMockServo{ std::make_shared<MockServo>() }
        , mServoMotor{ kControlPin, kMinPulseLength, kIdlePulseLength, kMaxPulseLength, mRuntime}
    {
        mServoMotor.mMockServo = mMockServo;
    }

    NiceMock<MockRuntime> mRuntime;
    NiceMock<std::shared_ptr<MockServo>> mMockServo;
    ServoMotor mServoMotor;
};

TEST_F(ServoMotorTest, setSpeed_WhenCalledAndNotAttached_WillAttach)
{
    EXPECT_CALL(*mMockServo, attach(kControlPin, kMinPulseLength, kMaxPulseLength));

    mServoMotor.setSpeed(0);
}

TEST_F(ServoMotorTest, setSpeed_WhenNegativeSpeed_WillWriteCorrectMicroseconds)
{
    int expectedMicroseconds = kMinPulseLength + (kIdlePulseLength - kMinPulseLength) / 2;
    EXPECT_CALL(*mMockServo, writeMicroseconds(expectedMicroseconds));

    mServoMotor.setSpeed(kMinMotorSpeed / 2);
}

TEST_F(ServoMotorTest, setSpeed_WhenPositiveSpeed_WillWriteCorrectMicroseconds)
{
    int expectedMicroseconds = kIdlePulseLength + (kMaxPulseLength - kIdlePulseLength) / 2;
    EXPECT_CALL(*mMockServo, writeMicroseconds(expectedMicroseconds));

    mServoMotor.setSpeed(kMaxMotorSpeed / 2);
}

TEST_F(ServoMotorTest, setSpeed_WhenIdleSpeed_WillWriteCorrectMicroseconds)
{
    EXPECT_CALL(*mMockServo, writeMicroseconds(kIdlePulseLength));

    mServoMotor.setSpeed(kIdleMotorSpeed);
}

TEST_F(ServoMotorTest, setSpeed_WhenLowerThanMinimumSpeed_WillWriteMinimumMicroseconds)
{
    EXPECT_CALL(*mMockServo, writeMicroseconds(kMinPulseLength));

    mServoMotor.setSpeed(kMinMotorSpeed - 1);
}

TEST_F(ServoMotorTest, setSpeed_WhenHigherThanMaximumSpeed_WillWriteMaximumMicroseconds)
{
    EXPECT_CALL(*mMockServo, writeMicroseconds(kMaxPulseLength));

    mServoMotor.setSpeed(kMaxMotorSpeed + 1);
}
