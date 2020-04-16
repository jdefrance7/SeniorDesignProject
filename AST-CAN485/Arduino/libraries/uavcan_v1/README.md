# UAVCAN (Version 1)

[UAVCAN v1](https://uavcan.org/) data type encoding, decoding, and debugging.

## DSDL Sizes

Bit size of DSDL primatives.

Example:

```c
#define BOOL        1
//...
#define INT5        5
//...
#define UINT8       8
//...
#define FLOAT16    16
//...
#define VOID7       7
//...
```

## Data Types

Declaration of UAVCAN data types implemented as C structs and primatives.

Information regarding data type size, id, and signature is also included.

Constants defined in a data type's DSDL are included.

Example:

```c
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
```

Reference: [UAVCAN Specification 6. List of Standard Data Types](https://uavcan.org/specification/UAVCAN_Specification_v1.0-alpha.pdf)

## Encoding

Support functions to handle encoding UAVCAN data types into serialized bytes.

Example:

```c
int32_t encode_heartbeat(
  uint8_t buffer[],
  uint32_t buffer_size,
  uint32_t bit_offset,
  Heartbeat heartbeat
);
```

## Decoding

Support functions to handle decoding serialized bytes to UAVCAN data types.

Example:

```c
int32_t decode_heartbeat(
  uint8_t buffer[],
  uint32_t buffer_size,
  uint32_t bit_offset,
  Heartbeat* heartbeat
);
```

## Node Structure

Custom struct for basic UAVCAN node information.

```c
typedef struct
{
  CanardNodeID id;
  Heartbeat status;
  GetInfo info;
} UavcanNode;
```

## Debugging 

Formatted Serial print statements for all data types and structures.

```c
void printHeartbeat(Heartbeat* heartbeat);
void printSynchronizedTimestamp(SynchronizedTimestamp* timestamp);
void printQuaternion(Quaternion* q);
void printID(ID* id);
void printVersion(Version* version);
void printGetInfo(GetInfo* info);
void printSeverity(Severity* severity);
void printRecord(Record* record);
void printNode(UavcanNode* node);
```
