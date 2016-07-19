#include "Smartcar.h"

BrushedDCMotor::BrushedDCMotor(){ //represents programmatically the very simple, 2 wire, brushed dc motor
    _forwardPin = 0, _backwardPin = 0, _enablePin = 0; //give some default values to the pins
}

void BrushedDCMotor::attach(unsigned short forwardPin, unsigned short backwardPin, unsigned short enablePin) {
	_forwardPin = forwardPin; //the direction pin that when set to HIGH (and the other to LOW) makes the motor move "forward"
	_backwardPin = backwardPin; //the direction pin that when set to HIGH (and the other to LOW) makes the motor move "backward"
	_enablePin = enablePin; //the enable pin, that controls the motor's speed
	pinMode(_forwardPin, OUTPUT);
	pinMode(_backwardPin, OUTPUT);
	pinMode(_enablePin, OUTPUT);
}

void BrushedDCMotor::setDirection(unsigned short direction){ //receives the direction the particular motor should have and sets it accordingly
	switch (direction){
		case BACKWARD:
			digitalWrite(_forwardPin, LOW);
			digitalWrite(_backwardPin, HIGH);
			break;
		case FORWARD:
			digitalWrite(_forwardPin, HIGH);
			digitalWrite(_backwardPin, LOW);
			break;
		default: //IDLE
			digitalWrite(_forwardPin, LOW);
			digitalWrite(_backwardPin, LOW);
			break;
	}
}

void BrushedDCMotor::setSpeed(unsigned short pwmSpeed){ //receives the speed of the motor as a PWM raw value
	analogWrite(_enablePin, pwmSpeed);
}
