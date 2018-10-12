/**
 * Class to calculate the median value of an array with numbers
 */
#pragma once

class Median
{
public:
    /**
     * Gets the median value out of the supplied number array
     * @param  unsortedNumbers   An array containing numbers
     * @param  arraySize         Amount of numbers contained in the array
     * @return                   Median number of the supplied array
     */
    template <typename AnyNumber>
    static AnyNumber getMedian(AnyNumber unsortedNumbers[], unsigned int arraySize)
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
};
