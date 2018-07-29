/* An abstract class to represent a SHARP infrared sensor. Attaches itself to an analog pin that is set as an input. */

#include "Smartcar.h"

SHARP_IR::SHARP_IR(){
    //_sensorMedianDelay = 15; //median delay for all the sharp infrared sensors
    _pin = 99; //give it an initial (likely invalid) value
    _maxDistance = 0;
    _minDistance = 0;
}

void SHARP_IR::attach(unsigned short pin){
    _pin = pin;
    pinMode(_pin, INPUT);
}

unsigned int SHARP_IR::getDistance(){
    unsigned int distance = voltsToCentimeters(readAnalogInput());
    if ((distance < _minDistance) || (distance > _maxDistance)){
        return 0;
    }
    return distance;
}

unsigned int SHARP_IR::readAnalogInput(){
    return analogRead(_pin);
}
