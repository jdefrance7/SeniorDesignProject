#include "uavcan_debugging_v1.h"

void printHeartbeat(Heartbeat* heartbeat)
{
  Serial.println("\nHeartbeat");
  Serial.print("  Uptime: ");   Serial.println((unsigned int)heartbeat->uptime);
  Serial.print("  Health: ");   Serial.println((byte)heartbeat->health);
  Serial.print("  Mode: ");     Serial.println((byte)heartbeat->mode);
  Serial.print("  Vendor: ");   Serial.println((unsigned int)heartbeat->vendor_specific_status_code);
}

void printSynchronizedTimestamp(SynchronizedTimestamp* timestamp)
{
  Serial.println((unsigned long)*timestamp);
}

void printQuaternion(Quaternion* q)
{
  Serial.println("\nQuaternion");
  Serial.print("  qW: "); Serial.println(q->wxyz[QW_INDEX]);
  Serial.print("  qX: "); Serial.println(q->wxyz[QX_INDEX]);
  Serial.print("  qY: "); Serial.println(q->wxyz[QY_INDEX]);
  Serial.print("  qZ: "); Serial.println(q->wxyz[QZ_INDEX]);
}

void printID(ID* id)
{
  Serial.println((unsigned int)*id);
}

void printVersion(Version* version)
{
  Serial.println("\nVersion");
  Serial.print("  Major: "); Serial.println((byte)version->major);
  Serial.print("  Minor: "); Serial.println((byte)version->minor);
}

void printGetInfo(GetInfo* info)
{
  Serial.println("\nInfo");
  Serial.print("  Protocol ");      printVersion(&(info->protocol_version));
  Serial.print("  Hardware ");      printVersion(&(info->hardware_version));
  Serial.print("  Software ");      printVersion(&(info->software_version));
  Serial.print("  VCS Revision: "); Serial.println((unsigned long)info->software_vcs_revision_id);
  Serial.print("  Unique ID: ");
  for(int n = 0; n < GET_INFO_UNIQUE_ID_SIZE; n++)
  {
    Serial.print((byte)info->unique_id[n], HEX);
  }
  Serial.print("\n");
  Serial.print("  Name: ");
  for(int n = 0; n < GET_INFO_NAME_SIZE; n++)
  {
    Serial.print((unsigned char)info->name[n]);
    if((unsigned char)info->name[n] == '\0')
    {
      Serial.print("\n");
      break;
    }
  }
  Serial.print("  Image CRC: ");    Serial.println((unsigned long)info->software_image_crc);
  Serial.print("  Certificate: ");
  for(int n = 0; n < GET_INFO_NAME_SIZE; n++)
  {
    Serial.print((unsigned char)info->certificate[n]);
    if((unsigned char)info->certificate[n] == '\0')
    {
      Serial.print("\n");
      break;
    }
  }
}

void printSeverity(Severity* severity)
{
  Serial.println((byte)*severity);
}

void printRecord(Record* record)
{
  Serial.println("\nRecord");
  Serial.print("  Timestamp: ");  printSynchronizedTimestamp(&(record->timestamp));
  Serial.print("  Severity: ");   printSeverity(&(record->severity));
  Serial.print("  Text: ");
  for(int n = 0; n < RECORD_TEXT_SIZE; n++)
  {
    Serial.print((unsigned char)record->text[n]);
    if((unsigned char)record->text[n] == '\0')
    {
      Serial.print("\n");
      break;
    }
  }
}

void printNode(UavcanNode* node)
{
  Serial.println("\nNode");
  Serial.print("  ID: "); Serial.println((byte)node->id);
  printHeartbeat(&(node->status));
  printGetInfo(&(node->info));
}
