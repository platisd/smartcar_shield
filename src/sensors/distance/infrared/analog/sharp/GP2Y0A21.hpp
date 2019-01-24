/**
 * \class GP2Y0A21
 * Programmatically represents a SHARP GP2Y0A21 infrared sensor.
 * It can measure distances between 12 and 78 centimeters.
 */
#pragma once

#include "../../../../../runtime/Runtime.hpp"
#include "../InfraredAnalogSensor.hpp"

#ifdef SMARTCAR_BUILD_FOR_ARDUINO
#include "../../../../../runtime/arduino_runtime/ArduinoRuntime.hpp"
extern ArduinoRuntime arduinoRuntime;
#endif

class GP2Y0A21 : public InfraredAnalogSensor
{
public:
#ifdef SMARTCAR_BUILD_FOR_ARDUINO
    /**
     * Constructs a GP2Y0A21 sensor
     * @param pin   The analog pin receiving sensor signals
     *
     * **Example:**
     * \code
     * const unsigned short IR_PIN = A1;
     * GP2Y0A21 infraredSensor(IR_PIN);
     * \endcode
     */
    GP2Y0A21(uint8_t pin, Runtime& runtime = arduinoRuntime);
#else
    GP2Y0A21(uint8_t pin, Runtime& runtime);
#endif

    /* Check `DistanceSensor` interface for documentation */
    unsigned int getDistance() override;

private:
    const uint8_t kPin;
    Runtime& mRuntime;
};

/**
* \example GP2Y0A21.ino
* A basic example on how to get a distance measurement from a GP2Y0A21 infrared sensor.
*/
