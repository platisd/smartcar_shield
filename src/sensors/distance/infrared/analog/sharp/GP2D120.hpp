/**
 * \class GP2D120
 * Programmatically represents a SHARP GP2D120 infrared sensor.
 * It can measure distances between 5 and 25 centimeters.
 */
#pragma once

#include "../../../../../runtime/Runtime.hpp"
#include "../InfraredAnalogSensor.hpp"

#ifdef SMARTCAR_BUILD_FOR_ARDUINO
#include "../../../../../runtime/arduino_runtime/ArduinoRuntime.hpp"
extern ArduinoRuntime arduinoRuntime;
#endif

class GP2D120 : public InfraredAnalogSensor
{
public:
#ifdef SMARTCAR_BUILD_FOR_ARDUINO
    /**
     * Constructs a GP2D120 sensor
     * @param pin   The analog pin receiving sensor signals
     *
     * **Example:**
     * \code
     * const unsigned short IR_PIN = A1;
     * GP2D120 infraredSensor(IR_PIN);
     * \endcode
     */
    GP2D120(uint8_t pin, Runtime& runtime = arduinoRuntime);
#else
    GP2D120(uint8_t pin, Runtime& runtime);
#endif

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
