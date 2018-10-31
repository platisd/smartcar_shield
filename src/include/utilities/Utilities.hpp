/**
 * Utilities commonly used within the Smartcar library
 */
#pragma once

namespace smartcarlib
{
namespace utils
{
template <typename AnyNumber>
constexpr AnyNumber getAbsolute(const AnyNumber& number)
{
    return number < 0 ? -number : number;
}

template <typename AnyNumber>
constexpr AnyNumber getConstrain(const AnyNumber& number, const AnyNumber& min, const AnyNumber& max)
{
    return number < min ? min : (number > max ? max : number);
}

/**
 * Gets the median value out of the supplied number array
 * @param  unsortedNumbers   An array containing numbers
 * @param  arraySize         Amount of numbers contained in the array
 * @return                   Median number of the supplied array
 */
template <typename AnyNumber>
AnyNumber getMedian(AnyNumber unsortedNumbers[], unsigned int arraySize)
{
    // Sort the array using bubble sort
    for (auto i = 0; i < arraySize - 1; i++)
    {
        for (auto j = 0; j < arraySize - 1 - i; j++)
        {
            if (unsortedNumbers[j] > unsortedNumbers[j + 1])
            {
                auto swappedValue      = unsortedNumbers[j];
                unsortedNumbers[j]     = unsortedNumbers[j + 1];
                unsortedNumbers[j + 1] = swappedValue;
            }
        }
    }
    // Return the Median value now that the array is sorted
    return unsortedNumbers[arraySize / 2];
}
} // namespace utils
} // namespace smartcarlib
