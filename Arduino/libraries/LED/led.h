#ifndef CUSTOM_LED
#define CUSTOM_LED

#include <Arduino.h>

#define DEFAULT_PIN   13
#define DEFAULT_RATE 500

class LED
{
public:
  LED();
  LED(int pin);
  void setPin(int pin);
  void on();
  void off();
  void toggle();
  void toggle(long rate);
private:
  int _pin;
  bool _state;
};

#endif // CUSTOM_LED
