#include <imu.h>

#define SERIAL_BAUDRATE 9600

#define PRINT_DELAY_MS  1000

void printOrientation();
void printQuaternion();
void printAngularVelocity();
void printLinearAcceleration();
void printMagneticField();
void printTemperature();

void setup()
{
  Serial.begin(SERIAL_BAUDRATE);
  while(!Serial);

  init_imu();
}

void loop()
{
  #if defined(FILTER_UPDATE_RATE_HZ)

  static long last_update = millis();
  if((millis() - last_update) > (1000/FILTER_UPDATE_RATE_HZ))
  {
    update_imu();
    last_update = millis();
  }

  #endif

  static long last_print = millis();
  if((millis()-last_print) > PRINT_DELAY_MS)
  {
    printOrientation();
    printQuaternion();
    printAngularVelocity();
    printLinearAcceleration();
    printMagneticField();
    printTemperature();

    last_print = millis();
  }
}

void printOrientation()
{
  Serial.println("\nORIENTATION")
  Serial.print("Roll: ");
  Serial.println(orientation(ROLL_AXIS);
  Serial.print("Pitch: ");
  Serial.println(orientation(PITCH_AXIS));
  Serial.print("Yaw: ");
  Serial.println(orientation(YAW_AXIS));
}

void printQuaternion()
{
  Serial.println("\nQUATERNION");
  Serial.print("X: ");
  Serial.println(quaternion(X_AXIS));
  Serial.print("Y: ");
  Serial.println(quaternion(Y_AXIS));
  Serial.print("Z: ");
  Serial.println(quaternion(Z_AXIS));
  Serial.print("W: ");
  Serial.println(quaternion(W_AXIS));
}

void printAngularVelocity()
{
  Serial.println("\nANGULAR VELOCITY");
  Serial.print("Roll: ");
  Serial.println(angularVelocity(X_AXIS));
  Serial.print("Pitch: ");
  Serial.println(angularVelocity(Y_AXIS));
  Serial.print("Yaw: ");
  Serial.println(angularVelocity(Z_AXIS));
}

void printLinearAcceleration()
{
  Serial.println("\nLINEAR ACCELERATION")
  Serial.print("X: ");
  Serial.println(linearAcceleration(X_AXIS));
  Serial.print("Y: ");
  Serial.println(linearAcceleration(Y_AXIS));
  Serial.print("Z: ");
  Serial.println(linearAcceleration(Z_AXIS));
}

void printMagneticField()
{
  Serial.println("\nMAGNETIC FIELD");
  Serial.print("X: ");
  Serial.println(magneticField(X_AXIS));
  Serial.print("Y: ");
  Serial.println(magneticField(Y_AXIS));
  Serial.print("Z: ");
  Serial.println(magneticField(Z_AXIS));
}

void printTemperature()
{
  Serial.println("\nTEMPERATURE");
  Serial.print("C: ");
  Serial.println(temperature(TEMPERATURE_CELSIUS));
  Serial.print("K: ");
  Serial.println(temperature(TEMPERATURE_KELVIN));
}
