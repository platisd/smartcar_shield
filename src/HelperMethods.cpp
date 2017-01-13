#include "Smartcar.h"

ServoMotor* useServo(unsigned short servoPin){
	return new ServoMotor(servoPin);
}

ESCMotor* useESC(unsigned short escPin){
	return new ESCMotor(escPin);
}

ShieldMotors* useShieldMotors(unsigned short shieldOrientation){
	return new ShieldMotors(shieldOrientation);
}

DCSteerThrottle* useDCMotor(short shieldSide){
	if (shieldSide == LEFT){
		return useDCMotor(8,10,9);
	}else{
		return useDCMotor(12,13,11);
	}
}

DCSteerThrottle* useDCMotor(unsigned short directionPinA, unsigned short directionPinB, unsigned short enablePin){
	DCSteerThrottle* motor = new DCSteerThrottle();
	motor->attach(directionPinA, directionPinB, enablePin);
	return motor;
}

boolean almostEqual(float i, float j){ //take the absolute difference of the two floats and if it is smaller than a small value they are almost equal
	float diff = i-j;
	return abs(diff) < 0.001; 
}
