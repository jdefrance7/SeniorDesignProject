/**
 * @file uavcan_v0.h
 *
 * Header to include custom UAVCAN v0 support files.
 *
 * @author Joe DeFrance
 */

#ifndef UAVCAN_H
#define UAVCAN_H

//------------------------------------------------------------------------------

/*
  Support library for Libcanard v0 handling data type encoding/decoding.
*/

//------------------------------------------------------------------------------

// UAVCAN structure implementation in C
#include <libcanard_v0.h>

// DSDL primative data type sizes
#include "uavcan_dsdl_sizes_v0.h"

// DSDL data types as primatives and structs
#include "uavcan_data_types_v0.h"

// Basic UAVCAN node struct
#include "uavcan_node_struct_v0.h"

// Functions to encode UAVCAN messages from data type structs
#include "uavcan_encoding_v0.h"

// Functions to decode UAVCAN messages into data type structs
#include "uavcan_decoding_v0.h"

// Serial printing functions for debugging
#include "uavcan_debugging_v0.h"

//------------------------------------------------------------------------------

#endif // UAVCAN_H
