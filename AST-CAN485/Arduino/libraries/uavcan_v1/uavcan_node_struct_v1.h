/**
 * @file uavcan_node_struct_v1.h
 *
 * Custom struct for basic UAVCAN node information.
 *
 * @author Joe DeFrance
 */

#ifndef UAVCAN_NODE_STRUCT
#define UAVCAN_NODE_STRUCT

//------------------------------------------------------------------------------

#include <stdint.h>

#include "uavcan_data_types_v1.h"

#include "libcanard_v1.h"

//------------------------------------------------------------------------------

#define UAVCAN_NODE_NAME_SIZE 80

typedef struct
{
  CanardNodeID id;
  Heartbeat status;
  GetInfo info;
} UavcanNode;

//------------------------------------------------------------------------------

#endif // UAVCAN_NODE_STRUCT
