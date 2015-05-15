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
/* ---- SONAR ---- */
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

static const int MAX_US_DISTANCE = 70; // Maximum usable sensor distance is around 70cm.
static const int US_ROUNDTRIP_CM = 57;      // Microseconds (uS) it takes sound to travel round-trip 1cm (2cm total), uses integer to save compiled code space.
static const int DISABLE_ONE_PIN = true;   // Set to "true" to save up to 26 bytes of compiled code space if you're NOT using one pin sensor connections.

// Probably shoudln't change these values unless you really know what you're doing.
static const int NO_ECHO = 0;               // Value returned if there's no ping echo within the specified MAX_SENSOR_DISTANCE
static const int MAX_SENSOR_DELAY = 18000;  // Maximum uS it takes for sensor to start the ping (SRF06 is the highest measured, just under 18ms).
static const int ECHO_TIMER_FREQ = 24;      // Frequency to check for a ping echo (every 24uS is about 0.4cm accuracy).
static const int PING_MEDIAN_DELAY = 29;    // Millisecond delay between pings in the getMedianDistance method.
static const int SONAR_DEFAULT_ITERATIONS = 5;    // The default value of iterations used in getMedianDistance() method.


// Macro to convert from microseconds to centimeters.
#define MicrosecondsToCentimeters(echoTime) (max((echoTime + US_ROUNDTRIP_CM / 2) / US_ROUNDTRIP_CM, (echoTime ? 1 : 0)))

/* ---- SHARP INFRARED SENSOR ---- */
static const float SHARP_SENSITIVITY = 0.0048828125; //only use with GP2Y0A02
//static const int MIN_IR_DISTANCE = 20; //Minimum reliable distance the sensor can measure. Derived from datasheet. For GP2Y0A02
//static const int MAX_IR_DISTANCE = 80;//Maximum reliable distance the sensor can measure in real life conditions. Determined experimentally. GP2Y0A02
static const int MIN_IR_DISTANCE = 4; //Minimum reliable distance the sensor can measure. Derived from datasheet. For GP2D120
static const int MAX_IR_DISTANCE = 25;//Maximum reliable distance the sensor can measure in real life conditions.Determined experimentally. GP2D120
static const int IR_DEFAULT_ITERATIONS = 5;    // The default value of iterations used in getMedianDistance() method.
static const int IR_MEDIAN_DELAY = 15; //Millisecond delay between measurements in the getMedianDistance method.

// Macro to convert from volts to centimeters.
//#define VoltsToCentimeters(volts) (65*pow(volts, -1.10)) //decomment for Sharp GP2Y0A02 as well as in Sharp_IR::getDistance()
#define VoltsToCentimeters(volts) ((2914 / (volts + 5)) - 1) //decomment for Sharp GP2D120 as well as in Sharp_IR::getDistance()

/* ---- ODOMETER ---- */
void updateCounter(); //ISR for the odometer
// Macro to convert from odometer pulses to centimeters. (ca. 48 pulses per meter, that's why we multiply by 2 to find value in cm)
#define PulsesToCentimeters(pulses) (pulses << 1)

/* --- CAR --- */
static const short DEFAULT_SERVO_PIN = 8;
static const short DEFAULT_ESC_PIN = 9;

/* ---- GYROSCOPE (L3G4200D) ---- */
	static const int DEFAULT_GYRO_SAMPLING = 25;
	static const int GYRO_OFFSET = 57; //The value that is usually given by the gyroscope when not moving. Determined experimentally, adapt accordingly.
	static const float GYRO_SENSITIVITY = 0.07; //L3G4200D specific.
	static const int GYRO_THRESHOLD = 12; //Tolerance threshold. Determined experimentally, adapt accordingly.

	static const int CTRL_REG1 = 0x20;
	static const int CTRL_REG2 =  0x21;
	static const int CTRL_REG3 = 0x22;
	static const int CTRL_REG4 = 0x23;
	static const int CTRL_REG5 = 0x24;

	static const int L3G4200D_Address = 105; //gyroscope I2C address

class Car {
	public:
		Car(unsigned short steeringWheelPin = DEFAULT_SERVO_PIN, unsigned short escPin = DEFAULT_ESC_PIN);
		void begin();
		void setSpeed(int speed);
		void setSteeringWheel(int degrees);
		int getSpeed();
		int getAngle();
	private:
		void setSteeringWheelPin(int steeringWheelPin);
		void setESCPin(int escPin);
		unsigned short _steeringWheelPin, _escPin;
		Servo motor, steeringWheel;
		int _speed, _angle;
};

class Sonar {
	public:
		Sonar();
		void attach(int triggerPin, int echoPin);
		unsigned int getDistance();
		unsigned int getMedianDistance();
		unsigned int getMedianDistance(int iterations);

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
};

class Sharp_IR {
	public:
		Sharp_IR();
		void attach(int IR_pin);
		unsigned int getDistance();
		unsigned int getMedianDistance();
		unsigned int getMedianDistance(int iterations);
	private:
		int _IR_pin;

};


class Odometer {
	public:
		Odometer();
		int attach(int odometerPin);
		void begin();
		unsigned long getDistance();
		void detach();
	private:
		int _odometerInterruptPin;
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
		void stop();
		int getAngularDisplacement();
		void update();
	private:
		void initMeasurement();
		void initializeGyro();
		int setupL3G4200D(int scale);
		void writeRegister(int deviceAddress, byte address, byte val);
		static int getGyroValues();
		static int readRegister(int deviceAddress, byte address);
		unsigned short _samplingRate;


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

#endif
