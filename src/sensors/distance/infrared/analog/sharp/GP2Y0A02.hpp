/**
 * \class GP2Y0A02
 * Programmatically represents a SHARP GP2Y0A02 infrared sensor.
 * It can measure distances between 25 and 120 centimeters.
 */
#pragma once

#include "../../../../../runtime/Runtime.hpp"
#include "../InfraredAnalogSensor.hpp"

class GP2Y0A02 : public InfraredAnalogSensor
{
public:
    /**
     * Constructs a GP2Y0A02 sensor
     * @param runtime The runtime environment you want to run the class for
     * @param pin     The analog pin receiving sensor signals
     *
     * **Example:**
     * \code
     * ArduinoRuntime arduino,
     * const unsigned short IR_PIN = A1;
     * GP2Y0A02 infraredSensor(arduino, IR_PIN);
     * \endcode
     */
    GP2Y0A02(Runtime& runtime, uint8_t pin);

    /* Check `DistanceSensor` interface for documentation */
    unsigned int getDistance() override;

private:
    const uint8_t kPin;
    Runtime& mRuntime;
};

/**
 * \example GP2Y0A02.ino
 * A basic example on how to get a distance measurement from a GP2Y0A02 infrared sensor.
 */
