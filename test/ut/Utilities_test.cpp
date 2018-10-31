#include <algorithm>
#include <vector>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "Utilities.hpp"

using namespace ::testing;
using namespace smartcarlib::utils;

namespace
{
unsigned int expectedMedian = 25;
std::vector<unsigned int> readings{ 12, expectedMedian, 0, 55, 300 };
} // namespace

TEST(MedianTest, getMedian_WhenCalledWithUnsortedArray_WillReturnMedian)
{
    EXPECT_EQ(getMedian(&readings[0], readings.size()), expectedMedian);
}

TEST(MedianTest, getMedian_WhenCalledWithAscendingArray_WillReturnMedian)
{
    std::sort(readings.begin(), readings.end());
    EXPECT_EQ(getMedian(&readings[0], readings.size()), expectedMedian);
}

TEST(MedianTest, getMedian_WhenCalledWithDescendingArray_WillReturnMedian)
{
    std::reverse(readings.begin(), readings.end());
    EXPECT_EQ(getMedian(&readings[0], readings.size()), expectedMedian);
}

TEST(ConstrainTest, getConstrain_WhenSmallerThanMinimum_WillReturnMinimum)
{
    auto min = 40;
    auto max = 120;
    auto valueToConstrain = -2;
    EXPECT_EQ(getConstrain(valueToConstrain, min, max), min);
}

TEST(ConstrainTest, getConstrain_WhenLargerThanMaximum_WillReturnMaximum)
{
    auto min = -300;
    auto max = -50;
    auto valueToConstrain = 0;
    EXPECT_EQ(getConstrain(valueToConstrain, min, max), max);
}

TEST(ConstrainTest, getConstrain_WhenWithinRange_WillReturnNumber)
{
    auto min = -4;
    auto max = 24;
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
