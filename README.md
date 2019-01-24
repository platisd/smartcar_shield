# Smartcar shield [![Build Status](https://travis-ci.org/platisd/smartcar_shield.svg?branch=master)](https://travis-ci.org/platisd/smartcar_shield) [![codecov](https://codecov.io/gh/platisd/smartcar_shield/branch/master/graph/badge.svg)](https://codecov.io/gh/platisd/smartcar_shield) [![CII Best Practices](https://bestpractices.coreinfrastructure.org/projects/466/badge)](https://bestpractices.coreinfrastructure.org/projects/466)
A versatile and easy to use vehicle platform for hobby-grade projects

## Contents
* [Description](#description)
* [Hardware](#hardware)
* [Software](#software)
* [Materials needed](#materials-needed-default-setup)
* [Supported sensors](#supported-sensors)
* [How to get started](#how-to-get-started)
* [Dependencies](#dependencies)
* [Extensions](#extensions)
* [Project documentation](#project-documentation)
* [License](#license)

### Description
This repository, includes everything necessary to set up a Smartcar platform.
The Smartcar, is a vehicular educational platform, which enables the user to develop
various robotic applications in an affordable, pedagogic and easy way. It is intended
for persons with basic programming knowledge and little to no experience of electronics,
mechanics or robotics. More experienced users, should also find this approach convenient,
due to its open source nature, high level API and object oriented design.

### Hardware
The platform, hardware wise is conceptually focused around the Smartcar shield. The Smartcar
shield is an open hardware PCB, that is effortlessly stacked on top of an Arduino. It includes
an L293D H-bridge chip, that drives the motors, screw terminals to connect the motors
and a battery to them and a GY-50 gyroscope module (based on the L3G4200D sensor).
In [extras/eagle/smartcar_shield](/extras/eagle/smartcar_shield) you can find the
design files for the board, in a modifiable form, using free software. There are
also the options, to order it as-is, from either USA or China based manufacturers.
The Smartcar shield, facilitates efficient cable management, helps to preserve space
and is as small as possible, in order to keep the manufacturing price low. Additionally,
the components necessary to assemble it, are widely available over the Internet. Furthermore,
you can find 2D CAD drawings of the Smartcar chassis in various formats, at
[/extras/cad/2D](extras/cad/2D) which you can freely use in order to fabricate your own,
e.g. with a milling machine or a laser cutter.

### Software
The software, is the Smartcar shield Arduino library. It sports a high level API, meant to
enable its users to perform tasks easily with relatively few lines of code. It intends to
hide implementation details or low level hardware operations. The library, handles
the various components (i.e. the vehicle and the various sensors) as objects, therefore
enabling trivial control over the car's functions. The various operations exposed by the API,
are thoroughly explained in [API Documentation](https://platisd.github.io/smartcar_shield/).
You can find examples on how to use the library, in [/examples](/examples).

The library is versatile and is independent from the shield or specific hardware.
Particularly, its most important feature is that it supports multiple ways of steering and
throttling with different types of motors. Moreover, it is possible to steer the vehicle by
rotating the wheels on each side at a different speed, like a tank as well as by using a
servo motor. Another option is to use a brushed DC motor for steering, a setup that is very
common in cheap RC cars. Additionally, apart of the default brushed DC motors that are
connected to the Smartcar shield, you can also use an electronic speed controller (ESC)
in order to drive (throttle) the vehicle, using the very same library.

If the Smartcar shield library is installed through the Library Manager of Arduino IDE, as
recommended, the user will receive a notification in their IDE, when new versions are released
making updates particularly easy, without the need to use GitHub or git.

### Materials needed (default setup)
 * Smartcar chassis
 * [Smartcar shield PCB](extras/eagle/smartcar_shield)
 * L293D H-bridge
 * GY-50 gyroscope module
 * Two odometers (speed encoders) ([[1]](http://www.aliexpress.com/item/5pcs-IR-Infrared-Slotted-Optical-Speed-Measuring-Sensor-Optocoupler-Module-For-Motor-Test-For-Arduino-PIC/32465670452.html)
 and [[2]](http://www.aliexpress.com/item/HC-020K-Double-Speed-Measuring-Sensor-Module-with-Photoelectric-Encoders-Kit-top/32453020060.html)
 will suit commercially available chassis.
 [[3]](http://www.aliexpress.com/item/Correlation-photoelectric-sensor-lot-Infrared-correlation-count-sensor-module/2038553959.html)
 require some slight modifications)
 * Male & female pin headers (optionally get stackable Arduino headers too)
 * Five 2-pin screw terminals
 * Pin jumper
 * DIP16 socket
 * 0.1uF capacitors (2)
 * 100uF capacitor
 * 9V or 12V battery pack (do not use a single 9V battery)

### Supported sensors
 * Distance sensors
    * Ultrasonic
      * HC-SR04
      * SRF08
    * Infrared
      * SHARP GP2D120
      * SHARP GP2Y0A02
      * SHARP GP2Y0A21
 * Gyroscopes
   * GY-50 (L3G4200D)
 * Odometers
   * Directional (4+ pins)
   * Non-directional (3-pins)

### How to get started
 * Assemble the vehicle
 * [Install the library](../../wiki/Start-up-guide#get-libraries)
 * [Connect the motors appropriately](examples/Car/shieldMotorsTest/shieldMotorsTest.ino) (if using the default setup)
 * [Getting started with the Smartcar Platform](https://www.hackster.io/platisd/getting-started-with-the-smartcar-platform-1648ad)

### Dependencies
- [Wire library](http://arduino.cc/en/reference/Wire) (included with the Arduino IDE)
- [Servo library](https://www.arduino.cc/en/reference/servo) (included with the Arduino IDE)

### Extensions
You can utilize the object oriented design of this library in order to easily extend it.
Have a look at the [Library Architecture](../../wiki/Library-architecture).

### Project documentation
* [API Documentation](https://platisd.github.io/smartcar_shield/)
* [The Smartcar platform](http://plat.is/smartcar)
* [Wiki](../../wiki)
* [Turning software engineers into Makers](https://platis.solutions/blog/2016/06/17/turning-software-engineers-into-makers/)

### License
MIT
