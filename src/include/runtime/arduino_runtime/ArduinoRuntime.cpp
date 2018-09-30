#include "ArduinoRuntime.hpp"
#include <Arduino.h>
#include <Wire.h>

ArduinoRuntime arduinoRuntime;

void ArduinoRuntime::setPinDirection(uint8_t pin, uint8_t direction)
{
    return pinMode(pin, direction);
}

void ArduinoRuntime::setPinState(uint8_t pin, uint8_t state)
{
    return digitalWrite(pin, state);
}

int ArduinoRuntime::getPinState(uint8_t pin)
{
    return digitalRead(pin);
}

int ArduinoRuntime::getAnalogPinState(uint8_t pin)
{
    return analogRead(pin);
}

void ArduinoRuntime::setPWM(uint8_t pin, int dutyCycle)
{
    return analogWrite(pin, dutyCycle);
}

void ArduinoRuntime::i2cInit()
{
    return Wire.begin();
}

void ArduinoRuntime::i2cInit(uint8_t slaveAddress)
{
    return Wire.begin(slaveAddress);
}

void ArduinoRuntime::i2cBeginTransmission(uint8_t address)
{
    return Wire.beginTransmission(address);
}

size_t ArduinoRuntime::i2cWrite(uint8_t value)
{
    return Wire.write(value);
}

size_t ArduinoRuntime::i2cWrite(const uint8_t* bytes, size_t bytesLength)
{
    return Wire.write(bytes, bytesLength);
}

uint8_t ArduinoRuntime::i2cEndTransmission()
{
    return Wire.endTransmission();
}

uint8_t ArduinoRuntime::i2cEndTransmission(uint8_t stopMessage)
{
    return Wire.endTransmission(stopMessage);
}

uint8_t ArduinoRuntime::i2cRequestFrom(uint8_t address, uint8_t numberOfBytes)
{
    return Wire.requestFrom(address, numberOfBytes);
}

int ArduinoRuntime::i2cAvailable()
{
    return Wire.available();
}

int ArduinoRuntime::i2cRead()
{
    return Wire.read();
}

void ArduinoRuntime::i2cOnReceive(void (*receptionHandler)(int numberOfBytes))
{
    return Wire.onReceive(receptionHandler);
}

void ArduinoRuntime::i2cOnRequest(void (*requestHandler)(void))
{
    return Wire.onRequest(requestHandler);
}

uint8_t ArduinoRuntime::pinToInterrupt(uint8_t pin)
{
    return digitalPinToInterrupt(pin);
}

uint8_t ArduinoRuntime::pinToPort(uint8_t pin)
{
    return digitalPinToPort(pin);
}

uint8_t ArduinoRuntime::pinToBitMask(uint8_t pin)
{
    return digitalPinToBitMask(pin);
}

uint8_t ArduinoRuntime::portToOutputRegister(uint8_t port)
{
    return portOutputRegister(port);
}

uint8_t ArduinoRuntime::portToInputRegister(uint8_t port)
{
    return portInputRegister(port);
}

unsigned long ArduinoRuntime::currentTimeMillis()
{
    return millis();
}

unsigned long ArduinoRuntime::currentTimeMicros()
{
    return micros();
}

void ArduinoRuntime::delayMillis(unsigned long milliseconds)
{
    return delay(milliseconds);
}

void ArduinoRuntime::delayMicros(unsigned int microseconds)
{
    return delayMicroseconds(microseconds);
}

long ArduinoRuntime::mapValue(long value, long fromLow, long fromHigh, long toLow, long toHigh)
{
    return map(value, fromLow, fromHigh, toLow, toHigh);
}

long ArduinoRuntime::constrainValue(long value, long min, long max)
{
    return constrain(value, min, max);
}
