# Carolo Cup 2016 Arduino sketch
This is the sketch that runs on Team Pegasus' autonomous vehicle, taking part in the Carolo Cup 2016 competition in Germany.

## How to install
- Install the [CaroloCup2016 library](https://github.com/platisd/CaroloCup2016)
- Install the [Netstrings library](https://github.com/platisd/Netstrings)
- Open this sketch in the Arduino IDE (you can also find it in the library's examples after you have installed it correctly)
- Install the [Nano protobuff library](https://github.com/hspilkov/nanopb) compiled for this specific project
- Upload after making possible changes

## Netstrings VS Protobuff
If you want to transmit the sensor data over plain text, encoded as Netstrings, leave `#define DEBUG` uncommented. Alternatively, if you want to use the faster and more lightweight protobuffer (make sure you install the appropriate library first, optimized for your setup), comment out `#define DEBUG`.

## How to test components
You can find Arduino sketches to test various car components, in the [Testing](https://github.com/platisd/CaroloCup2016/tree/master/examples/testing) folder.
