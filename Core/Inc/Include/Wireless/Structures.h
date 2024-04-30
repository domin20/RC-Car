#pragma once
#include <stdint.h>

#define MAX_WIRELESS_FRAME_DATA_SIZE 20
#define CRC_SIZE 2
#define MIN_FRAME_SIZE 4
#define FRAME_SIZE_WITHOUT_DATA 4

enum WirelessFrameState { ADDRESS = 0, COMMAND = 1, DATA_WITH_CRC = 2 };

#pragma pack(push, 1)

struct WirelessFrame {
  uint8_t master;
  uint8_t command;
  uint8_t dataSize;
  uint8_t data[MAX_WIRELESS_FRAME_DATA_SIZE];
};

struct ReceiverContext {
  uint8_t rxFrameIndex;
  volatile uint8_t rxFrame[255];
  volatile uint16_t rxFrameSize;
  bool isRxCompleted;
};

#pragma pack(pop)
