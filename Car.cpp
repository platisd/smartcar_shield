/*
*	Car.cpp
*/
#include "CaroloCup.h"
/* --- CAR --- */

const unsigned short Car::DEFAULT_SERVO_PIN = 8;
const unsigned short Car::DEFAULT_ESC_PIN = 9;
const unsigned short Car::DEFAULT_PID_LOOP_INTERVAL = 40;
const float Car::DEFAULT_KP = 5.0;
const float Car::DEFAULT_KI = 0.0;
const float Car::DEFAULT_KD = 10.0;

const int IDLE_RAW_SPEED = 1500;
const int MAX_FRONT_RAW_SPEED = 1650; //can go to 1800
const int MAX_BACK_RAW_SPEED = 1200; //can go to 1200
const int IDLE_SPEED = 0; //the user-set frequency where the car is idle
const int MAX_BACK_SPEED = MAX_BACK_RAW_SPEED - IDLE_RAW_SPEED; //the minimum user-set frequency we are allowed to go
const int MAX_FRONT_SPEED = MAX_FRONT_RAW_SPEED - IDLE_RAW_SPEED; //the max user-set frequency we are allowed to go
const float MAX_BACK_CRUISE_SPEED = -2;
const float MAX_FRONT_CRUISE_SPEED = 2;
const int STRAIGHT_WHEELS = 90;
const int MAX_RIGHT_DEGREES = 120;
const int MAX_LEFT_DEGREES = 60;

Car::Car(unsigned short steeringWheelPin, unsigned short escPin){
	setSteeringWheelPin(steeringWheelPin);
	setESCPin(escPin);
	cruiseControl = false;
}

void Car::begin(){
	motor.attach(_escPin);
	steeringWheel.attach(_steeringWheelPin);
	setSpeed(IDLE_SPEED);
	setAngle(IDLE_SPEED);
}

void Car::setSteeringWheelPin(unsigned short steeringWheelPin){
	_steeringWheelPin = steeringWheelPin;
}

void Car::setESCPin(unsigned short escPin){
	_escPin = escPin;
}

void Car::setSpeed(float newSpeed){
	if (cruiseControl){
		if (_speed && (_speed != IDLE_RAW_SPEED) && (newSpeed * _speed) <= 0) stop(); //if the speeds are signed differently, stop the car and then set the new speed
		_speed = constrain(newSpeed, MAX_BACK_CRUISE_SPEED, MAX_FRONT_CRUISE_SPEED);
	}else{
		_speed = constrain(IDLE_RAW_SPEED + int(newSpeed), MAX_BACK_RAW_SPEED, MAX_FRONT_RAW_SPEED);
		setRawSpeed(_speed);
	}
}

void Car::updateMotors(){
	if (cruiseControl && (millis() > _lastMotorUpdate + _pidLoopInterval)){
		if (_speed){ //if _speed is 0, we have already made sure the car is stopped. don't try to adjust if car is just drifting
			float measuredSpeed = getGroundSpeed(); //speed in cm/milliseconds
			measuredSpeed *= 10; //transform it into m/seconds, divide by 100 to turn cm into m and multiply by 1000, to turn ms to sec
			if (_speed < 0) measuredSpeed *= -1; //if we are going reverse, illustrate that in the value of measuredSpeed
			int controlledSpeed = motorPIDcontrol(_previousControlledSpeed, _speed, measuredSpeed);
			setRawSpeed(controlledSpeed);
			_previousControlledSpeed = controlledSpeed;
		}
		_lastMotorUpdate = millis();
	}
}

int Car::motorPIDcontrol(const int previousSpeed, const float targetSpeed, const float actualSpeed){
	float correction = 0;
	float error = targetSpeed - actualSpeed;
	_integratedError += error;
	correction = (_Kp * error) + (_Ki * _integratedError) + (_Kd * (error - _previousError));                            
	_previousError = error;
//	Serial.print("\t\tError: ");
//	Serial.print(error);
	return constrain(previousSpeed + int(correction), MAX_BACK_RAW_SPEED, MAX_FRONT_RAW_SPEED);
}

 float Car::getGroundSpeed(){
	unsigned long currentDistance = _encoder.getDistance();
	unsigned long dX = currentDistance - _previousDistance;
	_previousDistance = currentDistance;
	unsigned long dT = millis() - _lastMotorUpdate;
	float velocity = float(dX)/ float(dT);
	return velocity;
}

void Car::setRawSpeed(int rawSpeed){ //platform specific method
	unsigned int speed = constrain(rawSpeed, MAX_BACK_RAW_SPEED, MAX_FRONT_RAW_SPEED);
	motor.write(speed);
}

void Car::setAngle(int degrees){ //platform specific method
	_angle = constrain(STRAIGHT_WHEELS + degrees, MAX_LEFT_DEGREES, MAX_RIGHT_DEGREES);
	steeringWheel.write(_angle);
}

void Car::stop(){ //platform specific method
	if (getSpeed()>IDLE_SPEED){
		setRawSpeed(-100);
	}else if(getSpeed()<IDLE_SPEED){
		setRawSpeed(100);
	}
	delay(35);
	if (cruiseControl){
		setRawSpeed(IDLE_SPEED); //shut the engines down, we should be stopped by now
		enableCruiseControl(_encoder); //re-initialize the cruise control, se we get rid of previous error and pid output
		_speed = IDLE_SPEED;
	}else{
		setSpeed(IDLE_SPEED);
	}
}

float Car::getSpeed(){
	if (cruiseControl) return _speed; //return speed in meters per second
	return _speed - IDLE_RAW_SPEED; //return speed in microseconds
}

int Car::getAngle(){
	return _angle - STRAIGHT_WHEELS;
}

void Car::enableCruiseControl(Odometer encoder, float Kp, float Ki, float Kd, unsigned short pidLoopInterval){
	_encoder = encoder;
	cruiseControl = true;
	_pidLoopInterval = pidLoopInterval;
	_lastMotorUpdate = 0;
	_previousControlledSpeed = IDLE_RAW_SPEED;
	_previousDistance = _encoder.getDistance();
	_integratedError = 0;
	_Kp = Kp;
	_Ki = Ki;
	_Kd = Kd;
}

void Car::disableCruiseControl(){
	cruiseControl = false;
	_speed = _previousControlledSpeed; //update the speed with the PWM equivalent
}
