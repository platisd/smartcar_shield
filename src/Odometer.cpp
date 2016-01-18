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

unsigned int _millimetersPerPulse[2] = {0};
volatile unsigned long _previousPulse[2] = {0};
volatile unsigned long _dt[2] = {0};
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

float Odometer::getSpeed(){//when we need the speed, we just devide the milimeters per pulse of the specific sensor with the length between 2 pulses
	if (_dt[_odometerID]){ //if _dt is not 0 (in the beginning it is)
		return (float) _millimetersPerPulse[_odometerID] / _dt[_odometerID]; // essentially this is calculating the speed by doing: dx/dt
	}else{ //if _dt is 0, then avoid dividing by it and return 0 as speed (it should happen only in the beginning)
		return 0;
	}
}

void updateDt(unsigned short odometerID){
	unsigned long currentPulse = millis();
	unsigned long dt = currentPulse - _previousPulse[odometerID]; //calculate the difference in time between the two pulses
	if (dt){ //update the duration between two pulses for odometer 0, if the dt is not too small (0) (in case the pulses arrived too fast)
		_dt[odometerID] = dt; //update the _dt value
		_previousPulse[odometerID] = currentPulse; //update when the last pulse arrive (if it didn't arrive too fast)
	}
}

void updateCounter1(){
	updateDt(0); //updates the respective index of _dt with the difference in time between the last two pulses
	_pulseCounter[0]++; //updates the pulse counter of odometer 0
}
void updateCounter2(){
	updateDt(1); //updates the respective index of _dt with the difference in time between the last two pulses
	_pulseCounter[1]++;//updates the pulse counter of odometer 1
}
