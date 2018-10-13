#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "InfraredAnalogSensor.hpp"
#include "MockInfraredAnalogSensor.hpp"
#include "MockRuntime.hpp"

using namespace ::testing;

namespace
{
const auto kErrorReading                    = static_cast<unsigned int>(-1);
} // namespace

class InfraredAnalogSensorTest : public Test
{
public:
    InfraredAnalogSensorTest()
        : mInfraredAnalogSensor(mRuntime)
    {
    }

    NiceMock<MockRuntime> mRuntime;
    NiceMock<MockInfraredAnalogSensor> mInfraredAnalogSensor;
};

TEST_F(InfraredAnalogSensorTest, attach_WhenCalled_WillNotCrash)
{
    // Nothing to test here in this context, doing this for the coverage
    uint8_t pin = 12;
    mInfraredAnalogSensor.attach(12);
}

TEST_F(InfraredAnalogSensorTest, getMedianDistance_WhenNoIterations_WillReturnError)
{
    uint8_t expectedMeasurements = 0;
    EXPECT_CALL(mInfraredAnalogSensor, getDistance()).Times(expectedMeasurements);

    EXPECT_EQ(mInfraredAnalogSensor.getMedianDistance(expectedMeasurements), kErrorReading);
}

TEST_F(InfraredAnalogSensorTest, getMedianDistance_WhenCalled_WillMakeCorrectNumberOfMeasurements)
{
    uint8_t expectedMeasurements = 6;
    EXPECT_CALL(mInfraredAnalogSensor, getDistance()).Times(expectedMeasurements);

    mInfraredAnalogSensor.getMedianDistance(expectedMeasurements);
}
