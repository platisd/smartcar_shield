/**
 * \class ServoMotor
 * A motor that is controlled via the length of pulses that are sent to it
 * through a single signal line.
 * It can be either a traditional servo motor that controls a shaft (e.g. for steering)
 * or an Electronic Speed Controller (ESC) that interpets speed in the same manner
 * and controls a (typically brushless) motor.
 */
#pragma once

#include <Servo.h>
#include <stdint.h>

#include "../../Motor.hpp"

namespace smartcarlib
{
namespace constants
{
namespace servomotor
{
const int kDefaultMinPulseLength  = 1000;
const int kDefaultIdlePulseLength = 1500;
const int kDefaultMaxPulseLength  = 2000;
} // namespace servomotor
} // namespace constants
} // namespace smartcarlib

class ServoMotor : public Servo, public Motor
{
public:
    /**
     * Constructs a servo motor
     * @param controlPin      The pin controlling the speed of the motor
     * @param minPulseLength  The motor's minimum pulse length in microseconds
     * @param idlePulseLength The pulse length that the motor is idle in microseconds
     * @param maxPulseLength  The motor's maximum pulse length in microseconds
     *
     * **Example:**
     * \code
     * const unsigned short ESC_PIN = 7;
     * ServoMotor esc(ESC_PIN);
     * \endcode
     */
    ServoMotor(uint8_t controlPin,
               int minPulseLength  = smartcarlib::constants::servomotor::kDefaultMinPulseLength,
               int idlePulseLength = smartcarlib::constants::servomotor::kDefaultIdlePulseLength,
               int maxPulseLength  = smartcarlib::constants::servomotor::kDefaultMaxPulseLength);

    /* Check `Motor` interface for documentation */
    void setSpeed(int speed) override;

private:
    const uint8_t kControlPin;
    const int kMinPulseLength;
    const int kIdlePulseLength;
    const int kMaxPulseLength;
    bool mAttached;
};
