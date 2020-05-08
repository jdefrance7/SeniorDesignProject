# UAVCAN (Version 0)

[UAVCAN v0](https://legacy.uavcan.org/) data type encoding, decoding, and debugging (legacy).

## DSDL Sizes

Bit size of DSDL primatives to make encoding/decoding more readable.

Examples:

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
```

Reference: [UAVCAN Specification 7. List of Standard Data Types](https://legacy.uavcan.org/Specification/7._List_of_standard_data_types/)

## Encoding

Support functions to handle encoding UAVCAN data types into serialized bytes.

Example:

```c
int32_t encode_node_status(
  uint8_t buffer[],
  uint32_t buffer_size,
  uint32_t bit_offset,
  NodeStatus node_status
);
```

## Decoding

Support functions to handle decoding serialized bytes to UAVCAN data types.

Example:

```c
int32_t decode_node_status(
  CanardRxTransfer* transfer,
  uint32_t bit_offset,
  NodeStatus* node_status
);
```

## Node Structure

Custom struct for basic UAVCAN node information.

```c
#define UAVCAN_NODE_NAME_SIZE 80

typedef struct
{
  uint8_t id;
  uint8_t name[UAVCAN_NODE_NAME_SIZE];
  NodeStatus status;
  SoftwareVersion software;
  HardwareVersion hardware;
} UavcanNode;
```

## Debugging

Formatted Serial print statements for all data types and structures.

Example:

```c
void printNodeStatus(NodeStatus* status);
```
