/**
 * \class GP2Y0A02
 * Programmatically represents a SHARP GP2Y0A02 infrared sensor.
 * It can measure distances between 25 and 120 centimeters.
 */
#pragma once

#include "../../../../../runtime/Runtime.hpp"
#include "../InfraredAnalogSensor.hpp"

#ifdef SMARTCAR_BUILD_FOR_ARDUINO
#include "../../../../../runtime/arduino_runtime/ArduinoRuntime.hpp"
extern ArduinoRuntime arduinoRuntime;
#endif

class GP2Y0A02 : public InfraredAnalogSensor
{
public:
#ifdef SMARTCAR_BUILD_FOR_ARDUINO
    /**
     * Constructs a GP2Y0A02 sensor
     * @param pin   The analog pin receiving sensor signals
     *
     * **Example:**
     * \code
     * const unsigned short IR_PIN = A1;
     * GP2Y0A02 infraredSensor(IR_PIN);
     * \endcode
     */
    GP2Y0A02(uint8_t pin, Runtime& runtime = arduinoRuntime);
#else
    GP2Y0A02(uint8_t pin, Runtime& runtime);
#endif

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
