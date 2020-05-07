/**
 * @file uavcan_node_struct_v0.h
 *
 * Custom struct for basic UAVCAN node information.
 *
 * @author Joe DeFrance
 */

#ifndef UAVCAN_NODE_STRUCT_H
#define UAVCAN_NODE_STRUCT_H

//------------------------------------------------------------------------------

#include <stdint.h>

#include "uavcan_data_types_v0.h"

//------------------------------------------------------------------------------

#define UAVCAN_NODE_NAME_SIZE 80

typedef struct
{
  uint8_t id;
  uint8_t name[UAVCAN_NODE_NAME_SIZE];
  NodeStatus status;
  SoftwareVersion software;
  HardwareVersion hardware;
} UavcanNode;

//------------------------------------------------------------------------------

#endif // UAVCAN_NODE_STRUCT_H
