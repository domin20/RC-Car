#pragma once
#include "Config/StaticConfig.h"
#include "SecurityLayers/BasicSecurityLayer.h"

#define MASTER_NUMBER 69

constexpr uint8_t CRC_SIZE = 2;
constexpr uint8_t MIN_FRAME_SIZE = 5;
constexpr uint8_t FRAME_KEY_SIZE = 8;

constexpr uint8_t MAX_WIRELESS_FRAME_DATA_SIZE = 30;
constexpr uint8_t MAX_FRAME_SIZE = 255;
constexpr uint8_t ACK_BYTE = 0xDD;

constexpr uint32_t ONE_CHAR_SEND_TIME_US = (1000000 / WIRELESS_BAUDRATE);
constexpr uint32_t _1_5_CHAR_SEND_TIME_US = (1.5f * ONE_CHAR_SEND_TIME_US);
constexpr uint32_t _3_5_CHAR_SEND_TIME_US = (3.5f * _1_5_CHAR_SEND_TIME_US);

enum WirelessFrameState { ADDRESS = 0, COMMAND = 1, DATA_WITH_CRC = 2 };

enum WirelessCommand {
  STEERING_DATA = 100,
  LIGHT_SERVICE = 101,
  SYNCHRONIZE_RTC = 199,
};
