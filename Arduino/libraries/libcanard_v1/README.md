# Libcanard (Version 1)

Contains libcanard source code files 
 * `canard.h`
 * `canard.c`
 * `canard_dsdl.h`
 * `canard_dsdl.c`
 Pulled from [UAVCAN/libcanard](https://github.com/UAVCAN/libcanard/tree/master/libcanard).

Also provides custom Serial debugging print statements for Canard objects.

## Serial Debugging

`void printCanardFrame(...)` - prints details about a Canard frame

`void printCanardTransfer(...)` - prints details about a Canard transfer

`void printCanardInstance(...)` - prints details about a Canard instance
