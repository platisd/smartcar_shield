/**
 * \class GP2D120
 * Programmatically represents a SHARP GP2D120 infrared sensor.
 * It can measure distances between 5 and 25 centimeters.
 */
#pragma once

#include <stdint.h>

#include "../../../../../runtime/Runtime.hpp"
#include "../InfraredAnalogSensor.hpp"

class GP2D120 : public InfraredAnalogSensor
{
public:
    /**
     * Constructs a GP2D120 sensor
     * @param runtime The runtime environment you want to run the class for
     * @param pin     The analog pin receiving sensor signals
     *
     * **Example:**
     * \code
     * ArduinoRuntime arduinoRuntime;
     * const unsigned short IR_PIN = A1;
     * GP2D120 infraredSensor(arduinoRuntime, IR_PIN);
     * \endcode
     */
    GP2D120(Runtime& runtime, uint8_t pin);

    /* Check `DistanceSensor` interface for documentation */
    unsigned int getDistance() override;

private:
    const uint8_t kPin;
    Runtime& mRuntime;
};

/**
 * \example GP2D120.ino
 * A basic example on how to get a distance measurement from a GP2D120 infrared sensor.
 */
