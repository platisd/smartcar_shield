/*
*	Odometer.cpp - Handles the gyroscope (L3G4200D) sensor of the Smartcar.
*	Version: 0.3
*	Author: Dimitris Platis (based on the bildr.org example: http://bildr.org/2011/06/l3g4200d-arduino/)
* 	License: GNU GPL v3 http://www.gnu.org/licenses/gpl-3.0.html
*/

#include "Smartcar.h"


/* ---- GYROSCOPE (L3G4200D) ---- */
const unsigned short Gyroscope::DEFAULT_GYRO_SAMPLING = 100; //how often (in ms) will the gyroscope be updated
const int Gyroscope::DEFAULT_GYRO_OFFSET = 8; //value that's usually given by the gyroscope when not moving. Use calibrate method to get an approximation
const float GYRO_SENSITIVITY = 0.07; //L3G4200D specific.
const int GYRO_THRESHOLD = 12; //Tolerance threshold. Determined experimentally, adapt accordingly.
const int CTRL_REG1 = 0x20;
const int CTRL_REG2 = 0x21;
const int CTRL_REG3 = 0x22;
const int CTRL_REG4 = 0x23;
const int CTRL_REG5 = 0x24;
const int L3G4200D_Address = 105; //gyroscope I2C address

Gyroscope::Gyroscope(int offset){
	_gyroOffset = offset;
}

void Gyroscope::attach(){
	initializeGyro();
}

void Gyroscope::begin(unsigned short samplingRate){
	_angularDisplacement = 0;
	_prevSample = millis();
	_samplingRate = samplingRate;
}

int Gyroscope::getAngularDisplacement(){
	return - _angularDisplacement; //we negate the value, in order to get negative values when turning counter clockwise (local convention)
}

/* based on http://www.pieter-jan.com/node/7 integration algorithm */
void Gyroscope::update(){
	if (millis()- _prevSample > _samplingRate){
		float gyroRate = 0;
		int gyroValue = getGyroValues(); 
		short drift = _gyroOffset - gyroValue;
 		if (abs(drift) > GYRO_THRESHOLD){ //if there has been a big enough drift (trying to contemplate for the noise)
			gyroRate = (gyroValue - _gyroOffset) * GYRO_SENSITIVITY;
		}
		unsigned long now = millis();
		_angularDisplacement += gyroRate / (1000 / (now - _prevSample));
		_prevSample = now;
	}
}

void Gyroscope::initializeGyro(){
	Wire.begin();
	setupL3G4200D(2000); // Configure L3G4200 at 2000 deg/sec. Other options: 250, 500 (NOT suggested, will have to redetermine offset) 
}

/* based on the bildr.org example: http://bildr.org/2011/06/l3g4200d-arduino/ */
int Gyroscope::getGyroValues(){
	byte zMSB = readRegister(L3G4200D_Address, 0x2D);
	byte zLSB = readRegister(L3G4200D_Address, 0x2C);
	return ((zMSB << 8) | zLSB);
}

int Gyroscope::setupL3G4200D(int scale){
	//From  Jim Lindblom of Sparkfun's code

	// Enable x, y, z and turn off power down:
	writeRegister(L3G4200D_Address, CTRL_REG1, 0b00001111);

	// If you'd like to adjust/use the HPF, you can edit the line below to configure CTRL_REG2:
	writeRegister(L3G4200D_Address, CTRL_REG2, 0b00000000);

	// Configure CTRL_REG3 to generate data ready interrupt on INT2
	// No interrupts used on INT1, if you'd like to configure INT1
	// or INT2 otherwise, consult the datasheet:
	writeRegister(L3G4200D_Address, CTRL_REG3, 0b00001000);

	// CTRL_REG4 controls the full-scale range, among other things:

	if(scale == 250){
		writeRegister(L3G4200D_Address, CTRL_REG4, 0b00000000);
	}else if(scale == 500){
		writeRegister(L3G4200D_Address, CTRL_REG4, 0b00010000);
	}else{
		writeRegister(L3G4200D_Address, CTRL_REG4, 0b00110000);
	}

	// CTRL_REG5 controls high-pass filtering of outputs, use it
	// if you'd like:
	writeRegister(L3G4200D_Address, CTRL_REG5, 0b00000000);
}

void Gyroscope::writeRegister(int deviceAddress, byte address, byte val) {
	Wire.beginTransmission(deviceAddress); // start transmission to device 
	Wire.write(address);       // send register address
	Wire.write(val);         // send value to write
	Wire.endTransmission();     // end transmission
}

int Gyroscope::readRegister(int deviceAddress, byte address){
	int v;
	Wire.beginTransmission(deviceAddress);
	Wire.write(address); // register to read
	Wire.endTransmission();
	Wire.requestFrom(deviceAddress, 1); // read a byte

	while(!Wire.available()) {
		//waiting
 	}

	v = Wire.read();

	return v;
}

unsigned int Gyroscope::calibrate(unsigned int measurements){ //use this function in order to determine the offset and change GYRO_OFFSET accordingly
	unsigned int sum = 0;
	for (int i = 0; i < measurements; i++){	
		sum += getGyroValues();
		delay(50);
	}
	return sum/measurements; //return the average	
}

boolean Gyroscope::isInstanciated(){
	return _gyroOffset; //if 0, it will return false, otherwise true. 0 is an error value
}
