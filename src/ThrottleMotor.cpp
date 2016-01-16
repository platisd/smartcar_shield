#include "Smartcar.h"

ThrottleMotor::ThrottleMotor() {}

ThrottleMotor::~ThrottleMotor(){}

void ThrottleMotor::setSpeed(int speed){ //to be overriden by child classes
//receives a speed in the scale of -100 to 100
//updates _speed which now holds a value between MAX_BACK_ALLOWED_SPEED and MAX_FRONT_ALLOWED_SPEED
//sets the speed on each motor
}

void ThrottleMotor::setFreqsAndSpeeds(){ //to be overriden by child classes
//sets the various motor-specific, maximum and minimum values
}

int ThrottleMotor::getSpeed(){
	return _speed; //returns the speed written in the motor (could be signed)
}

void ThrottleMotor::setAllowedSpeedLimits(){
	MAX_FRONT_ALLOWED_SPEED = MAX_FRONT_RAW_SPEED * (FULL_FORWARD / 100.0);
	MAX_BACK_ALLOWED_SPEED = MAX_BACK_RAW_SPEED * (FULL_BACKWARD / 100.0);
}

int ThrottleMotor::filterSpeed(int speed){
//	constrain the speed between the allowed percentages [-100,100]
	speed = constrain(speed, -100, 100);
//	map the speed. We map negative and positive values separately in case the respective maximum percentages aren't symmetric
	if (speed <= 0){
		speed = map(speed, -100, 0, MAX_BACK_ALLOWED_SPEED, IDLE_RAW_SPEED);
	}else{
		speed = map(speed, 0, 100, IDLE_RAW_SPEED, MAX_FRONT_ALLOWED_SPEED);
	}
	return speed;
}


