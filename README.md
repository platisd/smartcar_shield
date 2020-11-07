# Smartcar shield

![Tests](https://github.com/platisd/smartcar_shield/workflows/Tests/badge.svg)
![Examples](https://github.com/platisd/smartcar_shield/workflows/Examples/badge.svg)
![Documentation](https://github.com/platisd/smartcar_shield/workflows/Documentation/badge.svg)
[![codecov](https://codecov.io/gh/platisd/smartcar_shield/branch/master/graph/badge.svg)](https://codecov.io/gh/platisd/smartcar_shield)
[![CII Best Practices](https://bestpractices.coreinfrastructure.org/projects/466/badge)](https://bestpractices.coreinfrastructure.org/projects/466)
[![BCH compliance](https://bettercodehub.com/edge/badge/platisd/smartcar_shield?branch=master)](https://bettercodehub.com/)

A versatile and easy to use vehicle platform for hobby-grade projects

- [Description](#description)
- [Hardware](#hardware)
- [Software](#software)
- [Reference platforms](#reference-platforms)
- [How to get started](#how-to-get-started)
- [Dependencies](#dependencies)
- [Project documentation](#project-documentation)
- [License](#license)

## Description
This repository, includes everything necessary to set up a Smartcar platform.
The Smartcar, is a vehicular educational platform, which enables the user to develop
various robotic applications in an affordable, pedagogic and easy way. It is intended
for persons with basic programming knowledge and little to no experience of electronics,
mechanics or robotics. More experienced users, should also find this approach convenient,
due to its open source nature, high level API and object oriented design.

## Hardware
Despite the software being hardware-agnostic, historically, development has been based on specific
hardware. Initially, the **Smartcar shield** was used on top of an Arduino, using cheap car kits.
This setup will be referred to as `shield` or `v1`.

Since the winter of 2019, a new platform was developed from scratch, using the WiFi and Bluetooth
enabled ESP32 as the vehicle's "brain". This setup will be referred to as `platform` or `v2` and
will be considered the default one from now on. It offers greater robustness and many more
possibilities, as it offers out-of-the-box connectivity.

### Platform

![smartcar platform](https://i.imgur.com/cFrq2Wj.jpg)

The Smartcar platform is a complete hardware solution for your hobby-grade, connected vehicle project.
It is comprised of the chassis, made out of printed circuit boards, stacked on each other and an
ESP32 development board that drives the motors, controls the sensors and communicates with the
world via WiFi and Bluetooth.

There are 4 motors with directional odometers which enable not only to determine how much the
wheels have moved but also the direction they spin. It is equipped with a GY-50 gyroscope module
(based on the L3G4200D sensor) to provide heading information and sockets to optionally connect
4 `VL45L0X` LIDAR sensors to provide distance measurements. It is powered up by 8 AA batteries
and you can find the boards' source files in [extras/eagle/smartcar_platform](/extras/eagle/smartcar_shield).

### Shield

![smartcar_shield](https://i.imgur.com/7g5ts49.png)

The Smartcar shield is an open hardware PCB, that is effortlessly stacked on top of an Arduino. It
includes an L293D H-bridge chip, that drives the motors, screw terminals to connect the motors
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

## Software
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

## Reference platforms

### Smartcar platform (default setup)

* 1 x [Modules board](https://www.pcbway.com/project/shareproject/Smartcar___Module_board.html)
* 1 x [Motor board](https://www.pcbway.com/project/shareproject/Smartcar___Motors_board.html)
* 4 x [Brushed motors](https://www.aliexpress.com/item/32872592243.html) (530rpm @ 12V)
* 2 x [TXS0108E TTL level shifters](https://www.aliexpress.com/item/32472491036.html)
* 1 x DOIT ESP32 devkit v1 (30 GPIO pins)
* 1 x [TB6612FNG dual motor driver (Sparkfun)](https://www.sparkfun.com/products/14451)
* 4 x M6 (30mm) screws
* 4 x M6 nuts
* 16 x M3 (10mm) screws
* 16 x M3 nuts
* 2 x [HSP 02052 Front Bumper](https://www.aliexpress.com/item/32753903115.html)
* 10 x 6pin 2.54 pitch female pin headers
* 2 x 15pin 2.54 pitch female pin headers
* 1 x GY-50 gyroscope
* 4 x [8pin 2.54 pitch long male pin headers](https://www.aliexpress.com/item/32911455899.html) (30mm long)
* 4 x 8pin 2.54 pitch female pin headers
* 3 x 10KOhm resistors
* 8 x [AA battery holders](https://www.electrokit.com/produkt/batterihallare-1xaa-pcb/)
* 1 x [DD40AJSA adjustable buck converter](https://www.aliexpress.com/item/32816584868.html)
* 4 x [VL45L0x ToF distance sensors](https://www.aliexpress.com/item/32828144370.html) (optional)
  * 4 x 4pin 2.54 angled headers to mount the sensors vertically

### Smartcar shield

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

## How to get started
 * Assemble the vehicle
 * [Install the library](https://www.ardu-badge.com/Smartcar%20shield)
 * [Connect the motors appropriately](examples/Car/shieldMotorsTest/shieldMotorsTest.ino) (if using the shield)
 * [Getting started with the Smartcar Platform](https://www.hackster.io/platisd/getting-started-with-the-smartcar-platform-1648ad)

## Dependencies
All dependencies should be downloaded automatically by the Arduino IDE's library manager.

- [Wire library](http://arduino.cc/en/reference/Wire)
- [Servo library](https://www.arduino.cc/en/reference/servo)
- [ServoESP32](https://github.com/RoboticsBrno/ServoESP32)
- [ESP32 AnalogWrite](https://github.com/ERROPiX/ESP32_AnalogWrite)

## Project documentation
* [API Documentation](https://platisd.github.io/smartcar_shield/)
* [The Smartcar platform](http://plat.is/smartcar)
* [Wiki](https://github.com/platisd/smartcar_shield/wiki)
* [Turning software engineers into Makers](https://platis.solutions/blog/2016/06/17/turning-software-engineers-into-makers/)

## License
MIT
