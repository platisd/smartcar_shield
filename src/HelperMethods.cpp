#include "Smartcar.h"

ServoMotor* useServo(unsigned short servoPin){
	return new ServoMotor(servoPin);
}

ESCMotor* useESC(unsigned short escPin){
	return new ESCMotor(escPin);
}

DCMotor* useDC(){
	return new DCMotor();
}
