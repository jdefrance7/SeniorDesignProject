#ifndef RANGE_SENSOR_MEASUREMENT_H
#define RANGE_SENSOR_MEASUREMENT_H

// DSDL Constants
#include "dsdl.h"

// UAVCAN Formatting
#include <canard.h>

// Other Standard Data Types
#include "Node_Timestamp.h"
#include "Coarse_Orientation.h"

#define RANGE_SENSOR_MEASUREMENT_MESSAGE_SIZE           (120 / 8)
#define RANGE_SENSOR_MEASUREMENT_DATA_TYPE_SIGNATURE    0x68fffe70fc771952
#define RANGE_SENSOR_MEASUREMENT_DATA_TYPE_ID           1050

#define SENSOR_TYPE_UNDEFINED       0
#define SENSOR_TYPE_SONAR           1
#define SENSOR_TYPE_LIDAR           2
#define SENSOR_TYPE_RADAR           3

#define READING_TYPE_UNDEFINED      0
#define READING_TYPE_VALID_RANGE    1
#define READING_TYPE_TOO_CLOSE      2
#define READING_TYPE_TOO_FAR        3

int8_t range_sensor(
  uint8_t buffer[],
  uint16_t offset,
  uint8_t usec[NODE_TIMESTAMP_MESSAGE_SIZE],
  uint8_t sensor_id,
  uint8_t coarse_orientation[COARSE_ORIENTATION_MESSAGE_SIZE],
  float16_t field_of_view,
  uint5_t sensor_type,
  float16_t range
);

/* range_sensor.Measurement DSDL

Full name: uavcan.equipment.range_sensor.Measurement
Default data type ID: 1050

#
# Generic narrow-beam range sensor data.
#

uavcan.Timestamp timestamp

uint8 sensor_id

uavcan.CoarseOrientation beam_orientation_in_body_frame

float16 field_of_view                # Radians

uint5 SENSOR_TYPE_UNDEFINED = 0
uint5 SENSOR_TYPE_SONAR     = 1
uint5 SENSOR_TYPE_LIDAR     = 2
uint5 SENSOR_TYPE_RADAR     = 3
uint5 sensor_type

uint3 READING_TYPE_UNDEFINED   = 0   # Range is unknown
uint3 READING_TYPE_VALID_RANGE = 1   # Range field contains valid distance
uint3 READING_TYPE_TOO_CLOSE   = 2   # Range field contains min range for the sensor
uint3 READING_TYPE_TOO_FAR     = 3   # Range field contains max range for the sensor
uint3 reading_type

float16 range                        # Meters

*/

#endif // RANGE_SENSOR_MEASUREMENT_H
