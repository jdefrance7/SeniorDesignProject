# PX4 Firmware

Code was written for the PX4 Firmware to read incomming UAVCAN AngularCommand messages from the wing segments and publish the data to a custom, mulit-instance uORB topic called `sensor_winglet`.

## Authors

Joe DeFrance (defra022@umn.edu)

Nathan Raab (raabx028@umn.edu)

## Getting Started

This project was created using [Visual Studio Code IDE](https://code.visualstudio.com/) for the [PX4 Firmware](https://github.com/PX4/Firmware) and tested with [QGroundControl](https://docs.qgroundcontrol.com/en/getting_started/download_and_install.html).

### Installing

1. Download [Visual Studio Code](https://code.visualstudio.com/).

2. Download [QGroundControl](https://docs.qgroundcontrol.com/en/getting_started/download_and_install.html).

### Compiling

1. Open the `Firmware` folder in Visual Studio Code.

2. Wait for the project's extensions to load by reading the text in the `OUTPUT` window until `"Build files have been written to: ..."` appears.

3. Enter the `TERMINAL` window and type `make px4_fmu-v3_default`.

   * Alternatively click `CMake: ...` on the bottom toolbar and then select `px4_fmu-v3_default` from the menu.
  
   * Then click `Build` on the bottom toolbar.

Read the [PX4 Developer's Guide](https://dev.px4.io/v1.9.0/en/setup/building_px4.html) section `"Building"` for more information.

### Uploading

1. Connect the PX4 device to a computer using a data capable Mirco-B USB cable.

2. Enter the `TERMINAL` window and type `make px4_fmu-v3_default upload`.

3. Wait for the upload to complete.

Read the [PX4 Developer's Guide](https://dev.px4.io/v1.9.0/en/setup/building_px4.html) section `"Uploading Firmware"` for more information.

### Testing

1. Plug a winglet device into the CAN bus connected to the PX4 device.

2. Power up the PX4 device and winglet device.

3. Open QGroundControl and navigate to the [MAVLink Console](https://docs.qgroundcontrol.com/en/analyze_view/mavlink_console.html).

4. Enter `uavcan status` in the MAVLink Console and read the response.

    * A `Sensor 'winglet': ...` section should be printed somewhere in the response, and channel 0 should show the node id of the connected winglet device. If it shows empty instead, check the CAN bus connection and try again.
  
    * An `Online nodes: ...` section should be printed near the end of the response, and the node id of the connected winglet device should be printed alongside health and mode information. If no nodes show up, check the CAN bus connection and try again.

5. Enter `winglet` in the MAVLink Console and read the response.

    * A series of prints should occur with winglet channel statuses and uORB message information, ending with angle computations based on the data received (invalid with only one winglet device connected).
