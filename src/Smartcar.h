/*
* The entrypoint for the Smartcar platform. Everything needed to control
* the platform (i.e. the vehicle as well as its sensors) are included through
* this header file.
*/
#pragma once

#include "include/sensors/distance/ultrasound/i2c/SRF08.hpp"
#include "include/sensors/distance/ultrasound/ping/SR04.hpp"
#include "include/sensors/distance/infrared/analog/sharp/GP2D120.hpp"
#include "include/sensors/distance/infrared/analog/sharp/GP2Y0A02.hpp"
#include "include/sensors/distance/infrared/analog/sharp/GP2Y0A21.hpp"
#include "include/sensors/odometry/Odometer.hpp"
#include "include/sensors/odometry/DirectionalOdometer.hpp"
#include "include/sensors/heading/gyroscope/GY50.hpp"
#include "include/motors/analog/pwm/BrushedMotor.hpp"
#include "include/motors/digital/servo/ServoMotor.hpp"

using ESCMotor = ServoMotor; // An ESC is programmatically the same as a Servo motor

const unsigned short STANDARD = 1; //shield's orientation, used for ShieldMotors
const unsigned short INVERTED = 0; //shield's orientation, used for ShieldMotors
const unsigned short BACKWARD = 0; //the motor's direction of rotation
const unsigned short FORWARD = 1;
const unsigned short IDLE = 2;
const short LEFT = -1;
const short RIGHT = 1;
const short STRAIGHT = 0; //refers to the wheel direction/orientation

// class UltrasonicSensor : public DistanceSensor {//placeholder for a possible future ultrasonic abstract class
//     public:
//         UltrasonicSensor();
// };
//
// class HeadingSensor {
//     public:
//         HeadingSensor();
//         virtual ~HeadingSensor();
//         virtual void update();
//         unsigned int getAngularDisplacement();
//     protected:
//         float _angularDisplacement;
// };
//
// class Gyroscope : public HeadingSensor{
//     public:
//         explicit Gyroscope(int offset = DEFAULT_GYRO_OFFSET);
//         void attach();
//         void begin(unsigned short samplingPeriod = DEFAULT_GYRO_SAMPLING);
//         void update();
//         int calibrate(int measurements = 100);
//     private:
//         void initializeGyro();
//         void setupL3G4200D(int scale);
//         void writeRegister(int deviceAddress, byte address, byte val);
//         int getGyroValues();
//         int readRegister(int deviceAddress, byte address);
//         unsigned short _samplingRate;
//         static const unsigned short DEFAULT_GYRO_SAMPLING;
//         static const int DEFAULT_GYRO_OFFSET;
//         unsigned long _prevSample;
//         int _gyroOffset;
// };
//
// class Odometer{
//     public:
//         explicit Odometer(unsigned long pulsesPerMeter = DEFAULT_PULSES_PER_METER);
//         int attach(unsigned short odometerPin);
//         int attach(unsigned short odometerPin, unsigned short directionPin, boolean forwardDirState); // direction
//         void begin();
//         unsigned long getDistance();
//         long getRelativeDistance(); // direction
//         short getDirection(); // direction
//         float getSpeed();
//     private:
//         unsigned long pulsesToCentimeters(unsigned long pulses);
//         int init(unsigned short odometerPin);
//         unsigned long _pulsesPerMeter;
//         static const unsigned int DEFAULT_PULSES_PER_METER;
//         static const unsigned short DEFAULT_DIRECTION_PIN;
//         unsigned short _odometerInterruptPin, _odometerID;
//         unsigned int _millimetersPerPulse;
// };
//
// class Motor {
//     public:
//         Motor();
//         virtual ~Motor();
//         virtual void init();
// };
//
// class SteeringMotor : public Motor {
//     public:
//         SteeringMotor();
//         virtual ~SteeringMotor();
//         virtual void setAngle(int degrees); //to be overriden by the child classes
//         int getMaxRightAngle();
//         int getMaxLeftAngle();
//     private:
//         virtual void setDegrees(); //to be overriden by the child classes
//     protected:
//         unsigned int filterAngle(int degrees);
//         void setAllowedAngles();
//         unsigned int _angle;
//         unsigned int STRAIGHT_RAW_DEGREES, MAX_RIGHT_RAW_DEGREES, MAX_LEFT_RAW_DEGREES; //the values (in degrees) that can get written to the steering motor, with 0 being the leftmost position, 180 the rightmost and 90 around the middle
//         int MAX_RIGHT_ANGLE, MAX_LEFT_ANGLE;
// };
//
// class ThrottleMotor : public Motor {
//     public:
//         ThrottleMotor();
//         virtual ~ThrottleMotor();
//         virtual void setSpeed(int speed); //to be overriden by the child classes
//         virtual void setMotorSpeed(int leftMotorSpeed, int rightMotorSpeed); //to be overriden by the child classes
//     private:
//         virtual void setFreqsAndSpeeds(); //to be overriden by the child classes
//     protected:
//         int filterSpeed(int speed);
//         void setAllowedSpeedLimits();
//         int _speed;
//         unsigned short FULL_FORWARD, FULL_BACKWARD; //what percentage of the motor's power is allowed be used at most
//         int IDLE_RAW_SPEED, MAX_FRONT_RAW_SPEED, MAX_BACK_RAW_SPEED; //the raw value (in whatever metric, usually pwm signal) that the motors are idle, throttling full speed backwards and forward
//         int MAX_FRONT_ALLOWED_SPEED, MAX_BACK_ALLOWED_SPEED; //the raw allowed min and max values
//
// };
//
// class ESCMotor : public ThrottleMotor, public Servo {
//     public:
//         explicit ESCMotor(unsigned short pin);
//         void setSpeed(int speed);
//         void init();
//     private:
//         void setFreqsAndSpeeds();
//         unsigned short _pin; //the pin the ESC is attached to
// };
//
// class BrushedDCMotor {
//     public:
//         BrushedDCMotor();
//         void attach(unsigned short forwardPin, unsigned short backwardPin, unsigned short enablePin);
//         void setDirection(unsigned short direction);
//         void setSpeed(unsigned short pwmSpeed);
//     private:
//         unsigned short _forwardPin, _backwardPin, _enablePin;
// };
//
// class DCSteerThrottle : public ThrottleMotor, public SteeringMotor {
//     public:
//         DCSteerThrottle();
//         void attach(unsigned short directionPinA, unsigned short directionPinB, unsigned short enablePin);
//         void setSpeed(int speed);
//         void setAngle(int degrees);
//     private:
//         BrushedDCMotor _motor;
//         void setDegrees();
//         void setFreqsAndSpeeds();
//
// };
//
// class ShieldMotors : public ThrottleMotor, public SteeringMotor {
//     public:
//         explicit ShieldMotors(unsigned short shieldOrientation = STANDARD);
//         ShieldMotors(unsigned short leftMotor[], unsigned short rightMotor[]);
//         void setSpeed(int speed);
//         void setAngle(int degrees);
//         void setMotorSpeed(int leftMotorSpeed, int rightMotorSpeed);
//     private:
//         BrushedDCMotor _leftMotor, _rightMotor;
//         void setDegrees();
//         void setFreqsAndSpeeds();
//         void setDirection(unsigned short direction);
//         void setMotors();
//         void init();
// };
//
// class ServoMotor : public SteeringMotor, public Servo {
//     public:
//         explicit ServoMotor(unsigned short pin);
//         void setAngle(int degrees);
//         void init();
//     private:
//         void setDegrees();
//         unsigned short _pin; //the pin the Servo motor is attached to
// };
//
// class Car {
//     public:
//         explicit Car(unsigned short shieldOrientation = STANDARD);
//         explicit Car(ShieldMotors *shieldMotors);
//         Car(SteeringMotor *steering, ThrottleMotor *throttle);
//         void begin();
//         void begin(Odometer &encoder);
//         void begin(HeadingSensor &heading);
//         void begin(Odometer &encoder, HeadingSensor &heading);
//         void begin(Odometer &encoder1, Odometer &encoder2);
//         void begin(Odometer &encoder1, Odometer &encoder2, HeadingSensor &heading);
//         void setSpeed(float speed);
//         void setAngle(int degrees);
//         float getSpeed();
//         int getAngle();
//         void stop();
//         void enableCruiseControl(float Kp = DEFAULT_KP, float Ki = DEFAULT_KI, float Kd = DEFAULT_KD, unsigned short frequency = DEFAULT_PID_LOOP_INTERVAL);
//         void disableCruiseControl();
//         void updateMotors();
//         void go(int centimeters);
//         void rotate(int degrees);
//         void setMotorSpeed(int leftMotorSpeed, int rightMotorSpeed);
//         boolean cruiseControlEnabled();
//     private:
//         void setup();
//         void setupOdometer(Odometer &encoder);
//         void setupHeadingSensor(HeadingSensor &heading);
//         void init(SteeringMotor *steering, ThrottleMotor *throttle);
//         float motorPIDcontrol(const float previousSpeed, const float targetSpeed, const float actualSpeed);
//         float getGroundSpeed();
//         unsigned long getEncoderDistance();
//         short getEncoderDirection();
//         void initializeEncoders();
//         float getEncoderSpeed();
//         ThrottleMotor *_throttle;
//         SteeringMotor *_steering;
//         unsigned short _pidLoopInterval, _numOfEncoders;
//         int _angle;
//         float _speed, _lastGroundSpeed;
//         static const unsigned short DEFAULT_PID_LOOP_INTERVAL;
//         static const float DEFAULT_KP, DEFAULT_KI, DEFAULT_KD;
//         float _Kp, _Ki, _Kd;
//         float _measuredSpeed;
//         Odometer *_encoders[2];
//         HeadingSensor *_heading;
//         boolean _headingAttached;
//         boolean _cruiseControl;
//         unsigned long _lastMotorUpdate, _previousDistance;
//         float _previousControlledSpeed;
//         int _previousError, _integratedError;
// };
//
// /* Helper classes */
// DCSteerThrottle* useDCMotor(short shieldSide); //user-friendly attach of a brushed dc motor to one of the sides of the shield (LEFT/RIGHT)
// DCSteerThrottle* useDCMotor(unsigned short directionPinA, unsigned short directionPinB, unsigned short enablePin); //attach a brushed motor to the specified pins, that will be used either for steering and or throttling
// ServoMotor* useServo(unsigned short servoPin); //used in the Car constructor to indicate the use of a servo motor for steering
// ESCMotor* useESC(unsigned short escPin); //used in the Car constructor to indicate the use of an ESC for throttling
// ShieldMotors* useShieldMotors(unsigned short shieldOrientation = STANDARD); //used in the Car constructor to indicate the use of the motors on the shield, according to default setup
// ShieldMotors* useShieldMotors(unsigned short leftMotor[], unsigned short rightMotor[]);
// boolean almostEqual(float i, float j); //used to compare two floats
//
// /* Class aliases, for back compatibility with AndroidCar, CaroloCup2016 and Smartcar sensors libraries */
// typedef GP2D120 Sharp_IR; //GP2D120 was used as Sharp_IR in AndroidCar and CaroloCup2016. Change to GP2Y0A02 for compatibility with Smartcar Sensors
