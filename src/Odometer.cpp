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

volatile unsigned long _previousPulse[2] = {0};
volatile unsigned long _dt[2] = {0};
volatile unsigned long _pulseCounter[2] = {0};
static unsigned short odometers = 0;
const unsigned int Odometer::DEFAULT_PULSES_PER_METER = 180;
const unsigned long MINIMUM_PULSE_GAP = 700; //the minimum acceptable time between two pulses in microseconds. Less is invalid unstable signal

Odometer::Odometer(unsigned int pulsesPerMeter){
	if (pulsesPerMeter){ //if user supplied 0, then put the default value instead to avoid errors later
		_pulsesPerMeter = pulsesPerMeter;
	}else{
		_pulsesPerMeter = DEFAULT_PULSES_PER_METER;	
	}
	_odometerID = odometers++;
	_millimetersPerPulse = lroundf(1000.0 / _pulsesPerMeter); //round a float to the nearest long
	_odometerInterruptPin = 0; //give it an initial (likely invalid) value
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

float Odometer::getSpeed(){//when we need the speed, we just devide the milimeters per pulse of the specific sensor with the length between 2 pulses
	unsigned long dt = _dt[_odometerID];
	if (dt){ //if dt is not 0 (in the beginning it is)
		return 1000.0 * _millimetersPerPulse / dt; // calculating the speed in meters/second
	}else{ //if _dt is 0, then avoid dividing by it and return 0 as speed (it should happen only in the beginning)
		return 0;
	}
}

void updateDtAndCounter(unsigned short odometerID){ //updates dt with the time difference between the last two pulses and increases the pulse counter
	unsigned long currentPulse = micros();
	unsigned long dt = currentPulse - _previousPulse[odometerID]; //calculate the difference in time between the two pulses in microseconds
	if (dt > MINIMUM_PULSE_GAP){ //if the pulses have not arrived too fast, which is a sign of unstable signal (too much jitter) in microseconds
		_dt[odometerID] = dt; //update the _dt value
		_previousPulse[odometerID] = currentPulse; //update when the last pulse arrived (if it didn't arrive too fast)
		_pulseCounter[odometerID]++; //updates the pulse counter of odometer 0
	}
}

void updateCounter1(){
	updateDtAndCounter(0);
}
void updateCounter2(){
	updateDtAndCounter(1);
}
