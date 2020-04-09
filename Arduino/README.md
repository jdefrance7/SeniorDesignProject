# Arduino Project Sketches

Multiple sub-projects were created to test the functionality of various portions of this project. Brief descriptions of the main project and supporting sketches are included below.

Please note that the `libraries` folder is not itself a project sketch but rather contains the source code for libraries used throughout the project. Read the `libraries` documentation to learn more.

---

### Wing Segment v0 and Wing Segment v1 (Main Project)

Wing segment node implementation using [UAVCAN v0](https://legacy.uavcan.org/) and [UAVCAN v1](https://uavcan.org/).

---

### Callibration and Callibration Test

Callibrates the magnetometers of the [LSM9DS1](https://www.adafruit.com/product/3387) and [NXP_FXOS_FXAS](https://www.adafruit.com/product/3463) sensors and then tests this callibration.

Code courtesy of [adafruit/Adafruit_AHRS/examples](https://github.com/adafruit/Adafruit_AHRS).

---

### IMU Test

Tests data aquisition for different IMU data types using the [BNO055](https://www.adafruit.com/product/2472), [LSM9DS1](https://www.adafruit.com/product/3387), or [NXP_FXOS_FXAS](https://www.adafruit.com/product/3463).

---

### Power Test

Tests the power draw of up to four different power lines using the [Adafruit_INA219](https://www.adafruit.com/product/904).

---

### CAN Transmitter and Receiver Test

Tests the transmitting and receiving operation of the [AST-CAN485](https://www.sparkfun.com/products/14483) CAN transceiver 

Code courtesy of [Atlantis-Specialist-Technologies/AST_CAN_Arduino_Library/examples](https://github.com/Atlantis-Specialist-Technologies/AST_CAN_Arduino_Library).

---

### UAVCAN Transmitter and Receiver Test

Tests the transmitting and receiving operation of the [AST-CAN485](https://www.sparkfun.com/products/14483) CAN transceiver and the encoding and decoding of [UAVCAN v0](https://legacy.uavcan.org/) messages.
