/**
 * @file uavcan_debugging_v0.h
 *
 * Printing functions for UAVCAN v0 data types.
 *
 * @author Joe DeFrance
 */


#ifndef UAVCAN_DEBUGGING
#define UAVCAN_DEBUGGING

//------------------------------------------------------------------------------

#include <Arduino.h>

//------------------------------------------------------------------------------

#include "uavcan_data_types_v0.h"
#include "uavcan_dsdl_sizes_v0.h"
#include "uavcan_node_struct_v0.h"

//------------------------------------------------------------------------------

void printNodeStatus(NodeStatus* status);
void printHardwareVersion(HardwareVersion* hardware);
void printSoftwareVersion(SoftwareVersion* software);
void printCoarseOrientation(CoarseOrientation* coarse);
void printTimestamp(Timestamp* timestamp);
void printAhrsSolution(AhrsSolution** solution);
void printCameraGimbalMode(CameraGimbalMode* mode);
void printCameraGimbalStatus(CameraGimbalStatus* status);
void printDeviceTemperature(DeviceTemperature* temp);
void printRangeSensorMeasurement(RangeSensorMeasurement* measurement);
void printGetInfo(GetNodeInfo* info);
void printDataTypeKind(DataTypeKind* kind);
void printGetDataTypeInfo(GetDataTypeInfo* info);
void printRestartNode(RestartNode* restart);
void printKeyValue(KeyValue* items);
void printLogLevel(LogLevel* level);
void printLogMessage(LogMessage* msg);
void printMode(Mode *mode);
void printAngularCommand(AngularCommand *angles);
void printNode(UavcanNode* node);

//------------------------------------------------------------------------------

#endif // UAVCAN_DEBUGGING
