#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "GP2D120.hpp"
#include "GP2Y0A02.hpp"
#include "GP2Y0A21.hpp"
#include "MockRuntime.hpp"

using namespace ::testing;

namespace
{
const uint8_t kPin = 5;
} // namespace

class GP2D120Test : public Test
{
public:
    NiceMock<MockRuntime> mRuntime;
    GP2D120 mGP2D120{ mRuntime, kPin };
};

TEST_F(GP2D120Test, getDistance_WhenReadingTooLow_WillReturnZero)
{
    uint8_t expectedMeasurement = 0;
    EXPECT_CALL(mRuntime, getAnalogPinState(kPin)).WillOnce(Return(0));

    EXPECT_EQ(mGP2D120.getDistance(), expectedMeasurement);
}

TEST_F(GP2D120Test, getDistance_WhenReadingTooHigh_WillReturnZero)
{
    uint8_t expectedMeasurement = 0;
    EXPECT_CALL(mRuntime, getAnalogPinState(kPin)).WillOnce(Return(1024));

    EXPECT_EQ(mGP2D120.getDistance(), expectedMeasurement);
}

TEST_F(GP2D120Test, getDistance_WhenReadingWithinRange_WillReturnCorrectResult)
{
    uint8_t expectedMeasurement = 13; // (2914 / (200 + 5)) - 1)
    EXPECT_CALL(mRuntime, getAnalogPinState(kPin)).WillOnce(Return(200));

    EXPECT_EQ(mGP2D120.getDistance(), expectedMeasurement);
}

class GP2Y0A02Test : public Test
{
public:
    NiceMock<MockRuntime> mRuntime;
    GP2Y0A02 mGP2Y0A02{ mRuntime, kPin };
};

TEST_F(GP2Y0A02Test, getDistance_WhenReadingTooLow_WillReturnZero)
{
    uint8_t expectedMeasurement = 0;
    EXPECT_CALL(mRuntime, getAnalogPinState(kPin)).WillOnce(Return(0));

    EXPECT_EQ(mGP2Y0A02.getDistance(), expectedMeasurement);
}

TEST_F(GP2Y0A02Test, getDistance_WhenReadingTooHigh_WillReturnZero)
{
    uint8_t expectedMeasurement = 0;
    EXPECT_CALL(mRuntime, getAnalogPinState(kPin)).WillOnce(Return(1024));

    EXPECT_EQ(mGP2Y0A02.getDistance(), expectedMeasurement);
}

TEST_F(GP2Y0A02Test, getDistance_WhenReadingWithinRange_WillReturnCorrectResult)
{
    uint8_t expectedMeasurement = 113; // 9462 / (100 - 16.92))
    EXPECT_CALL(mRuntime, getAnalogPinState(kPin)).WillOnce(Return(100));

    EXPECT_EQ(mGP2Y0A02.getDistance(), expectedMeasurement);
}

class GP2Y0A21Test : public Test
{
public:
    NiceMock<MockRuntime> mRuntime;
    GP2Y0A21 mGP2Y0A21{ mRuntime, kPin };
};

TEST_F(GP2Y0A21Test, getDistance_WhenReadingTooLow_WillReturnZero)
{
    uint8_t expectedMeasurement = 0;
    EXPECT_CALL(mRuntime, getAnalogPinState(kPin)).WillOnce(Return(0));

    EXPECT_EQ(mGP2Y0A21.getDistance(), expectedMeasurement);
}

TEST_F(GP2Y0A21Test, getDistance_WhenReadingTooHigh_WillReturnZero)
{
    uint8_t expectedMeasurement = 0;
    EXPECT_CALL(mRuntime, getAnalogPinState(kPin)).WillOnce(Return(1024));

    EXPECT_EQ(mGP2Y0A21.getDistance(), expectedMeasurement);
}

TEST_F(GP2Y0A21Test, getDistance_WhenReadingWithinRange_WillReturnCorrectResult)
{
    uint8_t expectedMeasurement = 13; // (1 / (0.0002391473 * 13 - 0.0100251467))
    EXPECT_CALL(mRuntime, getAnalogPinState(kPin)).WillOnce(Return(350));

    EXPECT_EQ(mGP2Y0A21.getDistance(), expectedMeasurement);
}
