#include "Smartcar.h"

GP2D120::GP2D120(){
	_minDistance = 5; //GP2D120's minimum distance
	_maxDistance = 25; //GP2D120's maximum distance
}

unsigned int GP2D120::voltsToCentimeters(unsigned int volts){
	return ((2914 / (volts + 5)) - 1);
}
