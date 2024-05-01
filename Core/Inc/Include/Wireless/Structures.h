#pragma once
#include <stdint.h>

#include "Types.h"

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

#pragma pack(pop)
