#ifndef UAVCAN_H
#define UAVCAN_H

// UAVCAN structure implementation in C
#include <libcanard_v1.h>

// DSDL primative data type sizes
#include "uavcan_dsdl_sizes_v1.h"

// DSDL data types as primatives and structs
#include "uavcan_data_types_v1.h"

// Basic UAVCAN node struct
#include "uavcan_node_struct_v1.h"

// Functions to encode UAVCAN messages from data type structs
#include "uavcan_encoding_v1.h"

// Functions to decode UAVCAN messages into data type structs
#include "uavcan_decoding_v1.h"

#endif // UAVCAN_H
