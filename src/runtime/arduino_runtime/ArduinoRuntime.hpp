/**
 * \class ArduinoRuntime
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
    void i2cBeginTransmission(uint8_t address) override;
    size_t i2cWrite(uint8_t value) override;
    uint8_t i2cEndTransmission() override;
    uint8_t i2cRequestFrom(uint8_t address, uint8_t numberOfBytes) override;
    int i2cAvailable() override;
    int i2cRead() override;
    int8_t pinToInterrupt(uint8_t pin) override;
    unsigned long currentTimeMillis() override;
    unsigned long currentTimeMicros() override;
    void delayMillis(unsigned long milliseconds) override;
    void delayMicros(unsigned int microseconds) override;
    unsigned long getPulseDuration(uint8_t pin, uint8_t state, unsigned long timeout) override;
    void setInterrupt(uint8_t pin, InterruptCallback callback, int mode) override;
    uint8_t getLowState() const override;
    uint8_t getHighState() const override;
    uint8_t getOutputState() const override;
    uint8_t getInputState() const override;
    int getRisingEdgeMode() const override;
};
