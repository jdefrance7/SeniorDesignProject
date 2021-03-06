# Libraries

Multiple third-party libraries were implemented in various degrees throughout the projects. 

A number of custom libraries were written to interface with these third-party libraries and are described below.

Please read the documentation of each custom library to gain a better understanding of its implementation.

---

### LED

A simple LED class for hardware debugging.

---

### BNO055

Provides standardized IMU functions for interfacing with the [BNO055](https://www.adafruit.com/product/2472).

Examples: 
  * [imu_test.ino](https://github.com/jdefrance7/SeniorDesignProject/tree/master/AST-CAN485/Arduino/libraries/BNO055/examples/IMU_Test)
  
---

### LSM9DS1

Provides standardized IMU functions for interfacing with the [LSM9DS1](https://www.adafruit.com/product/3387).

Examples: 
  * [imu_test.ino](https://github.com/jdefrance7/SeniorDesignProject/tree/master/AST-CAN485/Arduino/libraries/LSM9DS1/examples/IMU_Test)

---

### NXP_FXOS_FXAS

Provides standardized IMU functions for interfacing with the [NXP_FXOS_FXAS](https://www.adafruit.com/product/3463).

Examples: 
  * [imu_test.ino](https://github.com/jdefrance7/SeniorDesignProject/tree/master/AST-CAN485/Arduino/libraries/NXP_FXOS_FXAS/examples/IMU_Test)

---

### AST_CAN_v0

CAN driver interface for the [AST-CAN485](https://www.sparkfun.com/products/14483) using [ASTCanLib](https://github.com/Atlantis-Specialist-Technologies/AST_CAN_Arduino_Library/blob/master/src/ASTCanLib.h) and [UAVCAN v0](https://legacy.uavcan.org/).

Examples: 
  * [uavcan_receiver_test_v0.ino](https://github.com/jdefrance7/SeniorDesignProject/tree/master/AST-CAN485/Arduino/libraries/ast_can_v0/examples/UAVCAN_Receiver_Test_v0)
  * [uavcan_transmitter_test_v0.ino](https://github.com/jdefrance7/SeniorDesignProject/tree/master/AST-CAN485/Arduino/libraries/ast_can_v0/examples/UAVCAN_Transmitter_Test_v0)

---

### AST_CAN_v1

CAN driver interface for the [AST-CAN485](https://www.sparkfun.com/products/14483) using [ASTCanLib](https://github.com/Atlantis-Specialist-Technologies/AST_CAN_Arduino_Library/blob/master/src/ASTCanLib.h) and [UAVCAN v1](https://uavcan.org/).

Examples: Unimplemented

---

### Libcanard_v0

[Libcanard v0](https://github.com/UAVCAN/libcanard/tree/legacy-v0) source files and Serial debugging print statements.

Examples: 
  * [libcanard_test.ino](https://github.com/jdefrance7/SeniorDesignProject/tree/master/AST-CAN485/Arduino/libraries/libcanard_v0/examples/licanard_test)

---

### Libcanard_v1

[Libcanard v1](https://github.com/UAVCAN/libcanard/tree/master) source files and Serial debugging print statements.

Examples: Unimplemented

---

### UAVCAN_v0

[UAVCAN v0](https://legacy.uavcan.org/) data type encoding, decoding, and debugging.

Examples: None

---

### UAVCAN_v1

[UAVCAN v1](https://uavcan.org/) data type encoding, decoding, and debugging.

Examples: Unimplemented
