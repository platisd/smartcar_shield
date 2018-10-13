#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "GP2D120.hpp"
#include "GP2Y0A02.hpp"
#include "GP2Y0A21.hpp"
#include "MockRuntime.hpp"

using namespace ::testing;

namespace
{
const auto kErrorReading = static_cast<unsigned int>(-1);
}

class GP2D120Test : public Test
{
public:
    GP2D120Test()
        : mGP2D120{ mRuntime }
    {
    }

    NiceMock<MockRuntime> mRuntime;
    GP2D120 mGP2D120;
};

TEST_F(GP2D120Test, getDistance_WhenSensorNotAttached_WillReturnError)
{
    EXPECT_CALL(mRuntime, getAnalogPinState(_)).Times(0);

    EXPECT_EQ(mGP2D120.getDistance(), kErrorReading);
}

TEST_F(GP2D120Test, getDistance_WhenReadingTooLow_WillReturnZero)
{
    uint8_t pin = 2;
    uint8_t expectedMeasurement = 0;
    mGP2D120.attach(pin);
    EXPECT_CALL(mRuntime, getAnalogPinState(pin)).WillOnce(Return(0));

    EXPECT_EQ(mGP2D120.getDistance(), expectedMeasurement);
}

TEST_F(GP2D120Test, getDistance_WhenReadingTooHigh_WillReturnZero)
{
    uint8_t pin = 2;
    uint8_t expectedMeasurement = 0;
    mGP2D120.attach(pin);
    EXPECT_CALL(mRuntime, getAnalogPinState(pin)).WillOnce(Return(1024));

    EXPECT_EQ(mGP2D120.getDistance(), expectedMeasurement);
}

TEST_F(GP2D120Test, getDistance_WhenReadingWithinRange_WillReturnCorrectResult)
{
    uint8_t pin = 2;
    uint8_t expectedMeasurement = 13; // (2914 / (200 + 5)) - 1)
    mGP2D120.attach(pin);
    EXPECT_CALL(mRuntime, getAnalogPinState(pin)).WillOnce(Return(200));

    EXPECT_EQ(mGP2D120.getDistance(), expectedMeasurement);
}

class GP2Y0A02Test : public Test
{
public:
    GP2Y0A02Test()
        : mGP2Y0A02{ mRuntime }
    {
    }

    NiceMock<MockRuntime> mRuntime;
    GP2Y0A02 mGP2Y0A02;
};

TEST_F(GP2Y0A02Test, getDistance_WhenReadingTooLow_WillReturnZero)
{
    uint8_t pin = 3;
    uint8_t expectedMeasurement = 0;
    mGP2Y0A02.attach(pin);
    EXPECT_CALL(mRuntime, getAnalogPinState(pin)).WillOnce(Return(0));

    EXPECT_EQ(mGP2Y0A02.getDistance(), expectedMeasurement);
}

TEST_F(GP2Y0A02Test, getDistance_WhenReadingTooHigh_WillReturnZero)
{
    uint8_t pin = 3;
    uint8_t expectedMeasurement = 0;
    mGP2Y0A02.attach(pin);
    EXPECT_CALL(mRuntime, getAnalogPinState(pin)).WillOnce(Return(1024));

    EXPECT_EQ(mGP2Y0A02.getDistance(), expectedMeasurement);
}

TEST_F(GP2Y0A02Test, getDistance_WhenReadingWithinRange_WillReturnCorrectResult)
{
    uint8_t pin = 3;
    uint8_t expectedMeasurement = 113; // 9462 / (100 - 16.92))
    mGP2Y0A02.attach(pin);
    EXPECT_CALL(mRuntime, getAnalogPinState(pin)).WillOnce(Return(100));

    EXPECT_EQ(mGP2Y0A02.getDistance(), expectedMeasurement);
}

class GP2Y0A21Test : public Test
{
public:
    GP2Y0A21Test()
        : mGP2Y0A21{ mRuntime }
    {
    }

    NiceMock<MockRuntime> mRuntime;
    GP2Y0A21 mGP2Y0A21;
};

TEST_F(GP2Y0A21Test, getDistance_WhenReadingTooLow_WillReturnZero)
{
    uint8_t pin = 4;
    uint8_t expectedMeasurement = 0;
    mGP2Y0A21.attach(pin);
    EXPECT_CALL(mRuntime, getAnalogPinState(pin)).WillOnce(Return(0));

    EXPECT_EQ(mGP2Y0A21.getDistance(), expectedMeasurement);
}

TEST_F(GP2Y0A21Test, getDistance_WhenReadingTooHigh_WillReturnZero)
{
    uint8_t pin = 4;
    uint8_t expectedMeasurement = 0;
    mGP2Y0A21.attach(pin);
    EXPECT_CALL(mRuntime, getAnalogPinState(pin)).WillOnce(Return(1024));

    EXPECT_EQ(mGP2Y0A21.getDistance(), expectedMeasurement);
}

TEST_F(GP2Y0A21Test, getDistance_WhenReadingWithinRange_WillReturnCorrectResult)
{
    uint8_t pin = 4;
    uint8_t expectedMeasurement = 13; // (1 / (0.0002391473 * 13 - 0.0100251467))
    mGP2Y0A21.attach(pin);
    EXPECT_CALL(mRuntime, getAnalogPinState(pin)).WillOnce(Return(350));

    EXPECT_EQ(mGP2Y0A21.getDistance(), expectedMeasurement);
}
