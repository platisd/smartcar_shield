#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "SimpleCar.hpp"
#include "MockControl.hpp"

using namespace ::testing;

class SimpleCarTest : public Test
{
public:
    SimpleCarTest() : mSimpleCar{mControl} {}

    NiceMock<MockControl> mControl;
    SimpleCar mSimpleCar;
};

TEST_F(SimpleCarTest, setSpeed_WhenCalled_WillSetCorrectSpeed)
{
    float speed = 45.2;
    int expectedSpeed = static_cast<int>(speed);
    EXPECT_CALL(mControl, setSpeed(expectedSpeed));

    mSimpleCar.setSpeed(speed);
}

TEST_F(SimpleCarTest, setAngle_WhenCalled_WillSetCorrectAngle)
{
    int angle = -23;
    EXPECT_CALL(mControl, setAngle(angle));

    mSimpleCar.setAngle(angle);
}

TEST_F(SimpleCarTest, overrideMotorSpeed_WhenCalled_WillSetCorrectSpeed)
{
    int firstMotorSpeed = 50;
    int secondMotorSpeed = -20;
    EXPECT_CALL(mControl, overrideMotorSpeed(firstMotorSpeed, secondMotorSpeed));

    mSimpleCar.overrideMotorSpeed(firstMotorSpeed, secondMotorSpeed);
}
