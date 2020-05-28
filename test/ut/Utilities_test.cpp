#include <algorithm>
#include <vector>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "Utilities.hpp"

using namespace ::testing;
using namespace smartcarlib::utils;

namespace
{
const unsigned int expectedMedian = 25;
std::vector<unsigned int> readings{ 12, expectedMedian, 0, 55, 300 };
const auto kReadingsSize = static_cast<unsigned int>(readings.size());
} // namespace

TEST(MedianTest, getMedian_WhenCalledWithUnsortedArray_WillReturnMedian)
{
    EXPECT_EQ(getMedian(&readings[0], kReadingsSize), expectedMedian);
}

TEST(MedianTest, getMedian_WhenCalledWithAscendingArray_WillReturnMedian)
{
    std::sort(readings.begin(), readings.end());
    EXPECT_EQ(getMedian(&readings[0], kReadingsSize), expectedMedian);
}

TEST(MedianTest, getMedian_WhenCalledWithDescendingArray_WillReturnMedian)
{
    std::reverse(readings.begin(), readings.end());
    EXPECT_EQ(getMedian(&readings[0], kReadingsSize), expectedMedian);
}

TEST(ConstrainTest, getConstrain_WhenSmallerThanMinimum_WillReturnMinimum)
{
    auto min              = 40;
    auto max              = 120;
    auto valueToConstrain = -2;
    EXPECT_EQ(getConstrain(valueToConstrain, min, max), min);
}

TEST(ConstrainTest, getConstrain_WhenLargerThanMaximum_WillReturnMaximum)
{
    auto min              = -300;
    auto max              = -50;
    auto valueToConstrain = 0;
    EXPECT_EQ(getConstrain(valueToConstrain, min, max), max);
}

TEST(ConstrainTest, getConstrain_WhenWithinRange_WillReturnNumber)
{
    auto min              = -4;
    auto max              = 24;
    auto valueToConstrain = 1;
    EXPECT_EQ(getConstrain(valueToConstrain, min, max), valueToConstrain);
}

TEST(AbsoluteTest, getAbsolute_WhenNegativeNumber_WillReturnPositive)
{
    auto numberToAbsolute = -245.2f;
    EXPECT_EQ(getAbsolute(numberToAbsolute), -numberToAbsolute);
}

TEST(AbsoluteTest, getAbsolute_WhenNonNegativeNumber_WillReturnNumber)
{
    auto numberToAbsolute = 23;
    EXPECT_EQ(getAbsolute(numberToAbsolute), numberToAbsolute);
}

TEST(MapTest, getMap_WhenInvalidRange_WillReturnLowerTargetRangeLimit)
{
    int valueToMap    = 5;
    int fromLow       = 100;
    int fromHigh      = 100;
    int toLow         = 0;
    int toHigh        = 1000;
    int expectedValue = toLow;

    EXPECT_EQ(getMap(valueToMap, fromLow, fromHigh, toLow, toHigh), expectedValue);
}

TEST(MapTest, getMap_WhenValidArguments_WillReturnValidMappedValue)
{
    int fromLow       = 0;
    int fromHigh      = 100;
    int valueToMap    = fromHigh / 4;
    int toLow         = 0;
    int toHigh        = 1000;
    int expectedValue = toHigh / 4;

    EXPECT_EQ(getMap(valueToMap, fromLow, fromHigh, toLow, toHigh), expectedValue);
}

TEST(MapTest, getMap_WhenFloatArguments_WillReturnValidFloatValue)
{
    float fromLow       = 0;
    float fromHigh      = 100.0f;
    float valueToMap    = fromHigh / 3.5f;
    float toLow         = 0;
    float toHigh        = 1000.0f;
    float expectedValue = toHigh / 3.5f;

    EXPECT_FLOAT_EQ(getMap(valueToMap, fromLow, fromHigh, toLow, toHigh), expectedValue);
}

TEST(AlmostEqualTest, areAlmostEqual_WhenNotEqual_WillReturnFalse)
{
    EXPECT_FALSE(areAlmostEqual(1.2f, 11.523f));
}

TEST(AlmostEqualTest, areAlmostEqual_WhenSignedDifferentlyButAlmostEqual_WillReturnTrue)
{
    EXPECT_TRUE(areAlmostEqual(-0.0001f, 0.0001f));
}

TEST(AlmostEqualTest, areAlmostEqual_WhenSignedSameAndAlmostEqual_WillReturnTrue)
{
    EXPECT_TRUE(areAlmostEqual(10.0f / 3.0f, 3.333f));
}
