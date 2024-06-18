#pragma once
#include <stdint.h>
#include <string.h>

#include "Types.h"

#pragma pack(push, 1)

struct WirelessFrame {
  uint8_t master;
  uint8_t command;
  uint8_t dataSize;
  uint8_t data[MAX_WIRELESS_FRAME_DATA_SIZE];
  uint16_t crc16;
  uint64_t key;

  size_t ToBufferIfEncrypted(uint8_t* data, uint8_t frameLength) const {
    if (frameLength < MIN_FRAME_SIZE + FRAME_KEY_SIZE) {
      return 0;
    }
    uint8_t idx = 0;
    data[idx++] = master;
    data[idx++] = command;
    data[idx++] = dataSize;
    memcpy(&data[idx], this->data, frameLength - MIN_FRAME_SIZE);
    idx += frameLength - MIN_FRAME_SIZE - FRAME_KEY_SIZE;
    memcpy(&data[idx], &this->crc16, sizeof(this->crc16));
    idx += sizeof(this->crc16);
    memcpy(&data[idx], &this->key, sizeof(this->key));
    idx += sizeof(this->key);
    return idx;
  }

  size_t ToBuffer(uint8_t* data, uint8_t size) const {
    if (size < MIN_FRAME_SIZE + this->dataSize) {
      return 0;
    }
    uint8_t idx = 0;
    data[idx++] = master;
    data[idx++] = command;
    data[idx++] = dataSize;
    memcpy(&data[idx], this->data, this->dataSize);
    idx += this->dataSize;
    memcpy(&data[idx], &this->crc16, sizeof(this->crc16));
    idx += sizeof(this->crc16);
    return idx;
  }

  size_t ToFrame(uint8_t* data, uint8_t size) {
    if (size < MIN_FRAME_SIZE) {
      return 0;
    }
    uint8_t idx = 0;
    master = data[idx++];
    command = data[idx++];
    dataSize = data[idx++];

    if (size < MIN_FRAME_SIZE + dataSize) {
      return 0;
    }
    memcpy(this->data, &data[idx], dataSize);
    idx += dataSize;
    memcpy(&this->crc16, &data[idx], sizeof(this->crc16));
    idx += sizeof(this->crc16);
    memcpy(&this->key, &data[idx], sizeof(this->key));
    if (this->key != 0) {
      idx += sizeof(this->key);
    }
    return idx;
  }
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
