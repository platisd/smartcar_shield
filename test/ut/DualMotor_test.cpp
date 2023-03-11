#include "DualMotor.hpp"
#include "MockMotor.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

TEST(DualMotorTest, setSpeed_WhenSet_WillSetSameSpeedOnBothMotors)
{
    MockMotor motor1;
    MockMotor motor2;
    DualMotor dualMotor{ motor1, motor2 };

    int speedToSet = 100;

    EXPECT_CALL(motor1, setSpeed(speedToSet));
    EXPECT_CALL(motor2, setSpeed(speedToSet));

    dualMotor.setSpeed(speedToSet);
}
