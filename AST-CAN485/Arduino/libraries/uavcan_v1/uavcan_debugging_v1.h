#ifndef UAVCAN_DEBUGGING
#define UAVCAN_DEBUGGING

//------------------------------------------------------------------------------

#include <Arduino.h>

//------------------------------------------------------------------------------

#include "uavcan_data_types_v1.h"
#include "uavcan_dsdl_sizes_v1.h"
#include "uavcan_node_struct_v1.h"

//------------------------------------------------------------------------------

void printHeartbeat(Heartbeat* heartbeat);
void printSynchronizedTimestamp(SynchronizedTimestamp* timestamp);
void printQuaternion(Quaternion* q);
void printID(ID* id);
void printVersion(Version* version);
void printGetInfo(GetInfo* info);
void printSeverity(Severity* severity);
void printRecord(Record* record);
void printNode(UavcanNode* node);

//------------------------------------------------------------------------------

#endif // UAVCAN_DEBUGGING
