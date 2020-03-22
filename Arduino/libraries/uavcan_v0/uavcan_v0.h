#ifndef UAVCAN_H
#define UAVCAN_H

#include <canard.h>

// Select CAN Driver
// #include <canard_avr.h>
#include <ASTCanLib.h>

// DSDL primative data type sizes
#include "uavcan_dsdl_sizes_v0.h"

// DSDL custom data type primatives and structs
#include "uavcan_data_types_v0.h"

// Basic UAVCAN node struct
#include "uavcan_node_struct_v0.h"

// Basic Canard struct
#include "uavcan_canard_struct_v0.h"

// Functions to encode UAVCAN messages from data type structs
#include "uavcan_encoding_v0.h"

// Functions to decode UAVCAN messages into data type structs
#include "uavcan_decoding_v0.h"

// Functions to send UAVCAN messages given data type structs
#include "uavcan_sending_v0.h"

// Functions to read UAVCAN messages as known data type structs
#include "uavcan_receiving_v0.h"

// User Functions

int16_t uavcan_init(UavcanNode node, Canard can);

#endif // UAVCAN_H
