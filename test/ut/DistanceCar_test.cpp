#include <cmath>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "DistanceCar.hpp"
#include "MockControl.hpp"
#include "MockOdometer.hpp"
#include "MockRuntime.hpp"

using namespace ::testing;
using namespace smartcarlib::constants::control;
using namespace smartcarlib::constants::car;

class DistanceCarTest : public Test
{
public:
    DistanceCarTest()
        : mDistanceCar{ mControl, mOdometerLeft, mOdometerRight, mRuntime }
    {
    }

    NiceMock<MockControl> mControl;
    NiceMock<MockOdometer> mOdometerLeft;
    NiceMock<MockOdometer> mOdometerRight;
    NiceMock<MockRuntime> mRuntime;
    DistanceCar mDistanceCar;
};

class DistanceCarSingleOdometerTest : public Test
{
public:
    DistanceCarSingleOdometerTest()
        : mDistanceCar{ mControl, mOdometer, mRuntime }
    {
    }

    virtual void SetUp()
    {
        ON_CALL(mOdometer, isAttached()).WillByDefault(Return(true));
    }

    NiceMock<MockControl> mControl;
    NiceMock<MockOdometer> mOdometer;
    NiceMock<MockRuntime> mRuntime;
    DistanceCar mDistanceCar;
};

class DistanceCarOdometersAttachedTest : public DistanceCarTest
{
public:
    virtual void SetUp()
    {
        ON_CALL(mOdometerLeft, isAttached()).WillByDefault(Return(true));
        ON_CALL(mOdometerRight, isAttached()).WillByDefault(Return(true));
    }
};

class DistanceCarCruiseControlTest : public DistanceCarTest
{
public:
    DistanceCarCruiseControlTest()
        : mNumberOfCurrentTimeMillisCalls{ 0 }
    {
    }

    virtual void SetUp()
    {
        auto validCallTimer
            = [this]() { return ++mNumberOfCurrentTimeMillisCalls * kDefaultPidFrequency; };
        ON_CALL(mRuntime, currentTimeMillis()).WillByDefault(InvokeWithoutArgs(validCallTimer));
        ON_CALL(mOdometerLeft, isAttached()).WillByDefault(Return(true));
        ON_CALL(mOdometerRight, isAttached()).WillByDefault(Return(true));

        mDistanceCar.enableCruiseControl();
    }

    int mNumberOfCurrentTimeMillisCalls;
};

TEST_F(DistanceCarTest, getDistance_WhenOdometersNotAttached_WillReturnError)
{
    EXPECT_CALL(mOdometerLeft, isAttached()).Times(AtMost(1)).WillOnce(Return(false));
    EXPECT_CALL(mOdometerRight, isAttached()).Times(AtMost(1)).WillOnce(Return(false));
    EXPECT_CALL(mOdometerLeft, getDistance()).Times(0);
    EXPECT_CALL(mOdometerRight, getDistance()).Times(0);

    ASSERT_EQ(mDistanceCar.getDistance(), static_cast<long>(kOdometersNotAttachedError));
}

TEST_F(DistanceCarTest, getDistance_WhenOdometersAttached_WillReturnAverageDistance)
{
    unsigned long leftOdometerDistance  = 205;
    unsigned long rightOdometerDistance = 180;
    unsigned long expectedDistance      = (leftOdometerDistance + rightOdometerDistance) / 2;
    EXPECT_CALL(mOdometerLeft, isAttached()).WillOnce(Return(true));
    EXPECT_CALL(mOdometerRight, isAttached()).WillOnce(Return(true));
    EXPECT_CALL(mOdometerLeft, getDistance()).WillOnce(Return(leftOdometerDistance));
    EXPECT_CALL(mOdometerRight, getDistance()).WillOnce(Return(rightOdometerDistance));

    ASSERT_EQ(mDistanceCar.getDistance(), expectedDistance);
}

TEST_F(DistanceCarTest, getDistance_WhenOneOdometerAttached_WillReturnItsDistance)
{
    unsigned long leftOdometerDistance = 400;
    EXPECT_CALL(mOdometerLeft, isAttached()).Times(2).WillRepeatedly(Return(true));
    EXPECT_CALL(mOdometerLeft, getDistance()).Times(2).WillRepeatedly(Return(leftOdometerDistance));

    DistanceCar distanceCar(mControl, mOdometerLeft, mOdometerLeft, mRuntime);
    EXPECT_EQ(distanceCar.getDistance(), leftOdometerDistance);
}

TEST_F(DistanceCarTest, setSpeed_WhenNotAttached_WillNotConsiderCruiseControlAndSetOnlySpeed)
{
    float speed = 50.0;

    EXPECT_CALL(mOdometerLeft, isAttached()).Times(AtMost(1)).WillOnce(Return(false));
    EXPECT_CALL(mOdometerRight, isAttached()).Times(AtMost(1)).WillOnce(Return(false));
    EXPECT_CALL(mOdometerLeft, providesDirection()).Times(0);
    EXPECT_CALL(mOdometerRight, providesDirection()).Times(0);
    EXPECT_CALL(mControl, setSpeed(speed));

    mDistanceCar.setSpeed(speed);
}

TEST_F(
    DistanceCarOdometersAttachedTest,
    setSpeed_WhenOdometersAreDirectionalAndCruiseControlIsEnabled_WillNotBrakeWhenChangingSpeedDirection)
{
    float speed = 50.0;
    ON_CALL(mOdometerLeft, providesDirection()).WillByDefault(Return(true));
    ON_CALL(mOdometerRight, providesDirection()).WillByDefault(Return(true));
    // The speed should be taken care only by the PID controller
    EXPECT_CALL(mControl, setSpeed(_)).Times(0);

    mDistanceCar.enableCruiseControl();
    mDistanceCar.setSpeed(speed);
    mDistanceCar.setSpeed(-speed);
}

TEST_F(DistanceCarOdometersAttachedTest, setSpeed_WhenCarShouldStop_WillBrake)
{
    auto initialSpeed = 50.0;
    mDistanceCar.setSpeed(initialSpeed);

    EXPECT_CALL(mControl, setSpeed(-initialSpeed));
    EXPECT_CALL(mControl, setSpeed(static_cast<float>(kIdleControlSpeed)));

    mDistanceCar.setSpeed(static_cast<float>(kIdleControlSpeed));
}

TEST_F(DistanceCarOdometersAttachedTest, setSpeed_WhenDirectionChangesInCruiseControl_WillBrake)
{
    auto initialSpeed = 50.0;
    mDistanceCar.enableCruiseControl();
    mDistanceCar.setSpeed(initialSpeed);
    // When braking we will sample the distance at least two times (to determine a dX)
    // and will eventually set the speed to idle
    EXPECT_CALL(mOdometerLeft, getDistance()).Times(AtLeast(2));
    EXPECT_CALL(mOdometerRight, getDistance()).Times(AtLeast(2));
    EXPECT_CALL(mControl, setSpeed(static_cast<float>(kIdleControlSpeed))).Times(AtLeast(1));

    mDistanceCar.setSpeed(-initialSpeed / 2);
}

TEST_F(DistanceCarOdometersAttachedTest, setSpeed_WhenNotMoving_WillNotBrake)
{
    // Speed might be set to idle but without sampling the dX or other attempts to stop the car
    EXPECT_CALL(mOdometerLeft, getDistance()).Times(0);
    EXPECT_CALL(mOdometerRight, getDistance()).Times(0);
    EXPECT_CALL(mControl, setSpeed(static_cast<float>(kIdleControlSpeed))).Times(AtMost(1));

    mDistanceCar.setSpeed(static_cast<float>(kIdleControlSpeed));
}

TEST_F(DistanceCarOdometersAttachedTest,
       setSpeed_WhenCruiseControlAndNoBrake_WillSetSpeedInPidController)
{
    auto speed = -30.0;
    // Speed should be set via the PID controller algorithm and not directly here
    EXPECT_CALL(mControl, setSpeed(_)).Times(0);

    mDistanceCar.enableCruiseControl();
    mDistanceCar.setSpeed(speed);
}

TEST_F(DistanceCarOdometersAttachedTest, setSpeed_WhenNoCruiseControlAndNoBrake_WillSetSpeed)
{
    auto speed = 10.0;
    EXPECT_CALL(mControl, setSpeed(speed));

    mDistanceCar.setSpeed(speed);
}

TEST_F(DistanceCarTest, getSpeed_WhenOdometersNotAttaached_WillReturnError)
{
    EXPECT_CALL(mOdometerLeft, isAttached()).Times(AtMost(1)).WillOnce(Return(false));
    EXPECT_CALL(mOdometerRight, isAttached()).Times(AtMost(1)).WillOnce(Return(false));

    EXPECT_EQ(mDistanceCar.getSpeed(), static_cast<float>(kOdometersNotAttachedError));
}

TEST_F(DistanceCarOdometersAttachedTest, getSpeed_WhenOdometersAttached_WillReturnAverageSpeed)
{
    float leftSpeed    = 2.1;
    float rightSpeed   = 1.8;
    auto expectedSpeed = (leftSpeed + rightSpeed) / 2;
    EXPECT_CALL(mOdometerLeft, getSpeed()).WillOnce(Return(leftSpeed));
    EXPECT_CALL(mOdometerRight, getSpeed()).WillOnce(Return(rightSpeed));

    EXPECT_EQ(mDistanceCar.getSpeed(), expectedSpeed);
}

TEST_F(DistanceCarTest, overrideMotorSpeed_WhenCruiseControlEnabled_WillNotSetSpeed)
{
    auto leftSpeed  = 0;
    auto rightSpeed = 90;
    EXPECT_CALL(mControl, overrideMotorSpeed(_, _)).Times(0);

    mDistanceCar.enableCruiseControl();
    mDistanceCar.overrideMotorSpeed(leftSpeed, rightSpeed);
}

TEST_F(DistanceCarTest, overrideMotorSpeed_WhenCruiseControlNotEnabled_WillSetSpeed)
{
    auto leftSpeed  = -20;
    auto rightSpeed = -100;

    EXPECT_CALL(mControl, overrideMotorSpeed(leftSpeed, rightSpeed));
    mDistanceCar.overrideMotorSpeed(leftSpeed, rightSpeed);
}

TEST_F(DistanceCarTest, enableCruiseControl_WhenCalled_WillEnableCruiseControl)
{
    auto leftSpeed  = 0;
    auto rightSpeed = 10;

    EXPECT_CALL(mControl, overrideMotorSpeed(_, _)).Times(0);
    mDistanceCar.enableCruiseControl();
    mDistanceCar.overrideMotorSpeed(leftSpeed, rightSpeed);
}

TEST_F(DistanceCarTest, disableCruiseControl_WhenCalled_WillDisableCruiseControl)
{
    auto leftSpeed  = 20;
    auto rightSpeed = -30;

    EXPECT_CALL(mControl, overrideMotorSpeed(_, _)).Times(0);
    mDistanceCar.enableCruiseControl();
    mDistanceCar.overrideMotorSpeed(leftSpeed, rightSpeed);

    EXPECT_CALL(mControl, overrideMotorSpeed(leftSpeed, rightSpeed));
    mDistanceCar.disableCruiseControl();
    mDistanceCar.overrideMotorSpeed(leftSpeed, rightSpeed);
}

TEST_F(DistanceCarTest, update_WhenOdometersNotAttached_WillReturnImmediately)
{
    EXPECT_CALL(mRuntime, currentTimeMillis()).Times(0);

    mDistanceCar.update();
}

TEST_F(DistanceCarOdometersAttachedTest, update_WhenCruiseControlNotEnabled_WillReturnImmediately)
{
    EXPECT_CALL(mRuntime, currentTimeMillis()).Times(0);

    mDistanceCar.update();
}

TEST_F(DistanceCarOdometersAttachedTest, update_WhenCalledTooFast_WillReturn)
{
    EXPECT_CALL(mRuntime, currentTimeMillis()).WillOnce(Return(kDefaultPidFrequency - 1));
    EXPECT_CALL(mOdometerLeft, providesDirection()).Times(0);
    EXPECT_CALL(mOdometerRight, providesDirection()).Times(0);

    mDistanceCar.enableCruiseControl();
    mDistanceCar.update();
}

TEST_F(DistanceCarOdometersAttachedTest,
       update_WhenOdometersNotDirectionalAndIdleSpeed_WillNotUpdateSpeed)
{
    mDistanceCar.enableCruiseControl();
    mDistanceCar.setSpeed(kIdleControlSpeed);
    EXPECT_CALL(mRuntime, currentTimeMillis()).WillOnce(Return(kDefaultPidFrequency));
    EXPECT_CALL(mOdometerLeft, providesDirection()).Times(AtMost(1)).WillOnce(Return(false));
    EXPECT_CALL(mOdometerRight, providesDirection()).Times(AtMost(1)).WillOnce(Return(false));
    EXPECT_CALL(mOdometerLeft, getSpeed()).Times(0);
    EXPECT_CALL(mOdometerRight, getSpeed()).Times(0);

    mDistanceCar.update();
}

TEST_F(DistanceCarCruiseControlTest, update_WhenGroundSpeedBelowTarget_WillIncreaseSpeedToMotors)
{
    float targetSpeed = 1.5;
    float groundSpeed = targetSpeed / 2;

    ON_CALL(mOdometerLeft, getSpeed()).WillByDefault(Return(groundSpeed));
    ON_CALL(mOdometerRight, getSpeed()).WillByDefault(Return(groundSpeed));
    mDistanceCar.setSpeed(targetSpeed);

    int previousSpeedToMotors = 0;
    for (auto i = 0; i < 10; i++)
    {
        int speedToMotors = 0;
        EXPECT_CALL(mControl, setSpeed(_)).WillOnce(SaveArg<0>(&speedToMotors));
        mDistanceCar.update();
        ASSERT_GT(speedToMotors, previousSpeedToMotors);
        previousSpeedToMotors = speedToMotors;
    }
}

TEST_F(DistanceCarCruiseControlTest,
       update_WhenGroundSpeedAboveTargetAndOdometersAreDirectionless_WillSetIdleSpeed)
{
    float targetSpeed = 10;
    float groundSpeed = targetSpeed * 1.1;

    ON_CALL(mOdometerLeft, getSpeed()).WillByDefault(Return(groundSpeed));
    ON_CALL(mOdometerRight, getSpeed()).WillByDefault(Return(groundSpeed));
    mDistanceCar.setSpeed(targetSpeed);

    for (auto i = 0; i < 10; i++)
    {
        int speedToMotors = 0;
        EXPECT_CALL(mControl, setSpeed(_)).WillOnce(SaveArg<0>(&speedToMotors));
        mDistanceCar.update();
        // Since the odometers are not directional, we should just stop the motors
        // so to avoid moving accidentally towards the opposite direction.
        // This test case is so to avoid #5 regressions.
        ASSERT_EQ(speedToMotors, kIdleControlSpeed);
    }
}

TEST_F(DistanceCarCruiseControlTest,
       update_WhenGroundSpeedAboveTargetAndOdometersAreDirectional_WillDecreaseSpeedToMotors)
{
    float targetSpeed = 6.5;
    float groundSpeed = targetSpeed * 1.1;

    ON_CALL(mOdometerLeft, providesDirection()).WillByDefault(Return(true));
    ON_CALL(mOdometerRight, providesDirection()).WillByDefault(Return(true));
    ON_CALL(mOdometerLeft, getSpeed()).WillByDefault(Return(groundSpeed));
    ON_CALL(mOdometerRight, getSpeed()).WillByDefault(Return(groundSpeed));
    mDistanceCar.setSpeed(targetSpeed);

    int previousSpeedToMotors = 0;
    for (auto i = 0; i < 10; i++)
    {
        int speedToMotors = 0;
        EXPECT_CALL(mControl, setSpeed(_)).WillOnce(SaveArg<0>(&speedToMotors));
        mDistanceCar.update();
        ASSERT_GT(previousSpeedToMotors, speedToMotors);
        previousSpeedToMotors = speedToMotors;
    }
}

TEST_F(DistanceCarCruiseControlTest,
       update_WhenBackwardSpeedBelowTarget_WillIncreaseMotorSpeedBackward)
{
    float targetSpeed = -3.5;
    float groundSpeed = abs(targetSpeed * 0.9); // Directionless odometers return unsigned speed

    ON_CALL(mOdometerLeft, getSpeed()).WillByDefault(Return(groundSpeed));
    ON_CALL(mOdometerRight, getSpeed()).WillByDefault(Return(groundSpeed));
    mDistanceCar.setSpeed(targetSpeed);

    int previousSpeedToMotors = 0;
    for (auto i = 0; i < 10; i++)
    {
        int speedToMotors = 0;
        EXPECT_CALL(mControl, setSpeed(_)).WillOnce(SaveArg<0>(&speedToMotors));
        mDistanceCar.update();
        // We compare the absolute values of the speeds since we are moving backward
        ASSERT_GT(abs(speedToMotors), abs(previousSpeedToMotors));
        previousSpeedToMotors = speedToMotors;
    }
}

TEST_F(DistanceCarSingleOdometerTest, constructor_WhenOneOdometerSupplied_WillTreatItAsTwo)
{
    float speed = 4.5;
    EXPECT_CALL(mOdometer, getSpeed()).Times(2).WillRepeatedly(Return(speed));

    EXPECT_EQ(mDistanceCar.getSpeed(), speed);
}
