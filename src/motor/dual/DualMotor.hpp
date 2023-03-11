/**
 * \class DualMotor
 * A set of two motors that should always spin in the same way while being controlled by
 * different pins. This class should be used when you have two motors that are connected
 * to the same driver on different pins but you want to control them as one.
 */

#pragma once

#include <stdint.h> // NOLINT(modernize-deprecated-headers)

#include "../Motor.hpp"

class DualMotor : public Motor
{
public:
    /**
     * @brief Constructs a DualMotor object
     * The two motors will spin in the same direction and at the same "speed"
     *
     * @param motor1 The first motor
     * @param motor2 The second motor
     */
    DualMotor(Motor& motor1, Motor& motor2);

    /* Check `Motor` interface for documentation */
    void setSpeed(int speed) override;

private:
    Motor& mMotor1;
    Motor& mMotor2;
};
