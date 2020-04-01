#include "uavcan_debugging_v0.h"

void printNodeStatus(NodeStatus status)
{
  Serial.println("Node Status");
  Serial.print("  Uptime Sec: "); Serial.println(status.uptime_sec);
  Serial.print("  Health: ");     Serial.println(status.health);
  Serial.print("  Mode: ");       Serial.println(status.mode);
  Serial.print("  Sub Mode: ");   Serial.println(status.sub_mode);
  Serial.print("  Vendor: ");     Serial.println(status.vendor_specific_status_code);
}

void printHardwareVersion(HardwareVersion hardware)
{
  Serial.println("Hardware Version");
  Serial.print("  Major: ");        Serial.println(hardware.major);
  Serial.print("  Minor: ");        Serial.println(hardware.minor);
  Serial.print("  Unique ID: ");    Serial.println(hardware.unique_id);
  Serial.print("  Certificate: ");  Serial.println(hardware.certificate);
}

void printSoftwareVersion(SoftwareVersion software)
{
  Serial.println("Software Version");
  Serial.print("  Major: ");  Serial.println(software.major);
  Serial.print("  Minor: ");  Serial.println(software.minor);
  Serial.print("  Flags: ");  Serial.println(software.optional_field_flags);
  Serial.print("  VCS: ");    Serial.println(software.vcs_commit);
  Serial.print("  CRC: ");    Serial.println(software.image_crc);
}

void printCoarseOrientation(CoarseOrientation coarse)
{
  Serial.println("Coarse Orientation");
  Serial.print("  Roll: ");     Serial.println(coarse.fixed_axis_roll_pitch_yaw[0]);
  Serial.print("  Pitch: ");    Serial.println(coarse.fixed_axis_roll_pitch_yaw[1]);
  Serial.print("  Yaw: ");      Serial.println(coarse.fixed_axis_roll_pitch_yaw[2]);
  Serial.print("  Defined: ");  Serial.println(coarse.orientation_defined);
}

void printTimestamp(Timestamp timestamp)
{
  Serial.println((long)timestamp);
}

void printAhrsSolution(AhrsSolution solution)
{
  Serial.println("AHRS Solution");
  Serial.print("  Orientation X: ");          Serial.println(solution.orientation_xyzw[0]);
  Serial.print("  Orientation Y: ");          Serial.println(solution.orientation_xyzw[1]);
  Serial.print("  Orientation Z: ");          Serial.println(solution.orientation_xyzw[2]);
  Serial.print("  Orientation W: ");          Serial.println(solution.orientation_xyzw[3]);
  Serial.print("  Angular Velocity Roll: ");  Serial.println(solution.angular_velocity[0]);
  Serial.print("  Angular Velocity Pitch: "); Serial.println(solution.angular_velocity[1]);
  Serial.print("  Angular Velocity Yaw: ");   Serial.println(solution.angular_velocity[2]);
  Serial.print("  Linear Acceleration X: ");  Serial.println(solution.linear_acceleration[0]);
  Serial.print("  Linear Acceleration Y: ");  Serial.println(solution.linear_acceleration[1]);
  Serial.print("  Linear Acceleration Z: ");  Serial.println(solution.linear_acceleration[2]);
  // Note: skipping covariance matrices
}

void printCameraGimbalMode(CameraGimbalMode mode)
{
  Serial.println((byte)mode);
}

void printCameraGimbalStatus(CameraGimbalStatus status)
{
  Serial.println("Camera Gimbal Status");
  Serial.print("  Gimbal ID: ");      Serial.println(status.gimbal_id);
  Serial.print("  Mode: ");           printCameraGimbalMode(status.mode);
  Serial.print("  Orientation X: ");  Serial.print(status.camera_orientation_in_body_frame_xyzw[0]);
  Serial.print("  Orientation Y: ");  Serial.print(status.camera_orientation_in_body_frame_xyzw[1]);
  Serial.print("  Orientation Z: ");  Serial.print(status.camera_orientation_in_body_frame_xyzw[2]);
  Serial.print("  Orientation W: ");  Serial.print(status.camera_orientation_in_body_frame_xyzw[3]);
  // Note: skipping covariance matrices
}

void printDeviceTemperature(DeviceTemperature temp)
{
  Serial.println("Device Temperature");
  Serial.print("  Device ID: ");    Serial.println(temp.device_id);
  Serial.print("  Temperature: ");  Serial.println(temp.temperature);
  Serial.print("  Error Flags: ");  Serial.println(temp.error_flags);
}

void printRangeSensorMeasurement(RangeSensorMeasurement measurement)
{
  Serial.println("Range Sensor Measurement");
  Serial.print("  Timestamp: ");    printTimestamp(measurement.timestamp);
  Serial.print("  Sensor ID: ");    Serial.println(measurement.sensor_id);
  Serial.print("  Beam ");          printCoarseOrientation(measurement.beam_orientation_in_body_frame);
  Serial.print("  FOV: ");          Serial.println(measurement.field_of_view);
  Serial.print("  Sensor Type: ");  Serial.println(measurement.sensor_type);
  Serial.print("  Reading Type: "); Serial.println(measurement.reading_type);
  Serial.print("  Range: ");        Serial.println(measurement.range);
}

void printGetInfo(GetNodeInfo info)
{
  Serial.println("Get Node Info");
  Serial.print("  ");       printNodeStatus(info.status);
  Serial.print("  ");       printSoftwareVersion(info.software_version);
  Serial.print("  ");       printHardwareVersion(info.hardware_version);
  Serial.print("  Name: "); Serial.println(info.name);
}

void printDataTypeKind(DataTypeKind kind)
{
  Serial.println((byte)DataTypeKind);
}

void printGetDataTypeInfo(GetDataTypeInfo info)
{
  Serial.println("Get Data Type Info");
  Serial.print("  Signature: ");  Serial.println(info.signature);
  Serial.print("  ID: ");         Serial.println(info.id);
  Serial.print("  Kind: ");       printDataTypeKind(info.kind);
  Serial.print("  Flags: ");      Serial.println(info.flags);
  Serial.print("  Name: ");       Serial.println(info.name);
}

void printRestartNode(RestartNode restart)
{
  Serial.println((bool)restart);
}

void printKeyValue(KeyValue items)
{
  Serial.println("Key Value");
  Serial.print("  Value: ");  Serial.println(items.value);
  Serial.print("  Key: ");    Serial.println(items.key);
}

void printLogLevel(LogLevel level)
{
  Serial.println((byte)level);
}

void printLogMessage(LogMessage msg)
{
  Serial.println("Log Message");
  Serial.print("  Level: ");  Serial.println(msg.level);
  Serial.print("  Source: "); Serial.println(msg.source);
  Serial.print("  Text: ");   Serial.println(msg.text);
}

void printNode(UavcanNode node)
{
    Serial.println("Node");
    Serial.print("  ID: ");   Serial.println(node.id);
    Serial.print("  Name: "); Serial.println(node.name);
    Serial.print("  ");       printNodeStatus(node.status);
    Serial.print("  ");       printSoftwareVersion(node.software);
    Serial.print("  ");       printHardwareVersion(node.hardware);
}
