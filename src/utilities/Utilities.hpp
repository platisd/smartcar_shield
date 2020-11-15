/**
 * \class Utilities
 * Utilitiy functions commonly used within the Smartcar library
 */
#pragma once

namespace smartcarlib
{
namespace utils
{
/**
 * Gets the absolute of the supplied number
 * @param  number The number to get the absolute of
 * @return        The absolute value
 *
 * **Example:**
 * \code
 * int num = -5;
 * num = smartcarlib::utils::getAbsolute(num);
 * // `num` is now `5`
 * \endcode
 */
template <typename AnyNumber>
constexpr AnyNumber getAbsolute(AnyNumber number)
{
    return number < 0 ? -number : number;
}

/**
 * Limit the number between a range
 * @param  number The number to limit
 * @param  min    The minimum limit of the range
 * @param  max    The maximum limit of the range
 * @return        The number unchanged if it was within the range, otherwise the
 *                lower limit of the range if it was smaller or the higher limit
 *                if it was larger
 *
 * **Example:**
 * \code
 * int num = -5;
 * num = smartcarlib::utils::getConstrain(num, 0, 100);
 * // `num` is now `0`
 * \endcode
 */
template <typename AnyNumber>
constexpr AnyNumber getConstrain(AnyNumber number, AnyNumber min, AnyNumber max)
{
    return number < min ? min : (number > max ? max : number);
}

/**
 * Gets the median value out of the supplied number array
 * @param  unsortedNumbers   An array containing numbers
 * @param  arraySize         Amount of numbers contained in the array
 * @return                   Median number of the supplied array
 *
 * **Example:**
 * \code
 * const int ARRAY_SIZE = 5;
 * int array[ARRAY_SIZE] = {0, 3, 4, 1, 15};
 * int median = smartcarlib::utils::getMedian(array, ARRAY_SIZE);
 * // `median` is `3`
 * \endcode
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
 *
 * **Example:**
 * \code
 * int num = 3;
 *  // Scale a `num` from 0-10 to 0-100
 *  num = smartcarlib::utils::getMap(num, 0, 10, 0, 100);
 *  // `num` is now `30`
 * \endcode
 */
template <typename AnyNumber>
constexpr AnyNumber getMap(
    AnyNumber valueToMap, AnyNumber fromLow, AnyNumber fromHigh, AnyNumber toLow, AnyNumber toHigh)
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
 *
 * **Example:**
 * \code
 * bool result = smartcarlib::utils::areAlmostEqual(0.000001, 0,0000012);
 * // `result` is `true`
 * \endcode
 */
constexpr bool areAlmostEqual(float a, float b)
{
    // C++11 does not allow us to declare the delta as a variable within the function body
    // NOLINTNEXTLINE(readability-magic-numbers)
    return getAbsolute(getAbsolute(a) - getAbsolute(b)) <= 0.001F;
}
} // namespace utils
} // namespace smartcarlib
