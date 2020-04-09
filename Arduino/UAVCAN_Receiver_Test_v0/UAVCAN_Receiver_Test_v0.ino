// CAN Bitrate
#define CAN_BITRATE 500000

// Serial Baudrate
#define SERIAL_BAUDRATE 115200

// Mask Values for UAVCAN EID
#define UAVCAN_PRIORITY_MASK      0xF000000
#define UAVCAN_TYPE_ID_MASK       0x0FFFF00
#define UAVCAN_SERVICE_MASK       0x0000080
#define UAVCAN_SOURCE_ID_MASK     0x000007F

// Shift Values for UAVCAN EID
#define UAVCAN_PRIORITY_SHIFT     24
#define UAVCAN_TYPE_ID_SHIFT      8
#define UAVCAN_SERVICE_SHIFT      7
#define UAVCAN_SOURCE_ID_SHIFT    0

// Mask Values for UAVCAN Tail Byte
#define UAVCAN_START_BIT_MASK     0x80
#define UAVCAN_END_BIT_MASK       0x40
#define UAVCAN_TOGGLE_BIT_MASK    0x20
#define UAVCAN_TRANSFER_ID_MASK   0x1F

// Shift Values for UAVCAN Tail Byte
#define UAVCAN_START_BIT_SHIFT    7
#define UAVCAN_END_BIT_SHIFT      6
#define UAVCAN_TOGGLE_BIT_SHIFT   5
#define UAVCAN_TRANSFER_ID_SHIFT  0

// ASTCanLib st_cmd_t Values
#define MESSAGE_ID        0       // Message ID
#define MESSAGE_PROTOCOL  1       // CAN protocol (0: CAN 2.0A, 1: CAN 2.0B)
#define MESSAGE_LENGTH    8       // Data length: 8 bytes
#define MESSAGE_RTR       0       // rtr bit

// CAN Driver Library
#include <ASTCanLib.h>

// UAVCAN Data Processing
#include <uavcan_v0.h>

// UAVCAN Source
#include <libcanard_v0.h>

// Canard Instance
CanardInstance canard;

// Canard Memory Pool
uint8_t canard_memory_pool[1024];

// Canard Callbacks (see function definitons)
void onTransferReceived(CanardInstance* ins, CanardRxTransfer* transfer);
bool shouldAcceptTransfer(const CanardInstance* ins,
                                 uint64_t* out_data_type_signature,
                                 uint16_t data_type_id,
                                 CanardTransferType transfer_type,
                                 uint8_t source_node_id);

// Function prototypes
void printCanardStats();
void serialPrintData(st_cmd_t *msg);
void processMessage(st_cmd_t *msg);

// CAN message object
st_cmd_t Msg;

// Buffer for CAN data
uint8_t Buffer[8] = {};

void setup() {

  // Initialise CAN port. Must be before Serial.begin()
  canInit(CAN_BITRATE);

  // Initialize Serial module
  Serial.begin(SERIAL_BAUDRATE);
  while(!Serial);

  // Reference message data buffer
  Msg.pt_data = &Buffer[0];

  // Initialise CAN packet (overwritten by a received packet)
  Msg.ctrl.ide = MESSAGE_PROTOCOL;  // Set CAN protocol (0: CAN 2.0A, 1: CAN 2.0B)
  Msg.id.ext   = MESSAGE_ID;        // Set message ID
  Msg.dlc      = MESSAGE_LENGTH;    // Data length: 8 bytes
  Msg.ctrl.rtr = MESSAGE_RTR;       // Set rtr bit

  // Initalize Canard Instance (see canard.h for details)
  canardInit(
    &canard,
    &canard_memory_pool,
    sizeof(canard_memory_pool),
    onTransferReceived,
    shouldAcceptTransfer,
    NULL
  );
}

void loop() {

  // Clear the message buffer
  clearBuffer(&Buffer[0]);

  // Send command to the CAN port controller
  Msg.cmd = CMD_RX_DATA;

  // Wait for the command to be accepted by the controller

  while(can_cmd(&Msg) != CAN_CMD_ACCEPTED);

  // Wait for command to finish executing
  while(can_get_status(&Msg) == CAN_STATUS_NOT_COMPLETED);


  // Print received data to the terminal (slow)
//  serialPrintData(&Msg);

  // Process UAVCAN transfer (fast)
  processMessage(&Msg);

  // Print Canard Stats (fast)
//  printCanardStats();

  // Print Milliseconds since last CAN message (fast)
//  printTimePeriod();
}

void printTimePeriod()
{
  static long time = 0;

  Serial.print("Time Period: "); Serial.println(millis()-time);

  time = millis();
}

void printCanardStats()
{
  Serial.println("\n########## CANARD STATS ########");

  CanardPoolAllocatorStatistics stats = canardGetPoolAllocatorStatistics(&canard);
  printCanardPoolAllocatorStatistics(&stats);
}

void processMessage(st_cmd_t *msg)
{
  // Format received data into CanardCanFrame
  CanardCANFrame rxf;

  // Copy EID
  rxf.id = msg->id.ext;

  // Add Canard flag
  rxf.id |= CANARD_CAN_FRAME_EFF;

  // Copy Data
  memset(rxf.data, 0, CANARD_CAN_FRAME_MAX_DATA_LEN);
  memcpy(rxf.data, msg->pt_data, msg->dlc);

  // Copy DLC
  rxf.data_len = msg->dlc;

  // Process Frame (see onTransferReceived())
  int reVal = canardHandleRxFrame(&canard, &rxf, 1000*millis());

  // Print Response
  Serial.println("\n########## CANARD HANDLE RX ########");
  Serial.print("\nCanard Handle Rx Frame Response: "); Serial.println(reVal);

  return;
}

void serialPrintData(st_cmd_t *msg){

  char textBuffer[50] = {0};

  Serial.println("\n############ MESSAGE ############");

  // ASTCanLib Message Structure
  Serial.println("\nCAN Info");

  // Extended ID
  if (msg->ctrl.ide > 0){
    sprintf(textBuffer, "  EID: %u ",msg->id.ext);
  }
  // Standard ID
  else
  {
    sprintf(textBuffer, "  SID: %04x ",msg->id.std);
  }
  Serial.println(textBuffer);

  //  IDE
  sprintf(textBuffer, "  IDE: %d ",msg->ctrl.ide);
  Serial.println(textBuffer);

  //  RTR
  sprintf(textBuffer, "  RTR: %d ",msg->ctrl.rtr);
  Serial.println(textBuffer);

  //  DLC
  sprintf(textBuffer, "  DLC: %d ",msg->dlc);
  Serial.println(textBuffer);

  //  Data
  sprintf(textBuffer, "  Data: ");
  Serial.print(textBuffer);
  for(int i = 0; i < msg->dlc; i++){
    sprintf(textBuffer,"%02X ",msg->pt_data[i]);
    Serial.print(textBuffer);
  }
  Serial.print("\n");

  // UAVCAN Information
  Serial.println("\nUAVCAN ID Info");

  int priority = ((msg->id.ext) & UAVCAN_PRIORITY_MASK) >> UAVCAN_PRIORITY_SHIFT;
  sprintf(textBuffer, "  Priority: %d", priority);
  Serial.println(textBuffer);

  int type_id = ((msg->id.ext) & UAVCAN_TYPE_ID_MASK) >> UAVCAN_TYPE_ID_SHIFT;
  sprintf(textBuffer, "  Data Type ID: %d", type_id);
  Serial.println(textBuffer);

  int service = ((msg->id.ext) & UAVCAN_SERVICE_MASK) >> UAVCAN_SERVICE_SHIFT;
  sprintf(textBuffer, "  Service Bit: %d", service);
  Serial.println(textBuffer);

  int source_id = ((msg->id.ext) & UAVCAN_SOURCE_ID_MASK) >> UAVCAN_SOURCE_ID_SHIFT;
  sprintf(textBuffer, "  Source Node ID: %d", source_id);
  Serial.println(textBuffer);

  Serial.println("\nUAVCAN Tail Byte Info");

  byte tail_byte = msg->pt_data[(msg->dlc - 1)];
  sprintf(textBuffer, "  Tail Byte: %02X", tail_byte);
  Serial.println(textBuffer);

  int start_bit = (tail_byte & UAVCAN_START_BIT_MASK) >> UAVCAN_START_BIT_SHIFT;
  sprintf(textBuffer, "  Start Bit: %d", start_bit);
  Serial.println(textBuffer);

  int end_bit = (tail_byte & UAVCAN_END_BIT_MASK) >> UAVCAN_END_BIT_SHIFT;
  sprintf(textBuffer, "  End Bit: %d", end_bit);
  Serial.println(textBuffer);

  int toggle_bit = (tail_byte & UAVCAN_TOGGLE_BIT_MASK) >> UAVCAN_TOGGLE_BIT_SHIFT;
  sprintf(textBuffer, "  Toggle Bit: %d", toggle_bit);
  Serial.println(textBuffer);

  int transfer_id = (tail_byte & UAVCAN_TRANSFER_ID_MASK) >> UAVCAN_TRANSFER_ID_SHIFT;
  sprintf(textBuffer, "  Transfer ID: %d", transfer_id);
  Serial.println(textBuffer);

  // Single Frame Transfer
  if(start_bit == 1 && end_bit == 1)
  {
    Serial.println("  Transfer: Single Frame");

    Serial.println("\nUAVCAN Data");

    for(int n = 0; n < msg->dlc - 1; n++)
    {
      sprintf(textBuffer, "  Data Byte %d: %02X", n, msg->pt_data[n]);
      Serial.println(textBuffer);
    }
  }
  // Multi Frame Start
  else if(start_bit == 1 && end_bit == 0)
  {
    Serial.println("  Transfer: Multi Frame Start");

    byte crc_high = msg->pt_data[0];
    byte crc_low  = msg->pt_data[1];
    word crc = ((0xFFFF&crc_high) << 8) | crc_low;

    sprintf(textBuffer, "  CRC High: %02X", crc_high);
    Serial.println(textBuffer);

    sprintf(textBuffer, "  CRC Low: %02X", crc_low);
    Serial.println(textBuffer);

    sprintf(textBuffer, "  CRC: %04X", crc);
    Serial.println(textBuffer);

    Serial.println("\nUAVCAN Data");

    for(int n = 2; n < msg->dlc - 1; n++)
    {
      sprintf(textBuffer, "  Data Byte %d: %02X", n, msg->pt_data[n]);
      Serial.println(textBuffer);
    }
  }
  // Multi Frame Middle
  else if(start_bit == 0 && end_bit == 0)
  {
    Serial.println("  Transfer: Multi Frame Middle");

    Serial.println("\nUAVCAN Data");

    for(int n = 0; n < msg->dlc - 1; n++)
    {
      sprintf(textBuffer, "  Data Byte %d: %02X", n, msg->pt_data[n]);
      Serial.println(textBuffer);
    }
  }
  // Multi Frame End
  else if(start_bit == 0 && end_bit == 1)
  {
    Serial.println("  Transfer: Multi Frame End");

    Serial.println("\nUAVCAN Data");

    for(int n = 0; n < msg->dlc - 1; n++)
    {
      sprintf(textBuffer, "  Data Byte %d: %02X", n, msg->pt_data[n]);
      Serial.println(textBuffer);
    }
  }
}

/**
 * This callback is invoked by the library when a new message or request or response is received.
 */
void onTransferReceived(CanardInstance* ins, CanardRxTransfer* transfer)
{
  if(transfer->transfer_type == CanardTransferTypeResponse)
  {
    // Add response handlers here

    //...
  }
  else if(transfer->transfer_type == CanardTransferTypeRequest)
  {
    // Add request handlers here

    //...
  }
  else if(transfer->transfer_type == CanardTransferTypeBroadcast)
  {
    if(transfer->data_type_id == NODE_STATUS_DATA_TYPE_ID)
    {
      Serial.println("\n######## NODE STATUS RECEIVED ########");

      NodeStatus status;
      decode_node_status(transfer, 0, &status);
      printNodeStatus(&status);

      canardReleaseRxTransferPayload(&canard, transfer);
    }
    else if(transfer->data_type_id == LOG_MESSAGE_DATA_TYPE_ID)
    {
      Serial.println("\n######## LOG MESSAGE RECEIVED ########");

      LogMessage message;
      decode_log_message(transfer, 0, &message);
      printLogMessage(&message);

      canardReleaseRxTransferPayload(&canard, transfer);
    }
    else if(transfer->data_type_id == KEY_VALUE_DATA_TYPE_ID)
    {
      Serial.println("\n######## KEY VALUE RECEIVED ########");

      KeyValue pair;
      decode_key_value(transfer, 0, &pair);
      printKeyValue(&pair);

      canardReleaseRxTransferPayload(&canard, transfer);
    }
    else if(transfer->data_type_id == CAMERA_GIMBAL_STATUS_DATA_TYPE_ID)
    {
      Serial.println("\n######## CAMERA GIMBAL STATUS RECEIVED #########");

      CameraGimbalStatus gimbal;
      decode_camera_gimbal_status(transfer, 0, &gimbal);
      printCameraGimbalStatus(&gimbal);

      canardReleaseRxTransferPayload(&canard, transfer);
    }
  }
}

/**
 * This callback is invoked by the library when it detects beginning of a new transfer on the bus that can be received
 * by the local node.
 * If the callback returns true, the library will receive the transfer.
 * If the callback returns false, the library will ignore the transfer.
 * All transfers that are addressed to other nodes are always ignored.
 */
bool shouldAcceptTransfer(const CanardInstance* ins,
                                 uint64_t* out_data_type_signature,
                                 uint16_t data_type_id,
                                 CanardTransferType transfer_type,
                                 uint8_t source_node_id)
{
  (void)source_node_id;

  static bool accept_transfer;

  accept_transfer = false;

  if(transfer_type == CanardTransferTypeResponse)
  {
    // Add response handlers here

    //...
  }
  else if(transfer_type == CanardTransferTypeRequest)
  {
    // Add request handlers here

    //...
  }
  else if(transfer_type == CanardTransferTypeBroadcast)
  {
    if(data_type_id == NODE_STATUS_DATA_TYPE_ID)
    {
      *out_data_type_signature = NODE_STATUS_DATA_TYPE_SIGNATURE;
      accept_transfer = true;
    }
    else if(data_type_id == LOG_MESSAGE_DATA_TYPE_ID)
    {
      *out_data_type_signature = LOG_MESSAGE_DATA_TYPE_SIGNATURE;
      accept_transfer = true;
    }
    else if(data_type_id == KEY_VALUE_DATA_TYPE_ID)
    {
      *out_data_type_signature = KEY_VALUE_DATA_TYPE_SIGNATURE;
      accept_transfer = true;
    }
    else if(data_type_id == CAMERA_GIMBAL_STATUS_DATA_TYPE_ID)
    {
      *out_data_type_signature = CAMERA_GIMBAL_STATUS_DATA_TYPE_SIGNATURE;
      accept_transfer = true;
    }
  }

  return accept_transfer;
}
