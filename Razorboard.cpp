/*
*	Razorboard.cpp - Reads data from razorboard
*	Author: Dimitris Platis (based on the Smartcar project by Team Pegasus)
* 	License: GNU GPL v3 http://www.gnu.org/licenses/gpl-3.0.html
*/
#include "AndroidCar.h"

Razorboard::Razorboard(){
}

void Razorboard::attach(HardwareSerial *razorSerial){
	_serial = razorSerial;
	_serial->begin(9600);
	_serial->setTimeout(50);
}

boolean Razorboard::available(){
	return _serial->available();
}

String Razorboard::readLine(){
	if (available()){
		return readRawSerialLine();
	}else{
		return "error";	
	}
}

/* read whatever is in the buffer untill you reach the last available line and return it */
String Razorboard::readLastLine(){
	if (available()){
		String input = "";
		input.reserve(32);
		while (available()) input = readRawSerialLine();
		return input;
	}else{
		return "error";
	}
}

int Razorboard::getHeading(){
	if (available()){
		return getRawHeading();
	}else {
		return -1000;
	}
}

int Razorboard::getLatestHeading(){
	if (available()){
		int heading = 0;
		while (available()) heading = getRawHeading();
		return heading;
	}else{
		return -1000;
	}
}

String Razorboard::readRawSerialLine(){
	return _serial->readStringUntil('\n');
}

int Razorboard::getRawHeading(){
		String line = readRawSerialLine();
		return line.substring(line.indexOf('=') + 1,line.indexOf(',')).toInt();
}
