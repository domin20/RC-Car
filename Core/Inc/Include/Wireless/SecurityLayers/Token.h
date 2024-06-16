#pragma once

#include "BasicSecurityLayer.h"

#define TOKEN_HISTORY_SIZE 20

class Token : public BasicSecurityLayer {
 public:
  Token() : shiftTokenGen(1, 7), smallTokenGen(0, 255) {}
  bool isValid(uint64_t key) override;
  uint64_t generateNewKey() override;

  void encrypt(uint8_t* data, size_t size) override;
  void decrypt(uint8_t* data, size_t size) override;

 private:
  bool isUnique(uint8_t smallToken);
  void saveTokenToHistory(uint8_t smallToken);

 private:
  uint8_t tokenHistory[TOKEN_HISTORY_SIZE] = {0};
  uint8_t tokenHistoryIdx = 0;

  std::uniform_int_distribution<> shiftTokenGen;
  std::uniform_int_distribution<> smallTokenGen;
  std::uniform_int_distribution<> keyGen;
};
