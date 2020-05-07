/**
 *  @file libcanard_test.ino
 *
 *  Loop to test Libcanard processing of received UAVCAN frames.
 *
 *  Includes example of how to decode AngularCommand messages.
 *
 *  @author Joe DeFrance
 */

// Arduino library
#include <Arduino.h>

// UAVCAN data types library
#include <uavcan_v0.h>

// Libcanard library
#include <libcanard_v0.h>

// Canard instance
CanardInstance canard;

// Canard memory pool
uint8_t canard_memory_pool[1024];

// Buffer to store encoded AngularCommand data
uint8_t buffer[(ANGULAR_COMMAND_DATA_TYPE_SIZE+7)/8];

// Callback used by Libcanard to process a fully received UAVCAN transfer
void onTransferReceived(CanardInstance* canard, CanardRxTransfer* transfer)
{
  // Filter by transfer type (broadcast, request, service)
  if(transfer->transfer_type == CanardTransferTypeBroadcast)
  {
    // Filter by data type (NodeStatus, AngularCommand, LogMessage, etc...)
    if(transfer->data_type_id == ANGULAR_COMMAND_DATA_TYPE_ID)
    {
      // Decoding variables
      uint16_t bit_offset = 0;
      uint8_t received_id;
      uint8_t received_mode;
      float received_values[4];

      // Extract gimbal_id from transfer
      canardDecodeScalar(transfer, bit_offset, 8, false, &received_id);
      bit_offset += 8;

      // Extract mode from transfer
      canardDecodeScalar(transfer, bit_offset, 8, false, &received_mode);
      bit_offset += 8;

      // Extract quaternion_xyzw[4] from transfer
      uint16_t float16;
      for(int n = 0; n < 4; n++)
      {
        canardDecodeScalar(transfer, bit_offset, 16, false, &float16);
        received_values[n] = canardConvertFloat16ToNativeFloat(float16);
        bit_offset += 16;
      }

      // Print extracted Angular Command data
      Serial.println("\nReceived Angular Command:");
      Serial.print("ID: "); Serial.println(received_id);
      Serial.print("Mode: "); Serial.println(received_mode);
      for(int n = 0; n < 4; n++)
      {
        Serial.print("Quat "); Serial.print(n); Serial.print(": "); Serial.println(received_values[n], 3);
      }
    }
  }
}

// Callback used by Libcanard to filter incomming UAVCAN transfers
bool shouldAcceptTransfer(const CanardInstance* canard, uint64_t* out_data_type_signature, uint16_t data_type_id, CanardTransferType transfer_type, uint8_t source_node_id)
{
  (void)source_node_id;

  // Filter by transfer type (broadcast, request, service)
  if(transfer_type == CanardTransferTypeBroadcast)
  {
    // Filter by data type (NodeStatus, AngularCommand, LogMessage, etc...)
    if(data_type_id == ANGULAR_COMMAND_DATA_TYPE_ID)
    {
      // Designate data type signature for CRC computation
      *out_data_type_signature = ANGULAR_COMMAND_DATA_TYPE_SIGNATURE;

      return true; //< transfer is accepted and processed by Libcanard
    }
  }

  return false; //< transfer is rejected and ignored by Libcanard
}

// Called once before entering loop()
void setup() {

  // Initialize the Serial module
  Serial.begin(115200);
  while(!Serial);

  // Initialize the Canard instance
  canardInit(
    &canard,
    canard_memory_pool,
    sizeof(canard_memory_pool),
    onTransferReceived,
    shouldAcceptTransfer,
    NULL
  );

  // Set the local node ID
  canardSetLocalNodeID(&canard, 8);

  // Start a random seed for number generation
  randomSeed(analogRead(A0));
}

// Main loop
void loop()
{
  // Generate array of random floats in the range [0, 1]
  float value[4];
  Serial.println("\nValues: ");
  for(int n = 0; n < 4; n++)
  {
    value[n] = float(random(100)+1)/float(100*(random(100)+1));
    Serial.print(n); Serial.print(". "); Serial.println(value[n]);
  }

  // Create and populate AngularCommand data structure
  AngularCommand angle;
  angle.gimbal_id = 1;
  angle.mode = 0;
  for(int n = 0; n < 4; n++)
  {
    angle.quaternion_xyzw[n] = value[n];
  }

  // Print the formatted AngularCommand data structure
  printAngularCommand(&angle);

  // Return value to check for errors
  int reVal;

  // Encode AngularCommand data structure into buffer
  reVal = encode_angular_command(buffer, sizeof(buffer), 0, angle);

  // Print return value
  Serial.print("\nEncoding: "); Serial.println(reVal);

  // Unique transfer id for AngularCommand
  static uint8_t transfer_id;

  // Compute payload size in bytes
  uint16_t payload_len = (reVal+7)/8;

  // Push AngularCommand onto Canard message queue
  reVal = canardBroadcast(
    &canard,
    ANGULAR_COMMAND_DATA_TYPE_SIGNATURE,
    ANGULAR_COMMAND_DATA_TYPE_ID,
    &transfer_id,
    CANARD_TRANSFER_PRIORITY_MEDIUM,
    buffer,
    payload_len
  );

  // Print return value
  Serial.print("\nBroadcast: "); Serial.println(reVal);

  // "Receive" the pushed frames from the transfer queue
  for(CanardCANFrame* txf = NULL; (txf = canardPeekTxQueue(&canard)) != NULL;)
  {
    // Print the "received" frame
    printCanardFrame(txf);

    // Call the Canard handler function for received frames
    reVal = canardHandleRxFrame(&canard, txf, 1000*millis());

    // Print the return value
    Serial.print("\nReceive: "); Serial.println(reVal);

    // Pop the "received" frame from the transfer queue
    canardPopTxQueue(&canard);
  }

  // Wait a second and repeat
  delay(1000);
}
