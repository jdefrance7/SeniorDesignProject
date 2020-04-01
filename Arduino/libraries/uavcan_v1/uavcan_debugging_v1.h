#ifndef UAVCAN_DEBUGGING
#define UAVCAN_DEBUGGING

//------------------------------------------------------------------------------

#include <Arduino.h>

//------------------------------------------------------------------------------

#include "uavcan_data_types_v1.h"
#include "uavcan_dsdl_sizes_v1.h"

//------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

//------------------------------------------------------------------------------

void printHeartbeat(Heartbeat heartbeat);
void printSynchronizedTimestamp(SynchronizedTimestamp timestamp);
void printQuaternion(Quaternion q);
void printID(ID id);
void printVersion(Version version);
void printGetInfo(GetInfo info);
void printSeverity(Severity severity);
void printRecord(Record record);

void printNode(UavcanNode node);

//------------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

//------------------------------------------------------------------------------

#endif // UAVCAN_DEBUGGING
