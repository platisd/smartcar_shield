#include "Smartcar.h"

HeadingSensor::HeadingSensor() {
	_angularDisplacement = 0;
}

HeadingSensor::~HeadingSensor(){}

void HeadingSensor::update(){ //to be overriden by child classes
//updates the measurements conducted by the sensor
}

//gets the current reading from the sensor, which usually represents the displacement from a point of reference (e.g. the beginning of a measurement)
unsigned int HeadingSensor::getAngularDisplacement(){
        int normalizedReading = (int) _angularDisplacement % 360; //get the reading into the -360,360 scale
        if (normalizedReading < 0) normalizedReading += 360; //get the reading into the 0,360 scale
	return normalizedReading;
}
