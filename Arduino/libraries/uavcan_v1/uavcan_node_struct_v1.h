#ifndef UAVCAN_NODE_STRUCT
#define UAVCAN_NODE_STRUCT

#include <stdint.h>

#include "uavcan_data_types_v1.h"

#define UAVCAN_NODE_NAME_SIZE 80

typedef struct
{
  CanardNodeID id;  // uint8_t
  Heartbeat status;
  GetInfo info;
} UavcanNode;

#endif // UAVCAN_NODE_STRUCT
