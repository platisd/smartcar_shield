/**
 * The class to programatically represent the Arduino runtime environment or
 * alternatively calls to the AVR and Arduino libraries.
 */

#pragma once

#include "../Runtime.hpp"

class ArduinoRuntime : public Runtime
{
public:
    void setPinDirection(uint8_t pin, uint8_t direction) override;
    void setPinState(uint8_t pin, uint8_t state) override;
    int getPinState(uint8_t pin) override;
    int getAnalogPinState(uint8_t pin) override;
    void setPWM(uint8_t pin, int dutyCycle) override;
    void i2cInit() override;
    void i2cInit(uint8_t slaveAddress) override;
    void i2cBeginTransmission(uint8_t address) override;
    size_t i2cWrite(uint8_t value) override;
    size_t i2cWrite(const uint8_t* bytes, size_t bytesLength) override;
    uint8_t i2cEndTransmission() override;
    uint8_t i2cEndTransmission(uint8_t stopMessage) override;
    uint8_t i2cRequestFrom(uint8_t address, uint8_t numberOfBytes) override;
    int i2cAvailable() override;
    int i2cRead() override;
    void i2cOnReceive(void (*receptionHandler)(int numberOfBytes)) override;
    void i2cOnRequest(void (*requestHandler)(void)) override;
    uint8_t pinToInterrupt(uint8_t pin) override;
    uint8_t pinToPort(uint8_t pin) override;
    uint8_t pinToBitMask(uint8_t pin) override;
    uint8_t portToOutputRegister(uint8_t port) override;
    uint8_t portToInputRegister(uint8_t port) override;
    unsigned long currentTimeMillis() override;
    unsigned long currentTimeMicros() override;
    void delayMillis(unsigned long milliseconds) override;
    void delayMicros(unsigned int microseconds) override;
    long mapValue(long value, long fromLow, long fromHigh, long toLow, long toHigh) override;
    long constrainValue(long value, long min, long max) override;
};
