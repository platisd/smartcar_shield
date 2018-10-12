#include <algorithm>
#include <vector>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "Median.hpp"
#include "MockRuntime.hpp"

using namespace ::testing;

namespace
{
unsigned int expectedMedian = 25;
std::vector<unsigned int> readings{ 12, expectedMedian, 0, 55, 300 };
} // namespace

TEST(MedianTest, getMedian_WhenCalledWithUnsortedArray_WillReturnMedian)
{
    EXPECT_EQ(Median::getMedian(&readings[0], readings.size()), expectedMedian);
}

TEST(MedianTest, getMedian_WhenCalledWithAscendingArray_WillReturnMedian)
{
    std::sort(readings.begin(), readings.end());
    EXPECT_EQ(Median::getMedian(&readings[0], readings.size()), expectedMedian);
}

TEST(MedianTest, getMedian_WhenCalledWithDescendingArray_WillReturnMedian)
{
    std::reverse(readings.begin(), readings.end());
    EXPECT_EQ(Median::getMedian(&readings[0], readings.size()), expectedMedian);
}
