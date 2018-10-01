/**
 * An interface to serve as a wrapper for the native runtime environment,
 * greatly inspired by the Arduino library API.
 * Its purpose is to facilitate testing and porting onto different platforms.
 * All used environment (e.g. Arduino) free functions should be included in
 * this interface so they can be mocked in tests and ported to different
 * environments.
 */
#pragma once
#include <stddef.h>
#include <stdint.h>

class Runtime
{
public:
    virtual ~Runtime() = default;

    /**
     * Set pin direction, equivalent of `pinMode` in Arduino
     * @param pin       The pin to set direction
     * @param direction The pin's direction (e.g. `INPUT`, `OUTPUT` etc)
     */
    virtual void setPinDirection(uint8_t pin, uint8_t direction) = 0;

    /**
     * Set pin state, equivalent of `digitalWrite` in Arduino
     * @param pin   The pin to set state
     * @param state The pin's state (i.e. `HIGH` or `LOW`)
     */
    virtual void setPinState(uint8_t pin, uint8_t state) = 0;

    /**
     * Get the pin state, equivalent of `digitalRead` in Arduino
     * @param  pin Pin to get state
     * @return     Pin's state (i.e. HIGH or LOW)
     */
    virtual int getPinState(uint8_t pin) = 0;

    /**
     * Get pin's analog (ADC) reading, equivalent of `analogRead` in Arduino
     * @param  pin Pin to get ADC reading
     * @return     ADC reading
     */
    virtual int getAnalogPinState(uint8_t pin) = 0;

    /**
     * Set PWM state, equivalent of `analogWrite` in Arduino
     * @param pin       Pin to set PWM
     * @param dutyCycle Duty cycle of PWM
     */
    virtual void setPWM(uint8_t pin, int dutyCycle) = 0;

    /**
     * Initialize I2C bus as master, equivalent of `Wire.begin` in Arduino
     */
    virtual void i2cInit() = 0;

    /**
     * Initialize I2C bus as slave, equivalent of `Wire.begin` in Arduino
     * @param slaveAddress Address to assume as an I2C slave
     */
    virtual void i2cInit(uint8_t slaveAddress) = 0;

    /**
     * Initiate a transmission to the specified I2C slave device, equivalent
     * of `Wire.beginTransmission` in Arduino
     * @param address I2C address to begin a transmission to
     */
    virtual void i2cBeginTransmission(uint8_t address) = 0;

    /**
     * Send the specified byte via i2c, equivalent of `Wire.write` in Arduino
     * @param  value Byte to send
     * @return       Number of bytes sent
     */
    virtual size_t i2cWrite(uint8_t value) = 0;

    /**
     * Send the specified bytes via i2c, equivalent of `Wire.write` in Arduino
     * @param  bytes       Bytes to send
     * @param  bytesLength Number of bytes to send
     * @return             Number of bytes sent
     */
    virtual size_t i2cWrite(const uint8_t* bytes, size_t bytesLength) = 0;

    /**
     * Ends a transmission to an I2C device equivalent of
     * `Wire.endTransmission` in Arduino
     * @return Transmission status
     */
    virtual uint8_t i2cEndTransmission() = 0;

    /**
     * Ends a transmission to an I2C device equivalent of
     * `Wire.endTransmission` in Arduino
     * @param  stopMessage Stop message to release the I2C bus
     * @return             Transmission status
     */
    virtual uint8_t i2cEndTransmission(uint8_t stopMessage) = 0;

    /**
     * Request a number of bytes from the specified I2C slave, equivalent of
     * `Wire.requestFrom` in Arduino
     * @param  address       I2C slave device address
     * @param  numberOfBytes Number of bytes to request
     * @return               Number of bytes returned from the slave
     */
    virtual uint8_t i2cRequestFrom(uint8_t address, uint8_t numberOfBytes) = 0;

    /**
     * Gets the number of bytes available to be retrieved, equivalent of
     * `Wire.available`O in Arduino
     * @return The number of bytes available for reading
     */
    virtual int i2cAvailable() = 0;

    /**
     * Reads a byte from I2C bus, equivalent of `Wire.read` in Arduino
     * @return Byte read from I2C
     */
    virtual int i2cRead() = 0;

    /**
     * Registers a callback to be executed upon reception of an I2C
     * transmission from a master, equivalent of `Wire.onReceive` in Arduino
     * @param receptionHandler Function to be called
     */
    virtual void i2cOnReceive(void (*receptionHandler)(int numberOfBytes)) = 0;

    /**
     * Registers a callback to be executed upon request for an I2C
     * transmission from a master, equivalent of `Wire.onRequest` in Arduino
     * @param requestHandler Function to be called
     */
    virtual void i2cOnRequest(void (*requestHandler)(void)) = 0;

    /**
     * Gets the interrupt number of the specified pin, equivalent of
     * `digitalPinToInterrupt` in Arduino
     * @param  pin Pin to get interrupt number
     * @return     Interrupt number
     */
    virtual uint8_t pinToInterrupt(uint8_t pin) = 0;

    /**
     * Gets the port of the specified pin, equivalent of `digitalPinToPort` in
     * Arduino
     * @param  pin Pin to get the port
     * @return     Port of the pin
     */
    virtual uint8_t pinToPort(uint8_t pin) = 0;

    /**
     * Gets the bitmask of the specified pin, equivalent of
     * `digitalPinToBitMask` in Arduino
     * @param  pin Pin to get the bitmask
     * @return     Bitmask of the pin
     */
    virtual uint8_t pinToBitMask(uint8_t pin) = 0;

    /**
     * Gets an output port register of the specified port, equivalent of
     * `portOutputRegister` in Arduino
     * @param  port Port to get the output port register
     * @return      Output port register of the port
     */
    virtual uint8_t portToOutputRegister(uint8_t port) = 0;

    /**
     * Gets an input port register of the specified port, equivalent of
     * `portInputRegister` in Arduino
     * @param  port Port to get the input port register
     * @return      Input port register of the port
     */
    virtual uint8_t portToInputRegister(uint8_t port) = 0;

    /**
     * Gets the amount of milliseconds since the microcontroller started
     * running, equivalent of `millis` in Arduino
     * @return Milliseconds since beginning of operation
     */
    virtual unsigned long currentTimeMillis() = 0;

    /**
     * Gets the amount of microseconds since the microcontroller started
     * running, equivalent of `micros` in Arduino
     * @return Microseconds since beginning of operation
     */
    virtual unsigned long currentTimeMicros() = 0;

    /**
     * Block the execution for the specified number of milliseconds, equivalent
     * of `delay` in Arduino
     * @param milliseconds How much time to block execution in milliseconds
     */
    virtual void delayMillis(unsigned long milliseconds) = 0;

    /**
     * Block the execution for the specified number of microseconds, equivalent
     * of `delayMicroseconds` in Arduino
     * @param microseconds How much time to block execution in microseconds
     */
    virtual void delayMicros(unsigned int microseconds) = 0;

    /**
     * Maps a number from one range to another, equivalent of `map` in Arduino
     * @param  value    Value to be mapped
     * @param  fromLow  Initial lowest limit of the range
     * @param  fromHigh Initial highest limit of the range
     * @param  toLow    Lowest limit of the output range
     * @param  toHigh   Highest limit of the output range
     * @return          Value in the new range
     */
    virtual long mapValue(long value, long fromLow, long fromHigh, long toLow, long toHigh) = 0;

    /**
     * Constrain a value within a range, equivalent of `constrain` in Arduino
     * @param  value Value to be constrained
     * @param  min   Minimum range limit
     * @param  max   Maximum range limit
     * @return       Value within the limit
     */
    virtual long constrainValue(long value, long min, long max) = 0;
};
