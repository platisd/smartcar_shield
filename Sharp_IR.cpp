/*
*	Sharp_IR.cpp - Handles the infra red (Sharp GP2Y0A02) sensors of the Smartcar
*	sensors. 
*	Version: 0.1
*	Author: Dimitris Platis
*	Formula to convert volts to centimeters found in: http://luckylarry.co.uk/arduino-projects/arduino-using-a-sharp-ir-sensor-for-distance-calculation/
* 	Get original library at: http://code.google.com/p/arduino-new-ping/
* 	License: GNU GPL v3 http://www.gnu.org/licenses/gpl-3.0.html
*/

#include "Smartcar_sensors.h"

Sharp_IR::Sharp_IR(){
}

void Sharp_IR::attach(int IR_pin){
	_IR_pin = IR_pin;
}

unsigned int Sharp_IR::getDistance(){
	float volts = analogRead(_IR_pin) * SHARP_SENSITIVITY;
	int distance = VoltsToCentimeters(volts);
	if ((distance < MIN_IR_DISTANCE) || (distance > MAX_IR_DISTANCE)){
		return 0;
	}
	return distance;
}
