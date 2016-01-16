#include "Smartcar.h"

DCMotors::DCMotors(unsigned short shieldOrientation) {
	if (shieldOrientation == STANDARD){ //the default shield setup, where right is right
		MOTOR_LEFT1_PIN = 8;
		MOTOR_LEFT_EN_PIN = 9;
		MOTOR_LEFT2_PIN = 10;
		MOTOR_RIGHT_EN_PIN = 11;
		MOTOR_RIGHT1_PIN = 12;
		MOTOR_RIGHT2_PIN = 13;
	}else{ //the reversed shield setup, where right is left
		MOTOR_RIGHT1_PIN = 8;
		MOTOR_RIGHT_EN_PIN = 9;
		MOTOR_RIGHT2_PIN = 10;
		MOTOR_LEFT_EN_PIN = 11;
		MOTOR_LEFT1_PIN = 12;
		MOTOR_LEFT2_PIN = 13;
	}
	setDegrees();
	setAllowedAngles();
	setFreqsAndSpeeds();
	setAllowedSpeedLimits();
}

void DCMotors::setDegrees(){
	STRAIGHT_RAW_DEGREES = 90;
	MAX_RIGHT_RAW_DEGREES = 180;
	MAX_LEFT_RAW_DEGREES = 0;
}

void DCMotors::setFreqsAndSpeeds(){
	FULL_FORWARD = 100;
        FULL_BACKWARD = 100;
	IDLE_RAW_SPEED = 0;
	MAX_FRONT_RAW_SPEED = 255;
	MAX_BACK_RAW_SPEED = -255;
}

void DCMotors::init(){
	for (uint8_t i = MOTOR_LEFT1_PIN; i <= MOTOR_RIGHT2_PIN; i++) {
		pinMode(i, OUTPUT); //declare pins as outputs
	}
}

void DCMotors::setSpeed(int speed){ //receives a speed in the scale of -100 to 100
	_speed = filterSpeed(speed); //_speed now holds a value between MAX_BACK_ALLOWED_SPEED and MAX_FRONT_ALLOWED_SPEED
}

void DCMotors::setAngle(int degrees){// receives some degrees in the scale of MAX_LEFT_ANGLE, MAX_RIGHT_ANGLE	
	_angle = filterAngle(degrees); //_angle now holds a value between MAX_LEFT_RAW_DEGREES and MAX_RIGHT_RAW_DEGREES
}
