#ifndef CUSTOM_LED
#define CUSTOM_LED

#include <Arduino.h>

#define DEFAULT_PIN   13

// Basic LED class for hardware debugging
class LED
{
public:
  
  // Default constructor, sets pin to DEFAULT_PIN
  LED();
  
  // Constructor with pin declaration
  LED(int pin);
  
  // Set LED pin
  void setPin(int pin);
  
  // Turn LED on
  void on();
  
  // Turn LED off
  void off();
  
  // Toggle LED state
  void toggle();
  
  // Toggle LED state if elapsed time since last call is greater than rate (ms)
  void toggle(long rate);
  
private:
  
  // LED pin
  int _pin;
  
  // LED state
  bool _state;
};

#endif // CUSTOM_LED
