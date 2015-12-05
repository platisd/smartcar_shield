/* An abstract class to represent a distance sensor, that offers the ability to measure a distance and perform a series of measurements
 in order to get the median distance. */

#include "Smartcar.h"

const short DistanceSensor::DEFAULT_ITERATIONS = 5;

DistanceSensor::DistanceSensor() {}

DistanceSensor::~DistanceSensor(){}

unsigned int DistanceSensor::getMedianDistance(short iterations){ //adopted from NewPing library
	unsigned int measurements[iterations], last;
	uint8_t j, i = 0;
	measurements[0] = 0; //initializing the array
	while (i < iterations) {
		last = getDistance();           //get measurements
		if (!last) {   // measurement out of range.
			iterations--;                // Skip, don't include as part of median.
		} else {                       // measurement in range, include as part of median.
			if (i > 0) {               // Don't start sort till second measurement.
				for (j = i; j > 0 && measurements[j - 1] < last; j--) // Insertion sort loop.
					measurements[j] = measurements[j - 1]; // Shift measurement array to correct position for sort insertion.
			} else j = 0;              // First measurement is starting point for sort.
			measurements[j] = last;              // Add last measurement to array in sorted position.
			i++;                       // Move to next measurement.
		}
		if (i < iterations) delay(_sensorMedianDelay); // Millisecond delay between measurements.
	}
	return (measurements[iterations >> 1]); // Return the measurement distance median.
}
