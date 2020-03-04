#ifndef LIBCANARD_H
#define LIBCANARD_H

#ifndef HAS_CAN_CONFIG_H
#define HAS_CAN_CONFIG_H
#endif

//##############################################################################
// INCLUDES ////////////////////////////////////////////////////////////////////
//##############################################################################

// Implementation of UAVCAN protocol in C
#include <canard.h>

// CAN driver for the AT90CAN128
#include <can.h>

// Interface between Libcanard and the AT90CAN128 driver
#include <canard_avr.h>

//##############################################################################
// LIBCANARD OBJECTS ///////////////////////////////////////////////////////////
//##############################################################################

// Library instance
CanardInstance g_canard;
uint8_t g_canard_memory_pool[1024];

//##############################################################################
// END OF FILE /////////////////////////////////////////////////////////////////
//##############################################################################

#endif // LIBCANARD_H
