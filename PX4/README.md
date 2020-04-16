# PX4 Firmware

A custom PX4 Firmware stack to read the incomming orientation data and publish it to the flight controller's internal messaging system.

## Contributors

Joe DeFrance (defra022@umn.edu)

Nathan Raab (raabx028@umn.edu)

## Getting Started

This project was created using the [Visual Studio Code IDE](https://code.visualstudio.com/) to edit the existing [PX4 Firmware stack](https://github.com/PX4/Firmware) 

This project was tested using [QGroundControl](https://docs.qgroundcontrol.com/en/getting_started/download_and_install.html) and the [HobbyKing HKPilot32](https://docs.px4.io/v1.9.0/en/flight_controller/HKPilot32.html) flight controller.

### Installing

1. Download [Visual Studio Code](https://code.visualstudio.com/).

2. Download [QGroundControl](https://docs.qgroundcontrol.com/en/getting_started/download_and_install.html).

### Compiling

1. Open the `Firmware` folder in the Visual Studio Code IDE.

2. Click on the `TERMINAL` window tab and type `make px4_fmu-v3_default`.

   * Alternatively, click `CMake: ...` on the bottom toolbar, select `px4_fmu-v3_default` from the menu, and then click `Build` on the bottom toolbar.

Read the [PX4 Developer's Guide](https://dev.px4.io/v1.9.0/en/setup/building_px4.html) section `"Building"` for more information.

### Uploading

1. Connect the PX4 device to a computer using a data capable Mirco-B USB cable.

2. Click on the `TERMINAL` window tab and type `make px4_fmu-v3_default upload`.

3. Wait for the upload to complete.

Read the [PX4 Developer's Guide](https://dev.px4.io/v1.9.0/en/setup/building_px4.html) section `"Uploading Firmware"` for more information.

### Testing

1. Connect the PX4 device and a winglet device to the same CAN bus.

2. Power up the PX4 device and winglet device.

3. Open QGroundControl and navigate to the [MAVLink Console](https://docs.qgroundcontrol.com/en/analyze_view/mavlink_console.html).

4. Enter `uavcan status` in the MAVLink Console and read the response.

    * A `Sensor 'winglet': ...` section should be printed somewhere in the response, and channel 0 should show the node id and class instance of the connected winglet device. If it shows empty, check the CAN bus connection and try again.
  
    * An `Online nodes: ...` section should be printed near the end of the response, and the node id of the connected winglet device should be printed alongside health and mode information. If it is not there, check the CAN bus connection and try again.

5. Enter `winglet` in the MAVLink Console and read the response.

    * A series of prints should occur with uORB `sensor_winglet` topic and channel information, ending with angle computations based on the data received (only valid when two or more winglet devices are connected).
    
## Overview

### Sensor Winglet Message

A custom, multi-instance `sensor_winglet` uORB topic was created containing fields for a sample timestamp, device id, and up to four floats of rotational data.

#### sensor_winglet

  ```
  uint64 timestamp
  uint8 id
  float32 x
  float32 y
  float32 z
  float32 w
  ```
    
These fields are populated from UAVCAN v0 AngularCommand broadcasts received by the PX4 device from the connected winglet devices.

#### AngularCommand

  ```
  uint8 gimbal_id
  Mode mode
  float16[4] quaternion_xyzw
  ```

### Uavcan Winglet Bridge

The PX4 Firmware stack provides a parent `UavcanCDevSensorBridgeBase` class to allow implemented child classes to interface with the main `IUavcanSensorBridge` sensor bridge and subscribe to incomming UAVCAN messages.

This project implements a child of the parent `UavcanCDevSensorBridgeBase` class called `UavcanWingletBridge` that subscribes to UAVCAN_v0 AngularCommand broadcasts, processes the incomming data, and forwards it to a multi-instance uORB publication handler.

### PX4Winglet

The `PublicationMulti` class of uORB allows for there to exist multiple instances of a singular topic. Combining this with the provided `CDev` class allows for different sensors of the same type to publish their data to separate channels of the same topic.

This project implements a child of the `CDev` class called `PX4Winglet` that takes the forwarded data from the `UavcanWingletBridge`, assigns it a channel, and publishes a `sensor_winglet` message to that channel.

### Winglet Example Module

An example module was written to subscribe to the custom `sensor_winglet` uORB topic and extract useable data from its channels.

By entering `winglet` in the MAVLink Console, a series of polls to the multiple `sensor_winglet` topic channels are conducted and the contents are printed. The angles between adjacent winglets are computed and printed as well.

The method used to subscribe to all four winglet channels is shown below:

```c
int sensor_winglet_fd[4];

px4_pollfd_struct_t winglets[4];

for(int n = 0; n < 4; n++)
{
  sensor_winglet_fd[n] = orb_subscribe_multi(ORB_ID(sensor_winglet), n);
  winglets[n] = {.fd = sensor_winglet_fd[n], .events = POLLIN};
}
```
