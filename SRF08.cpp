#include "Smartcar.h"

const uint8_t SRF08::DEFAULT_PING_DELAY = 70;
const uint8_t SRF08::DEFAULT_SRF08_ADDRESS = 112;

static const uint8_t FIRST_ADDRESS = 112; //please refer to: http://www.robot-electronics.co.uk/htm/srf08tech.html
static const uint8_t LAST_ADDRESS = 127;

SRF08::SRF08(){}

void SRF08::attach(const uint8_t address){
	if (!TWCR) Wire.begin(); //if it hasn't been started (TWCR==0), start it
	_address = address;
	_delay = DEFAULT_PING_DELAY;
}

void SRF08::setGain(const uint8_t gainValue){
	Wire.beginTransmission(_address); //start i2c transmission
	Wire.write(0x01); //write to GAIN register (1)
	Wire.write(gainValue); //write the value
	Wire.endTransmission(); //end transmission
}

void SRF08::setRange(const uint8_t rangeValue){
	Wire.beginTransmission(_address); //start i2c transmission
	Wire.write(0x02); //write to range register (1)
	Wire.write(rangeValue); //write the value // Max_Range = (rangeValue * 3.4) + 3.4 in centimeters
	Wire.endTransmission(); //end transmission
}

void SRF08::setPingDelay(const uint8_t milliseconds){
	_delay = milliseconds;
}

int SRF08::getDistance(){
	return ping();
}

int SRF08::ping(){
	Wire.beginTransmission(_address);
	Wire.write(byte(0x00));
	Wire.write(byte(0x51));
	Wire.endTransmission();
	delay(_delay);
	Wire.beginTransmission(_address);
	Wire.write(byte(0x02));
	Wire.endTransmission();
	Wire.requestFrom(_address, uint8_t (2));
	while (Wire.available() < 2);
	byte high = Wire.read();
	byte low = Wire.read();
	return (high << 8) + low;
}

void SRF08::changeAddress(uint8_t newAddress){
	newAddress = constrain(newAddress, FIRST_ADDRESS, LAST_ADDRESS); //allow only valid values, between 112 and 127
	Wire.beginTransmission(_address);
	Wire.write(byte(0x00));
	Wire.write(byte(0xA0));
	Wire.endTransmission();

	Wire.beginTransmission(_address);
	Wire.write(byte(0x00));
	Wire.write(byte(0xAA));
	Wire.endTransmission();

	Wire.beginTransmission(_address);
	Wire.write(byte(0x00));
	Wire.write(byte(0xA5));
	Wire.endTransmission();

	Wire.beginTransmission(_address);
	Wire.write(byte(0x00));
	Wire.write(newAddress << 1);
	Wire.endTransmission();

	_address = newAddress;
}
