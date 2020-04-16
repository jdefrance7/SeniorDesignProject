# UAVCAN (Version 0)

[UAVCAN v0](legacy.uavcan.org) data type encoding, decoding, and debugging (legacy).

## DSDL Sizes

Bit size of DSDL primatives.

Examples:

```
BOOL = 1
...
INT3 = 3
...
UINT8 = 8
...
FLOAT16 = 16
...
VOID2 = 2
...
```

## Data Types

Declaration of UAVCAN data types implemented as C structs and primatives.

Information regarding data type size, id, and signature is also included.

Constants defined in a data type's DSDL are included.

Reference: [UAVCAN Specification 7. List of Standard Data Types](https://legacy.uavcan.org/Specification/7._List_of_standard_data_types/)

## Encoding

Support functions to handle encoding UAVCAN data types into serialized bytes.

Example:

``
int32_t encode_node_status(
  uint8_t buffer[],
  uint32_t buffer_size,
  uint32_t bit_offset,
  NodeStatus node_status
);
``

## Decoding

Support functions to handle decoding serialized bytes to UAVCAN data types.

Example:

```
int32_t decode_node_status(
  CanardRxTransfer* transfer,
  uint32_t bit_offset,
  NodeStatus* node_status
);
```

## Node Structure

Custom struct for basic UAVCAN node information.

```
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

```
void printNodeStatus(NodeStatus* status);
```
