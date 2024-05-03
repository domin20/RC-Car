#pragma once
#include <stdint.h>

#include "Types.h"

enum SpeedDirection { FORWARD = 0, REVERSE = 1 };
enum TurnDirection { LEFT = 0, RIGHT = 1 };

#pragma pack(push, 1)

struct WirelessFrame {
  uint8_t master;
  uint8_t command;
  uint8_t dataSize;
  uint8_t data[MAX_WIRELESS_FRAME_DATA_SIZE];
  uint16_t crc16;
};

struct ReceiverContext {
  uint8_t rxByte;
  volatile uint8_t rxFrame[MAX_FRAME_SIZE];
  volatile uint16_t rxFrameSize;
  bool isRxCompleted;
};

struct SteeringData {
  uint8_t speedJoystickDeflection;
  uint8_t speedJoystickDirection;
  uint8_t turnJoystickDeflection;
  uint8_t turnJoystickDirection;
};

#pragma pack(pop)
