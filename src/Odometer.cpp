/*
*	Odometer.cpp - Handles the odometer (speed encoder) sensor of the Smartcar.
*	Version: 0.2
*	Author: Dimitris Platis (based on the Smartcar project by Team Pegasus)
* 	License: GNU GPL v3 http://www.gnu.org/licenses/gpl-3.0.html
*/
#include "Smartcar.h"

/* ---- ODOMETER ---- */
void updateCounter1(); //ISR for the odometer
void updateCounter2();

volatile unsigned int _millimetersPerPulse[2] = {0};
volatile float _measuredSpeed[2] = {0};
volatile unsigned long _previousPulseTime[2] = {0};
volatile unsigned long _pulseCounter[2] = {0};
static unsigned short odometers = 0;
const unsigned int Odometer::DEFAULT_PULSES_PER_METER = 92;

Odometer::Odometer(unsigned int pulsesPerMeter){
	if (pulsesPerMeter){ //if user supplied 0, then put the default value instead to avoid errors later
		_pulsesPerMeter = pulsesPerMeter;
	}else{
		_pulsesPerMeter = DEFAULT_PULSES_PER_METER;	
	}
	_odometerID = odometers++;
	_millimetersPerPulse[_odometerID] = lroundf(1000.0 / _pulsesPerMeter); //round a float to the nearest long
}

int Odometer::attach(unsigned short odometerPin){
	_odometerInterruptPin = digitalPinToInterrupt(odometerPin);
	if (_odometerInterruptPin != NOT_AN_INTERRUPT){
		if (!_odometerID){
			attachInterrupt(_odometerInterruptPin, updateCounter1, RISING);
		}else if (_odometerID == 1){
			attachInterrupt(_odometerInterruptPin, updateCounter2, RISING);
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
	return pulsesToCentimeters(_pulseCounter[_odometerID]);
}

unsigned long Odometer::pulsesToCentimeters(unsigned long pulses){
	return pulses/(_pulsesPerMeter/100.0);
}

boolean Odometer::isInstanciated(){
	return _pulsesPerMeter; //if 0, it will return false, otherwise true. 0 is an error value
}

float Odometer::getSpeed(){
	return _measuredSpeed[_odometerID];
}

void updateOdometerSpeed(unsigned short odometerID){ //calculates the speed for the given odometer
	unsigned long currentTime = millis();
	unsigned long dt = currentTime - _previousPulseTime[odometerID]; //in milliseconds
	if (dt){ //avoid division by 0 in case we had a very fast (less than 1 ms) pulse
		_measuredSpeed[odometerID] = (float) _millimetersPerPulse[odometerID] / dt; //x & t are in milli scale, so the result can be in m/s
	}
	_previousPulseTime[odometerID] = currentTime;
}

void updateCounter1(){
	updateOdometerSpeed(0);
	_pulseCounter[0]++;
}
void updateCounter2(){
	updateOdometerSpeed(1);
	_pulseCounter[1]++;
}
