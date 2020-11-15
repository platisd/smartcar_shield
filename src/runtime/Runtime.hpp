/**
 * \interface Runtime
 * An interface to serve as a wrapper for the native runtime environment,
 * greatly inspired by the Arduino library API.
 * Its purpose is to facilitate testing and porting onto different platforms.
 * All used environment (e.g. Arduino) free functions should be included in
 * this interface so they can be mocked in tests and ported to different
 * environments.
 */
#pragma once
#include <stddef.h> // NOLINT(modernize-deprecated-headers)
#include <stdint.h> // NOLINT(modernize-deprecated-headers)

// Determine if we use an Espressif platform (i.e. an ESP32 or ESP8266 board).
// We need this since they are not fully compatible with the Arduino API
// and therefore we have to do some extra adaptations.
#if defined(ESP32) || defined(ESP8266)
#define ESP_BOARD
#endif

#include "InterruptCallback.hpp"

#ifdef ESP_BOARD
#if defined(ESP32)
#include "esp_attr.h"
#define STORED_IN_RAM IRAM_ATTR
#else // ESP8266
#include "c_types.h"
#define STORED_IN_RAM ICACHE_RAM_ATTR
#endif
#else // Other architectures
#define STORED_IN_RAM
#endif

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
     * Ends a transmission to an I2C device equivalent of
     * `Wire.endTransmission` in Arduino
     * @return Transmission status
     */
    virtual uint8_t i2cEndTransmission() = 0;

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
     * Gets the interrupt number of the specified pin, equivalent of
     * `digitalPinToInterrupt` in Arduino
     * @param  pin Pin to get interrupt number
     * @return     Interrupt number
     */
    virtual int8_t pinToInterrupt(uint8_t pin) = 0;

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
     * Gets the incomming pulse length in microseconds starting from the nearest
     * `state`, equivalent to `pulseIn` in Arduino
     * @param  pin     The pin to expect the pulse
     * @param  state   The state (`HIGH` or `LOW`) of the incoming pulse
     * @param  timeout How long to wait for a complete pulse in microseconds
     * @return         `0` if no pulse arrived otherwise the pulse length in
     *                     in microseconds
     */
    virtual unsigned long getPulseDuration(uint8_t pin, uint8_t state, unsigned long timeout) = 0;

    /**
     * Enables an external hardware interrupt and provides a callback, equivalent to
     * `attachInterrupt` in Arduino
     * @param pin       The interrupt pin to attach the interrupt
     * @param callback  The callback to be executed
     * @param mode      The state of the pin to run the callback
     */
    virtual void setInterrupt(uint8_t pin, InterruptCallback callback, int mode) = 0;

    /**
     * @brief Get the runtime-specific value representing a logical `LOW` voltage state
     *
     * @return uint8_t The `LOW` value
     */
    virtual uint8_t getLowState() const = 0;

    /**
     * @brief Get the runtime-specific value representing a logical `HIGH` voltage state
     *
     * @return uint8_t The `HIGH` value
     */
    virtual uint8_t getHighState() const = 0;

    /**
     * @brief Get the runtime-specific value representing an `OUTPUT` pin state
     *
     * @return uint8_t The `OUTPUT` state
     */
    virtual uint8_t getOutputState() const = 0;

    /**
     * @brief Get the runtime-specific value representing an `INPUT` pin state
     *
     * @return uint8_t The `INPUT` state
     */
    virtual uint8_t getInputState() const = 0;

    /**
     * @brief Get the rising edge constant for setting an interrupt
     *
     * @return int The rising edge mode
     */
    virtual int getRisingEdgeMode() const = 0;
};
