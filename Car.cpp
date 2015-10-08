/*
*	Car.cpp
*/
#include "CaroloCup.h"
/* --- CAR --- */

const unsigned short Car::DEFAULT_SERVO_PIN = 8;
const unsigned short Car::DEFAULT_ESC_PIN = 9;

const int IDLE_SPEED = 1500;
const int MAX_FRONT_SPEED = 1600; //can go to 1800
const int MAX_BACK_SPEED = 1200; //can go to 1200
const int STRAIGHT_WHEELS = 45;
const int MAX_RIGHT_DEGREES = 135;
const int MAX_LEFT_DEGREES = 60;

Car::Car(unsigned short steeringWheelPin, unsigned short escPin){
	setSteeringWheelPin(steeringWheelPin);
	setESCPin(escPin);
}

void Car::begin(){
	motor.attach(_escPin);
	steeringWheel.attach(_steeringWheelPin);
	setSpeed(0);
	setAngle(0);
}

void Car::setSteeringWheelPin(unsigned short steeringWheelPin){
	_steeringWheelPin = steeringWheelPin;
}

void Car::setESCPin(unsigned short escPin){
	_escPin = escPin;
}

void Car::setSpeed(int speed){
	_speed = constrain(IDLE_SPEED + speed, MAX_BACK_SPEED, MAX_FRONT_SPEED);
	motor.write(_speed);
}

void Car::setAngle(int degrees){
	_angle = constrain(STRAIGHT_WHEELS + degrees, MAX_LEFT_DEGREES, MAX_RIGHT_DEGREES);
	steeringWheel.write(_angle);
}

void Car::stop(){
	if (getSpeed()>0){
		setSpeed(-100);
		delay(35);
		setSpeed(0);
	}else if(getSpeed()<0){
		setSpeed(100);
		delay(35);
		setSpeed(0);
	}
}

int Car::getSpeed(){
	return _speed - IDLE_SPEED;
}

int Car::getAngle(){
	return _angle - STRAIGHT_WHEELS;
}
