#include "Smartcar.h"

ServoMotor::ServoMotor(unsigned short pin) {
    _pin = pin;
    setDegrees();
    setAllowedAngles();
}

void ServoMotor::setDegrees(){
    STRAIGHT_RAW_DEGREES = 90;
    MAX_RIGHT_RAW_DEGREES = 140;
    MAX_LEFT_RAW_DEGREES = 40;
    _angle = STRAIGHT_RAW_DEGREES;
}

void ServoMotor::init(){
    attach(_pin); //attach the servo to its pin
}

void ServoMotor::setAngle(int degrees){// receives some degrees in the scale of MAX_LEFT_ANGLE, MAX_RIGHT_ANGLE    
    _angle = filterAngle(degrees); //_angle now holds a value between MAX_LEFT_RAW_DEGREES and MAX_RIGHT_RAW_DEGREES
    write(_angle); //writes the angle to the servo motor
}
