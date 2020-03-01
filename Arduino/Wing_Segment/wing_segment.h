#ifndef WING_SEGMENT_H
#define WING_SEGMENT_H

#include "imu_config.h"
#include "uavcan_config.h"
#include "node_config.h"

//##############################################################################
// DEBUG MODE //////////////////////////////////////////////////////////////////
//##############################################################################

#define SERIAL_DEBUG      // Uncomment to disable all serial debug functions
#define SERIAL_BAUDRATE   9600
#define SERIAL_TIMEOUT    1000

//##############################################################################
// APPLICATION FUNCTIONS ///////////////////////////////////////////////////////
//##############################################################################

inline void update_uptime() {node.uptime = millis();}
uint16_t queue_node_status(uint64_t timestamp_msec);
uint16_t queue_camera_gimbal_status(uint64_t timestamp_msec);

//##############################################################################
// DEBUG FUNCTIONS /////////////////////////////////////////////////////////////
//##############################################################################

/* Automatically set to empty inlines when SERIAL_DEBUG flag is disabled */

#if defined(SERIAL_DEBUG)

bool init_serial();
void print_uptime();

#else

inline bool init_serial() {return true;}
inline void print_uptime() {}

#endif

//##############################################################################
// END OF FILE /////////////////////////////////////////////////////////////////
//##############################################################################

#endif // WING_SEGMENT_H
