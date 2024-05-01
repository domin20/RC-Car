#pragma once
#include "Config/StaticConfig.h"

#define CRC_SIZE 2
#define MIN_FRAME_SIZE 4
#define FRAME_SIZE_WITHOUT_DATA 4

#define MAX_WIRELESS_FRAME_DATA_SIZE 30
#define MAX_FRAME_SIZE 255

constexpr uint32_t ONE_CHAR_SEND_TIME_US = (1000000 / WIRELESS_BAUDRATE);
constexpr uint32_t _1_5_CHAR_SEND_TIME_US = (1.5f * ONE_CHAR_SEND_TIME_US);
constexpr uint32_t _3_5_CHAR_SEND_TIME_US = (3.5f * _1_5_CHAR_SEND_TIME_US);

enum WirelessFrameState { ADDRESS = 0, COMMAND = 1, DATA_WITH_CRC = 2 };
