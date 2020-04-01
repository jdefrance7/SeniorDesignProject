#include "libcanard_v1.h"

#include <Arduino.h>

void printCanardFrame(CanardFrame* frame)
{
  Serial.println("Canard Frame");
  Serial.print("  Timestamp Usec: "); Serial.println((long)frame->timestamp_usec);
  Serial.print("  Extended ID: ");    Serial.println(frame->extended_can_id);
  Serial.print("  Payload Size: ");   Serial.println(frame->payload_size);
  Serial.print("  Payload: ");
  for(int n = 0; n < frame->payload_size; n++)
  {
    Serial.print((byte)frame->payload[n], HEX);

    if(n == (frame->payload_size-1))
    {
        Serial.print("\n");
        break;
    }

    Serial.print(",");
  }
}

void printCanardTransfer(CanardTransfer* transfer)
{
  Serial.println("Canard Transfer");
  Serial.print("  Timestamp Usec: ");   Serial.println((long)transfer->timestamp_usec);
  Serial.print("  Priority: ");         Serial.println(transfer->priority);
  Serial.print("  Kind: ");             Serial.println(transfer->transfer_kind);
  Serial.print("  Port ID: ");          Serial.println(transfer->port_id);
  Serial.print("  Remote Node ID: ");   Serial.println(transfer->remote_node_id);
  Serial.print("  Tranfer ID: ");       Serial.println(transfer->transfer_id);
  Serial.print("  Payload Size: ");     Serial.println(transfer->payload_size);
  for(int n = 0; n < transfer->payload_size; n++)
  {
    Serial.print((byte)transfer->payload[n], HEX);

    if(n == (transfer->payload_size-1))
    {
        Serial.print("\n");
        break;
    }

    Serial.print(",");
  }
}

void printCanardInstance(CanardInstance* ins)
{
  Serial.println("Canard Instance");
  Serial.print("  MTU Bytes: ");      Serial.println(ins->mtu_bytes);
  Serial.print("  Node ID: ");        Serial.println(ins->node_id);
}
