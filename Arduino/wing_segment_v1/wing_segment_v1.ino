// Serial Debugging
#define SERIAL_DEBUG // uncomment for debug information
#define SERIAL_BAUDRATE 9600

// Core Arduino Library
#include <Arduino.h>

// Custom LED Library
#include <led.h>

// Select IMU Sensor
#include <bno055.h>
// #include <lsm9ds1.h>
// #include <nxp_fxos_fxas.h>

// Select CAN Driver
#include <ast_can_v1.h>

// UAVCAN Version
#include <uavcan_v1.h>

// LED Toggle Rate (ms)
#define LED_TOGGLE 300

// CAN Bitrate
#define CAN_BITRATE 500000

 // CAN Timeout
#define TRANSMIT_TIMEOUT 5

// Node Information
#define NODE_ID   22
#define NODE_NAME "#.segment.wing"

// Protocol Information
#define UAVCAN_PROTOCOL_MAJOR 1
#define UAVCAN_PROTOCOL_MINOR 0

// Hardware Information
#define HARDWARE_VERSION_MAJOR 1
#define HARDWARE_VERSION_MINOR 0

#define HARDWARE_UNIQUE_ID    "123456789ABCDEF"
#define HARDWARE_CERTIFICATE  "Certificate"

// Software Information
#define SOFTWARE_VERSION_MAJOR 1
#define SOFTWARE_VERSION_MINOR 0

#define SOFTWARE_VCS_REVISION_ID  0
#define SOFTWARE_IMAGE_CRC        0

// Task Periods
#define UPDATE_IMU_PERIOD_MS        (1000 / FILTER_UPDATE_RATE_HZ)
#define SEND_HEARTBEAT_PERIOD_MS    (1000 * MAX_PUBLICATION_PERIOD / 2)
#define SEND_ORIENTATION_PERIOD_MS  (10)

// UAVCAN Node Instance
UavcanNode node;

// Canard Can Instance
Canard can;

// Builtin LED
LED led;

// Return value for error handling
inf reVal;

void setup()
{
  // Init LED
  led = LED();

  // Setup CAN
  can.bitrate = CAN_BITRATE;
  can.canard.mtu_bytes = CANARD_MTU_CAN_CLASSIC;

  // Setup UAVCAN Node
  // Node ID
  {
    node.id = NODE_ID;
  }
  // Node Status
  {
    node.status.uptime = 0;
    node.status.health = HEALTH_NOMINAL;
    node.status.mode = MODE_INITIALIZATION;
    node.status.vendor_specific_status_code = 0;
  }
  // Node Info
  {
    node.info.protocol_version.major = UAVCAN_PROTOCOL_MAJOR;
    node.info.protocol_version.minor = UAVCAN_PROTOCOL_MINOR;
    node.info.hardware_version.major = HARDWARE_VERSION_MAJOR;
    node.info.hardware_version.minor = HARDWARE_VERSION_MINOR;
    node.info.software_version.major = SOFTWARE_VERSION_MAJOR;
    node.info.software_version.minor = SOFTWARE_VERSION_MINOR;
    node.info.software_vcs_revision_id = SOFTWARE_VCS_REVISION_ID;
    memset(node.info.unique_id, 0, sizeof(node.info.unique_id));
    memcpy(node.info.unique_id, HARDWARE_UNIQUE_ID, sizeof(HARDWARE_UNIQUE_ID));
    memset(node.info.name, 0, sizeof(node.info.name));
    memcpy(node.info.name, NODE_NAME, sizeof(NODE_NAME));
    node.info.software_image_crc = SOFTWARE_IMAGE_CRC;
    memset(node.info.certificate, 0, sizeof(node.info.certificate));
    memcpy(node.info.certificate, HARDWARE_CERTIFICATE, sizeof(HARDWARE_CERTIFICATE));
  }

  // Initialize CAN
  while(init_can(can, node.id) != 0)
  {
    led.toggle(LED_TOGGLE);
  }
  led.off()

  // Initialize IMU
  while(init_imu() != 0)
  {
    led.toggle(LED_TOGGLE);
  }
  led.off();

  // Initialization Complete!
  node.status.uptime = millis()/1000;
  node.status.mode = MODE_OPERATIONAL;

  // Serial debugging
  #if defined(SERIAL_DEBUG)

  // Start Serial module
  Serial.begin(SERIAL_BAUDRATE);
  while(!Serial)
  {
    led.toggle(LED_TOGGLE);
  }
  led.off();

  // Print initialization information
  Serial.println("\nWing Segment Config\n");
  printCanard(can);
  printNode(node);
  Serial.println("\nInitialization complete!");

  #endif // SERIAL_DEBUG
}

void loop()
{
  /*
    Task: Update IMU
    Note: IMU's running AHRS filters need to be updated for calculations.
  */
  static uint64_t update_imu_time = millis();
  if((millis() - update_imu_time) > UPDATE_IMU_PERIOD_MS)
  {
    // Update IMU's filters
    reVal = update_imu();

    if(reVal < 0)
    {
      // Serial debugging
      #if defined(SERIAL_DEBUG)
      Serial.print("ERROR: Failed to update IMU, reVal = ");
      Serial.println(reVal);
      #endif // SERIAL_DEBUG

      led.toggle(LED_TOGGLE);
    }
    else
    {
      // Serial debugging
      #if defined(SERIAL_DEBUG)
      Serial.println("IMU update successful!");
      #endif // SERIAL_DEBUG

      led.off();
    }

    // Update time reference
    update_imu_time = millis();
  }

  /*
    Task: Send Heartbeat
  */
  static uint64_t heartbeat_time = millis();
  if((millis() - heartbeat_time) > SEND_HEARTBEAT_PERIOD_MS)
  {
    // Serial debugging
    #if defined(SERIAL_DEBUG)
    printHeartbeat(node.status);
    #endif // SERIAL_DEBUG

    // Unique ID for heartbeat transfers
    static uint8_t heartbeat_transfer_id = 0;

    // Create data field buffer
    uint8_t buffer[(HEARTBEAT_DATA_TYPE_SIZE / 8)];

    // Clear data field buffer
    memset(buffer, 0, sizeof(buffer));

    // Encode data field buffer
    reVal = encode_heartbeat(buffer, sizeof(buffer), 0, node.status)

    if(reVal < 0)
    {
      // Serial debugging
      #if defined(SERIAL_DEBUG)
      Serial.print("ERROR: Failed to encode Heartbeat, reVal = "); Serial.println(reVal);
      #endif // SERIAL_DEBUG

      led.toggle(LED_TOGGLE);
    }
    else
    {
      // Serial debugging
      #if defined(SERIAL_DEBUG)
      Serial.print("Heartbeat Encoded Buffer: ");
      for(unsigned int n = 0; n < sizeof(buffer); n++)
      {
        Serial.print(buffer[n], HEX);
        if(n == (sizeof(buffer)-1))
        {
          Serial.print("\n");
          break;
        }
        Serial.print(",");
      }
      Serial.print("Heartbeat Encoded Bits: "); Serial.print(reVal);
      Serial.println("Heartbeat encoding successful!");
      #endif // SERIAL_DEBUG

      led.off();
    }

    // Create Canard transfer
    CanardTransfer transfer;

    // Set transfer timestamp
    transfer.timestamp_usec = 1000*millis();

    // Set transfer priority
    transfer.priority = CanardPriorityNominal;

    // Set transfer kind
    transfer.transfer_kind = CanardTransferKindMessage;

    // Set data type id
    transfer.port_id = HEARTBEAT_DATA_TYPE_ID;

    // messages ignore transfer.remote_node_id

    // Set transfer id
    transfer.transfer_id = heartbeat_transfer_id;

    // Set transfer size
    transfer.payload_size = sizeof(buffer);

    // Set transfer data
    transfer.payload = buffer;

    // Format and push message frame(s) onto Canard queue
    reVal = canardTxPush(
      &can.canard,
      &transfer
    );

    if(reVal < 0)
    {
      // Serial debugging
      #if defined(SERIAL_DEBUG)
      Serial.print("ERROR: Failed to queue Heartbeat, reVal = "); Serial.println(reVal);
      #endif // SERIAL_DEBUG

      led.toggle(LED_TOGGLE);
    }
    else
    {
      // Serial debugging
      #if defined(SERIAL_DEBUG)
      Serial.print("Heartbeat ");
      printCanardTransfer(&transfer);
      #endif // SERIAL_DEBUG

      led.off();
    }

    // Transmit all frames in Canard queue
    reVal = transmitCanardQueue(&can.canard, TRANSMIT_TIMEOUT);

    if(reVal < 0)
    {
      // Serial debugging
      #if defined(SERIAL_DEBUG)
      Serial.print("ERROR: Failed to transmit Heartbeat, reVal = "); Serial.println(reVal);
      #endif // SERIAL_DEBUG

      led.toggle(LED_TOGGLE);
    }
    else
    {
      // Serial debugging
      #if defined(SERIAL_DEBUG)
      Serial.println("Heartbeat transmission successful!");
      #endif // SERIAL_DEBUG

      led.off();
    }

    // Update time reference
    heartbeat_time = millis();
  }

  /*
    Task: Send Orientation as Record
  */
  static uint64_t send_orientation_time = millis();
  if((millis() - send_orientation_time) > SEND_ORIENTATION_PERIOD_MS)
  {
    // Create and store quaternion unit values
    Quaternion quat;
    quat.wxyz[QW_INDEX] = quaternion(W_AXIS);
    quat.wxyz[QX_INDEX] = quaternion(X_AXIS);
    quat.wxyz[QY_INDEX] = quaternion(Y_AXIS);
    quat.wxyz[QZ_INDEX] = quaternion(Z_AXIS);

    // Serial debugging
    #if defined(SERIAL_DEBUG)
    printQuaternion(quat);
    #endif // SERIAL_DEBUG

    // Format quaternion into comma separated list
    String text = "";
    text += String(quat.wxyz[QW_INDEX], 4); text += ",";
    text += String(quat.wxyz[QX_INDEX], 4); text += ",";
    text += String(quat.wxyz[QY_INDEX], 4); text += ",";
    text += String(quat.wxyz[QZ_INDEX], 4); text += "\n";

    // Serial debugging
    #if defined(SERIAL_DEBUG)
    Serial.print("Quaternion Text: ");
    Serial.println(text);
    #endif // SERIAL_DEBUG

    // Create Record data structure
    Record record;

    // Set Record timestamp
    record.timestamp = 0;

    // Set Record severity
    record.severity = INFO;

    // Set Record text
    memset(record.text, 0, RECORD_TEXT_SIZE);
    memcpy(record.text, text.c_str(), text.length());

    // Serial debugging
    #if defined(SERIAL_DEBUG)
    printRecord(record);
    #endif // SERIAL_DEBUG

    // Unique ID for Record transfers
    static uint8_t orientation_transfer_id = 0;

    // Create data field buffer
    uint8_t buffer[(RECORD_DATA_TYPE_SIZE / 8)];

    // Clear data field buffer
    memset(buffer, 0, sizeof(buffer));

    // Encode data field buffer
    reVal = encode_record(buffer, sizeof(buffer), 0, record);

    if(reVal < 0)
    {
      // Serial debugging
      #if defined(SERIAL_DEBUG)
      Serial.print("ERROR: Failed to encode Record, reVal = "); Serial.println(reVal);
      #endif // SERIAL_DEBUG

      led.toggle(LED_TOGGLE);
    }
    else
    {
      // Serial debugging
      #if defined(SERIAL_DEBUG)
      Serial.print("Record Encoded Buffer: ");
      for(unsigned int n = 0; n < sizeof(buffer); n++)
      {
        Serial.print(buffer[n], HEX);
        if(n == (sizeof(buffer)-1))
        {
          Serial.print("\n");
          break;
        }
        Serial.print(",");
      }
      Serial.print("Record Encoded Bits: "); Serial.println(reVal);
      Serial.println("Record encoding successful!");
      #endif // SERIAL_DEBUG

      led.off();
    }

    // Create Canard transfer
    CanardTransfer transfer;

    // Set transfer timestamp
    transfer.timestamp_usec = 1000*millis();

    // Set transfer priority
    transfer.priority = CanardPriorityNominal;

    // Set transfer kind
    transfer.transfer_kind = CanardTransferKindMessage;

    // Set transfer data type id
    transfer.port_id = RECORD_DATA_TYPE_ID;

    // messages ignore transfer.remote_node_id

    // Set transfer id
    transfer.transfer_id = orientation_transfer_id;

    // Set transfer size
    transfer.payload_size = sizeof(buffer);

    // Set transfer data
    transfer.payload = buffer;

    // Format and push message frame(s) onto Canard queue
    reVal = canardTxPush(
      &can.canard,
      &transfer
    );

    if(reVal < 0)
    {
      // Serial debugging
      #if defined(SERIAL_DEBUG)
      Serial.print("ERROR: Failed to queue Record, reVal = "); Serial.println(reVal);
      #endif // SERIAL_DEBUG

      led.toggle(LED_TOGGLE);
    }
    else
    {
      // Serial debugging
      #if defined(SERIAL_DEBUG)
      Serial.print("Record ");
      printCanardTransfer(&transer);
      #endif // SERIAL_DEBUG

      led.off();
    }

    // Transmit all frames in Canard queue
    reVal = transmitCanardQueue(&can.canard, TRANSMIT_TIMEOUT);

    if(r < 0)
    {
      // Serial debugging
      #if defined(SERIAL_DEBUG)
      Serial.print("ERROR: Failed to transmit Record, reVal = "); Serial.println(reVal);
      #endif // SERIAL_DEBUG

      led.toggle(LED_TOGGLE);
    }
    else
    {
      // Serial debugging
      #if defined(SERIAL_DEBUG)
      Serial.println("Record transmission successful!");
      #endif // SERIAL_DEBUG

      led.off();
    }

    // Update time reference
    send_orientation_time = millis();
  }
}
