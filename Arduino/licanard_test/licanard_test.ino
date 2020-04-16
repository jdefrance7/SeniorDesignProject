#include <Arduino.h>
#include <uavcan_v0.h>
#include <libcanard_v0.h>

CanardInstance canard;
uint8_t canard_memory_pool[1024];

uint8_t buffer[(ANGULAR_COMMAND_DATA_TYPE_SIZE+7)/8];

void onTransferReceived(CanardInstance* canard, CanardRxTransfer* transfer)
{
  if(transfer->transfer_type == CanardTransferTypeBroadcast)
  {
    if(transfer->data_type_id == ANGULAR_COMMAND_DATA_TYPE_ID)
    {
      uint16_t bit_offset = 0;
      uint8_t received_id;
      uint8_t received_mode;
      float received_values[4];

      canardDecodeScalar(transfer, bit_offset, 8, false, &received_id);
      bit_offset += 8;

      canardDecodeScalar(transfer, bit_offset, 8, false, &received_mode);
      bit_offset += 8;

      uint16_t float16;
      for(int n = 0; n < 4; n++)
      {
        canardDecodeScalar(transfer, bit_offset, 16, false, &float16);
        received_values[n] = canardConvertFloat16ToNativeFloat(float16);
        bit_offset += 16;
      }

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

bool shouldAcceptTransfer(const CanardInstance* canard, uint64_t* out_data_type_signature, uint16_t data_type_id, CanardTransferType transfer_type, uint8_t source_node_id)
{
  (void)source_node_id;

  if(transfer_type == CanardTransferTypeBroadcast)
  {
    if(data_type_id == ANGULAR_COMMAND_DATA_TYPE_ID)
    {
      *out_data_type_signature = ANGULAR_COMMAND_DATA_TYPE_SIGNATURE;
      return true;
    }
  }

  return false;
}

void setup() {

  Serial.begin(115200);
  while(!Serial);

  canardInit(
    &canard,
    canard_memory_pool,
    sizeof(canard_memory_pool),
    onTransferReceived,
    shouldAcceptTransfer,
    NULL
  );

  canardSetLocalNodeID(&canard, 8);
  
  // put your setup code here, to run once:
  randomSeed(analogRead(A0));
}

void loop() {

  float value[4];
  Serial.println("\nValues: ");
  for(int n = 0; n < 4; n++)
  {
    value[n] = float(random(100))/float(random(100));
    Serial.print(n); Serial.print(". "); Serial.println(value[n]);
  }

  AngularCommand angle;
  angle.gimbal_id = 1;
  angle.mode = 0;
  for(int n = 0; n < 4; n++)
  {
    angle.quaternion_xyzw[n] = value[n];
  }

  printAngularCommand(&angle);

  int reVal;
  
  reVal = encode_angular_command(buffer, sizeof(buffer), 0, angle);

  Serial.print("\nEncoding: "); Serial.println(reVal);

  static uint8_t transfer_id;

  uint16_t payload_len = (reVal+7)/8;

  reVal = canardBroadcast(
    &canard,
    ANGULAR_COMMAND_DATA_TYPE_SIGNATURE,
    ANGULAR_COMMAND_DATA_TYPE_ID,
    &transfer_id,
    CANARD_TRANSFER_PRIORITY_MEDIUM,
    buffer,
    payload_len
  );

  Serial.print("\nBroadcast: "); Serial.println(reVal);

  for(CanardCANFrame* txf = NULL; (txf = canardPeekTxQueue(&canard)) != NULL;)
  {
    printCanardFrame(txf);
    
    reVal = canardHandleRxFrame(&canard, txf, 1000*millis());

    Serial.print("\nReceive: "); Serial.println(reVal);
    
    canardPopTxQueue(&canard);
  }

  delay(1000);
}
