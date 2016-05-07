#include "Smartcar.h"

const unsigned short BACKWARD = 0;
const unsigned short FORWARD = 1;
const unsigned short IDLE = 2;

ShieldMotors::ShieldMotors(unsigned short shieldOrientation) {
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

void ShieldMotors::setDegrees(){
	STRAIGHT_RAW_DEGREES = 90;
	MAX_RIGHT_RAW_DEGREES = 180;
	MAX_LEFT_RAW_DEGREES = 0;
	_angle = STRAIGHT_RAW_DEGREES;
}

void ShieldMotors::setFreqsAndSpeeds(){
	FULL_FORWARD = 100;
        FULL_BACKWARD = 100;
	IDLE_RAW_SPEED = 0;
	MAX_FRONT_RAW_SPEED = 255;
	MAX_BACK_RAW_SPEED = -255;
	_speed = IDLE_RAW_SPEED;
}

void ShieldMotors::init(){
	for (uint8_t i = MOTOR_LEFT1_PIN; i <= MOTOR_RIGHT2_PIN; i++) {
		pinMode(i, OUTPUT); //declare pins as outputs
	}
}

void ShieldMotors::setSpeed(int speed){ //receives a speed in the scale of -100 to 100
	_speed = filterSpeed(speed); //_speed now holds a value between MAX_BACK_ALLOWED_SPEED and MAX_FRONT_ALLOWED_SPEED
	setMotors(); //update the motors with the new speed
}

void ShieldMotors::setAngle(int degrees){// receives some degrees in the scale of MAX_LEFT_ANGLE, MAX_RIGHT_ANGLE	
	_angle = filterAngle(degrees); //_angle now holds a value between MAX_LEFT_RAW_DEGREES and MAX_RIGHT_RAW_DEGREES
	setMotors(); //update the motors with the new angle
}

void ShieldMotors::setMotors(){ //sets the speed to the two motors on each side of the car, that are connected to the smartcar shield
//	set the direction of movement, calculate the ratio of difference between the two sides of the car and then set the speed
//	setting the direction, depending on the sign of _speed
	if (_speed < 0){
		setDirection(BACKWARD);	
	}else if (_speed > 0){
		setDirection(FORWARD);
	}else{
		setDirection(IDLE);
	}
//	calculate the difference between the speeds, on each side of the car, depending on the _angle
	unsigned int rawSpeed = abs(_speed);
//	find the relation between the degrees of the straight wheels with _angle.  e.g. when _angle is the same as STRAIGHT_RAW_DEGREES, we don't need to turn, therefore the ratio (or difference between the speed of each side of the car, will be 0%
	int relativeAngle = STRAIGHT_RAW_DEGREES - _angle; //the angle (as perceived by the motors) offsetted by STRAIGHT_RAW_DEGREES
	float ratio = (STRAIGHT_RAW_DEGREES - abs(relativeAngle)) / (float) STRAIGHT_RAW_DEGREES;
//	set the appropriate speed on each side, depending on the ratio and the sign of the _angle (whether we go left or right)
	if (_angle > STRAIGHT_RAW_DEGREES){ //turning to the right
		analogWrite(MOTOR_LEFT_EN_PIN, rawSpeed); //write the (unsigned) _speed to the left side motors
		analogWrite(MOTOR_RIGHT_EN_PIN, int(rawSpeed * ratio)); //write the decreased, according to ratio, speed to the right side motors
	}else{ //turning to the left or going straight
		analogWrite(MOTOR_RIGHT_EN_PIN, rawSpeed); //write the (unsigned) _speed to the right side motors
		analogWrite(MOTOR_LEFT_EN_PIN, int(rawSpeed * ratio)); //write the decreased, according to ratio, speed to the left side motors
	} 
}

void ShieldMotors::setDirection(unsigned short direction){ //sets the direction for the L293D h-bridge found on the Smartcar shield
	if (direction == BACKWARD){
		digitalWrite(MOTOR_RIGHT1_PIN, LOW);
		digitalWrite(MOTOR_RIGHT2_PIN, HIGH);
		digitalWrite(MOTOR_LEFT1_PIN, LOW);
		digitalWrite(MOTOR_LEFT2_PIN, HIGH);
	}else if (direction == FORWARD){
		digitalWrite(MOTOR_RIGHT1_PIN, HIGH);
		digitalWrite(MOTOR_RIGHT2_PIN, LOW);
		digitalWrite(MOTOR_LEFT1_PIN, HIGH);
		digitalWrite(MOTOR_LEFT2_PIN, LOW);	
	}else{
		digitalWrite(MOTOR_RIGHT1_PIN, LOW);
		digitalWrite(MOTOR_RIGHT2_PIN, LOW);
		digitalWrite(MOTOR_LEFT1_PIN, LOW);
		digitalWrite(MOTOR_LEFT2_PIN, LOW);
	}
}

void ShieldMotors::setMotorSpeed(int leftMotorSpeed, int rightMotorSpeed){ //sets manually the speed in the scale from -100 to 100 on each side
//	filter the speeds to hold a value between MAX_BACK_ALLOWED_SPEED and MAX_FRONT_ALLOWED_SPEED
	leftMotorSpeed = filterSpeed(leftMotorSpeed);
	rightMotorSpeed = filterSpeed(rightMotorSpeed);
//	we don't update _speed and _angle in this case, since it might not make sense, if sides are spinning towards different direction
	if (leftMotorSpeed<0){ //set left direction backwards
		digitalWrite(MOTOR_LEFT1_PIN, LOW);
		digitalWrite(MOTOR_LEFT2_PIN, HIGH);
	}else{ //front
		digitalWrite(MOTOR_LEFT1_PIN, HIGH);
		digitalWrite(MOTOR_LEFT2_PIN, LOW);	
	}
		analogWrite(MOTOR_LEFT_EN_PIN, abs(leftMotorSpeed));//write left speed
	if (rightMotorSpeed<0){ //set right direction backwards
		digitalWrite(MOTOR_RIGHT1_PIN, LOW);
		digitalWrite(MOTOR_RIGHT2_PIN, HIGH);
	}else{ //front
		digitalWrite(MOTOR_RIGHT1_PIN, HIGH);
		digitalWrite(MOTOR_RIGHT2_PIN, LOW);		
	}
		analogWrite(MOTOR_RIGHT_EN_PIN, abs(rightMotorSpeed));//write right speed
}
