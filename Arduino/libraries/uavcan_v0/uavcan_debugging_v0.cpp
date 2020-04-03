#include "uavcan_debugging_v0.h"

void printNodeStatus(NodeStatus status)
{
  Serial.println("Node Status");
  Serial.print("  Uptime Sec: "); Serial.println((unsigned long)status.uptime_sec);
  Serial.print("  Health: ");     Serial.println((byte)status.health);
  Serial.print("  Mode: ");       Serial.println((byte)status.mode);
  Serial.print("  Sub Mode: ");   Serial.println((byte)status.sub_mode);
  Serial.print("  Vendor: ");     Serial.println((unsigned int)status.vendor_specific_status_code);
}

void printHardwareVersion(HardwareVersion hardware)
{
  Serial.println("Hardware Version");
  Serial.print("  Major: ");        Serial.println((byte)hardware.major);
  Serial.print("  Minor: ");        Serial.println((byte)hardware.minor);
  Serial.print("  Unique ID: ");
  for(int n = 0; n < HARDWARE_VERSION_UNIQUE_ID_SIZE; n++)
  {
    Serial.print((byte)hardware.unique_id[n], HEX);
  }
  Serial.print("\n");
  Serial.print("  Certificate: ");
  for(int n = 0; n < HARDWARE_VERSION_CERTIFICATE_SIZE; n++)
  {
    Serial.print((unsigned char)hardware.certificate[n]);
    if((unsigned char)hardware.certificate[n] == '\0')
    {
      break;
    }
  }
  Serial.print("\n");
}

void printSoftwareVersion(SoftwareVersion software)
{
  Serial.println("Software Version");
  Serial.print("  Major: ");  Serial.println((byte)software.major);
  Serial.print("  Minor: ");  Serial.println((byte)software.minor);
  Serial.print("  Flags: ");  Serial.println((byte)software.optional_field_flags);
  Serial.print("  VCS: ");    Serial.println((unsigned int)software.vcs_commit);
  Serial.print("  CRC: ");    Serial.println((unsigned long)software.image_crc);
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
  Serial.println((unsigned long)timestamp);
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
  Serial.print("  Gimbal ID: ");      Serial.println((byte)status.gimbal_id);
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
  Serial.print("  Device ID: ");    Serial.println((unsigned int)temp.device_id);
  Serial.print("  Temperature: ");  Serial.println(temp.temperature);
  Serial.print("  Error Flags: ");  Serial.println((byte)temp.error_flags);
}

void printRangeSensorMeasurement(RangeSensorMeasurement measurement)
{
  Serial.println("Range Sensor Measurement");
  Serial.print("  Timestamp: ");    printTimestamp(measurement.timestamp);
  Serial.print("  Sensor ID: ");    Serial.println((byte)measurement.sensor_id);
  Serial.print("  Beam ");          printCoarseOrientation(measurement.beam_orientation_in_body_frame);
  Serial.print("  FOV: ");          Serial.println(measurement.field_of_view);
  Serial.print("  Sensor Type: ");  Serial.println((byte)measurement.sensor_type);
  Serial.print("  Reading Type: "); Serial.println((byte)measurement.reading_type);
  Serial.print("  Range: ");        Serial.println(measurement.range);
}

void printGetInfo(GetNodeInfo info)
{
  Serial.println("Get Node Info");
  Serial.print("  ");       printNodeStatus(info.status);
  Serial.print("  ");       printSoftwareVersion(info.software_version);
  Serial.print("  ");       printHardwareVersion(info.hardware_version);
  Serial.print("  Name: ");
  for(int n = 0; n < GET_NODE_INFO_NAME_SIZE; n++)
  {
    Serial.print((unsigned char)info.name[n]);
    if((unsigned char)info.name[n] == '\0')
    {
      break;
    }
  }
  Serial.print("\n");
}

void printDataTypeKind(DataTypeKind kind)
{
  Serial.println((byte)kind);
}

void printGetDataTypeInfo(GetDataTypeInfo info)
{
  Serial.println("Get Data Type Info");
  Serial.print("  Signature: ");  Serial.println((unsigned long)info.signature);
  Serial.print("  ID: ");         Serial.println((unsigned int)info.id);
  Serial.print("  Kind: ");       printDataTypeKind(info.kind);
  Serial.print("  Flags: ");      Serial.println((byte)info.flags);
  Serial.print("  Name: ");
  for(int n = 0; n < GET_DATA_TYPE_INFO_NAME_SIZE; n++)
  {
    Serial.print((unsigned char)info.name[n]);
    if((unsigned char)info.name[n] == '\0')
    {
      break;
    }
  }
}

void printRestartNode(RestartNode restart)
{
  Serial.println((bool)restart);
}

void printKeyValue(KeyValue items)
{
  Serial.println("Key Value");
  Serial.print("  Value: ");  Serial.println(items.value);
  Serial.print("  Key: ");
  for(int n = 0; n < KEY_VALUE_KEY_SIZE; n++)
  {
    Serial.print(items.key[n], HEX);
  }
}

void printLogLevel(LogLevel level)
{
  Serial.println((byte)level);
}

void printLogMessage(LogMessage msg)
{
  Serial.println("Log Message");
  Serial.print("  Level: ");  Serial.println(msg.level);
  Serial.print("  Source: ");
  for(int n = 0; n < LOG_MESSAGE_SOURCE_SIZE; n++)
  {
    Serial.print((unsigned char)msg.source[n]);
    if((unsigned char)msg.source[n] == '\0')
    {
      break;
    }
  }
  Serial.print("  Text: ");
  for(int n = 0; n < LOG_MESSAGE_TEXT_SIZE; n++)
  {
    Serial.print((unsigned char)msg.text[n]);
    if((unsigned char)msg.text[n] == '\0')
    {
      break;
    }
  }
}

void printNode(UavcanNode node)
{
    Serial.println("Node");
    Serial.print("  ID: ");   Serial.println((byte)node.id);
    Serial.print("  Name: ");
    for(int n = 0; n < UAVCAN_NODE_NAME_SIZE; n++)
    {
      Serial.print((unsigned char)node.name[n]);
      if((unsigned char)node.name[n] == '\0')
      {
        break;
      }
    }
    Serial.print("  ");       printNodeStatus(node.status);
    Serial.print("  ");       printSoftwareVersion(node.software);
    Serial.print("  ");       printHardwareVersion(node.hardware);
}