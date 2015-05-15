/*
*	Sonar.cpp - Handles the ultra sound (HC-SR04 & SRF05) sensors of the Smartcar
*	Version: 0.2
*	Author: Dimitris Platis
*	Sonar class is essentially a stripped-down version of the NewPing library by Tim Eckel adjusted to Smartcar needs
* 	Get original library at: http://code.google.com/p/arduino-new-ping/
* 	License: GNU GPL v3 http://www.gnu.org/licenses/gpl-3.0.html
*/
#include "AndroidCar.h"

/* ------ SONAR ------ */
static const int MAX_US_DISTANCE = 70; // Maximum usable sensor distance is around 70cm.
static const int US_ROUNDTRIP_CM = 57;      // Microseconds (uS) it takes sound to travel round-trip 1cm (2cm total), uses integer to save compiled code space.
static const int DISABLE_ONE_PIN = true;   // Set to "true" to save up to 26 bytes of compiled code space if you're NOT using one pin sensor connections.

// Probably shoudln't change these values unless you really know what you're doing.
static const int NO_ECHO = 0;               // Value returned if there's no ping echo within the specified MAX_SENSOR_DISTANCE
static const int MAX_SENSOR_DELAY = 18000;  // Maximum uS it takes for sensor to start the ping (SRF06 is the highest measured, just under 18ms).
static const int ECHO_TIMER_FREQ = 24;      // Frequency to check for a ping echo (every 24uS is about 0.4cm accuracy).
static const int PING_MEDIAN_DELAY = 29;    // Millisecond delay between pings in the getMedianDistance method.
static const int SONAR_DEFAULT_ITERATIONS = 5;    // The default value of iterations used in getMedianDistance() method.


// Macro to convert from microseconds to centimeters.
#define MicrosecondsToCentimeters(echoTime) (max((echoTime + US_ROUNDTRIP_CM / 2) / US_ROUNDTRIP_CM, (echoTime ? 1 : 0)))


Sonar::Sonar() {
}

void Sonar::attach(int triggerPin, int echoPin){
	_triggerBit = digitalPinToBitMask(triggerPin); // Get the port register bitmask for the trigger pin.
	_echoBit = digitalPinToBitMask(echoPin);       // Get the port register bitmask for the echo pin.

	_triggerOutput = portOutputRegister(digitalPinToPort(triggerPin)); // Get the output port register for the trigger pin.
	_echoInput = portInputRegister(digitalPinToPort(echoPin));         // Get the input port register for the echo pin.

	_triggerMode = (uint8_t *) portModeRegister(digitalPinToPort(triggerPin)); // Get the port mode register for the trigger pin.

	_maxEchoTime = MAX_US_DISTANCE * US_ROUNDTRIP_CM + (US_ROUNDTRIP_CM / 2); // Calculate the maximum distance in uS.

#if DISABLE_ONE_PIN == true
	*_triggerMode |= _triggerBit; // Set trigger pin to output.
#endif
}

unsigned int Sonar::ping() {
	if (!ping_trigger()) return NO_ECHO;                // Trigger a ping, if it returns false, return NO_ECHO to the calling function.
	while (*_echoInput & _echoBit)                      // Wait for the ping echo.
		if (micros() > _max_time) return NO_ECHO;       // Stop the loop and return NO_ECHO (false) if we're beyond the set maximum distance.
	return (micros() - (_max_time - _maxEchoTime) - 5); // Calculate ping time, 5uS of overhead.
}

unsigned int Sonar::getDistance() {
	unsigned int echoTime = Sonar::ping();          // Calls the ping method and returns with the ping echo distance in uS.
	return MicrosecondsToCentimeters(echoTime); // Convert uS to centimeters.
}

unsigned int Sonar::getMedianDistance() {
	Sonar::getMedianDistance(SONAR_DEFAULT_ITERATIONS);
}

unsigned int Sonar::getMedianDistance(short iterations) {
	unsigned int uS[iterations], last;
	uint8_t j, i = 0;
	uS[0] = NO_ECHO;
	while (i < iterations) {
		last = ping();           // Send ping.
		if (last == NO_ECHO) {   // Ping out of range.
			iterations--;                // Skip, don't include as part of median.
			last = _maxEchoTime; // Adjust "last" variable so delay is correct length.
		} else {                       // Ping in range, include as part of median.
			if (i > 0) {               // Don't start sort till second ping.
				for (j = i; j > 0 && uS[j - 1] < last; j--) // Insertion sort loop.
					uS[j] = uS[j - 1]; // Shift ping array to correct position for sort insertion.
			} else j = 0;              // First ping is starting point for sort.
			uS[j] = last;              // Add last ping to array in sorted position.
			i++;                       // Move to next ping.
		}
		if (i < iterations) delay(PING_MEDIAN_DELAY - (last >> 10)); // Millisecond delay between pings.
	}
	return MicrosecondsToCentimeters((uS[iterations >> 1])); // Return the ping distance median.
}


/* Standard ping method helper functions */
boolean Sonar::ping_trigger() {
#if DISABLE_ONE_PIN != true
	*_triggerMode |= _triggerBit;    // Set trigger pin to output.
#endif
	*_triggerOutput &= ~_triggerBit; // Set the trigger pin low, should already be low, but this will make sure it is.
	delayMicroseconds(4);            // Wait for pin to go low, testing shows it needs 4uS to work every time.
	*_triggerOutput |= _triggerBit;  // Set trigger pin high, this tells the sensor to send out a ping.
	delayMicroseconds(10);           // Wait long enough for the sensor to realize the trigger pin is high. Sensor specs say to wait 10uS.
	*_triggerOutput &= ~_triggerBit; // Set trigger pin back to low.
#if DISABLE_ONE_PIN != true
	*_triggerMode &= ~_triggerBit;   // Set trigger pin to input (when using one Arduino pin this is technically setting the echo pin to input as both are tied to the same Arduino pin).
#endif

	_max_time =  micros() + MAX_SENSOR_DELAY;                  // Set a timeout for the ping to trigger.
	while (*_echoInput & _echoBit && micros() <= _max_time) {} // Wait for echo pin to clear.
	while (!(*_echoInput & _echoBit))                          // Wait for ping to start.
		if (micros() > _max_time) return false;                // Something went wrong, abort.

	_max_time = micros() + _maxEchoTime; // Ping started, set the timeout.
	return true;                         // Ping started successfully.
}

