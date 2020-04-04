# Senior Design Project
Electrical Engineering Senior Design Project at the University of Minnesota - Twin Cities, Spring 2020

The purpose of this project is to read IMU sensor data from individual UAV wing segments and send it to a Pixhawk PX4 flight controller over CAN for the purpose of obtaining real-time wing segment orientation.

## Authors
Primary: Joe DeFrance
Secondary: Alex Mootz, Andrew Schweyen, George Sahyoun, Luke Timper, Nathan Raab

## Getting Started
This project was created using the [Arduino IDE](https://www.arduino.cc/en/main/software) for the [AST-CAN485](https://www.sparkfun.com/products/14483) board. 

### Installing

1. Download the Arduino IDE.
2. Replace the created `Arduino` folder with the one from this repository.

   * Alternatively copy the `Arduino\libraries` folder contents to your local `Arduino\libraries` and move desired project sketch folders to the `Arduino` folder.
3. Use the Arduino IDE Boards Manager to add the AST-CAN485 board to the Arduino IDE.

   * Follow the steps in the [AST-CAN485 Hookup Guide](https://learn.sparkfun.com/tutorials/ast-can485-hookup-guide?_ga=2.6567157.631132834.1583434944-271346267.1574810854) section 'Board Installation Using the Boards Manager'.

### Compiling

If you do not have the AVR-GCC compiler installed, please follow this link on how to install it: [AVR-GCC download](https://github.com/osx-cross/homebrew-avr)

1. Open the desired project sketch in the Arduino IDE.
2. Select the AST-CAN485 board from the `Tools->Board` menu in the Arduino IDE toolbar.
3. Either click the check mark button in the upper left hand corner of the Arduino IDE or select `Sketch->Verify/Compile` from the Arduino IDE toolbar.

### Uploading

1. Connect the AST-CAN485 to a computer through a [FTDI Adaptor](https://www.sparkfun.com/products/9716?_ga=2.13897968.631132834.1583434944-271346267.1574810854).
2. Select the proper port from the `Tools->Port` menu in the Arduino IDE toolbar.
3. Select "Parallel Programmer" from the `Tools->Programmer` menu in the Arduino IDE toolbar.
4. Select `Sketch->Upload Using Programmer` from the Arduino IDE toolbar.
5. Wait for the upload to complete and read diagnostic info in the Arduino IDE console.

## Notes

### Dependencies
Multiple third-party libraries were used and adapted to fit this project's needs. They are already included in the `Arduino\libraries` folder, though their references are included below.

IMU Libraries: [Adafruit_BNO055](https://github.com/adafruit/Adafruit_BNO055), [Adafruit_LSM9DS1](https://github.com/adafruit/Adafruit_LSM9DS1), [Adafruit_FXOS8700](https://github.com/adafruit/Adafruit_FXOS8700), [Adafruit_FXAS21002C](https://github.com/adafruit/Adafruit_FXAS21002C), [Adafruit_AHRS](https://github.com/adafruit/Adafruit_AHRS), [Adafruit_Sensor](https://github.com/adafruit/Adafruit_Sensor)

Power Sensor Library: [Adafruit_INA219](https://github.com/adafruit/Adafruit_INA219)

CAN Libraries: [Universal Can Library](https://github.com/rennerm/avr-can-lib/tree/9c6bc9118de66d6edaf1b8539e2b9717ba26d123#universelle-can-blibiothek-avr-can-lib), [Canard AVR](https://github.com/UAVCAN/libcanard/tree/legacy-v0/drivers/avr), [ASTCanLib](https://github.com/Atlantis-Specialist-Technologies/AST_CAN_Arduino_Library/blob/master/src/ASTCanLib.h)

### UAVCAN
Both [UAVCAN v0](https://github.com/UAVCAN/libcanard) (legacy) and [UAVCAN v1](https://github.com/UAVCAN/libcanard/tree/legacy-v0) protocols have been implemented in separate projects. See their documentation on implementation specifics.

## Acknowledgements
We would like to thank Professor Nikolaos Papanikolopoulos, Travis Hanson, and all of the team members in the Solar UAV lab at the University of Minnesota - Twin Cities for allowing us to work on this amazing project with them.

We would also like to acknowledge Professor Paul Imbertson for his time and effort as the Senior Design Faculty Mentor.
