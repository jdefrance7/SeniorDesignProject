#ifndef WING_SEGMENT_H
#define WING_SEGMENT_H

//##############################################################################
// DEBUG MODE //////////////////////////////////////////////////////////////////
//##############################################################################

#define SERIAL_DEBUG      // Uncomment to disable all serial debug functions
#define SERIAL_BAUDRATE   9600

//##############################################################################
// ARDUINO LIBRARY /////////////////////////////////////////////////////////////
//##############################################################################

#include <Arduino.h>

//##############################################################################
// SELECT SENSOR ///////////////////////////////////////////////////////////////
//##############################################################################

/* Select Sensor */

#define SENSOR_BNO055
// #define SENSOR_LSM9DS1
// #define SENSOR_NXP_FXOS_FXAS

//##############################################################################
// SELECT FILTER ///////////////////////////////////////////////////////////////
//##############################################################################

/* Select Filter from below (only for LSM9DS1 & NXP_FXOS_FXAS) */

// #define FILTER_NXP_SENSOR_FUSION
// #define FILTER_MADGWICK
// #define FILTER_MAHONY

//##############################################################################
// FILTER UPDATE RATE //////////////////////////////////////////////////////////
//##############################################################################

/* Set Filter Update Rate (only for LSM9DS1 & NXP_FXOS_FXAS) */

// #define FILTER_UPDATE_RATE_HZ 100

//##############################################################################
// IMU LIBRARY /////////////////////////////////////////////////////////////////
//##############################################################################

#include <imu.h>

//##############################################################################
// UAVCAN BITRATE //////////////////////////////////////////////////////////////
//##############################################################################

/* Select UAVCAN Bitrate */

// #define CAN_BITRATE BITRATE_10_KBPS	 // not tested
// #define CAN_BITRATE BITRATE_20_KBPS	 // not tested
// #define CAN_BITRATE BITRATE_50_KBPS	 // not tested
// #define CAN_BITRATE BITRATE_100_KBPS // not tested
#define CAN_BITRATE BITRATE_125_KBPS
// #define CAN_BITRATE BITRATE_250_KBPS // not tested
// #define CAN_BITRATE BITRATE_500_KBPS // not tested
// #define CAN_BITRATE BITRATE_1_MBPS   // not tested

//##############################################################################
// UAVCAN RESPONSES ////////////////////////////////////////////////////////////
//##############################################################################

/* Select Supported UAVCAN Responses Addressed to this Node  */

#define ACCEPT_RESPONSE_NONE

// Add other response flags here (must also write support code in callback/handler)

//##############################################################################
// UAVCAN REQUESTS /////////////////////////////////////////////////////////////
//##############################################################################

/* Select Supported UAVCAN Requests Addressed to this Node  */

// #define ACCEPT_REQUEST_NONE
#define ACCEPT_REQUEST_NODE_INFO
// #define ACCEPT_REQUEST_DATA_TYPE_INFO

// Add other request flags here (must also write support code in callback/handler)

//##############################################################################
// UAVCAN BROADCASTS ///////////////////////////////////////////////////////////
//##############################################################################

/* Select Supported UAVCAN Broadcasts Addressed to this Node  */

#define ACCEPT_BROADCAST_NONE

// Add other broadcast flags here (must also write support code in callback/handler)

//##############################################################################
// UAVCAN LIBRARY //////////////////////////////////////////////////////////////
//##############################################################################

/* UAVCAN Library */

#include <uavcan.h>

//##############################################################################
// APPLICATION FUNCTIONS ///////////////////////////////////////////////////////
//##############################################################################

inline void update_uptime() {g_node_uptime = millis();}
bool queue_node_status(uint64_t timestamp_msec);
bool queue_camera_gimbal_status(uint64_t timestamp_msec);

//##############################################################################
// DEBUG FUNCTIONS /////////////////////////////////////////////////////////////
//##############################################################################

/* Automatically set to empty inline voids when SERIAL_DEBUG flag is disabled */

#if defined(SERIAL_DEBUG)

void init_serial();
void print_uptime();

#else

inline void init_serial() {}
inline void print_uptime() {}

#endif

//##############################################################################
// END OF FILE /////////////////////////////////////////////////////////////////
//##############################################################################

#endif // WING_SEGMENT_H
