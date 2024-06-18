#include "Wireless/SecurityLayers/Token.h"

#include "AppHelper/Env/AppEnvironment.h"

bool Token::isValid(uint64_t key) {
  uint8_t shift = key & 7;
  uint8_t smallToken = (key >> (shift * 8)) & 0xFF;
  bool result = isUnique(smallToken);
  if (result) {
    this->saveTokenToHistory(smallToken);
  }
  return result;
}

uint64_t Token::generateNewKey() {
  uint64_t tempToken = this->keyGen(this->generator);

  uint8_t shift = shiftTokenGen(generator);
  uint8_t smallToken = smallTokenGen(generator);

  while (!isUnique(smallToken)) {
    smallToken = smallTokenGen(generator);
  }

  tempToken &= ~7;  // clear 3 LSB
  tempToken |= shift;

  uint64_t mask = ~(static_cast<uint64_t>(0xFF) << (shift * 8));
  tempToken = tempToken & mask;
  tempToken |= smallToken << (shift * 8);

  this->saveTokenToHistory(smallToken);
  return tempToken;
}

bool Token::isUnique(uint8_t smallToken) {
  for (uint8_t i = 0; i < TOKEN_HISTORY_SIZE; i++) {
    if (tokenHistory[i] == smallToken) {
      return false;
    }
  }
  return true;
}

void Token::encrypt(uint8_t* data, size_t size) {}

void Token::decrypt(uint8_t* data, size_t size) {}

void Token::saveTokenToHistory(uint8_t smallToken) {
  this->tokenHistory[this->tokenHistoryIdx] = smallToken;
  this->tokenHistoryIdx = (this->tokenHistoryIdx + 1) % TOKEN_HISTORY_SIZE;
}
