#include "BNO055.h"

#if defined(SENSOR_BNO055)

Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28);

bool init_sensors(void) {
  if(!bno.begin()) {
    return false;
  }
  bno.setExtCrystalUse(true);

  return true;
}

#endif

void setup_sensors(void) {}
