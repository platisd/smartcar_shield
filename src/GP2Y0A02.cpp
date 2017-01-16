#include "Smartcar.h"

GP2Y0A02::GP2Y0A02(){
    _minDistance = 25; //GP2Y0A02's minimum distance
    _maxDistance = 120; //GP2Y0A02's maximum distance
}

unsigned int GP2Y0A02::voltsToCentimeters(unsigned int volts){
    float calibratedVoltage = volts * 0.0048828125; //sensor's sensitivity
    return (65*pow(calibratedVoltage, -1.10));
}
