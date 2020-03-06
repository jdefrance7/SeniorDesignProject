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
}

static float voltages[4];
static float currents[4];
static float netpower[4];

String line;

void setup()
{
  Serial.begin(SERIAL_BAUDRATE);
  while(!Serial);

  for(int n = 0; n < 4; n++)
  {
    sensors[n].begin();
  }

  line = "";

  line += "S1 Voltage (V), S1 Current (mA), S1 Power (mW), ";
  line += "S2 Voltage (V), S2 Current (mA), S2 Power (mW), ";
  line += "S3 Voltage (V), S3 Current (mA), S3 Power (mW), ";
  line += "S4 Voltage (V), S4 Current (mA), S4 Power (mW)\n";

  Serial.print(line);
}

void loop()
{
  static long time = millis();

  if((millis() - time) > PRINT_DELAY_MS)
  {
    line = "";

    for(int n = 0; n < 4; n++)
    {
      voltages[n] = sensors[n].getBusVoltage_V();
      currents[n] = sensors[n].getCurrent_mA();
      netpower[n] = sensors[n].getPower_mW();

      line += String(voltages[n], 2);
      line += ",";
      line += String(currents[n], 2);
      line += ",";
      line += String(netpower[n], 2);

      if(n != 3)
      {
        line += ",";
      }
      else
      {
        line += "\n";
      }
    }
    Serial.print(line);

    time = millis();
  }
}
