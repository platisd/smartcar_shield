#pragma once

#include "gmock/gmock.h"

#include "Runtime.hpp"

class MockRuntime : public Runtime
{
public:
    MOCK_METHOD2(setPinDirection, void(uint8_t pin, uint8_t direction));
    MOCK_METHOD2(setPinState, void(uint8_t pin, uint8_t state));
    MOCK_METHOD1(getPinState, int(uint8_t pin));
    MOCK_METHOD1(getAnalogPinState, int(uint8_t pin));
    MOCK_METHOD2(setPWM, void(uint8_t pin, int dutyCycle));
    MOCK_METHOD0(i2cInit, void());
    MOCK_METHOD1(i2cInit, void(uint8_t slaveAddress));
    MOCK_METHOD1(i2cBeginTransmission, void(uint8_t address));
    MOCK_METHOD1(i2cWrite, size_t(uint8_t value));
    MOCK_METHOD2(i2cWrite, size_t(const uint8_t* bytes, size_t bytesLength));
    MOCK_METHOD0(i2cEndTransmission, uint8_t());
    MOCK_METHOD1(i2cEndTransmission, uint8_t(uint8_t stopMessage));
    MOCK_METHOD2(i2cRequestFrom, uint8_t(uint8_t address, uint8_t numberOfBytes));
    MOCK_METHOD0(i2cAvailable, int());
    MOCK_METHOD0(i2cRead, int());
    MOCK_METHOD1(i2cOnReceive, void(void (*receptionHandler)(int numberOfBytes)));
    MOCK_METHOD1(i2cOnRequest, void(void (*requestHandler)(void)));
    MOCK_METHOD1(pinToInterrupt, uint8_t(uint8_t pin));
    MOCK_METHOD1(pinToPort, uint8_t(uint8_t pin));
    MOCK_METHOD1(pinToBitMask, uint8_t(uint8_t pin));
    MOCK_METHOD1(portToOutputRegister, uint8_t(uint8_t port));
    MOCK_METHOD1(portToInputRegister, uint8_t(uint8_t port));
    MOCK_METHOD0(currentTimeMillis, unsigned long());
    MOCK_METHOD0(currentTimeMicros, unsigned long());
    MOCK_METHOD1(delayMillis, void(unsigned long milliseconds));
    MOCK_METHOD1(delayMicros, void(unsigned int microseconds));
    MOCK_METHOD5(mapValue, long(long value, long fromLow, long fromHigh, long toLow, long toHigh));
    MOCK_METHOD3(constrainValue, long(long value, long min, long max));
    MOCK_METHOD3(getPulseDuration,
                 unsigned long(uint8_t pin, uint8_t state, unsigned long timeout));
};
