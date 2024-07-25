#pragma once
#include "BasicSecurityLayer.h"

class Timestamp : public BasicSecurityLayer {
 public:
  bool isValid(uint64_t key) override;
  uint64_t generateNewKey() override;

  void updateKeys() override;

  void encrypt(uint8_t* data, size_t size) override;
  void encryptUsingSameKey(uint8_t* data, size_t size) override;

  bool decrypt(uint8_t* data, size_t size) override;
  bool decryptUsingPreviousKey(uint8_t* data, size_t size) override;

 private:
  void processDataUsingXor(uint8_t* data, uint8_t size, uint32_t key);
};
