#ifndef UAVCAN_FORMATTING_H
#define UAVCAN_FORMATTING_H

//##############################################################################
// Description /////////////////////////////////////////////////////////////////
//##############################################################################

// Provides functions to format data according to UAVCAN standards

//##############################################################################
// Includes ////////////////////////////////////////////////////////////////////
//##############################################################################

// Libcanard encoding functions
#include <canard.h>

// UAVCAN data type sizes
#include "uavcan_constants.h"

//##############################################################################
// CONSTANTS ///////////////////////////////////////////////////////////////////
//##############################################################################

#define AHRS_SOLUTION_MESSAGE_SIZE           (668 / 8)
#define AHRS_SOLUTION_DATA_TYPE_SIGNATURE    0x72a63a3c6f41fa9b
#define AHRS_SOLUTION_DATA_TYPE_ID           1000


#define CAMERA_GIMBAL_STATUS_MESSAGE_SIZE           (228 / 8)
#define CAMERA_GIMBAL_STATUS_DATA_TYPE_SIGNATURE    0xb9f127865be0d61e
#define CAMERA_GIMBAL_STATUS_DATA_TYPE_ID           1044

#define CAMERA_MODE_ANGULAR_VELOCITY          0
#define CAMERA_MODE_ORIENTATION_FIXED_FRAME   1
#define CAMERA_MODE_ORIENTATION_BODY_FRAME    2
#define CAMERA_MODE_GEO_POI                   4

#define CAMERA_ORIENTATION_SIZE               4

//------------------------------------------------------------------------------

#define COARSE_ORIENTATION_MESSAGE_SIZE             (16 / 8)
#define COARSE_ORIENTATION_DATA_TYPE_SIGNATURE      0x271ba10b0dac9e52
#define COARSE_ORIENTATION_DATA_TYPE_ID             DATA_TYPE_NA

#define COARSE_ORIENTATION_ANGLE_MULTIPLIER         4.7746482927568605

//------------------------------------------------------------------------------

#define DATA_TYPE_INFO_REQUEST_SIZE           (671 / 8)
#define DATA_TYPE_INFO_MESSAGE_SIZE           (743 / 8)
#define DATA_TYPE_INFO_DATA_TYPE_SIGNATURE    0x1b283338a7bed2d8
#define DATA_TYPE_INFO_DATA_TYPE_ID           2

#define DATA_TYPE_INFO_NAME_SIZE              80

#define DATA_TYPE_KIND_SERVICE                0
#define DATA_TYPE_KIND_MESSAGE                1

#define DATA_TYPE_FLAG_KNOWN                  1
#define DATA_TYPE_FLAG_SUBSCRIBED             2
#define DATA_TYPE_FLAG_PUBLISHING             4
#define DATA_TYPE_FLAG_SERVING                8

//------------------------------------------------------------------------------

#define DEBUG_KEY_VALUE_MESSAGE_SIZE            (502 / 8)
#define DEBUG_KEY_VALUE_DATA_TYPE_SIGNATURE     0xe02f25d6e0c98ae0
#define DEBUG_KEY_VALUE_DATA_TYPE_ID            16370

#define DEBUG_KEY_MAX_SIZE  58

//------------------------------------------------------------------------------

#define DEBUG_LOG_MESSAGE_MESSAGE_SIZE          (983 / 8)
#define DEBUG_LOG_MESSAGE_DATA_TYPE_SIGNATURE   0xd654a48e0c049d75
#define DEBUG_LOG_MESSAGE_DATA_TYPE_ID          16383

#define LOG_LEVEL_DEBUG     0
#define LOG_LEVEL_INFO      1
#define LOG_LEVEL_WARNING   2
#define LOG_LEVEL_ERROR     3

#define DEBUG_SOURCE_SIZE   31
#define DEBUG_TEXT_SIZE     90

//------------------------------------------------------------------------------

#define DEVICE_TEMPERATURE_MESSAGE_SIZE           (40 / 8)
#define DEVICE_TEMPERATURE_DATA_TYPE_SIGNATURE    0x70261c28a94144c6
#define DEVICE_TEMPERATURE_DATA_TYPE_ID           1110

#define ERROR_FLAG_NONE                           0
#define ERROR_FLAG_OVERHEATING                    1
#define ERROR_FLAG_OVERCOOLING                    2

//------------------------------------------------------------------------------

#define HARDWARE_VERSION_MESSAGE_SIZE             (2192 / 8)
#define HARDWARE_VERSION_DATA_TYPE_SIGNATURE      0x0ad5c4c933f4a0c4
#define HARDWARE_VERSION_DATA_TYPE_ID             DATA_TYPE_NA

#define HARDWARE_VERSION_UNIQUE_ID_SIZE                     16
#define HARDWARE_VERSION_CERTIFICATE_OF_AUTHENTICITY_SIZE   255

//------------------------------------------------------------------------------

#define MAGNETIC_FIELD_STRENGTH_MESSAGE_SIZE           (204 / 8)
#define MAGNETIC_FIELD_STRENGTH_DATA_TYPE_SIGNATURE    0xb6ac0c442430297e
#define MAGNETIC_FIELD_STRENGTH_DATA_TYPE_ID           1002

//------------------------------------------------------------------------------

#define NODE_INFO_REQUEST_SIZE            0
#define NODE_INFO_MESSAGE_SIZE            (3015 / 8)
#define NODE_INFO_DATA_TYPE_SIGNATURE     0xee468a8121c46a9e
#define NODE_INFO_DATA_TYPE_ID            1

//------------------------------------------------------------------------------

#define NODE_STATUS_MESSAGE_SIZE          (56 / 8)
#define NODE_STATUS_DATA_TYPE_SIGNATURE   0x0f0868d0c1a7c6f1
#define NODE_STATUS_DATA_TYPE_ID          341

#define NODE_HEALTH_OK                    0
#define NODE_HEALTH_WARNING               1
#define NODE_HEALTH_ERROR                 2
#define NODE_HEALTH_CRITICAL              3

#define NODE_MODE_OPERATIONAL             0
#define NODE_MODE_INITIALIZATION          1
#define NODE_MODE_MAINTENANCE             2
#define NODE_MODE_SOFTWARE_UPDATE         3
#define NODE_MODE_OFFLINE                 7

#define MAX_BROADCASTING_PERIOD_MS        1000
#define MIN_BROADCASTING_PERIOD_MS        2

#define OFFLINE_TIMEOUT_MS                3000

//------------------------------------------------------------------------------

#define RANGE_SENSOR_MEASUREMENT_MESSAGE_SIZE           (120 / 8)
#define RANGE_SENSOR_MEASUREMENT_DATA_TYPE_SIGNATURE    0x68fffe70fc771952
#define RANGE_SENSOR_MEASUREMENT_DATA_TYPE_ID           1050

#define SENSOR_TYPE_UNDEFINED       0
#define SENSOR_TYPE_SONAR           1
#define SENSOR_TYPE_LIDAR           2
#define SENSOR_TYPE_RADAR           3

#define READING_TYPE_UNDEFINED      0
#define READING_TYPE_VALID_RANGE    1
#define READING_TYPE_TOO_CLOSE      2
#define READING_TYPE_TOO_FAR        3

//------------------------------------------------------------------------------

#define NODE_TIMESTAMP_MESSAGE_SIZE           (56 / 8)
#define NODE_TIMESTAMP_DATA_TYPE_SIGNATURE    0x05bd0b5c81087e0d
#define NODE_TIMESTAMP_DATA_TYPE_ID           DATA_TYPE_NA

#define TIMESTAMP_UNKNOWN    0

//------------------------------------------------------------------------------

#define RESTART_NODE_REQUEST_SIZE           (40 / 8)
#define RESTART_NODE_MESSAGE_SIZE           1
#define RESTART_NODE_DATA_TYPE_SIGNATURE    0x569e05394a3017f0
#define RESTART_NODE_DATA_TYPE_ID           5

#define MAGIC_NUMBER          0xACCE551B1E

//------------------------------------------------------------------------------

#define SOFTWARE_VERSION_MESSAGE_SIZE             (120 / 8)
#define SOFTWARE_VERSION_DATA_TYPE_SIGNATURE      0xdd46fd376527fea1
#define SOFTWARE_VERSION_DATA_TYPE_ID             DATA_TYPE_NA

//##############################################################################
// AHRS SOLUTION ///////////////////////////////////////////////////////////////
//##############################################################################

#ifdef __cplusplus
extern "C" {
#endif

int8_t ahrs_solution(
  uint8_t buffer[],
  uint16_t offset,
  uint8_t timestamp[],
  float16_t orientation_x,
  float16_t orientation_y,
  float16_t orientation_z,
  float16_t orientation_w,
  float16_t angular_velocity_x,
  float16_t angular_velocity_y,
  float16_t angular_velocity_z,
  float16_t linear_acceleration_x,
  float16_t linear_acceleration_y,
  float16_t linear_acceleration_z
);

#ifdef __cplusplus
}
#endif

//##############################################################################
// CAMERA GIMBAL STATUS ////////////////////////////////////////////////////////
//##############################################################################

#ifdef __cplusplus
extern "C" {
#endif

int8_t camera_gimbal_status(
  uint8_t buffer[],
  uint16_t offset,
  uint8_t gimbal_id,
  uint8_t camera_mode,
  float16_t camera_orientation_x,
  float16_t camera_orientation_y,
  float16_t camera_orientation_z,
  float16_t camera_orientation_w
);

#ifdef __cplusplus
}
#endif

//##############################################################################
// COARSE ORIENTATION //////////////////////////////////////////////////////////
//##############################################################################

#ifdef __cplusplus
extern "C" {
#endif

int8_t coarse_orientation(
  uint8_t buffer[],
  uint8_t offset,
  int8_t roll,
  int8_t pitch,
  int8_t yaw,
  bool orientation_defined
);

#ifdef __cplusplus
}
#endif

//##############################################################################
// DATA TYPE INFO //////////////////////////////////////////////////////////////
//##############################################################################

#ifdef __cplusplus
extern "C" {
#endif

int8_t data_type_info(
  uint8_t buffer[],
  uint16_t offset,
  uint64_t signature,
  uint16_t id,
  uint8_t kind,
  uint8_t flags,
  uint8_t name[]
);

#ifdef __cplusplus
}
#endif

//##############################################################################
// DEBUG KEY VALUE /////////////////////////////////////////////////////////////
//##############################################################################

#ifdef __cplusplus
extern "C" {
#endif

int8_t debug_key_value(
  uint8_t buffer[],
  uint16_t offset,
  float32_t value,
  uint8_t key[]
);

#ifdef __cplusplus
}
#endif

//##############################################################################
// DEBUG LOG MESSAGE ///////////////////////////////////////////////////////////
//##############################################################################

#ifdef __cplusplus
extern "C" {
#endif

int8_t debug_log_message(
  uint8_t buffer[],
  uint16_t offset,
  uint8_t level,
  uint8_t source[],
  uint8_t text[]
);

#ifdef __cplusplus
}
#endif

//##############################################################################
// DEVICE TEMPERATURE //////////////////////////////////////////////////////////
//##############################################################################

#ifdef __cplusplus
extern "C" {
#endif

int8_t device_temperature(
  uint8_t buffer[],
  uint16_t offset,
  uint16_t device_id,
  float16_t temperature,
  uint8_t error_flags
);

#ifdef __cplusplus
}
#endif

//##############################################################################
// HARDWARE VERSION ////////////////////////////////////////////////////////////
//##############################################################################

#ifdef __cplusplus
extern "C" {
#endif

int8_t hardware_version(
  uint8_t buffer[],
  uint16_t offset,
  uint8_t major,
  uint8_t minor,
  uint8_t unique_id[],
  uint8_t certificate_of_authenticity[]
);

#ifdef __cplusplus
}
#endif

//##############################################################################
// MAGNETIC FIELD STRENGTH /////////////////////////////////////////////////////
//##############################################################################

#ifdef __cplusplus
extern "C" {
#endif

int8_t magnetic_field_strength(
  uint8_t buffer[],
  uint16_t offset,
  uint8_t sensor_id,
  float16_t magnetic_field_x,
  float16_t magnetic_field_y,
  float16_t magnetic_field_z
);

#ifdef __cplusplus
}
#endif

//##############################################################################
// NODE INFO ///////////////////////////////////////////////////////////////////
//##############################################################################

#ifdef __cplusplus
extern "C" {
#endif

int8_t node_info(
  uint8_t buffer[],
  uint16_t offset,
  uint8_t status[NODE_STATUS_MESSAGE_SIZE],
  uint8_t software_version[SOFTWARE_VERSION_MESSAGE_SIZE],
  uint8_t hardware_version[HARDWARE_VERSION_MESSAGE_SIZE],
  uint8_t name[]
);

#ifdef __cplusplus
}
#endif

//##############################################################################
// NODE STATUS /////////////////////////////////////////////////////////////////
//##############################################################################

#ifdef __cplusplus
extern "C" {
#endif

int8_t node_status(
  uint8_t buffer[],
  uint16_t offset,
  uint32_t uptime_sec,
  uint8_t health,
  uint8_t mode
);

#ifdef __cplusplus
}
#endif

//##############################################################################
// NODE TIMESTAMP //////////////////////////////////////////////////////////////
//##############################################################################

#ifdef __cplusplus
extern "C" {
#endif

int8_t node_timestamp(
  uint8_t buffer[],
  uint8_t offset,
  uint64_t usec
);

#ifdef __cplusplus
}
#endif

//##############################################################################
// RANGE SENSOR MEASUREMENT ////////////////////////////////////////////////////
//##############################################################################

#ifdef __cplusplus
extern "C" {
#endif

int8_t range_sensor_measurement(
  uint8_t buffer[],
  uint16_t offset,
  uint8_t usec[NODE_TIMESTAMP_MESSAGE_SIZE],
  uint8_t sensor_id,
  uint8_t coarse_orientation[COARSE_ORIENTATION_MESSAGE_SIZE],
  float16_t field_of_view,
  uint8_t sensor_type,
  float16_t range
);

#ifdef __cplusplus
}
#endif

//##############################################################################
// RESTART NODE ////////////////////////////////////////////////////////////////
//##############################################################################

#ifdef __cplusplus
extern "C" {
#endif

int8_t restart_node(
  uint8_t buffer[],
  uint16_t offset,
  bool ok
);

#ifdef __cplusplus
}
#endif

//##############################################################################
// SOFTWARE VERSION ////////////////////////////////////////////////////////////
//##############################################################################

#ifdef __cplusplus
extern "C" {
#endif

int8_t software_version(
  uint8_t buffer[],
  uint16_t offset,
  uint8_t major,
  uint8_t minor,
  uint8_t optional_field_flags,
  uint32_t vcs_commit,
  uint64_t image_crc
);

#ifdef __cplusplus
}
#endif

//##############################################################################
// END OF FILE /////////////////////////////////////////////////////////////////
//##############################################################################

#endif // UAVCAN_FORMATTING_H
