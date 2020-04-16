# Libcanard (Version 1)

Contains libcanard source code files pulled from [UAVCAN/libcanard](https://github.com/UAVCAN/libcanard/tree/master/libcanard):
 * `canard.h`
 * `canard.c`
 * `canard_dsdl.h`
 * `canard_dsdl.c`

Also provides custom Serial debugging print statements for Canard objects.

## Serial Debugging

`void printCanardFrame(CanardFrame* frame)` - prints details about a Canard frame

`void printCanardTransfer(CanardTransfer* transfer)` - prints details about a Canard transfer

`void printCanardInstance(CanardInstance* ins)` - prints details about a Canard instance
