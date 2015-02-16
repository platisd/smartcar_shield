/*
*	Odometer.cpp - Handles the odometer (speed encoder) sensor of the Smartcar.
*	Version: 0.2
*	Author: Dimitris Platis (based on the Smartcar project by Team Pegasus)
* 	License: GNU GPL v3 http://www.gnu.org/licenses/gpl-3.0.html
*/
#include "Smartcar_sensors.h"

volatile unsigned int _pulseCounter = 0;

Odometer::Odometer(){
}

int Odometer::attach(int odometerPin){
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
	attachInterrupt(_odometerInterruptPin, updateCounter, RISING);
	return 1;
}

void Odometer::begin(){
	_pulseCounter = 0; //initialize the counter
}

unsigned int Odometer::getDistance(){
	return PulsesToCentimeters(_pulseCounter);
}

void Odometer::detach(){
	_pulseCounter = 0; //reinitialize the counter so if distance is calculated again, result will be 0 and not what was left from before
	detachInterrupt(_odometerInterruptPin);
}

void updateCounter(){
	_pulseCounter++;
}
