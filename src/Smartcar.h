/*
*	Smartcar.h - A simple library for controlling the smartcar
*	sensors. 
*	Version: 1.0
*	Author: Dimitris Platis
* 	License: GNU GPL v3 http://www.gnu.org/licenses/gpl-3.0.html
*/

#ifndef Smartcar_h
#define Smartcar_h
#if defined(ARDUINO) && ARDUINO >= 100
	#include <Arduino.h>
#else
	#include <WProgram.h>
	#include <pins_arduino.h>
#endif
#include <avr/io.h>
#include <avr/interrupt.h>
#include <Wire.h>
#include <Servo.h>

const unsigned short STANDARD = 1; //shield's orientation, used for DCMotors
const unsigned short INVERTED = 0; //shield's orientation, used for DCMotors

class DistanceSensor{
	public:
		DistanceSensor();
		virtual ~DistanceSensor();
		virtual unsigned int getDistance() = 0; //to be implemented by the child classes
		unsigned int getMedianDistance(short iterations = DEFAULT_ITERATIONS);
	private:
		static const short DEFAULT_ITERATIONS;
	protected:
		unsigned short _sensorMedianDelay; //delay between measurements in the sensor's getMedianDistance
};

class InfraredSensor : public DistanceSensor {//placeholder for a possible future ultrasonic abstract class
	public:
		InfraredSensor();
};

class SHARP_IR : public InfraredSensor {
	public:
		SHARP_IR();
		void attach(unsigned short pin);
		unsigned int getDistance();
	private:
		virtual unsigned int voltsToCentimeters(unsigned int volts) = 0; //implemented by each child class (different sensor)
		unsigned int readAnalogInput();
		unsigned short _pin;
	protected:
		unsigned int _maxDistance, _minDistance;
};

class GP2D120 : public SHARP_IR { //Infrared sensor for distances between 5 and 25 cm (aka GP2Y0A41SK0F)
	public:
		GP2D120();
	private:
		unsigned int voltsToCentimeters(unsigned int volts);
};

class GP2Y0A02 : public SHARP_IR { //Infrared sensor for distances between 25 and 120 cm
	public:
		GP2Y0A02();
	private:
		unsigned int voltsToCentimeters(unsigned int volts);
};

class GP2Y0A21 : public SHARP_IR { //Infrared sensor for distances between 12 and 78 cm
	public:
		GP2Y0A21();
	private:
		unsigned int voltsToCentimeters(unsigned int volts);
};

class UltrasonicSensor : public DistanceSensor {//placeholder for a possible future ultrasonic abstract class
	public:
		UltrasonicSensor();
};

class SR04 : public UltrasonicSensor {
	public:
		SR04(unsigned int maxDistance = DEFAULT_MAX_US_DISTANCE);
		void attach(unsigned short triggerPin, unsigned short echoPin);
		unsigned int getDistance();
	private:
		unsigned int ping();
		boolean ping_trigger();
		uint8_t _triggerBit;
		uint8_t _echoBit;
		volatile uint8_t *_triggerOutput;
		volatile uint8_t *_triggerMode;
		volatile uint8_t *_echoInput;
		unsigned int _maxEchoTime;
		unsigned long _max_time;
		unsigned int _maxDistance;
		static const unsigned int DEFAULT_MAX_US_DISTANCE;
};

class Gyroscope {
	public:
		Gyroscope(int offset = DEFAULT_GYRO_OFFSET);
		void attach();
		void begin(unsigned short samplingPeriod = DEFAULT_GYRO_SAMPLING);
		int getAngularDisplacement();
		void update();
		int calibrate(int measurements = 100);
		boolean isInstanciated();
	private:
		void initializeGyro();
		int setupL3G4200D(int scale);
		void writeRegister(int deviceAddress, byte address, byte val);
		int getGyroValues();
		int readRegister(int deviceAddress, byte address);
		unsigned short _samplingRate;
		static const unsigned short DEFAULT_GYRO_SAMPLING;
		static const int DEFAULT_GYRO_OFFSET;
		int _angularDisplacement;
		unsigned long _prevSample;
		int _gyroOffset;
};

class Odometer{
	public:
		Odometer(unsigned int pulsesPerMeter = DEFAULT_PULSES_PER_METER);
		int attach(unsigned short odometerPin);
		void begin();
		unsigned long getDistance();
		float getSpeed();
		boolean isInstanciated();
	private:
		unsigned long pulsesToCentimeters(unsigned long pulses);
		unsigned int _pulsesPerMeter;
		static const unsigned int DEFAULT_PULSES_PER_METER;
		unsigned short _odometerInterruptPin, _odometerID;
};

class SRF08 : public UltrasonicSensor{
	public:
		SRF08();
		void attach(unsigned short address = DEFAULT_SRF08_ADDRESS);
		void setGain(unsigned short gainValue);
		void setRange(unsigned short rangeValue);
		void setPingDelay(unsigned short milliseconds = DEFAULT_PING_DELAY);
		unsigned int getDistance();
		unsigned short getLightReading();
		void changeAddress(unsigned short newAddress);
	private:
		unsigned int ping();
		uint8_t _address;
		unsigned short _delay;
		static const unsigned short DEFAULT_PING_DELAY, DEFAULT_SRF08_ADDRESS;

};

class Motor {
	public:
		Motor();
		virtual ~Motor();
		virtual void init();
};

class SteeringMotor : public Motor {
	public:
		SteeringMotor();
		virtual ~SteeringMotor();
		virtual void setAngle(int degrees); //to be overriden by the child classes
		int getMaxRightAngle();
		int getMaxLeftAngle();
	private:
		virtual void setDegrees(); //to be overriden by the child classes
	protected:
		unsigned int filterAngle(int degrees);
		void setAllowedAngles();
		unsigned int _angle;
		int STRAIGHT_RAW_DEGREES, MAX_RIGHT_RAW_DEGREES, MAX_LEFT_RAW_DEGREES; //the values (in degrees) that can get written to the steering motor, with 0 being the leftmost position, 180 the rightmost and 90 around the middle
		int MAX_RIGHT_ANGLE, MAX_LEFT_ANGLE;
};

class ThrottleMotor : public Motor {
	public:
		ThrottleMotor();
		virtual ~ThrottleMotor();
		virtual void setSpeed(int speed); //to be overriden by the child classes
		virtual void setMotorSpeed(int leftMotorSpeed, int rightMotorSpeed); //to be overriden by the child classes
	private:
		virtual void setFreqsAndSpeeds(); //to be overriden by the child classes
	protected:
		int filterSpeed(int speed);
		void setAllowedSpeedLimits();
		int _speed;
		unsigned short FULL_FORWARD, FULL_BACKWARD; //what percentage of the motor's power is allowed be used at most
		int IDLE_RAW_SPEED, MAX_FRONT_RAW_SPEED, MAX_BACK_RAW_SPEED; //the raw value (in whatever metric, usually pwm signal) that the motors are idle, throttling full speed backwards and forward
		int MAX_FRONT_ALLOWED_SPEED, MAX_BACK_ALLOWED_SPEED; //the raw allowed min and max values
		
};

class ESCMotor : public ThrottleMotor, public Servo {
	public:
		ESCMotor(unsigned short pin);
		void setSpeed(int speed);
		void init();
	private:
		void setFreqsAndSpeeds();
		unsigned short _pin; //the pin the ESC is attached to
};

class DCMotors : public ThrottleMotor, public SteeringMotor {
	public:
		DCMotors(unsigned short shieldOrientation = STANDARD);
		void setSpeed(int speed);
		void setAngle(int degrees);
		void setMotorSpeed(int leftMotorSpeed, int rightMotorSpeed);
		void init();
	private:
		void setDegrees();
		void setFreqsAndSpeeds();
		void setDirection(unsigned short direction);
		void setMotors();
		unsigned short MOTOR_LEFT1_PIN, MOTOR_LEFT_EN_PIN, MOTOR_LEFT2_PIN;
		unsigned short MOTOR_RIGHT_EN_PIN, MOTOR_RIGHT1_PIN, MOTOR_RIGHT2_PIN;
};

class ServoMotor : public SteeringMotor, public Servo {
	public:
		ServoMotor(unsigned short pin);
		void setAngle(int degrees);
		void init();
	private:
		void setDegrees();
		unsigned short _pin; //the pin the Servo motor is attached to
};

class Car {
	public:
		Car(unsigned short shieldOrientation = STANDARD);
		Car(SteeringMotor *steering, unsigned short shieldOrientation = STANDARD);
		Car(SteeringMotor *steering, ThrottleMotor *throttle);
		void begin(Gyroscope gyro);
		void begin(Odometer encoder, Gyroscope gyro);
		void begin(Odometer encoder1 = 0, Odometer encoder2 = 0, Gyroscope gyro = 0);
		void setSpeed(float speed);
		void setAngle(int degrees);
		float getSpeed();
		int getAngle();
		void stop();
		void enableCruiseControl(float Kp = DEFAULT_KP, float Ki = DEFAULT_KI, float Kd = DEFAULT_KD, unsigned short frequency = DEFAULT_PID_LOOP_INTERVAL);
		void disableCruiseControl();
		void updateMotors();
		void go(int centimeters);
		void rotate(int degrees);
		void setMotorSpeed(int leftMotorSpeed, int rightMotorSpeed);
		boolean cruiseControlEnabled();
	private:
		void init(SteeringMotor *steering, ThrottleMotor *throttle);
		int motorPIDcontrol(const int previousSpeed, const float targetSpeed, const float actualSpeed);
		float getGroundSpeed();
		unsigned long getEncoderDistance();
		void initializeEncoders();
		float getEncoderSpeed();
		ThrottleMotor *_throttle;
		SteeringMotor *_steering;
		unsigned short _pidLoopInterval, _numOfEncoders;
		int _angle;
		float _speed, _lastGroundSpeed;
		static const unsigned short DEFAULT_PID_LOOP_INTERVAL;
		static const float DEFAULT_KP, DEFAULT_KI, DEFAULT_KD;
		float _Kp, _Ki, _Kd;
		float _measuredSpeed;
		Odometer _encoders[2];
		Gyroscope _gyro;
		boolean _gyroAttached;
		boolean _cruiseControl;
		unsigned long _lastMotorUpdate, _previousDistance;
		int _previousControlledSpeed;
		int _previousError, _integratedError;
		int MAX_ALLOWED_RIGHT_STEER, MAX_ALLOWED_LEFT_STEER; //how much we allow the car to steer, in degrees (negative to the left)
};

/* Helper classes */
ServoMotor* useServo(unsigned short servoPin); //used in the Car constructor to indicate the use of a servo motor for steering
ESCMotor* useESC(unsigned short escPin); //used in the Car constructor to indicate the use of an ESC for throttling
DCMotors* useDC(unsigned short shieldOrientation); //used in the Car constructor to indicate the use of DC Motors (default setup)
boolean almostEqual(float i, float j); //used to compare two floats

/* Class aliases, for back compatibility with AndroidCar, CaroloCup2016 and Smartcar sensors libraries */
typedef SR04 Sonar; //HC-SR04 was Sonar in AndroidCar, CaroloCup2016 and Smartcar sensor libraries
typedef GP2D120 Sharp_IR; //GP2D120 was used as Sharp_IR in AndroidCar and CaroloCup2016. Change to GP2Y0A02 for compatibility with Smartcar Sensors

#endif
