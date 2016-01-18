#include "Smartcar.h"

SteeringMotor::SteeringMotor() {}

SteeringMotor::~SteeringMotor(){}

void SteeringMotor::setAngle(int degrees){ //to be overriden by child classes
// receives some degrees in the scale of MAX_LEFT_ANGLE, MAX_RIGHT_ANGLE
// updates _angle which now holds a value between MAX_LEFT_RAW_DEGREES and MAX_RIGHT_RAW_DEGREES
// sets the angle on each motor
}
void SteeringMotor::setDegrees(){ //to be overriden by child classes
//sets the various motor-specific, maximum and minimum values
}

//set the allowed degrees, using the straight wheels as an offset (in other words, the way the user will input it)
void SteeringMotor::setAllowedAngles(){
	MAX_RIGHT_ANGLE = MAX_RIGHT_RAW_DEGREES - STRAIGHT_RAW_DEGREES;
	MAX_LEFT_ANGLE = MAX_LEFT_RAW_DEGREES - STRAIGHT_RAW_DEGREES;
}

unsigned int SteeringMotor::filterAngle(int degrees){
//	constrain the given value within the allowed limits
	degrees = constrain(degrees, MAX_LEFT_ANGLE, MAX_RIGHT_ANGLE);
//	calculate what the value written to the motor should be, by removing the straight degrees offset and return it
	return degrees + STRAIGHT_RAW_DEGREES;
}

//returns the maximum allowed angle, signed, using the straight wheels as offset (in other words, the way the user inputs it)
int SteeringMotor::getMaxRightAngle(){
	return MAX_RIGHT_ANGLE;
}

//returns the minimum allowed angle, signed, using the straight wheels as offset (in other words, the way the user inputs it)
int SteeringMotor::getMaxLeftAngle(){
	return MAX_LEFT_ANGLE;
}
