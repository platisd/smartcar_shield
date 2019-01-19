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
constexpr AnyNumber
getConstrain(const AnyNumber& number, const AnyNumber& min, const AnyNumber& max)
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
AnyNumber getMedian(AnyNumber unsortedNumbers[], const unsigned int& arraySize)
{
    // Sort the array using bubble sort
    for (unsigned int i = 0; i < arraySize - 1; i++)
    {
        for (unsigned int j = 0; j < arraySize - 1 - i; j++)
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

/**
 * Maps a value from a range to another. Implemented as a templetized `map` of Arduino.
 * @param  valueToMap The value to be mapped
 * @param  fromLow    The lower limit of the initial range
 * @param  fromHigh   The upper limit of the initial range
 * @param  toLow      The lower limit of the final range
 * @param  toHigh     The higher limit of the final range
 * @return            The mapped value if initial range valid otherwise the lower limit
 *                    of the target range
 */
template <typename AnyNumber>
constexpr AnyNumber getMap(const AnyNumber& valueToMap,
                           const AnyNumber& fromLow,
                           const AnyNumber& fromHigh,
                           const AnyNumber& toLow,
                           const AnyNumber& toHigh)
{
    return fromHigh == fromLow
               ? toLow
               : (valueToMap - fromLow) * (toHigh - toLow) / (fromHigh - fromLow) + toLow;
}

/**
 * Compares two floating point numbers
 * @param  a The first floating point number to compare
 * @param  b The second floating point number to compare
 * @return   `true` if the two numbers are amost equal, false otherwise
 */
constexpr bool areAlmostEqual(float a, float b)
{
    return getAbsolute(getAbsolute(a) - getAbsolute(b)) <= 0.001f;
}
} // namespace utils
} // namespace smartcarlib
