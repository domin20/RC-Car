#pragma once

#define CRC_SIZE 2
#define MIN_FRAME_SIZE 4
#define FRAME_SIZE_WITHOUT_DATA 4

#define MAX_WIRELESS_FRAME_DATA_SIZE 30
#define MAX_DATA_SIZE 255

enum WirelessFrameState { ADDRESS = 0, COMMAND = 1, DATA_WITH_CRC = 2 };