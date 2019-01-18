#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "HeadingCar.hpp"
#include "MockControl.hpp"
#include "MockHeadingSensor.hpp"

using namespace ::testing;

class HeadingCarTest : public Test
{
public:
    HeadingCarTest()
        : mHeadingCar{ mControl, mHeadingSensor }
    {
    }

    NiceMock<MockControl> mControl;
    NiceMock<MockHeadingSensor> mHeadingSensor;
    HeadingCar mHeadingCar;
};

TEST_F(HeadingCarTest, getHeading_WhenCalled_WillReturnHeading)
{
    unsigned int expectedHeading = 260;
    EXPECT_CALL(mHeadingSensor, getHeading()).WillOnce(Return(expectedHeading));

    EXPECT_EQ(mHeadingCar.getHeading(), expectedHeading);
}

TEST_F(HeadingCarTest, update_WhenCalled_WillUpdateHeadingSensor)
{
    EXPECT_CALL(mHeadingSensor, update());

    mHeadingCar.update();
}
