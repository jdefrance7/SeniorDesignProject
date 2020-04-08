#include "libcanard_v0.h"

void printCanardFrame(CanardCANFrame* frame)
{
  Serial.println("\nCanard Frame");
  Serial.print("  ID: ");           Serial.println(frame->id);
  Serial.print("  Length: ");  Serial.println(frame->data_len);
  Serial.print("  Data: ");
  for(int n = 0; n < frame->data_len; n++)
  {
    Serial.print(frame->data[n], HEX);
    if(n == (frame->data_len-1))
    {
      Serial.print("\n");
      break;
    }

    Serial.print(",");
  }
}

void printCanardInstance(CanardInstance* ins)
{
  Serial.println("\nCanard Instance");
  Serial.print("  Node ID: "); Serial.println(ins->node_id);
}

void printCanardRxTransfer(CanardRxTransfer* rxt)
{
  Serial.println("\nCanard Rx Transfer");
  Serial.print("  Timestamp Usec: ");   Serial.println((long)rxt->timestamp_usec);
  Serial.print("  Payload Length: ");   Serial.println(rxt->payload_len);
  Serial.print("  Data Type ID: ");     Serial.println(rxt->data_type_id);
  Serial.print("  Transfer Type: ");    Serial.println(rxt->transfer_type);
  Serial.print("  Transfer ID: ");      Serial.println(rxt->transfer_id);
  Serial.print("  Priority: ");         Serial.println(rxt->priority);
  Serial.print("  Source Node ID: ");   Serial.println(rxt->source_node_id);
}

void printCanardPoolAllocatorStatistics(CanardPoolAllocatorStatistics* stats)
{
  Serial.println("\nCanard Pool Allocator Stats");
  Serial.print("  Capacity: ");       Serial.println(stats->capacity_blocks);
  Serial.print("  Current Usage: ");  Serial.println(stats->current_usage_blocks);
  Serial.print("  Peak Usage: ");     Serial.println(stats->peak_usage_blocks);
}
