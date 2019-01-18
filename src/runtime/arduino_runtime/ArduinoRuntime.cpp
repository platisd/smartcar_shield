#include "ArduinoRuntime.hpp"
#include <Arduino.h>
#include <Wire.h>
#include <math.h>

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

void ArduinoRuntime::i2cBeginTransmission(uint8_t address)
{
    return Wire.beginTransmission(address);
}

size_t ArduinoRuntime::i2cWrite(uint8_t value)
{
    return Wire.write(value);
}

uint8_t ArduinoRuntime::i2cEndTransmission()
{
    return Wire.endTransmission();
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

int8_t ArduinoRuntime::pinToInterrupt(uint8_t pin)
{
    return digitalPinToInterrupt(pin);
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

unsigned long ArduinoRuntime::getPulseDuration(uint8_t pin, uint8_t state, unsigned long timeout)
{
    return pulseIn(pin, state, timeout);
}

void ArduinoRuntime::setInterrupt(uint8_t pin, void (*callback)(void), int mode)
{
    return attachInterrupt(pin, callback, mode);
}
