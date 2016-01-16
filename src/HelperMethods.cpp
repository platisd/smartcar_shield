#include "Smartcar.h"

ServoMotor* useServo(unsigned short servoPin){
	return new ServoMotor(servoPin);
}

ESCMotor* useESC(unsigned short escPin){
	return new ESCMotor(escPin);
}

DCMotors* useDC(unsigned short shieldOrientation){
	return new DCMotors(shieldOrientation);
}
