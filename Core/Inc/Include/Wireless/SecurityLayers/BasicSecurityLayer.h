#pragma once

#include <stdint.h>

#include <random>

#include "../Structures.h"

struct SecurityLayerContext {
  bool isAckReceived = true;
  time_t lastTxFrameTimestamp = 0;
};

class BasicSecurityLayer;

using SecurityLayerPair = std::pair<SecurityLayerContext, BasicSecurityLayer*>;

class BasicSecurityLayer {
 public:
  virtual bool isValid(uint64_t key) = 0;
  virtual uint64_t generateNewKey() = 0;

  virtual void updateKeys() = 0;

  virtual void encrypt(uint8_t* data, size_t size) = 0;
  virtual void encryptUsingSameKey(uint8_t* data, size_t size) = 0;

  virtual void decrypt(uint8_t* data, size_t size) = 0;
  virtual void decryptUsingPreviousKey(uint8_t* data, size_t size) = 0;

 protected:
  static std::mt19937 generator;
};
