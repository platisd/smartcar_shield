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
	_speed = IDLE_RAW_SPEED;
}

void ESCMotor::init(){
	attach(_pin); //attach the servo to its pin
}

void ESCMotor::setSpeed(int speed){ //receives a speed in the scale of -100 to 100
	_speed = filterSpeed(speed); //_speed now holds a value between MAX_BACK_ALLOWED_SPEED and MAX_FRONT_ALLOWED_SPEED
	writeMicroseconds(_speed); //write the appropriate pwm signal to the servo
}
