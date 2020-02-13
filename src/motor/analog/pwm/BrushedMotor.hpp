/**
 * \class BrushedMotor
 * A brushed motor is controlled via two wires where the flow of current determines
 * the direction of rotation and the duty cycle the speed.
 *
 * For their control, we will assume the existince of a half-bridge such as the
 * L293D chip where three signals are needed, two for determining the direction
 * and another for determining the speed.
 */
#pragma once

#include <stdint.h>

#include "../../../runtime/Runtime.hpp"
#include "../../Motor.hpp"

#ifdef SMARTCAR_BUILD_FOR_ARDUINO
#include "../../../runtime/arduino_runtime/ArduinoRuntime.hpp"
extern ArduinoRuntime arduinoRuntime;
#endif

struct BrushedMotorPins
{
    /**
     * @brief Helper class to represent brushed motor pins
     *
     * @param forwardPin  The direction pin that when set to HIGH makes the motor spin forward
     * @param backwardPin The direction pin that when set to HIGH makes the motor spin forward
     * @param enablePin   The pin  that controls the motor's speed
     */
    BrushedMotorPins(uint8_t forwardPin, uint8_t backwardPin, uint8_t enablePin)
        : forward{ forwardPin }
        , backward{ backwardPin }
        , enable{ enablePin }
    {
    }

    const uint8_t forward;
    const uint8_t backward;
    const uint8_t enable;
};

class BrushedMotor : public Motor
{
public:
#ifdef SMARTCAR_BUILD_FOR_ARDUINO
    /**
     * Constructs a brushed DC motor instance
     * @param forwardPin  The direction pin that when set to HIGH makes the motor spin forward
     * @param backwardPin The direction pin that when set to HIGH makes the motor spin forward
     * @param enablePin   The pin  that controls the motor's speed
     *
     * **Example:**
     * \code
     * BrushedMotor leftMotor(8, 9, 10);
     * \endcode
     */
    BrushedMotor(uint8_t forwardPin,
                 uint8_t backwardPin,
                 uint8_t enablePin,
                 Runtime& runtime = arduinoRuntime);

    /**
     * Constructs a brushed DC motor instance
     * @param pins  The `BrushedMotorPins` object with the pins of the motor
     *
     * **Example:**
     * \code
     * BrushedMotor leftMotor(smartcarlib::hw::v2::leftMotorPins);
     * \endcode
     */
    BrushedMotor(BrushedMotorPins pins, Runtime& runtime = arduinoRuntime);

#else
    BrushedMotor(uint8_t forwardPin, uint8_t backwardPin, uint8_t enablePin, Runtime& runtime);
    BrushedMotor(BrushedMotorPins pins, Runtime& runtime);
#endif

    /* Check `Motor` interface for documentation */
    void setSpeed(int speed) override;

private:
    void attach();
    const uint8_t kForwardPin;
    const uint8_t kBackwardPin;
    const uint8_t kEnablePin;
    Runtime& mRuntime;
    const uint8_t kOutput;
    const uint8_t kLow;
    const uint8_t kHigh;
    bool mAttached{ false };
};
