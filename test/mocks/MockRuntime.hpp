#pragma once

#include "gmock/gmock.h"

#include "Runtime.hpp"

class MockRuntime : public Runtime
{
public:
    MOCK_METHOD(void, setPinDirection, (uint8_t pin, uint8_t direction), (override));
    MOCK_METHOD(void, setPinState, (uint8_t pin, uint8_t state), (override));
    MOCK_METHOD(int, getPinState, (uint8_t pin), (override));
    MOCK_METHOD(int, getAnalogPinState, (uint8_t pin), (override));
    MOCK_METHOD(void, setPWM, (uint8_t pin, int dutyCycle), (override));
    MOCK_METHOD(void, i2cInit, (), (override));
    MOCK_METHOD(void, i2cBeginTransmission, (uint8_t address), (override));
    MOCK_METHOD(size_t, i2cWrite, (uint8_t value), (override));
    MOCK_METHOD(uint8_t, i2cEndTransmission, (), (override));
    MOCK_METHOD(uint8_t, i2cRequestFrom, (uint8_t address, uint8_t numberOfBytes), (override));
    MOCK_METHOD(int, i2cAvailable, (), (override));
    MOCK_METHOD(int, i2cRead, (), (override));
    MOCK_METHOD(int8_t, pinToInterrupt, (uint8_t pin), (override));
    MOCK_METHOD(unsigned long, currentTimeMillis, (), (override));
    MOCK_METHOD(unsigned long, currentTimeMicros, (), (override));
    MOCK_METHOD(void, delayMillis, (unsigned long milliseconds), (override));
    MOCK_METHOD(void, delayMicros, (unsigned int microseconds), (override));
    MOCK_METHOD(unsigned long,
                getPulseDuration,
                (uint8_t pin, uint8_t state, unsigned long timeout),
                (override));
    MOCK_METHOD(void, setInterrupt, (uint8_t pin, void (*callback)(void), int mode), (override));
    MOCK_METHOD(uint8_t, getLowState, (), (const, override));
    MOCK_METHOD(uint8_t, getHighState, (), (const, override));
    MOCK_METHOD(uint8_t, getOutputState, (), (const, override));
    MOCK_METHOD(uint8_t, getInputState, (), (const, override));
    MOCK_METHOD(int, getRisingEdgeMode, (), (const, override));
    MOCK_METHOD(void, enableInterrupts, (), (override));
    MOCK_METHOD(void, disableInterrupts, (), (override));
};
