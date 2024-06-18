#pragma once
#include "BasicSecurityLayer.h"

class Timestamp : public BasicSecurityLayer {
 public:
  bool isValid(uint64_t key) override;
  uint64_t generateNewKey() override;

  void updateKeys() override;

  void encrypt(uint8_t* data, size_t size) override;
  void encryptUsingSameKey(uint8_t* data, size_t size) override;

  void decrypt(uint8_t* data, size_t size) override;
  void decryptUsingPreviousKey(uint8_t* data, size_t size) override;
};
