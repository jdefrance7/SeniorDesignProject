# Senior Design Project
Electrical Engineering Senior Design Project at the University of Minnesota - Twin Cities, 2020

The purpose of this project is to read IMU sensor data from individual wing segments and send it to the Pixhawk flight controller over CAN for the purpose of obtaining wing segment orientation data to be used for control calculations and diagnotstics.

## Getting Started
This project was created using the Arduino IDE for the AST-CAN485 development board. 

[Arduino IDE](https://www.arduino.cc/en/main/software)

[AST-CAN485](https://www.sparkfun.com/products/14483)

### Dependencies
Multiple third-party libraries were used and adapted to fit this project's needs.

IMU Libraries
* [Adafruit_BNO055](https://github.com/adafruit/Adafruit_BNO055)
* [Adafruit_LSM9DS1](https://github.com/adafruit/Adafruit_LSM9DS1)
* [Adafruit_FXOS8700](https://github.com/adafruit/Adafruit_FXOS8700)
* [Adafruit_FXAS21002C](https://github.com/adafruit/Adafruit_FXAS21002C)
* [Adafruit_AHRS](https://github.com/adafruit/Adafruit_AHRS)
* [Adafruit_Sensor](https://github.com/adafruit/Adafruit_Sensor)

CAN Libraries
* [libcanard](https://github.com/UAVCAN/libcanard)

## Implementation
For our purposes, the `libcanard` library was split into three sub-libraries:

1. `canard` - contains the basic C implementation of UAVCAN

    `canard.h`

    `canard.c`

    `canard_internals.h`

2. `can` - contains the CAN driver for the AT90CAN128 microcontroller

    `can.h`

    `libcan.a`

    `can_config.h`

3. `canard_avr` - used to bridge 'canard' and 'can'

    `canard_avr.h`

    `canard_avr.c`

## Authors
Joe DeFrance, Alex Mootz, Andrew Schweyen, George Sahyoun, Luke Timper, Nathan Raab

## Acknowledgements
We would like to thank Professor Nikolaos Papanikolopoulos, Travis Hanson, and all of the team members in the Solar UAV lab at the University of Minnesota - Twin Cities for allowing us to work on this amazing project with them.

We would also like to acknowledge Professor Paul Imbertson for his time and effort as the Senior Design Faculty Mentor.
