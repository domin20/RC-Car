#pragma once
#include "Config/StaticConfig.h"
#include "SecurityLayers/BasicSecurityLayer.h"

#define MASTER_NUMBER 69

constexpr uint8_t CRC_SIZE = 2;
constexpr uint8_t MIN_FRAME_SIZE = 5;
constexpr uint8_t FRAME_KEY_SIZE = 8;

constexpr uint32_t WIRELESS_BAUDRATE = 38400;

constexpr uint8_t MAX_WIRELESS_FRAME_DATA_SIZE = 30;
constexpr uint8_t MAX_FRAME_SIZE = 255;
constexpr uint8_t ACK_BYTE = 0xDD;

constexpr uint32_t ONE_CHAR_SEND_TIME_US10 = (1000000 / WIRELESS_BAUDRATE);
constexpr uint32_t _5_CHAR_SEND_TIME_US10 = (5.0f * ONE_CHAR_SEND_TIME_US10);

enum WirelessFrameState { ADDRESS = 0, COMMAND = 1, DATA_WITH_CRC = 2 };

enum WirelessCommand {
  STEERING_DATA = 100,
  LIGHT_SERVICE = 101,
  SYNCHRONIZE_RTC = 199,
};
