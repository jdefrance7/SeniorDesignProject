/*
  Print loop to test the power draw of up to four lines using Adafruit INA219 sensors.
*/

#include <Arduino.h>
#include <Adafruit_INA219.h>

#define SERIAL_BAUDRATE   9600

#define PRINT_DELAY_MS    250

#define I2C_ADDRESS_1     0x40
#define I2C_ADDRESS_2     0x41
#define I2C_ADDRESS_3     0x44
#define I2C_ADDRESS_4     0x45

static Adafruit_INA219 sensors[4] =
{
  Adafruit_INA219(I2C_ADDRESS_1),
  Adafruit_INA219(I2C_ADDRESS_2),
  Adafruit_INA219(I2C_ADDRESS_3),
  Adafruit_INA219(I2C_ADDRESS_4)
};

static float voltage;
static float current;
static float power;

String line;

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
  Serial.println("#, V, I, P");
}

void loop()
{
  static long time = millis();

  if((millis() - time) > PRINT_DELAY_MS)
  {
    for(int n = 0; n < 4; n++)
    {
      voltage = sensors[n].getBusVoltage_V();
      current = sensors[n].getCurrent_mA();
      power   = sensors[n].getPower_mW();

      if(power < 1)
      {
        continue;
      }
      else
      {
        Serial.print(n);        Serial.print(" (#)");     Serial.print(", ");
        Serial.print(voltage);  Serial.print(" (V)");     Serial.print(", ");
        Serial.print(current);  Serial.print(" (mA)");    Serial.print(", ");
        Serial.print(power);    Serial.print(" (mW)");    Serial.println("");
      }

      delay(10);
    }

    time = millis();
  }
}
