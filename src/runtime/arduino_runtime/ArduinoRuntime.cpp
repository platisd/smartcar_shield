#include "ArduinoRuntime.hpp"
#include <Arduino.h>
#include <Wire.h>
#include <math.h>
#ifdef ESP_BOARD
#if defined(ESP32)
#include "analogWrite.h"
#endif
#if defined(ESP8266)
#define TEN_BIT_ANALOG
#include "../../utilities/Utilities.hpp"
#endif
#endif

void ArduinoRuntime::setPinDirection(uint8_t pin, uint8_t direction)
{
    pinMode(pin, direction);
}

void STORED_IN_RAM ArduinoRuntime::setPinState(uint8_t pin, uint8_t state)
{
    digitalWrite(pin, state);
}

int STORED_IN_RAM ArduinoRuntime::getPinState(uint8_t pin)
{
    return digitalRead(pin);
}

int ArduinoRuntime::getAnalogPinState(uint8_t pin)
{
    return analogRead(pin);
}

void ArduinoRuntime::setPWM(uint8_t pin, int dutyCycle)
{
#ifdef TEN_BIT_ANALOG
    dutyCycle = smartcarlib::utils::getMap(dutyCycle, 0, 255, 0, 1023);
#endif
    analogWrite(pin, dutyCycle);
}

void ArduinoRuntime::i2cInit()
{
    Wire.begin();
}

void ArduinoRuntime::i2cBeginTransmission(uint8_t address)
{
    Wire.beginTransmission(address);
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
    delay(milliseconds);
}

void ArduinoRuntime::delayMicros(unsigned int microseconds)
{
    delayMicroseconds(microseconds);
}

unsigned long ArduinoRuntime::getPulseDuration(uint8_t pin, uint8_t state, unsigned long timeout)
{
    return pulseIn(pin, state, timeout);
}

void ArduinoRuntime::setInterrupt(uint8_t pin, InterruptCallback callback, int mode)
{
    attachInterrupt(pin, callback, mode);
}

uint8_t ArduinoRuntime::getLowState() const
{
    return LOW;
}

uint8_t ArduinoRuntime::getHighState() const
{
    return HIGH;
}

uint8_t ArduinoRuntime::getOutputState() const
{
    return OUTPUT;
}

uint8_t ArduinoRuntime::getInputState() const
{
    return INPUT;
}

int ArduinoRuntime::getRisingEdgeMode() const
{
    return RISING;
}
