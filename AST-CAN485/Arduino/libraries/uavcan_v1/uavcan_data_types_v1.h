/**
 * @file uavcan_data_types_v1.h
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

#ifndef UAVCAN_DATA_TYPES
#define UAVCAN_DATA_TYPES

//------------------------------------------------------------------------------

#include <stdint.h>
#include <stdbool.h>

//------------------------------------------------------------------------------

// Heartbeat

// Full name: uavcan.node.Heartbeat

#define HEARTBEAT_DATA_TYPE_ID    32085
#define HEARTBEAT_DATA_TYPE_SIZE  56

#define MAX_PUBLICATION_PERIOD  1
#define OFFLINE_TIMEOUT         3

#define HEALTH_NOMINAL  0
#define HEALTH_ADVISORY 1
#define HEALTH_CAUTION  2
#define HEALTH_WARNING  3

#define MODE_OPERATIONAL      0
#define MODE_INITIALIZATION   1
#define MODE_MAINTENENCE      2
#define MODE_SOFTWARE_UPDATE  3
#define MODE_OFFLINE          7

typedef struct
{
  uint32_t uptime;
  uint8_t health;
  uint8_t mode;
  uint32_t vendor_specific_status_code;
} Heartbeat;

//------------------------------------------------------------------------------

// SynchronizedTimestamp

// Full name: uavcan.time.SynchronizedTimestamp

#define SYNCHRONIZED_TIMESTAMP_DATA_TYPE_ID   -1
#define SYNCHRONIZED_TIMESTAMP_DATA_TYPE_SIZE  56

#define UNKNOWN 0

typedef uint64_t SynchronizedTimestamp;

//------------------------------------------------------------------------------

// Quaternion

// Full name: uavcan.si.unit.angle.Quaternion

#define QUATERNION_DATA_TYPE_ID   -1
#define QUATERNION_DATA_TYPE_SIZE 128

#define QUATERION_WXYZ_SIZE 4

#define QW_INDEX 0
#define QX_INDEX 1
#define QY_INDEX 2
#define QZ_INDEX 3

typedef struct
{
  float wxyz[QUATERION_WXYZ_SIZE];
} Quaternion;

//------------------------------------------------------------------------------

// ID

// Full name: uavcan.node.ID

#define ID_DATA_TYPE_ID   -1
#define ID_DATA_TYPE_SIZE 16

typedef uint16_t ID;

//------------------------------------------------------------------------------

// Version

// Full name: uavcan.node.Version

#define VERSION_DATA_TYPE_ID   -1
#define VERSION_DATA_TYPE_SIZE  16

typedef struct
{
  uint8_t major;
  uint8_t minor;
} Version;

//------------------------------------------------------------------------------

// GetInfo

// Full name: uavcan.node.GetInfo

#define GET_INFO_DATA_TYPE_ID   430
#define GET_INFO_DATA_TYPE_SIZE 2504

#define GET_INFO_UNIQUE_ID_SIZE 16

#define GET_INFO_NAME_SIZE 50

#define GET_INFO_CERTIFICATE_SIZE 222

typedef struct
{
  Version protocol_version;
  Version hardware_version;
  Version software_version;
  uint64_t software_vcs_revision_id;
  uint8_t unique_id[GET_INFO_UNIQUE_ID_SIZE];
  uint8_t name[GET_INFO_NAME_SIZE];
  uint64_t software_image_crc;
  uint8_t certificate[GET_INFO_CERTIFICATE_SIZE];
} GetInfo;

//------------------------------------------------------------------------------

// Severity

// Full name: : uavcan.diagnostic.Severity

#define SEVERITY_DATA_TYPE_ID   -1
#define SEVERITY_DATA_TYPE_SIZE 8

#define TRACE     0
#define DEBUG     1
#define INFO      2
#define NOTICE    3
#define WARNING   4
#define ERROR     5
#define CRITICAL  6
#define ALERT     7

typedef uint8_t Severity;

//------------------------------------------------------------------------------

// Record

// Full name: uavcan.diagnostic.Record

#define RECORD_DATA_TYPE_ID   32760
#define RECORD_DATA_TYPE_SIZE 968

#define RECORD_TEXT_SIZE 112

typedef struct
{
  SynchronizedTimestamp timestamp;
  Severity severity;
  uint8_t text[RECORD_TEXT_SIZE];
} Record;

//------------------------------------------------------------------------------

#endif // UAVCAN_DATA_TYPES
