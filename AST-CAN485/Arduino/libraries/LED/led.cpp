/**
 * @file led.cpp
 *
 * Basic LED class.
 *
 * @author Joe DeFrance
 */

#include "led.h"

LED::LED()
{
  _pin = DEFAULT_PIN;
}

LED::LED(int pin)
{
  _pin = pin;
}

void LED::setPin(int pin)
{
  _pin = pin;
}

void LED::on()
{
  _state = true;
  digitalWrite(_pin, _state);
}

void LED::off()
{
  _state = false;
  digitalWrite(_pin, _state);
}
void LED::toggle()
{
  _state = !_state;
  digitalWrite(_pin, _state);
}

void LED::toggle(long rate)
{
  static long last = millis();
  if(millis() - last > rate)
  {
    toggle();
    last = millis();
  }
}
