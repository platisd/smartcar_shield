/*
*	AndroidCar.h - A simple library for controlling the smartcar
*	sensors. 
*	Version: 0.3
*	Author: Dimitris Platis
*	Sonar class is essentially a stripped-down version of the NewPing library by Tim Eckel, adjusted to Smartcar needs
* 	Get original library at: http://code.google.com/p/arduino-new-ping/
* 	License: GNU GPL v3 http://www.gnu.org/licenses/gpl-3.0.html
*/

#ifndef AndroidCar_h
#define AndroidCar_h
#if defined(ARDUINO) && ARDUINO >= 100
	#include <Arduino.h>
#else
	#include <WProgram.h>
	#include <pins_arduino.h>
#endif
#include <avr/io.h>
#include <avr/interrupt.h>
#include <Servo.h>
#include <Wire.h>

class Car {
	public:
		Car(unsigned short steeringWheelPin = DEFAULT_SERVO_PIN, unsigned short escPin = DEFAULT_ESC_PIN);
		void begin();
		void setSpeed(int speed);
		void setAngle(int degrees);
		int getSpeed();
		int getAngle();
	private:
		void setSteeringWheelPin(unsigned short steeringWheelPin);
		void setESCPin(unsigned short escPin);
		unsigned short _steeringWheelPin, _escPin;
		Servo motor, steeringWheel;
		int _speed, _angle;
		static const unsigned short DEFAULT_SERVO_PIN, DEFAULT_ESC_PIN;
};

class Sonar {
	public:
		Sonar();
		void attach(unsigned short triggerPin, unsigned short echoPin);
		unsigned int getDistance();
		unsigned int getMedianDistance(short iterations = SONAR_DEFAULT_ITERATIONS);

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
		static const unsigned short SONAR_DEFAULT_ITERATIONS;
};

class Sharp_IR {
	public:
		Sharp_IR();
		void attach(unsigned short IR_pin);
		unsigned int getDistance();
		unsigned int getMedianDistance(short iterations = IR_DEFAULT_ITERATIONS);
	private:
		unsigned short _IR_pin;
		static const unsigned short IR_DEFAULT_ITERATIONS;
};


class Odometer {
	public:
		Odometer();
		int attach(unsigned short odometerPin);
		void begin();
		unsigned long getDistance();
		void detach();
	private:
		unsigned short _odometerInterruptPin;
};

class NewPing{
	public:
		static void timer_start(unsigned long frequency, void (*userFunc)(void));
		static void timer_stop();
	private:
		static void timer_setup();
		static void timer_start_cntdwn();
};

class Gyroscope {
	public:
		Gyroscope();
		void attach();
		void begin(unsigned short samplingRate = DEFAULT_GYRO_SAMPLING); //in milliseconds
		int getAngularDisplacement();
		void update();
	private:
		void initMeasurement();
		void initializeGyro();
		int setupL3G4200D(int scale);
		void writeRegister(int deviceAddress, byte address, byte val);
		int getGyroValues();
		int readRegister(int deviceAddress, byte address);
		unsigned short _samplingRate;
		static const unsigned short DEFAULT_GYRO_SAMPLING;
};

class Razorboard{
	public:
		Razorboard();
		void attach(HardwareSerial *razorSerial);
		boolean available();
		String readLine();
		String readLastLine();
		int getHeading();
		int getLatestHeading();
	private:
		String readRawSerialLine();
		int getRawHeading();
		HardwareSerial * _serial;
};

class MouseSensor{
	public:
		MouseSensor();
		void attach(HardwareSerial *mouseSerial);
		boolean available();
		String readLine();
		String readLastLine();
		int getX(String mouseInput);
		int getY(String mouseInput);
		void clear();
	private:
		String readRawSerialLine();
		HardwareSerial * _serial;
};

#endif
