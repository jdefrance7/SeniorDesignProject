# PX4 Firmware

Code was written for the PX4 firmware stack to read incomming UAVCAN AngularCommand messages from the wing segments and publish the data to a custom, mulit-instance uORB topic called `sensor_winglet`.

## Authors

Joe DeFrance (defra022@umn.edu)

## Getting Started

This project was created using [Visual Studio Code IDE](https://code.visualstudio.com/) for the [PX4 firmware stack](https://github.com/PX4/Firmware) and tested with [QGroundControl](https://docs.qgroundcontrol.com/en/getting_started/download_and_install.html).

### Installing

1. Download [Visual Studio Code](https://code.visualstudio.com/).

2. Download [QGroundControl](https://docs.qgroundcontrol.com/en/getting_started/download_and_install.html).

### Compiling

1. Open the `Firmware` folder in Visual Studio Code.

2. Wait for the project's extensions to load by reading the text in the `OUTPUT` window until `Build files have been written to: ...` appears.

3. Enter the `TERMINAL` window and type `make px4_fmu-v3_default`.

  * Alternatively click `CMake: ...` on the bottom toolbar and then select `px4_fmu-v3_default` from the menu.
  
  * Then click `Build` on the bottom toolbar.

### Uploading

1. Connect the PX4 device to a computer using a data capable Mirco-B USB cable.

2. Enter the `TERMINAL` window and type `make px4_fmu-v3_default upload`.

3. Wait for the upload to complete.

### Testing

1. Plug a winglet device into the CAN bus connected to the PX4 device.

2. Power up the PX4 device and winglet device.

3. Open QGroundControl and navigate to the [MAVLink Console](https://docs.qgroundcontrol.com/en/analyze_view/mavlink_console.html).

4. Enter `uavcan status` in the MAVLink Console and read the response.

    * A `Sensor 'winglet':` segment should be printed somewhere in the response with information about the sensor channels.
  
    * The node id of the connected winglet device should show up in `channel 0: node id ...`, if this says `empty` check the CAN bus connection and try again.
  
    * A `Online nodes:` section should be printed near the end of the response, the node id of the connected winglet device should show up with node health and mode information.
