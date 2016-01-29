#include "Smartcar.h"

HeadingSensor::HeadingSensor() {
	_angularDisplacement = 0;
}

HeadingSensor::~HeadingSensor(){}

void HeadingSensor::update(){ //to be overriden by child classes
//updates the measurements conducted by the sensor
}

//gets the current reading from the sensor, which usually represents the displacement from a point of reference (e.g. the beginning of a measurement)
int HeadingSensor::getAngularDisplacement(){
	return _angularDisplacement;
}
