# CAN Transmitter Test

Tests the transmitting operation of the [AST-CAN485](https://www.sparkfun.com/products/14483) CAN transceiver and the encoding of [UAVCAN v0](https://legacy.uavcan.org/) messages.

Examples of transmitted messages are included below with extra debug information.

## Node Status

```
Node Status
  Uptime Sec: 4
  Health: 0
  Mode: 0
  Sub Mode: 0
  Vendor: 0
Node Status Encoded Buffer: 4,0,0,0,0,0,0
Node Status Encoded Bits: 56
Node Status encoding successful!

Canard Frame
  ID: 2416006422
  Length: 8
  Data: 4,0,0,0,0,0,0,C1
Node Status queueing successful!
Node Status transmission successful!
```

## Log Message

```
Quaternion Text: 1.00,0.00,0.02,0.00

Log Message
  Level: 1
  Source: #.segment.wing
  Text: 1.00,0.00,0.02,0.00

Log Message Encoded Buffer: 24,65,CE,6C,AC,ED,AC,AD,CE,85,CE,ED,2D,CC,E0,6,25,C6,6,5,86,5,C6,6,5,86,5,C6,6,45,86,5,C6,6,1,40,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
Log Message Encoded Bits: 291
Log Message encoding successful!
Log Message queueing successful!
LogMessage transmission successful!
```

## Camera Gimbal Status

```
Camera Gimbal Status
  Gimbal ID: 22
  Mode: 1
  Orientation X: 0.00
  Orientation Y: 0.02
  Orientation Z: 0.00
  Orientation W: 1.00
Camera Gimbal Status Encoded Buffer: 16,1,0,0,0,0,0,0,0,F4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
Camera Gimbal Status Encoded Bits: 224
Camera Gimbal Status queueing successful.
Camera Gimbmal Status transmission successful!
```

## Key Value

```
Key Value
  Value: 1.00
  Key: W
Key Value Encoded Buffer: 0,F4,7F,3F,57,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
Key Value Encoded Bits: 48

Canard Frame
  ID: 2420109846
  Length: 7
  Data: 0,F4,7F,3F,57,0,D3

Key Value queueing successful!
Key Value transmission successful!

Key Value
  Value: 0.00
  Key: X
Key Value Encoded Buffer: 0,0,28,3B,58,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
Key Value Encoded Bits: 48

Canard Frame
  ID: 2420109846
  Length: 7
  Data: 0,0,28,3B,58,0,D4

Key Value queueing successful!
Key Value transmission successful!

Key Value
  Value: 0.02
  Key: Y
Key Value Encoded Buffer: 0,80,9E,3C,59,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
Key Value Encoded Bits: 48

Canard Frame
  ID: 2420109846
  Length: 7
  Data: 0,80,9E,3C,59,0,D5

Key Value queueing successful!
Key Value transmission successful!

Key Value
  Value: 0.00
  Key: Z
Key Value Encoded Buffer: 0,0,0,0,5A,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
Key Value Encoded Bits: 48

Canard Frame
  ID: 2420109846
  Length: 7
  Data: 0,0,0,0,5A,0,D6

Key Value queueing successful!
Key Value transmission successful!
```
