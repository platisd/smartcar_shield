#include "Smartcar.h"

ESCMotor::ESCMotor(unsigned short pin) {
	_pin = pin;
	setFreqsAndSpeeds();
	setAllowedSpeedLimits();
}

void ESCMotor::setFreqsAndSpeeds(){
	FULL_FORWARD = 50;
        FULL_BACKWARD = 80;
	IDLE_RAW_SPEED = 1500;
	MAX_FRONT_RAW_SPEED = 2000;
	MAX_BACK_RAW_SPEED = 1000;
}

void ESCMotor::init(){
//TO-DO
}

void ESCMotor::setSpeed(int speed){
	_speed = speed;
}
