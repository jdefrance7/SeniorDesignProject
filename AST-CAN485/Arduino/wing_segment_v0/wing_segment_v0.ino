/**
 *  @file wing_segment_v0.ino
 *
 *  Setup and main loop code for the wing segment orientation sensor system.
 *
 *  Implements the following task structure:
 *
 *    Task 0 - Setup
 *      Initializes all hardware and software modules according to configuration file included.
 *
 *    Task 1 - Update IMU
 *      Updates the IMU processing filters for the LSM9DS1 and NXP_FXOS_FXAS. Ignored by BNO055.
 *
 *    Task 2 - Send Node Status
 *      Sends a UAVCAN formatted Node Status message onto the CAN bus.
 *
 *    Task 3 - Send Orientation
 *      Sends IMU orientation encoded in a UAVCAN formatted Angular Command message onto the CAN bus.
 *
 *    Task 4 - Cleanup Memory Pool
 *      Frees up stale CAN transfers in the Canard memory pool
 *
 *  @author Joe DeFrance
 */

//******************************************************************************
// Select configuration file (uncomment one)

#include "wing_segment_config_0.h"
//#include "wing_segment_config_1.h"
//#include "wing_segment_config_2.h"
//#include "wing_segment_config_3.h"

//******************************************************************************

// Error if no config selected
#ifndef WING_SEGMENT_CONFIG_H
  #error Must select a config.
#endif

// Arduino constants
#include <Arduino.h>

// Status LED
#include <led.h>

// Include IMU chosen in config, error if no IMU selected
#if defined(IMU_BNO055)
  #include <bno055.h>
#elif defined(IMU_LSM9DS1)
  #include <lsm9ds1.h>
#elif defined(IMU_NXP_FXOS_FXAS)
  #include <nxp_fxos_fxas.h>
#else // NONE
  #error Must select an IMU in the config.
#endif

// CAN driver
#include <ast_can_v0.h>

// UAVCAN version
#include <uavcan_v0.h>

// Custom UAVCAN node instance
UavcanNode node;

// Custom Canard struct instance
Canard can;

// Builtin LED instance
LED led;

void setup()
{
  //////////////////////////////////////////////////////////////////////////////
  /// Task 0 - Setup
  //////////////////////////////////////////////////////////////////////////////

  // Initialize LED
  led = LED();

  // Initialize CAN
  {
    can.bitrate = CAN_BITRATE;
  }

  // Initialize UAVCAN Node
  // Node basics
  {
    node.id = NODE_ID;
    memset(node.name, 0, sizeof(node.name));
    memcpy(node.name, NODE_NAME, sizeof(NODE_NAME));
  }
  // Node status
  {
    node.status.uptime_sec = 0;
    node.status.health = HEALTH_OK;
    node.status.mode = MODE_INITIALIZATION;
    node.status.sub_mode = 0;
    node.status.vendor_specific_status_code = 0;
  }
  // Node hardware version
  {
    node.hardware.major = HARDWARE_VERSION_MAJOR;
    node.hardware.minor = HARDWARE_VERSION_MINOR;
    memset(node.hardware.unique_id, 0, sizeof(node.hardware.unique_id));
    memcpy(node.hardware.unique_id, HARDWARE_UNIQUE_ID, sizeof(HARDWARE_UNIQUE_ID));
    memset(node.hardware.certificate, 0, sizeof(node.hardware.certificate));
    memcpy(node.hardware.certificate, HARDWARE_CERTIFICATE, sizeof(HARDWARE_CERTIFICATE));
  }
  // Node software version
  {
    node.software.major = SOFTWARE_VERSION_MAJOR;
    node.software.minor = SOFTWARE_VERSION_MINOR;
    node.software.optional_field_flags = SOFTWARE_OPTIONAL_FIELD_FLAGS;
    node.software.vcs_commit = SOFTWARE_VCS_COMMIT;
    node.software.image_crc = SOFTWARE_IMAGE_CRC;
  }

  // Start CAN peripheral
  init_can(&can, node.id);
  while(can.canard.node_id == 0)
  {
    led.toggle(LED_FAST);
  }
  led.off();

  // Start IMU peripheral
  while(init_imu() != 0)
  {
    led.toggle(LED_MEDIUM);
  }
  led.off();

  // Serial debugging
  #ifdef SERIAL_DEBUG
  {
    // Start Serial peripheral
    Serial.begin(SERIAL_BAUDRATE);
    while(!Serial)
    {
      led.toggle(LED_SLOW);
    }
    led.off();

    // Print loaded configs
    Serial.println("\nWing Segment Config");
    printCanard(&can);
    printNode(&node);

    // Let the user read the info
    delay(1000);
  }
  #endif // SERIAL_DEBUG

  // Setup complete
  node.status.mode = MODE_OPERATIONAL;
  node.status.uptime_sec = millis()/1000;
}

void loop()
{
  //////////////////////////////////////////////////////////////////////////////
  /// Task 1: Update IMU
  //////////////////////////////////////////////////////////////////////////////

  // Millisecond time reference for performing task
  static uint64_t update_imu_time = millis();

  // Check that the desired time period has passed
  if((millis() - update_imu_time) >= UPDATE_IMU_PERIOD_MS)
  {
    // Period since last call
    long period = long(millis() - update_imu_time);

    // Update the IMU's filters
    int update = update_imu();

    if(update < 0) // Failure
    {
      node.status.mode = HEALTH_WARNING;
      led.toggle(LED_DEFAULT);
    }
    else // Success
    {
      node.status.mode = HEALTH_OK;
      led.off();
    }

    // Update time reference
    update_imu_time = millis();

    // Serial debugging
    #ifdef SERIAL_DEBUG
    {
      Serial.print("\nUpdate IMU - Period: ");  Serial.println(period);
      Serial.print("\nUpdate IMU - Value: ");   Serial.println(update);
    }
    #endif // SERIAL_DEBUG
  }

  //////////////////////////////////////////////////////////////////////////////
  /// Task 2: Send Node Status
  //////////////////////////////////////////////////////////////////////////////

  // Millisecond time reference for performing task
  static uint64_t send_node_status_time = millis();

  // Check that the desired time period has passed
  if((millis() - send_node_status_time) >= SEND_NODE_STATUS_PERIOD_MS)
  {
    // Task Variables
    int encoding = -1;  //< number of bits encoded in data buffer
    int queuing = -1;   //< number of CAN frames pushed to Canard message queue
    int transmit = -1;  //< number of CAN frames transmitted

    // Period since last call
    long period = long(millis()-send_node_status_time);

    // Update node uptime in seconds
    node.status.uptime_sec = millis()/1000;

    // Unique ID for Node Status transfers
    static uint8_t node_status_transfer_id = 0;

    // Create data field buffer
    uint8_t buffer[((NODE_STATUS_DATA_TYPE_SIZE + 7) / 8)];

    // Clear data field buffer
    memset(buffer, 0, sizeof(buffer));

    // Encode data field buffer
    encoding = encode_node_status(buffer, sizeof(buffer), 0, node.status);

    if(encoding > 0) // Success
    {
      // Format and push message frame(s) onto Canard queue
      queuing = canardBroadcast(
        &can.canard,
        NODE_STATUS_DATA_TYPE_SIGNATURE,
        NODE_STATUS_DATA_TYPE_ID,
        &node_status_transfer_id,
        CANARD_TRANSFER_PRIORITY_MEDIUM,
        buffer,
        sizeof(buffer)
      );

      if(queuing > 0) // Success
      {
        // Transmit all frames in Canard queue
        transmit = transmitCanardQueue(&can.canard, TRANSMIT_DELAY, TRANSMIT_TIMEOUT);

        if(transmit > 0) // Success
        {
          node.status.mode = HEALTH_OK;
          led.off();
        }
      }
    }

    if(encoding < 0 || queuing < 0 || transmit < 0) // Failure
    {
      node.status.mode = HEALTH_WARNING;
      led.toggle(LED_DEFAULT);
    }

    // Update time reference
    send_node_status_time = millis();

    // Serial debugging
    #ifdef SERIAL_DEBUG
    {
      Serial.print("\nNode Status - Period: "); Serial.println((long)(millis()-send_node_status_time));
      printNodeStatus(&node.status);
      Serial.print("\nNode Status - Encoding: "); Serial.println(encoding);
      if(encoding > 0)
      {
        Serial.print("\nNode Status - Buffer: ");
        for(int n = 0; n < (encoding+7)/8; n++)
        {
          Serial.print(buffer[n], HEX);
          if(n == (sizeof(buffer)-1))
          {
            Serial.print("\n");
            break;
          }
          Serial.print(",");
        }
      }
      Serial.print("\nNode Status - Queuing: "); Serial.println(queuing);
      Serial.print("\nNode Status - Transmit: "); Serial.println(transmit);
    }
    #endif // SERIAL_DEBUG
  }

  //////////////////////////////////////////////////////////////////////////////
  /// Task 3: Send Orientation
  //////////////////////////////////////////////////////////////////////////////

  // Millisecond time reference for performing task
  static uint64_t send_angular_command = millis();

  // Check that the desired time period has passed
  if((millis() - send_angular_command) >= SEND_ORIENTATION_PERIOD_MS)
  {
    // Task Variables
    int encoding = -1;  //< number of bits encoded in data buffer
    int queuing = -1;   //< number of CAN frames pushed to Canard message queue
    int transmit = -1;  //< number of CAN frames transmitted

    uint16_t payload_len = 0; //< length (in bytes) of serialized CAN data

    // Period since last call
    long period = long(millis()-send_angular_command);

    // Create AngularCommand data structure
    AngularCommand angles;

    // Intialize AngularCommand with Quaternion
    #if defined(SEND_ORIENTATION_AS_QUATERNION)
    {
      // Set AngularCommand gimbal_id as Wing Segment number
      angles.gimbal_id = WING_SEGMENT;

      // Set AngularCommand mode to COMMAND_MODE_ANGULAR_VELOCITY
      angles.mode = 0;

      // Set AngularCommand quaterion_xyzw to IMU quaternion
      angles.quaternion_xyzw[0] = quaternion(X_AXIS);
      angles.quaternion_xyzw[1] = quaternion(Y_AXIS);
      angles.quaternion_xyzw[2] = quaternion(Z_AXIS);
      angles.quaternion_xyzw[3] = quaternion(W_AXIS);
    }

    // Intialize AngularCommand with Euler Vector
    #elif defined(SEND_ORIENTATION_AS_EULER_VECTOR)
    {
      // Set AngularCommand gimbal_id as Wing Segment number
      angles.gimbal_id = WING_SEGMENT;

      // Set AngularCommand mode to COMMAND_MODE_ORIENTATION_FIXED_FRAME
      angles.mode = 1;

      // Set AngularCommand quaterion_xyzw to IMU orientation
      angles.quaternion_xyzw[0] = orientation(X_AXIS);
      angles.quaternion_xyzw[1] = orientation(Y_AXIS);
      angles.quaternion_xyzw[2] = orientation(Z_AXIS);
      angles.quaternion_xyzw[3] = 0;
    }

    // Error if no AngularCommand formatting chosen
    #else
    {
      #error Must select orientation type in the config.
    }
    #endif

    // Unique ID for angular command transfers
    static uint8_t angular_command_transfer_id = 0;

    // Create data field buffer
    uint8_t buffer[((ANGULAR_COMMAND_DATA_TYPE_SIZE + 7) / 8)];

    // Clear data field buffer
    memset(buffer, 0, sizeof(buffer));

    // Encode data field buffer
    encoding = encode_angular_command(buffer, sizeof(buffer), 0, angles);

    if(encoding > 0) // Success
    {
      // Payload length in bytes (rounded up)
      payload_len = (encoding + 7) / 8;

      // Format and push message frame(s) onto Canard queue
      queuing = canardBroadcast(
        &can.canard,
        ANGULAR_COMMAND_DATA_TYPE_SIGNATURE,
        ANGULAR_COMMAND_DATA_TYPE_ID,
        &angular_command_transfer_id,
        CANARD_TRANSFER_PRIORITY_MEDIUM,
        buffer,
        payload_len
      );

      if(queuing > 0) // Success
      {
        // Transmit all frames in Canard queue
        transmit = transmitCanardQueue(&can.canard, TRANSMIT_DELAY, TRANSMIT_TIMEOUT);

        if(transmit > 0) // Success
        {
          node.status.mode = HEALTH_OK;
          led.off();
        }
      }
    }

    if(encoding < 0 || queuing < 0 || transmit < 0) // Failure
    {
      node.status.mode = HEALTH_WARNING;
      led.toggle(LED_DEFAULT);
    }

    // Update time reference
    send_angular_command = millis();

    // Serial debugging
    #ifdef SERIAL_DEBUG
    {
      Serial.print("\nAngular Command - Period: ");   Serial.println((long)(millis()-send_angular_command));
      printAngularCommand(&angles);
      Serial.print("\nAngular Command - Encoding: "); Serial.println(encoding);
      if(encoding > 0)
      {
        Serial.print("\nAngular Command - Buffer: ");
        for(int n = 0; n < (encoding+7)/8; n++)
        {
          Serial.print(buffer[n], HEX);
          if(n == (sizeof(buffer)-1))
          {
            Serial.print("\n");
            break;
          }
          Serial.print(",");
        }
      }
      Serial.print("\nAngular Command - Queuing: ");  Serial.println(queuing);
      Serial.print("\nAngular Command - Transmit: "); Serial.println(transmit);
    }
    #endif // SERIAL_DEBUG
  }

  //////////////////////////////////////////////////////////////////////////////
  /// Task 4: Cleanup UAVCAN
  //////////////////////////////////////////////////////////////////////////////

  // Millisecond time reference for performing task
  static uint64_t cleanup_uavcan_time = millis();

  // Check that the desired time period has passed
  if((millis() - cleanup_uavcan_time) > CLEANUP_UAVCAN_PERIOD_MS)
  {
    // Removes stale transfers from Canard queue based on microsecond timestamp
    canardCleanupStaleTransfers(&can.canard, 1000*millis());

    // Update time reference
    cleanup_uavcan_time = millis();

    // Serial debugging
    #ifdef SERIAL_DEBUG
    {
      CanardPoolAllocatorStatistics stats = canardGetPoolAllocatorStatistics(&can.canard);
      printCanardPoolAllocatorStatistics(&stats);
    }
    #endif // SERIAL_DEBUG
  }
}
