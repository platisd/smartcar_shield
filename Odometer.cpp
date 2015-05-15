/*
*	Odometer.cpp - Handles the odometer (speed encoder) sensor of the Smartcar.
*	Version: 0.2
*	Author: Dimitris Platis (based on the Smartcar project by Team Pegasus)
* 	License: GNU GPL v3 http://www.gnu.org/licenses/gpl-3.0.html
*/
#include "AndroidCar.h"

/* ---- ODOMETER ---- */
void updateCounter(); //ISR for the odometer
// Macro to convert from odometer pulses to centimeters. (ca. 48 pulses per meter, that's why we multiply by 2 to find value in cm)
#define PulsesToCentimeters(pulses) (pulses << 1)

volatile unsigned long _pulseCounter = 0;

Odometer::Odometer(){
}

int Odometer::attach(unsigned short odometerPin){
	switch(odometerPin){ //converting digital pins to interrupts for Arduino Mega
		case 2:
			_odometerInterruptPin = 0;
			break;
		case 3:
			_odometerInterruptPin = 1;
			break;
		case 21:
			_odometerInterruptPin = 2;
			break;
		case 20:
			_odometerInterruptPin = 3;
			break;
		case 19:
			_odometerInterruptPin = 4;
			break;
		case 18:
			_odometerInterruptPin = 5;
			break;
		default:
			return 0; //signals invalid interrupt pin
	}
	attachInterrupt(_odometerInterruptPin, updateCounter, CHANGE);
	return 1;
}

void Odometer::begin(){
	_pulseCounter = 0; //initialize the counter
}

unsigned long Odometer::getDistance(){
	return PulsesToCentimeters(_pulseCounter);
}

void Odometer::detach(){
	_pulseCounter = 0; //reinitialize the counter so if distance is calculated again, result will be 0 and not what was left from before
	detachInterrupt(_odometerInterruptPin);
}

void updateCounter(){
	_pulseCounter++;
}
