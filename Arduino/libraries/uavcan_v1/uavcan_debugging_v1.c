#include "uavcan_debugging_v1.h"

void printHeartbeat(Heartbeat heartbeat)
{
  Serial.println("Heartbeat");
  Serial.print("  Uptime: ");   Serial.println(heartbeat.uptime);
  Serial.print("  Health: ");   Serial.println(heartbeat.health);
  Serial.print("  Mode: ");     Serial.println(heartbeat.mode);
  Serial.print("  Vendor: ");   Serial.println(heartbeat.vendor_specific_status_code);
}

void printSynchronizedTimestamp(SynchronizedTimestamp timestamp)
{
  Serial.println((long)timestamp);
}

void printQuaternion(Quaternion q)
{
  Serial.println("Quaternion");
  Serial.print("  qW: "); Serial.println(q.wxyz[QW_INDEX]);
  Serial.print("  qX: "); Serial.println(q.wxyz[QX_INDEX]);
  Serial.print("  qY: "); Serial.println(q.wxyz[QY_INDEX]);
  Serial.print("  qZ: "); Serial.println(q.wxyz[QZ_INDEX]);
}

void printID(ID id)
{
  Serial.println((int)id);
}

void printVersion(Version version)
{
  Serial.println("Version");
  Serial.print("  Major: "); Serial.println(version.major);
  Serial.print("  Minor: "); Serial.println(verison.minor);
}

void printGetInfo(GetInfo info)
{
  Serial.println("Info");
  Serial.print("  Protocol ");      printVersion(info.protocol_version);
  Serial.print("  Hardware ");      printVersion(info.hardware_version);
  Serial.print("  Software ");      printVersion(info.software_version);
  Serial.print("  VCS Revision: "); Serial.println(info.software_vcs_revision_id);
  Serial.print("  Unique ID: ");    Serial.println(info.unique_id);
  Serial.print("  Name: ");         Serial.println(info.name);
  Serial.print("  Image CRC: ");    Serial.println(info.software_image_crc);
  Serial.print("  Certificate: ");  Serial.println(info.certificate);
}

void printSeverity(Severity severity)
{
  Serial.println((int)severity);
}

void printRecord(Record record)
{
  Serial.println("Record");
  Serial.print("  Timestamp: "); printSynchronizedTimestamp(record.timestamp);
  Serial.print("  Severity: "); printSeverity(record.severity);
  Serial.print("  Text: "); Serial.println(record.text);
}

void printNode(UavcanNode node)
{
  Serial.println("Node");
  Serial.print("  ID: "); Serial.println(node.id);
  Serial.print("  ");     printHeartbeat(node.status);
  Serial.print("  ");     printGetInfo(node.info);
}
