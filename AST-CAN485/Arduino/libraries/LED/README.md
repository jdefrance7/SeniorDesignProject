# LED

A simple LED class for hardware debugging.

## Defines

`DEFAULT_PIN` - builtin LED pin for most Arduino compatible boards (Pin 13)

## Functions

`void LED()` - default constructor, sets LED pin to `DEFAULT_PIN`

`void LED(pin)` - secondary constructor, sets LED pin to `pin`

`void setPin(pin)` - sets LED pin to `pin`

`void on()` - turns LED on

`void off()` - turns LED off

`void toggle()` - toggles LED state

`void toggle(long rate)` - toggles LED state if last call to this function was greater than `rate` milliseconds ago
