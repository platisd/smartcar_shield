/*
*	MouseSensor.cpp - Reads data from MouseSensor
*	Author: Dimitris Platis (based on the Smartcar project by Team Pegasus)
* 	License: GNU GPL v3 http://www.gnu.org/licenses/gpl-3.0.html
*/
#include "AndroidCar.h"

MouseSensor::MouseSensor(){
}

void MouseSensor::attach(HardwareSerial *mouseSerial){
	_serial = mouseSerial;
	_serial->begin(115200);
	_serial->setTimeout(50);
}

boolean MouseSensor::available(){
	return _serial->available();
}

String MouseSensor::readLine(){
	if (available()){
		return readRawSerialLine();
	}else{
		return "error";	
	}
}

/* read whatever is in the buffer untill you reach the last available line and return it */
String MouseSensor::readLastLine(){
	if (available()){
		String input = "";
		while (available()) input = readRawSerialLine();
		return input;
	}else{
		return "error";
	}
}

int MouseSensor::getX(String mouseInput){
	String x = mouseInput.substring(mouseInput.indexOf(':') + 1,mouseInput.indexOf(' '));
	int out = x.toInt();
	if (out || x.equals("0")){
		return out;
	}else{
		return -9999;
	}
}

int MouseSensor::getY(String mouseInput){
	mouseInput.trim();
	String y = mouseInput.substring(mouseInput.lastIndexOf(':') + 1);
	int out = y.toInt();
	if (out || y.equals("0")){
		return out;
	}else{
		return -9999;
	} 
}


String MouseSensor::readRawSerialLine(){
	return _serial->readStringUntil('\n');
}

void MouseSensor::clear(){
	_serial->print("c");
}
