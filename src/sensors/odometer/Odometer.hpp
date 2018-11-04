/**
 * An interface to programmatically represent all odometers
 */
#pragma once

class Odometer
{
    /**
     * Returns the total travelled distance in centimeters
     * @return The total travelled distance in centimeters
     */
    virtual unsigned long getDistance() = 0;

    /**
     * Returns the current absolute speed in meters/sec
     * @return The absolute speed in meters/sec
     */
    virtual float getSpeed() = 0;
};
