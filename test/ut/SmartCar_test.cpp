#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "MockControl.hpp"
#include "MockHeadingSensor.hpp"
#include "MockOdometer.hpp"
#include "MockRuntime.hpp"
#include "SmartCar.hpp"

using namespace ::testing;

class SmartCarTest : public Test
{
public:
    SmartCarTest()
        : mSmartCar{ mControl, mHeadingSensor, mOdometerLeft, mOdometerRight, mRuntime }
    {
    }

    NiceMock<MockControl> mControl;
    NiceMock<MockHeadingSensor> mHeadingSensor;
    NiceMock<MockOdometer> mOdometerLeft;
    NiceMock<MockOdometer> mOdometerRight;
    NiceMock<MockRuntime> mRuntime;
    SmartCar mSmartCar;
};

class SmartCarOneOdometerTest : public Test
{
public:
    SmartCarOneOdometerTest()
        : mSmartCar{ mControl, mHeadingSensor, mOdometer, mRuntime }
    {
    }

    NiceMock<MockControl> mControl;
    NiceMock<MockHeadingSensor> mHeadingSensor;
    NiceMock<MockOdometer> mOdometer;
    NiceMock<MockRuntime> mRuntime;
    SmartCar mSmartCar;
};

TEST_F(SmartCarTest, update_WhenCalled_WillUpdateHeadingAndSpeed)
{
    // Expect DistanceCar::update to be called
    EXPECT_CALL(mOdometerLeft, isAttached()).Times(AtLeast(1)).WillOnce(Return(true));
    EXPECT_CALL(mOdometerRight, isAttached()).Times(AtLeast(1)).WillOnce(Return(true));
    // Expect HeadingCar::update to be called
    EXPECT_CALL(mHeadingSensor, update());

    mSmartCar.update();
}

TEST_F(SmartCarOneOdometerTest, constructor_WhenOneOdometerSupplied_WillTreatItAsTwo)
{
    auto distance = 45;
    ON_CALL(mOdometer, isAttached()).WillByDefault(Return(true));
    EXPECT_CALL(mOdometer, getDistance()).Times(2).WillRepeatedly(Return(distance));

    EXPECT_EQ(mSmartCar.getDistance(), distance);
}
