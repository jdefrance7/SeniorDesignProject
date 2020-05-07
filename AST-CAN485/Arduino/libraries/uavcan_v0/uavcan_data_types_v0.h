/**
 * @file uavcan_data_types_v0.h
 *
 * Declaration of UAVCAN data types implemented as C structs and primatives.
 *
 * Information regarding data type size, id, and signature is also included.
 *
 * Constants defined in a data type's DSDL are included.
 *
 * Reference: https://legacy.uavcan.org/Specification/7._List_of_standard_data_types/
 *
 * @author Joe DeFrance
 */

#ifndef UAVCAN_DATA_TYPES_H
#define UAVCAN_DATA_TYPES_H

//------------------------------------------------------------------------------

#include <stdint.h>
#include <stdbool.h>

//------------------------------------------------------------------------------

// Node Status

// Full name: uavcan.protocol.NodeStatus

#define NODE_STATUS_DATA_TYPE_SIZE 56
#define NODE_STATUS_DATA_TYPE_ID   341

#define NODE_STATUS_DATA_TYPE_SIGNATURE 0x0f0868d0c1a7c6f1

#define MAX_BROADCASTING_PERIOD_MS 1000
#define MIN_BROADCASTING_PERIOD_MS 2

#define OFFLINE_TIMEOUT_MS 3000

#define HEALTH_OK       0
#define HEALTH_WARNING  1
#define HEALTH_ERROR    2
#define HEALTH_CRITICAL 3

#define MODE_OPERATIONAL      0
#define MODE_INITIALIZATION   1
#define MODE_MAINTENENCE      2
#define MODE_SOFTWARE_UPDATE  3
#define MODE_OFFLINE          7

typedef struct
{
  uint32_t uptime_sec;
  uint8_t health;
  uint8_t mode;
  uint8_t sub_mode;
  uint16_t vendor_specific_status_code;
} NodeStatus;

//------------------------------------------------------------------------------

// Hardware Version

// Full name: uavcan.protocol.HardwareVersion

#define HARDWARE_VERSION_DATA_TYPE_SIZE 2192
#define HARDWARE_VERSION_DATA_TYPE_ID   -1

#define HARDWARE_VERSION_DATA_TYPE_SIGNATURE 0x0ad5c4c933f4a0c4

#define HARDWARE_VERSION_UNIQUE_ID_SIZE 16

#define HARDWARE_VERSION_CERTIFICATE_SIZE 255

typedef struct
{
  uint8_t major;
  uint8_t minor;
  uint8_t unique_id[HARDWARE_VERSION_UNIQUE_ID_SIZE];
  uint8_t certificate[HARDWARE_VERSION_CERTIFICATE_SIZE];
} HardwareVersion;

//------------------------------------------------------------------------------

// Software Version

// Full name: uavcan.protocol.SoftwareVersion

#define SOFTWARE_VERSION_DATA_TYPE_SIZE 120
#define SOFTWARE_VERSION_DATA_TYPE_ID   -1

#define SOFTWARE_VERSION_DATA_TYPE_SIGNATURE 0xdd46fd376527fea1

#define OPTIONAL_FIELD_FLAG_VCS_COMMIT  1
#define OPTIONAL_FIELD_FLAG_IMAGE_CRC   2

typedef struct
{
  uint8_t major;
  uint8_t minor;
  uint8_t optional_field_flags;
  uint32_t vcs_commit;
  uint64_t image_crc;
} SoftwareVersion;

//------------------------------------------------------------------------------

// Camera Gimbal Mode

// Full name: uavcan.equipment.camera_gimbal.Mode

#define CAMERA_GIMBAL_MODE_DATA_TYPE_SIZE 8
#define CAMERA_GIMBAL_MODE_DATA_TYPE_ID   -1

#define CAMERA_GIMBAL_MODE_DATA_TYPE_SIGNATURE 0x9108c7785aeb69c4

#define CAMERA_GIMBAL_MODE_ANGULAR_VELOCITY         0
#define CAMERA_GIMBAL_MODE_ORIENTATION_FIXED_FRAME  1
#define CAMERA_GIMBAL_MODE_ORIENTATION_BODY_FRAME   2
#define CAMERA_GIMBAL_MODE_GEO_POI                  3

typedef uint8_t CameraGimbalMode;

#define GIMBAL_MODE_ANGULAR_VELOCITY          0
#define GIMBAL_MODE_ORIENTATION_FIXED_FRAME   1
#define GIMBAL_MODE_ORIENTATION_BODY_FRAME    2
#define GIMBAL_MODE_GEO_POI                   3
typedef uint8_t Mode;

//------------------------------------------------------------------------------

// Angular Command

// Full name: uavcan.equipment.camera_gimbal.AngularCommand

#define ANGULAR_COMMAND_DATA_TYPE_SIZE      80
#define ANGULAR_COMMAND_DATA_TYPE_ID        1040
#define ANGULAR_COMMAND_DATA_TYPE_SIGNATURE 0x4af6e57b2b2be29c

typedef struct
{
  uint8_t gimbal_id;
  Mode mode;
  float quaternion_xyzw[4];
} AngularCommand;

//------------------------------------------------------------------------------

// Coarse Orientation

// Full Name: uavcan.CoarseOrientation

#define COARSE_ORIENTATION_DATA_TYPE_SIZE 16
#define COARSE_ORIENTATION_DATA_TYPE_ID   -1

#define COARSE_ORIENTATION_DATA_TYPE_SIGNATURE 0x271ba10b0dac9e52

#define ANGLE_MULTIPLIER 4.7746482927568605

typedef struct
{
  int8_t fixed_axis_roll_pitch_yaw[3];
  bool orientation_defined;
} CoarseOrientation;

//------------------------------------------------------------------------------

// Timestamp

// Full Name: uavcan.Timestamp

#define TIMESTAMP_DATA_TYPE_SIZE 56
#define TIMESTAMP_DATA_TYPE_ID   -1

#define TIMESTAMP_DATA_TYPE_SIGNATURE 0x05bd0b5c81087e0d

#define UNKNOWN 0

typedef uint64_t Timestamp;

//------------------------------------------------------------------------------

// AHRS Solution

// Full name: uavcan.equipment.ahrs.Solution

#define AHRS_SOLUTION_DATA_TYPE_SIZE 668
#define AHRS_SOLUTION_DATA_TYPE_ID   1000

#define AHRS_SOLUTION_DATA_TYPE_SIGNATURE 0x72a63a3c6f41fa9b

typedef struct
{
  Timestamp timestamp;
  float orientation_xyzw[4];
  float orientation_covariance[9];
  float angular_velocity[3];
  float angular_velocity_covariance[9];
  float linear_acceleration[3];
  float linear_acceleration_covariance[9];
} AhrsSolution;

//------------------------------------------------------------------------------

// Camera Gimbal Status

// Full name: uavcan.equipment.camera_gimbal.Status

#define CAMERA_GIMBAL_STATUS_DATA_TYPE_SIZE  228
#define CAMERA_GIMBAL_STATUS_DATA_TYPE_ID 1044

#define CAMERA_GIMBAL_STATUS_DATA_TYPE_SIGNATURE 0xb9f127865be0d61e

typedef struct
{
  uint8_t gimbal_id;
  CameraGimbalMode mode;
  float camera_orientation_in_body_frame_xyzw[4];
  float camera_orientation_in_body_frame_covariance[9];
} CameraGimbalStatus;

//------------------------------------------------------------------------------

// Device Temperature

// Full name: uavcan.equipment.device.Temperature

#define DEVICE_TEMPERATURE_DATA_TYPE_SIZE 40
#define DEVICE_TEMPERATURE_DATA_TYPE_ID 1110

#define DEVICE_TEMPERATURE_DATA_TYPE_SIGNATURE 0x70261c28a94144c6

#define ERROR_FLAG_OVERHEATING 1
#define ERROR_FLAG_OVERCOOLING 2

typedef struct
{
  uint16_t device_id;
  float temperature;
  uint8_t error_flags;
} DeviceTemperature;

//------------------------------------------------------------------------------

// Range Sensor Measurement

// Full name: uavcan.equipment.range_sensor.Measurement

#define RANGE_SENSOR_MEASUREMENT_DATA_TYPE_SIZE 120
#define RANGE_SENSOR_MEASUREMENT_DATA_TYPE_ID 1050

#define RANGE_SENSOR_MEASUREMENT_DATA_TYPE_SIGNATURE 0x68fffe70fc771952

#define SENSOR_TYPE_UNDEFINED 0
#define SENSOR_TYPE_SONAR     1
#define SENSOR_TYPE_LIDAR     2
#define SENSOR_TYPE_RADAR     3

#define READING_TYPE_UNDEFINED    0
#define READING_TYPE_VALID_RANGE  1
#define READING_TYPE_TOO_CLOSE    2
#define READING_TYPE_TOO_FAR      3

typedef struct
{
  Timestamp timestamp;
  uint8_t sensor_id;
  CoarseOrientation beam_orientation_in_body_frame;
  float field_of_view;
  uint8_t sensor_type;
  uint8_t reading_type;
  float range;
} RangeSensorMeasurement;

//------------------------------------------------------------------------------

// Get Node Info

// Full name: uavcan.protocol.GetNodeInfo

#define GET_NODE_INFO_DATA_TYPE_SIZE 3015
#define GET_NODE_INFO_DATA_TYPE_ID   1

#define GET_NODE_INFO_DATA_TYPE_SIGNATURE 0xee468a8121c46a9e

#define GET_NODE_INFO_NAME_SIZE 80

typedef struct
{
  NodeStatus status;
  SoftwareVersion software_version;
  HardwareVersion hardware_version;
  uint8_t name[GET_NODE_INFO_NAME_SIZE];
} GetNodeInfo;

//------------------------------------------------------------------------------

// Data Type Kind

// Full name: uavcan.protocol.DataTypeKind

#define DATA_TYPE_KIND_DATA_TYPE_SIZE 8
#define DATA_TYPE_KIND_DATA_TYPE_ID   -1

#define DATA_TYPE_KIND_DATA_TYPE_SIGNATURE 0x9420a73e008e5930

#define SERVICE 0
#define MESSAGE 1

typedef uint8_t DataTypeKind;

//------------------------------------------------------------------------------

// Get Data Type Info

// Full name: uavcan.protocol.GetDataTypeInfo

#define GET_DATA_TYPE_INFO_DATA_TYPE_SIZE 743
#define GET_DATA_TYPE_INFO_DATA_TYPE_ID   2

#define GET_DATA_TYPE_INFO_DATA_TYPE_SIGNATURE 0x1b283338a7bed2d8

#define GET_DATA_TYPE_INFO_NAME_SIZE 80

typedef struct
{
  uint64_t signature;
  uint16_t id;
  DataTypeKind kind;
  uint8_t flags;
  uint8_t name[GET_DATA_TYPE_INFO_NAME_SIZE];
} GetDataTypeInfo;

//------------------------------------------------------------------------------

// Restart Node

// Full name: uavcan.protocol.RestartNode

#define RESTART_NODE_DATA_TYPE_SIZE 1
#define RESTART_NODE_DATA_TYPE_ID   5

#define RESTART_NODE_DATA_TYPE_SIGNATURE 0x569e05394a3017f0

#define MAGIC_NUMBER 0xACCE551B1E

typedef bool RestartNode;

//------------------------------------------------------------------------------

// Key Value

// Full name: uavcan.protocol.debug.KeyValue

#define KEY_VALUE_DATA_TYPE_SIZE 502
#define KEY_VALUE_DATA_TYPE_ID   16370

#define KEY_VALUE_DATA_TYPE_SIGNATURE 0xe02f25d6e0c98ae0

#define KEY_VALUE_KEY_SIZE 58

typedef struct
{
  float value;
  uint8_t key[KEY_VALUE_KEY_SIZE];
} KeyValue;

//------------------------------------------------------------------------------

// Log Level

// Full name: uavcan.protocol.debug.LogLevel

#define LOG_LEVEL_DATA_TYPE_SIZE 3
#define LOG_LEVEL_DATA_TYPE_ID   -1

#define LOG_LEVEL_DATA_TYPE_SIGNATURE 0x711bf141af572346

#define LEVEL_DEBUG   0
#define LEVEL_INFO    1
#define LEVEL_WARNING 2
#define LEVEL_ERROR   3

typedef uint8_t LogLevel;

//------------------------------------------------------------------------------

// Log Message

// Full name: uavcan.protocol.debug.LogMessage

#define LOG_MESSAGE_DATA_TYPE_SIZE 983
#define LOG_MESSAGE_DATA_TYPE_ID   16383

#define LOG_MESSAGE_DATA_TYPE_SIGNATURE 0xd654a48e0c049d75

#define LOG_MESSAGE_SOURCE_SIZE 31

#define LOG_MESSAGE_TEXT_SIZE 90

typedef struct
{
  LogLevel level;
  uint8_t source[LOG_MESSAGE_SOURCE_SIZE];
  uint8_t text[LOG_MESSAGE_TEXT_SIZE];
} LogMessage;

//------------------------------------------------------------------------------

#endif // UAVCAN_DATA_TYPES_H
