/**
 *  @file power_test.ino
 *
 *  Power consumption test code using Adafruit INA219 power sensors.
 *
 *  Supports up to four sensors with default configurable addresses.
 *
 *  Prints voltage, current, and power information to the serial monitor.
 *
 *  @author Joe DeFrance
 */

// Arduino library
#include <Arduino.h>

// Adafruit INA219 library
#include <Adafruit_INA219.h>

// Serial baudrate
#define SERIAL_BAUDRATE   9600

// Printing frequency
#define PRINT_DELAY_MS    250

// Adafruit INA219 configurable addresses
#define I2C_ADDRESS_1     0x40
#define I2C_ADDRESS_2     0x41
#define I2C_ADDRESS_3     0x44
#define I2C_ADDRESS_4     0x45

// Array of INA219 sensors
static Adafruit_INA219 sensors[4] =
{
  Adafruit_INA219(I2C_ADDRESS_1),
  Adafruit_INA219(I2C_ADDRESS_2),
  Adafruit_INA219(I2C_ADDRESS_3),
  Adafruit_INA219(I2C_ADDRESS_4)
};

// Global storage for sensor readings
static float voltage;
static float current;
static float power;

// Setup function called once before entering loop()
void setup()
{
  Serial.begin(SERIAL_BAUDRATE);
  while(!Serial);

  for(int n = 0; n < 4; n++)
  {
    Serial.print("Initializing sensor "); Serial.print(n); Serial.println("...");
    sensors[n].begin();
  }

  Serial.println("Initialization complete!");

  Serial.println("CSV Labels");
  Serial.println("ID (#), V (V), I (mA), P (mW)");
}

// Main loop
void loop()
{
  // Time reference
  static long time = millis();

  // Print information every PRINT_DELAY_MS miliseconds
  if((millis() - time) > PRINT_DELAY_MS)
  {
    // Read values from all four sensors
    for(int n = 0; n < 4; n++)
    {
      // Voltage (V)
      voltage = sensors[n].getBusVoltage_V();

      // Current (mA)
      current = sensors[n].getCurrent_mA();

      // Power (mW)
      power   = sensors[n].getPower_mW();

      // Ignore power readings with magnitude less than 1 mW
      if(power < 1 && power > -1)
      {
        continue;
      }

      // Print values as a comma seperated list including sensor number
      else
      {
        Serial.print(n);            Serial.print(", "); //< Number    (#)
        Serial.print(voltage, 3);   Serial.print(", "); //< Voltage   (V)
        Serial.print(current, 3);   Serial.print(", "); //< Current   (mA)
        Serial.print(power, 3);     Serial.print("\n"); //< Power     (mW)
      }

      // Ensure serial module has enough time to print
      delay(10);
    }

    // Update time reference
    time = millis();
  }
}
