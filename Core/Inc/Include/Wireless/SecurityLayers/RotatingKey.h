#pragma once
#include <string.h>

#include "BasicSecurityLayer.h"

class RotatingKey : public BasicSecurityLayer {
 public:
  RotatingKey() : keyGen() {}

  bool isValid(uint64_t key) override;
  uint64_t generateNewKey() override;

  void updateKeys();

  void encrypt(uint8_t* data, size_t size) override;
  void encryptUsingSameKey(uint8_t* data, size_t size);

  bool decrypt(uint8_t* data, size_t size) override;
  bool decryptUsingPreviousKey(uint8_t* data, size_t size) override;

 private:
  void xorWithKey(uint8_t* data, size_t size, uint64_t key);

 private:
  std::uniform_int_distribution<> keyGen;

  WirelessFrame frame;
  uint64_t prevKey = 1;
  uint64_t newKey = 1;
  uint64_t tempNewKey = 1;
};
