/*
*	Car.cpp
*/
#include "Smartcar.h"
/* --- CAR --- */

const unsigned short Car::DEFAULT_PID_LOOP_INTERVAL = 80;
const unsigned short Car::STANDARD = 1;
const unsigned short Car::INVERTED = 0; 
const float Car::DEFAULT_KP = 5.0;
const float Car::DEFAULT_KI = 0.0;
const float Car::DEFAULT_KD = 10.0;

const unsigned short BACKWARD = 0;
const unsigned short FORWARD = 1;
const unsigned short IDLE = 2;

const int IDLE_RAW_SPEED = 0;
const int MAX_FRONT_RAW_SPEED = 255;
const int MAX_BACK_RAW_SPEED = -255;
const float MAX_BACK_CRUISE_SPEED = -3.0;
const float MAX_FRONT_CRUISE_SPEED = 3.0;
const int STRAIGHT_WHEELS = 90;
const int STRAIGHT_ANGLE = 0;
const int MAX_RIGHT_DEGREES = 180;
const int MAX_LEFT_DEGREES = 0;
const float GO_CRUISE_SPEED = 1.3;
const int GO_RAW_SPEED = 200;
const int MAX_RIGHT_STEERING_ANGLE = MAX_RIGHT_DEGREES - STRAIGHT_WHEELS;
const int MAX_LEFT_STEERING_ANGLE = MAX_LEFT_DEGREES - STRAIGHT_WHEELS;

Car::Car(const unsigned short setup){
	_cruiseControl = false;
	_speed = IDLE_RAW_SPEED;
	_angle = STRAIGHT_WHEELS;
	_numOfEncoders = 0;
	_gyroAttached = false;
	if (setup == STANDARD){ //the default motor setup, where right is right
		MOTOR_LEFT1_PIN = 8;
		MOTOR_LEFT_EN_PIN = 9;
		MOTOR_LEFT2_PIN = 10;
		MOTOR_RIGHT_EN_PIN = 11;
		MOTOR_RIGHT1_PIN = 12;
		MOTOR_RIGHT2_PIN = 13;
	}else{ //the reversed motor setup, where right is left
		MOTOR_RIGHT1_PIN = 8;
		MOTOR_RIGHT_EN_PIN = 9;
		MOTOR_RIGHT2_PIN = 10;
		MOTOR_LEFT_EN_PIN = 11;
		MOTOR_LEFT1_PIN = 12;
		MOTOR_LEFT2_PIN = 13;
	}
}

void Car::begin(Gyroscope gyro){
	begin(0,0,gyro); //pass 0 as a value that indicates null
}

void Car::begin(Odometer encoder, Gyroscope gyro){
	begin(encoder,0,gyro); //pass 0 as a value that indicates null
}

void Car::begin(Odometer encoder1, Odometer encoder2, Gyroscope gyro){
	if (encoder1.isInstanciated()){
		_encoders[0] = encoder1;
		_numOfEncoders++;
	}
	if (encoder2.isInstanciated()){
		_encoders[1] = encoder2;
		_numOfEncoders++;
	}
	if (gyro.isInstanciated()) {
		_gyro = gyro;
		_gyroAttached = true;
	}
	for (uint8_t i = MOTOR_LEFT1_PIN; i <= MOTOR_RIGHT2_PIN; i++) {
		pinMode(i, OUTPUT); //declare pins as outputs
	}
	setSpeed(IDLE_RAW_SPEED);
	setAngle(STRAIGHT_ANGLE);
}

void Car::setSpeed(float newSpeed){
	if (cruiseControlEnabled()){
		if (_speed && (_speed != IDLE_RAW_SPEED) && (newSpeed * _speed) <= 0) stop(); //if the speeds are signed differently, stop the car and then set the new speed. Ignore this if the speed is already 0 and if speed is at the idle raw speed i.e. leftovers from non-cruise control mode (if IDLE_RAW_SPEED is not 0, it makes sense)
		_speed = constrain(newSpeed, MAX_BACK_CRUISE_SPEED, MAX_FRONT_CRUISE_SPEED);
	}else{
		_speed = constrain(int(newSpeed), MAX_BACK_RAW_SPEED, MAX_FRONT_RAW_SPEED);
		setMotors(_speed, getAngle());
	}
}

void Car::updateMotors(){
	if (cruiseControlEnabled() && (millis() > _lastMotorUpdate + _pidLoopInterval)){
		if (_speed){ //if _speed is 0, we have already made sure the car is stopped. don't try to adjust if car is just drifting
			float measuredSpeed = getGroundSpeed(); //speed in cm/milliseconds
			measuredSpeed *= 10; //transform it into m/seconds, divide by 100 to turn cm into m and multiply by 1000, to turn ms to sec
			if (_speed < 0) measuredSpeed *= -1; //if we are going reverse, illustrate that in the value of measuredSpeed
			int controlledSpeed = motorPIDcontrol(_previousControlledSpeed, _speed, measuredSpeed);
			setMotors(controlledSpeed, getAngle());
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
	return constrain(previousSpeed + int(correction), MAX_BACK_RAW_SPEED, MAX_FRONT_RAW_SPEED);
}

float Car::getGroundSpeed(){
	unsigned long currentDistance = getEncoderDistance();
	unsigned long dX = currentDistance - _previousDistance;
	_previousDistance = currentDistance;
	unsigned long dT = millis() - _lastMotorUpdate;
	float velocity = float(dX)/ float(dT);
	return velocity;	
}

void Car::setMotors(int rawSpeed, int angle){ //platform specific method
	if (rawSpeed < 0){
		setDirection(BACKWARD);	
	}else if (rawSpeed > 0){
		setDirection(FORWARD);
	}else{
		setDirection(IDLE);
	}
	rawSpeed = abs(rawSpeed);
	float ratio = (STRAIGHT_WHEELS - abs(angle));
	ratio /= STRAIGHT_WHEELS;
	if (angle>0){ //going to the right
		analogWrite(MOTOR_LEFT_EN_PIN, rawSpeed);
		analogWrite(MOTOR_RIGHT_EN_PIN, int(rawSpeed * ratio));
	}else{ //going to the left or straight
		analogWrite(MOTOR_RIGHT_EN_PIN, rawSpeed);
		analogWrite(MOTOR_LEFT_EN_PIN, int(rawSpeed * ratio));
	}
	
	 
}

void Car::setDirection(const unsigned short direction){ //platform specific method
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

void Car::setAngle(int angle){ //platform specific method
	_angle = constrain(angle, MAX_LEFT_DEGREES - STRAIGHT_WHEELS, MAX_RIGHT_DEGREES - STRAIGHT_WHEELS); //constrain the value to the permitted valued the user is allowed to supply
	setMotors(getSpeed(), getAngle()); //pass the human-readable value that was just supplied
}

void Car::stop(){ //platform specific method
	if (getSpeed()>0){
		setMotors(-100, getAngle());
	}else if(getSpeed()<0){
		setMotors(100, getAngle());
	}
	delay(80); //go the opposite direction for a few milliseconds, to make sure we are stopped
	if (cruiseControlEnabled()){
		setMotors(0,getAngle()); //shut the engines down, we should be stopped by now
		enableCruiseControl(_Kp,_Ki,_Kd); //re-initialize the cruise control, se we get rid of previous error and pid output
		_speed = 0.0;
	}else{
		setSpeed(0); //no circular dependency between stop and setSpeed due to the cruiseControlEnabled switching between who uses what
	}
}

float Car::getSpeed(){
	if (cruiseControlEnabled()) return getGroundSpeed(); //return speed in meters per second
	return _speed; //return speed in PWM output
}

int Car::getAngle(){
	return _angle; //return what the user supplied
}

void Car::enableCruiseControl(float Kp, float Ki, float Kd, unsigned short pidLoopInterval){
	if (!_numOfEncoders) return; //continue only if at least one encoder was attached
	_cruiseControl = true;
	_pidLoopInterval = pidLoopInterval;
	_lastMotorUpdate = 0;
	_previousControlledSpeed = IDLE_RAW_SPEED;
	_previousDistance = getEncoderDistance();
	_integratedError = 0;
	_Kp = Kp;
	_Ki = Ki;
	_Kd = Kd;
}

void Car::disableCruiseControl(){
	_cruiseControl = false;
	_speed = _previousControlledSpeed; //update the speed with the PWM equivalent
}

boolean Car::cruiseControlEnabled(){
	return _cruiseControl;
}

unsigned long Car::getEncoderDistance(){
	unsigned long averageDistance = 0;
	for (int i = 0; i < _numOfEncoders; i++){
		averageDistance += _encoders[i].getDistance() / _numOfEncoders;
	}
	return averageDistance;
}

void Car::initializeEncoders(){
	for (int i = 0; i < _numOfEncoders; i++){
		_encoders[i].begin();
	}
}

void Car::go(int centimeters){
	if (!_numOfEncoders) return; //continue only if at least one encoder was attached
	unsigned long initialDistance = getEncoderDistance(); //save the current distance the car has covered
	float initialSpeed = getSpeed(); //save the current speed (so we restore it later)
	int initialAngle = getAngle(); //save the current angle (so we restore it later)
	unsigned long targetDistance = initialDistance + abs(centimeters); //how much the car should move, always a positive number
	if (initialDistance > targetDistance){ //this will occur only if the unsigned long variable overflows
		initializeEncoders(); //reinitialize the encoders counter to 0
		targetDistance = abs(centimeters); //now the new target is just the centimeters (since the encoder distance is 0)
	}
	setAngle(STRAIGHT_ANGLE); //set the angle straight
	short direction = 1;
	if (centimeters < 0) direction = -1; //if the user supplied a negative argument, means they want to move backwards
	if (cruiseControlEnabled()){ //depending on whether the cruise control is enabled set the speed
		setSpeed(direction * GO_CRUISE_SPEED);
	}else{
		setSpeed(direction * GO_RAW_SPEED);
	}
	while (getEncoderDistance() < targetDistance){ //while we haven't reached the target distance, keep moving
		if (cruiseControlEnabled()) updateMotors(); //otherwise the pid for the motors won't work
	}
	setSpeed(initialSpeed); //restore to the initial speed
	setAngle(initialAngle); //restore to the inital angle	
}
void Car::rotate(int targetDegrees){
	if (!_gyroAttached) return; //continue only if gyroscope is attached
	_gyro.begin(); //initialize the gyro
	float initialSpeed = getSpeed(); //save the current speed (so we restore it later)
	int initialAngle = getAngle(); //save the current angle (so we restore it later)
	if (targetDegrees > 0){ //we should turn to the right (clockwise)
		setAngle(MAX_RIGHT_STEERING_ANGLE); //turn right as steep as you can
	}else{ //turn left (counterclockwise)
		setAngle(MAX_LEFT_STEERING_ANGLE); //turn left as steep as you can
	}
	if (cruiseControlEnabled()){ //depending on whether the cruise control is enabled set the speed
		setSpeed(GO_CRUISE_SPEED);
	}else{
		setSpeed(GO_RAW_SPEED);
	}
	while (abs(_gyro.getAngularDisplacement()) < abs(targetDegrees)){ //while the absolute displacement hasn't reached the (absolute) target, keep turning
		if (cruiseControlEnabled()) updateMotors(); //otherwise the pid for the motors won't work
		_gyro.update(); //update to integrate the latest gyroscope readings
	}
	setSpeed(initialSpeed); //restore to the initial speed
	setAngle(initialAngle); //restore to the inital angle
}

void Car::setMotorSpeed(int leftMotorSpeed, int rightMotorSpeed){ //will set the speed manual to each motor, use with caution
	if (cruiseControlEnabled()) return; //we don't want to be manually messing with the speed, during cruise control
	leftMotorSpeed = constrain(leftMotorSpeed, MAX_BACK_RAW_SPEED, MAX_FRONT_RAW_SPEED); //constrain the speed within the allowed limits
	rightMotorSpeed = constrain(rightMotorSpeed, MAX_BACK_RAW_SPEED, MAX_FRONT_RAW_SPEED);
	if (leftMotorSpeed<0){ //set left direction backwards
		digitalWrite(MOTOR_LEFT1_PIN, LOW);
		digitalWrite(MOTOR_LEFT2_PIN, HIGH);
	}else{ //front
		digitalWrite(MOTOR_LEFT1_PIN, HIGH);
		digitalWrite(MOTOR_LEFT2_PIN, LOW);	
	}
		analogWrite(MOTOR_LEFT_EN_PIN, abs(leftMotorSpeed));//write left speed
	if (rightMotorSpeed<0){ //set right direction  backwards
		digitalWrite(MOTOR_RIGHT1_PIN, LOW);
		digitalWrite(MOTOR_RIGHT2_PIN, HIGH);
	}else{ //front
		digitalWrite(MOTOR_RIGHT1_PIN, HIGH);
		digitalWrite(MOTOR_RIGHT2_PIN, LOW);		
	}
		analogWrite(MOTOR_RIGHT_EN_PIN, abs(rightMotorSpeed));//write right speed
}
