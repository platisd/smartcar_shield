#include "Smartcar.h"

GP2Y0A21::GP2Y0A21(){
	_minDistance = 12; //GP2Y0A21's minimum distance
	_maxDistance = 78; //GP2Y0A21's maximum distance
}

unsigned int GP2Y0A21::voltsToCentimeters(unsigned int volts){
	return (1 / (0.0002391473 * volts - 0.0100251467)); //source: Jeroen Doggen http://plat.is/v3x25
}
