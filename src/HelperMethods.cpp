#include "Smartcar.h"

ServoMotor* useServo(unsigned short servoPin){
	return new ServoMotor(servoPin);
}

ESCMotor* useESC(unsigned short escPin){
	return new ESCMotor(escPin);
}

ShieldMotors* useDC(unsigned short shieldOrientation){
	return new ShieldMotors(shieldOrientation);
}

boolean almostEqual(float i, float j){ //take the absolute difference of the two floats and if it is smaller than a small value they are almost equal
	float diff = i-j;
	return abs(diff) < 0.001; 
}
