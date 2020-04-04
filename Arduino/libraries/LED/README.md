# LED

A simple LED class for hardware debugging.

## Functions

`LED()` - default constructor, sets LED pin to DEFAULT_PIN (13)

`LED(pin)` - secondary constructor, sets LED pin to `pin`

`setPin(pin)` - sets LED pin to `pin`

`on()` - turns LED on

`off()` - turns LED off

`toggle()` - toggles LED state

`toggle(rate)` - toggles LED state if last call to this function was greater than `rate` milliseconds ago
