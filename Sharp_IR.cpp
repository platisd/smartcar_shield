/*
*	Sharp_IR.cpp - Handles the infra red (Sharp GP2Y0A02 & GP2D120) sensors of the Smartcar
*	sensors. 
*	Version: 0.3
*	Author: Dimitris Platis
*	Formula to convert volts to centimeters found in: http://luckylarry.co.uk/arduino-projects/arduino-using-a-sharp-ir-sensor-for-distance-calculation/
* 	Get original library at: http://code.google.com/p/arduino-new-measurement/
* 	License: GNU GPL v3 http://www.gnu.org/licenses/gpl-3.0.html
*/

#include "AndroidCar.h"

/* ---- SHARP INFRARED SENSOR ---- */
static const float SHARP_SENSITIVITY = 0.0048828125; //only use with GP2Y0A02
//static const int MIN_IR_DISTANCE = 20; //Minimum reliable distance the sensor can measure. Derived from datasheet. For GP2Y0A02
//static const int MAX_IR_DISTANCE = 80;//Maximum reliable distance the sensor can measure in real life conditions. Determined experimentally. GP2Y0A02
static const int MIN_IR_DISTANCE = 4; //Minimum reliable distance the sensor can measure. Derived from datasheet. For GP2D120
static const int MAX_IR_DISTANCE = 25;//Maximum reliable distance the sensor can measure in real life conditions.Determined experimentally. GP2D120
static const int IR_DEFAULT_ITERATIONS = 5;    // The default value of iterations used in getMedianDistance() method.
static const int IR_MEDIAN_DELAY = 15; //Millisecond delay between measurements in the getMedianDistance method.

// Macro to convert from volts to centimeters.
//#define VoltsToCentimeters(volts) (65*pow(volts, -1.10)) //decomment for Sharp GP2Y0A02 as well as in Sharp_IR::getDistance()
#define VoltsToCentimeters(volts) ((2914 / (volts + 5)) - 1) //decomment for Sharp GP2D120 as well as in Sharp_IR::getDistance()


Sharp_IR::Sharp_IR(){
}

void Sharp_IR::attach(unsigned short IR_pin){
	pinMode(IR_pin, INPUT);
	_IR_pin = IR_pin;
}

unsigned int Sharp_IR::getDistance(){
	int volts = analogRead(_IR_pin); //* SHARP_SENSITIVITY;
	int distance = VoltsToCentimeters(volts); //decomment this line  and comment the two previous ones for GP2D120
	if ((distance < MIN_IR_DISTANCE) || (distance > MAX_IR_DISTANCE)){
		return 0;
	}
	return distance;
}

unsigned int Sharp_IR::getMedianDistance(){
	Sharp_IR::getMedianDistance(IR_DEFAULT_ITERATIONS);
}

unsigned int Sharp_IR::getMedianDistance(int iterations){
	unsigned int measurements[iterations], last;
	uint8_t j, i = 0;
	measurements[0] = 0; //initializing the array
	while (i < iterations) {
		last = Sharp_IR::getDistance();           //get IR measurements
		if (!last) {   // measurement out of range.
			iterations--;                // Skip, don't include as part of median.
			last = MAX_IR_DISTANCE; // Adjust "last" variable so delay is correct length.
		} else {                       // measurement in range, include as part of median.
			if (i > 0) {               // Don't start sort till second measurement.
				for (j = i; j > 0 && measurements[j - 1] < last; j--) // Insertion sort loop.
					measurements[j] = measurements[j - 1]; // Shift measurement array to correct position for sort insertion.
			} else j = 0;              // First measurement is starting point for sort.
			measurements[j] = last;              // Add last measurement to array in sorted position.
			i++;                       // Move to next measurement.
		}
		if (i < iterations) delay(IR_MEDIAN_DELAY - (last >> 10)); // Millisecond delay between measurements.
	}
	return (measurements[iterations >> 1]); // Return the measurement distance median.
}
