#include "Wireless/SecurityLayers/Timestamp.h"

#include <time.h>

#include "AppHelper/Env/AppEnvironment.h"

bool Timestamp::isValid(uint64_t key) {
  time_t currentTime = AppEnvironment::getEnvironmentContext()->getUnixTimeFromRTC();
  if (key >= (uint64_t)currentTime - 2 && key <= (uint64_t)currentTime + 2) {
    return true;
  } else {
    return false;
  }
}

uint64_t Timestamp::generateNewKey() {
  time_t currentTime = AppEnvironment::getEnvironmentContext()->getUnixTimeFromRTC();
  return currentTime;
}

void Timestamp::updateKeys() {}

void Timestamp::encrypt(uint8_t* data, size_t size) {
  uint32_t timestampUnix = this->generateNewKey();
  this->processDataUsingXor(data, size, timestampUnix);
}

void Timestamp::encryptUsingSameKey(uint8_t* data, size_t size) {}

bool Timestamp::decrypt(uint8_t* data, size_t size) {
  if (!data) {
    return false;
  }
  uint32_t timestampUnix = this->generateNewKey();
  this->processDataUsingXor(data, size, timestampUnix);
  return true;
}

bool Timestamp::decryptUsingPreviousKey(uint8_t* data, size_t size) {
  uint32_t timestampUnix = this->generateNewKey();
  this->processDataUsingXor(data, size, timestampUnix + 1);
  return true;
}

void Timestamp::processDataUsingXor(uint8_t* data, uint8_t size, uint32_t key) {
  uint8_t keyBytes[4];
  memcpy(keyBytes, &key, sizeof(key));

  for (size_t i = 0; i < size; ++i) {
    data[i] ^= keyBytes[i % 4];
  }
}
