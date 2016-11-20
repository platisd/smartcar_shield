#include "Smartcar.h"

ThrottleMotor::ThrottleMotor() {
    _speed = 0, FULL_FORWARD = 0, FULL_BACKWARD = 0, IDLE_RAW_SPEED = 0;
    MAX_FRONT_RAW_SPEED = 0, MAX_BACK_RAW_SPEED = 0, MAX_FRONT_ALLOWED_SPEED = 0;
    MAX_BACK_ALLOWED_SPEED = 0;
}

ThrottleMotor::~ThrottleMotor(){}

void ThrottleMotor::setSpeed(int speed){ //to be overriden by child classes
//receives a speed in the scale of -100 to 100
//updates _speed which now holds a value between MAX_BACK_ALLOWED_SPEED and MAX_FRONT_ALLOWED_SPEED
//sets the speed on each motor
}

void ThrottleMotor::setFreqsAndSpeeds(){ //to be overriden by child classes
//sets the various motor-specific, maximum and minimum values
}

void ThrottleMotor::setMotorSpeed(int leftMotorSpeed, int rightMotorSpeed){ //to be overriden by the child classes
//sets manually the speed in the scale from -100 to 100 on each side of the car. Is only implemented/overriden by ShieldMotors
}

void ThrottleMotor::setAllowedSpeedLimits(){
//    calculate the allowed speed as percentages of the difference between the max raw speeds and the idle speed
    MAX_FRONT_ALLOWED_SPEED = IDLE_RAW_SPEED + ((MAX_FRONT_RAW_SPEED - IDLE_RAW_SPEED) * (FULL_FORWARD / 100.0));
    MAX_BACK_ALLOWED_SPEED = IDLE_RAW_SPEED + ((MAX_BACK_RAW_SPEED - IDLE_RAW_SPEED) * (FULL_BACKWARD / 100.0));
//    for example, if we want to use mostly 50% of backward speed (FULL_BACKWARD == 50), then we need the half of max in relation to idle raw speed
}

int ThrottleMotor::filterSpeed(int speed){
//    constrain the speed between the allowed percentages [-100,100]
    speed = constrain(speed, -100, 100);
//    map the speed. We map negative and positive values separately in case the respective maximum percentages aren't symmetric
    if (speed <= 0){
        speed = map(speed, -100, 0, MAX_BACK_ALLOWED_SPEED, IDLE_RAW_SPEED);
    }else{
        speed = map(speed, 0, 100, IDLE_RAW_SPEED, MAX_FRONT_ALLOWED_SPEED);
    }
    return speed; //speed could be signed (in order to indicate direction to non-esc motors)
}
