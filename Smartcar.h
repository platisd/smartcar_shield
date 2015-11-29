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

class DistanceSensor {
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

class GP2D120 : public SHARP_IR { //Infrared sensor for distances between 4 and 25 cm (aka GP2Y0A41SK0F)
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
		void begin(unsigned short samplingRate = DEFAULT_GYRO_SAMPLING);
		int getAngularDisplacement();
		void update();
		unsigned int calibrate(unsigned int measurements = 100);
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

class Odometer {
	public:
		Odometer(unsigned int pulsesPerMeter = DEFAULT_PULSES_PER_METER);
		int attach(unsigned short odometerPin);
		void begin();
		unsigned long getDistance();
		void detach();
		unsigned long getPulses();
	private:
		unsigned long pulsesToCentimeters(unsigned long pulses);
		unsigned int _pulsesPerMeter;
		static const unsigned int DEFAULT_PULSES_PER_METER;
		unsigned short _odometerInterruptPin, _odometerID;
};

class Car {
	public:
		Car(unsigned short setup = STANDARD);
		void begin();
		void begin(Odometer encoder);
		void begin(Gyroscope gyro);
		void begin(Odometer encoder, Gyroscope gyro);
		void begin(Odometer encoder1, Odometer encoder2);
		void begin(Odometer encoder1, Odometer encoder2, Gyroscope gyro);
		void setSpeed(float speed);
		void setAngle(int degrees);
		float getSpeed();
		int getAngle();
		void stop();
		void enableCruiseControl(float Kp = DEFAULT_KP, float Ki = DEFAULT_KI, float Kd = DEFAULT_KD, unsigned short = DEFAULT_PID_LOOP_INTERVAL);
		void disableCruiseControl();
		void updateMotors();
		void go(int centimeters);
		void rotate(int degrees);
		void setMotorSpeed(int leftMotorSpeed, int rightMotorSpeed);
		static const unsigned short STANDARD, INVERTED;
	private:
		int motorPIDcontrol(const int previousSpeed, const float targetSpeed, const float actualSpeed);
		void setMotors(int rawSpeed, int angle);
		void setDirection(const unsigned short direction);
		boolean cruiseControlEnabled();
		float getGroundSpeed();
		unsigned long getEncoderDistance();
		void initializeEncoders();
		unsigned short _pidLoopInterval, _numOfEncoders;
		int _angle;
		float _speed;
		static const unsigned short DEFAULT_PID_LOOP_INTERVAL;
		static const float DEFAULT_KP, DEFAULT_KI, DEFAULT_KD;
		float _Kp, _Ki, _Kd;
		Odometer _encoders[2];
		Gyroscope _gyro;
		boolean _cruiseControl;
		unsigned long _lastMotorUpdate, _previousDistance;
		int _previousControlledSpeed;
		int _previousError, _integratedError;
		uint8_t MOTOR_LEFT1_PIN, MOTOR_LEFT_EN_PIN, MOTOR_LEFT2_PIN;
		uint8_t MOTOR_RIGHT_EN_PIN, MOTOR_RIGHT1_PIN, MOTOR_RIGHT2_PIN;
};

class NewPing{
	public:
		static void timer_start(unsigned long frequency, void (*userFunc)(void));
		static void timer_stop();
	private:
		static void timer_setup();
		static void timer_start_cntdwn();
};

class SRF08 : public UltrasonicSensor{
	public:
		SRF08();
		void attach(const uint8_t address = DEFAULT_SRF08_ADDRESS);
		void setGain(const uint8_t gainValue);
		void setRange(const uint8_t rangeValue);
		void setPingDelay(const uint8_t milliseconds = DEFAULT_PING_DELAY);
		unsigned int getDistance();
		void changeAddress(uint8_t newAddress);
	private:
		unsigned int ping();
		uint8_t _address, _delay;
		static const uint8_t DEFAULT_PING_DELAY, DEFAULT_SRF08_ADDRESS;

};

/* Class aliases, for back compatibility with AndroidCar, CaroloCup2016 and Smartcar sensors libraries */
typedef SR04 Sonar; //HC-SR04 was Sonar in AndroidCar, CaroloCup2016 and Smartcar sensor libraries
typedef GP2D120 Sharp_IR; //GP2D120 was used as Sharp_IR in AndroidCar and CaroloCup2016. Change to GP2Y0A02 for compatibility with Smartcar Sensors

#endif
