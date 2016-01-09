# Smartcar shield

### Description
This repository, includes everything necessary to set up a Smartcar platform. The Smartcar, is an vehicular educational platform, which enables the user to develop various robotic applications in an affordable, pedagogic and easy way. It is intended for persons with basic programming knowledge and little to no experience of electronics, mechanics or robotics. More experienced users, should also find this approach convenient, due to its open source nature, high level API and object oriented design.

The platform, hardware wise is focused (but not explicitely dependent) around the Smartcar shield. The Smartcar shield is an open hardware PCB, that is effortlessly stacked on top of an Arduino. It includes an L293D H-bridge chip, that drives the motors, screw terminals to connect the motors and a battery to them and a GY-50 gyroscope module (based on the L3G4200D sensor). In [extras/eagle](/extras/eagle) you can find the design files for the board, in a modifiable form, using free software. There are also the options, to order it as-is, from either USA or China based manufacturers. The Smartcar shield, facilitates efficient cable management, reserves space and is as small as possible, in order to keep the manufacturing price low. Additionally, the components necessary to assemble it, are widely available over the Internet.

The software, is the Smartcar shield Arduino library. It sports a high level API, meant to enable its users to perform tasks easily, with as few lines of code possible. It intends to hide implementation details or low level hardware operations, that might estrange the average user. The library, handles  the various components (i.e. the vehicle and the various sensors) as objects, therefore enabling trivial control over the car's functions. The various operations exposed by the API, are thoroughly explained in [wiki/API-documentation](../../wiki/API-documentation).

The library, currently supports steering of the vehicle by rotating the wheels on each side at a different speed, like a tank. In the future, the option to achieve this using a servo motor and the throttle to be controlled by an ESC (Electronic Speed Controller), will be added. This, will make the library independent from the shield.

### Materials needed
 * Smartcar chassis (CAD files for miling machines or laser cutters, coming soon)
 * L293D H-bridge
 * GY-50 gyroscope module
 * Two speed encoders ([[1]](http://www.aliexpress.com/item/5pcs-IR-Infrared-Slotted-Optical-Speed-Measuring-Sensor-Optocoupler-Module-For-Motor-Test-For-Arduino-PIC/32465670452.html) and [[2]](http://www.aliexpress.com/item/HC-020K-Double-Speed-Measuring-Sensor-Module-with-Photoelectric-Encoders-Kit-top/32453020060.html) will suit commercially available chassis. [[3]](http://www.aliexpress.com/item/Correlation-photoelectric-sensor-lot-Infrared-correlation-count-sensor-module/2038553959.html) require some slight modifications) 
 * Male & female pin headers (optionally get stackable arduino headers too)
 * Five 2-pin screw terminals
 * Pin jumper
 * DIP16 socket
 * 9V or 12V battery pack (do not use a single 9V battery)

### Compatible sensors
 * Distance sensors
    * Ultrasonic
      * HC-SR04
      * SRF08
    * Infrared
      * SHARP GP2D120
      * SHARP GP2Y0A02
      * SHARP GP2Y0A21

### How to get started
 * Assembling the vehicle
 * [Install the library](../../wiki/Start-up-guide#get-libraries)

### Dependencies
- [Wire library](http://arduino.cc/en/reference/Wire) (included with the Arduino IDE)

### Project documentation
* [The Smartcar platform](http://plat.is/smartcar)
* [Wiki](../../wiki)

### License
GPLv3
