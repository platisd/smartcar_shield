/*
*	Odometer.cpp - Handles the odometer (speed encoder) sensor of the Smartcar.
*	Version: 0.2
*	Author: Dimitris Platis (based on the Smartcar project by Team Pegasus)
* 	License: GNU GPL v3 http://www.gnu.org/licenses/gpl-3.0.html
*/
#include "CaroloCup.h"

/* ---- ODOMETER ---- */
void updateCounter1(); //ISR for the odometer
void updateCounter2();
// Macro to convert from odometer pulses to centimeters. (ca. 40 pulses per meter, that's why we multiply by 2.5 to find value in cm)
#define PulsesToCentimeters(pulses) ((pulses << 1)/5)

volatile unsigned long _pulseCounter[2];
static unsigned short odometers = 0;

Odometer::Odometer(){
_odometerID = odometers++;
}

int Odometer::attach(unsigned short odometerPin){
	_odometerInterruptPin = digitalPinToInterrupt(odometerPin);
	if (_odometerInterruptPin != NOT_AN_INTERRUPT){
		if (!_odometerID){
			attachInterrupt(_odometerInterruptPin, updateCounter1, CHANGE);
		}else if (_odometerID == 1){
			attachInterrupt(_odometerInterruptPin, updateCounter2, CHANGE);
		}else{
			return -1; //too many encoders attached
		}
		return 1; //everything went as it should
	}
	return 0; //invalid interrupt pin
}

void Odometer::begin(){
	_pulseCounter[_odometerID] = 0; //initialize the counter
}

unsigned long Odometer::getDistance(){
	return PulsesToCentimeters(_pulseCounter[_odometerID]);
}

void Odometer::detach(){
	_pulseCounter[_odometerID] = 0; //reinitialize the counter so if distance is calculated again, result will be 0 and not what was left from before
	detachInterrupt(_odometerInterruptPin);
}

void updateCounter1(){
	_pulseCounter[0]++;
}
void updateCounter2(){
	_pulseCounter[1]++;
}
