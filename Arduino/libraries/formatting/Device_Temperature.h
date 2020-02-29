#ifndef DEVICE_TEMPERATURE_H
#define DEVICE_TEMPERATURE_H

// DSDL Constants
#include "dsdl.h"

// UAVCAN Formatting
#include <canard.h>

#define DEVICE_TEMPERATURE_MESSAGE_SIZE           (40 / 8)
#define DEVICE_TEMPERATURE_DATA_TYPE_SIGNATURE    0x70261c28a94144c6
#define DEVICE_TEMPERATURE_DATA_TYPE_ID           1110

#define ERROR_FLAG_NONE                           0
#define ERROR_FLAG_OVERHEATING                    1
#define ERROR_FLAG_OVERCOOLING                    2

int8_t device_temperature(
  uint8_t buffer[],
  uint16_t offset,
  uint16_t device_id,
  uint8_t error_flags
);

/* device.Temperature DSDL

Full name: uavcan.equipment.device.Temperature
Default data type ID: 1110

#
# Generic device temperature
#

uint16 device_id

float16 temperature                  # in kelvin

uint8 ERROR_FLAG_OVERHEATING = 1
uint8 ERROR_FLAG_OVERCOOLING = 2
uint8 error_flags

*/

#endif // DEVICE_TEMPERATURE_H
